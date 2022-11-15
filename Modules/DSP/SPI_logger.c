/*
 * SPI.c
 *
 *  Created on: 25 de mai de 2022
 *      Author: rafae
 */

// To do: create a queue data structure to ensure that SPI functions

#include <Headers/SPI_logger.h>

// --- Globals --- //
unsigned char i = 0;
unsigned char SPI_bit_size_table[SPI_MAX_SAMPLE_NR];
unsigned char last_added_sample = 0;
SPI_STATUS SPI_global_status = SPI_MODULE_OK;

// --- Externs --- //
unsigned char SPI_FIFO_TX_nr = 0;               // Variable used to track total number of transmissions
unsigned char SPI_TX_current_batch_nr = 0;      // Variable used to track current number of the TX batch
bool SPI_transaction_occuring = 0;
unsigned char SPI_FIFO_RX_nr = 0;

// --- Queue functions --- //

// Queue logic
Uint16 SPI_TX_queue[SPI_MAX_QUEUE_SIZE];
int front = -1, rear = -1;

// Check if the queue is full
int SPI_TX_queue_isFull() {
  if ((front == rear + 1) || (front == 0 && rear == SPI_MAX_QUEUE_SIZE - 1)) return 1;
  return 0;
}

// Check if the queue is empty
int SPI_TX_queue_isEmpty() {
  if (front == -1) return 1;
  return 0;
}

// Adding an element
void SPI_TX_enQueue(Uint16 element) {
  if (SPI_TX_queue_isFull()){
      // Turns CS on to end transactions
      GPIO_WritePin(SPI_CS_PIN, 1);
      asm("     ESTOP0");  //Test failed!! Stop!
      for (;;);
  }
  else {
    if (front == -1) front = 0;
    rear = (rear + 1) % SPI_MAX_QUEUE_SIZE;
    SPI_TX_queue[rear] = element;
  }
}

// Removing an element
Uint16 SPI_TX_deQueue() {
  Uint16 element;
  if (SPI_TX_queue_isEmpty()) {
    /* Error code here */
    asm("     ESTOP0");  //Test failed!! Stop!
    for (;;);
    //return (-1);
  } else {
      element = SPI_TX_queue[front];
      if (front == rear) {
      front = -1;
      rear = -1;
      }
      // Q has only one element, so we reset the
      // queue after dequeing it. ?
      else {
        front = (front + 1) % SPI_MAX_QUEUE_SIZE;
      }
    return (element);
  }
}







void SPI_INIT(void){

    // Initialize SPI FIFO registers
    /* Settings:
        - FIFO mode;
        - No interrupt based on TXFFIL match (number of word registers remaining)
        - Interrupt in X words in RXFFIL (basically four transmissions)
        - No delay between FIFO transmit buffer to shift register
    */
    SpiaRegs.SPIFFTX.all = 0xC000;
    SpiaRegs.SPIFFRX.all = 0x0020;
    SpiaRegs.SPIFFRX.bit.RXFFIL = SPI_BATCH_NR+1;
    SpiaRegs.SPIFFTX.bit.TXFFIL = 0;
    // SpiaRegs.SPIFFRX.bit.RXFFIL = SPI_RX_INT_NR; // Set RX FIFO level to SPI_TX_BATCH_nr
    SpiaRegs.SPIFFCT.all = 0x00;


    SpiaRegs.SPIFFTX.bit.TXFIFO=1;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET=1;

    //
    // Initialize core SPI registers
    //

    // Initialize SPI-A

    // Set reset low before configuration changes
    // Shift clock polarity (0 == rising, 1 == falling)
    // 16-bit character
    // Enable loop-back (Tx pin and Rx pin are connected internally)
    // Sets high speed mode
    SpiaRegs.SPICCR.bit.SPISWRESET = 0;
    SpiaRegs.SPICCR.bit.CLKPOLARITY = 0;
    SpiaRegs.SPICCR.bit.SPICHAR = (16-1);
    SpiaRegs.SPICCR.bit.SPILBK = 0;
    SpiaRegs.SPICCR.bit.HS_MODE = 1;

    // Enable master (0 == slave, 1 == master)
    // Enable transmission (Talk)
    // Clock phase (0 == normal, 1 == delayed)
    // SPI interrupts are enabled
    SpiaRegs.SPICTL.bit.MASTER_SLAVE = 0;
    SpiaRegs.SPICTL.bit.TALK = 1;
    SpiaRegs.SPICTL.bit.CLK_PHASE = 0;
    SpiaRegs.SPICTL.bit.SPIINTENA = 0;

    // Set the baud rate to 20 MHz
    //SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 0x3; // 200 MHz /(3+1)
    // Set the baud rate to X MHz
    SpiaRegs.SPIBRR.bit.SPI_BIT_RATE = 9; // 200 MHz /(9+1)

    // Set FREE bit
    // Halting on a breakpoint will not halt the SPI
    SpiaRegs.SPIPRI.bit.FREE = 1;

    // Release the SPI from reset
    SpiaRegs.SPICCR.bit.SPISWRESET = 1;


    // Initializes variables from module
    //SPI_FIFO_queue_end_ptr = &SPI_FIFO_queue[0];
    //SPI_FIFO_queue_str_ptr = &SPI_FIFO_queue[0];
}

// Writes 16 bit data into SPI TX buffer
// also turns CS line on manually
/*
 * If SPI is already busy - just write to txbuffer
 * if it isn't, write and turn CS off
 *
 */
SPI_STATUS SPI_TX(Uint16 data){

    // Updates TX counter
    SPI_FIFO_TX_nr++;

    // Pushes data to queue
    SPI_TX_enQueue(data);

    // If SPI_FIFO_TX_nr >= SPI_BATCH_NR, initiates transaction
    if(SPI_FIFO_TX_nr >= SPI_BATCH_NR && !SPI_transaction_occuring){

    // Sends first a data header
    SpiaRegs.SPITXBUF = SPI_DATA;

    for(i = 0; i<SPI_BATCH_NR; i++){
        SpiaRegs.SPITXBUF = SPI_TX_deQueue();
    }

    SPI_transaction_occuring = 1;
    GPIO_WritePin(SPI_CS_PIN, 0);
    }
    return SPI_global_status;
}

Uint16 SPI_RX(void){
    return SpiaRegs.SPIRXBUF;
}

// Saves the bit number of a sample, in the order of function call
void SPI_register_sample(unsigned char sample_bit_nr){
    SPI_bit_size_table[last_added_sample] = sample_bit_nr;
    last_added_sample++;
    return;
}

// Signals to the ESP that DSP is ready for data collection
// note: not thread safe, should be used during initialization only
SPI_STATUS SPI_comm_setup(void){

    // Disable interrupts temporarily
    DINT;

    // Sends first a protocol header
    SpiaRegs.SPITXBUF = SPI_PROTOCOL;

    // Then writes the sample bit number
    for(i=0; i<last_added_sample; i++){
        SpiaRegs.SPITXBUF = (Uint16)SPI_bit_size_table[i];
    }

    // Starts setup transaction
    GPIO_WritePin(SPI_CS_PIN, 0);

    // Waits untill it ends
    while(SpiaRegs.SPIFFRX.bit.RXFFST < SPI_BATCH_NR+1);

    // Floods out the received data to empty RXFIFO
    volatile Uint16 trash;
    for(i = 0; i<SPI_BATCH_NR+1; i++){
            trash = SPI_RX();
    }

    // Waits until ESP responds with another 16 word transaction
    while(SpiaRegs.SPIFFRX.bit.RXFFST < SPI_BATCH_NR+1);

    // Ends setup transaction
    GPIO_WritePin(SPI_CS_PIN, 1);

    // Checks response of ESP
    Uint16 response;
    response = SPI_RX();

    // Also floods out rest of data
    for(i = 0; i<SPI_BATCH_NR; i++){
                trash = SPI_RX();
    }

    // End of interrupt stuff
    //SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
    //SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1;
    //SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;  // Clear Interrupt flag
    //PieCtrlRegs.PIEACK.all|=0x20;       // Issue PIE ack

    // Enable again interrupts
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 0;
    SpiaRegs.SPIFFRX.bit.RXFIFORESET = 1;
    SpiaRegs.SPIFFTX.bit.TXFIFO = 0;
    SpiaRegs.SPIFFTX.bit.TXFIFO = 1;
    //EINT;

    // Updates SPI_global_status
    if(response != SPI_ESP_READY){
        SPI_global_status = SPI_MODULE_FAILURE;
    }
    else{
        SPI_global_status = SPI_MODULE_OK;
    }

    return SPI_global_status;
}


