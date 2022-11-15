/*
 * interrupts.c
 *
 *  Created on: 19 de mai de 2022
 *      Author: rafae
 */


#include <Headers/SPI_logger.h>
#include "Headers/interrupts.h"
#include "Headers/ADC.h"
#include "Headers/EPWM.h"

//  --- Globals --- //
Uint16 resultsIndex;
volatile Uint16 bufferFull;
float32 Duty_cycle;
Uint16 r_data[SPI_BATCH_NR];
Uint16 dummy_data_2_bytes = 0;
//Uint16 dummy_data_1_byte = 0;
long long unsigned int words_sent = 0;


// --- Externs --- //
Uint16 ADC_CHA_RESULTS[ADC_RESULTS_BUFFER_SIZE];

// --- Test variables --- //
unsigned char test_flag = 1;


// --- Interrupts --- //

// adca1_isr - Read ADC Buffer in ISR - every 100 kHz
interrupt void adca1_isr(void)
{
    ADC_CHA_RESULTS[resultsIndex++] = AdcaResultRegs.ADCRESULT0;


    if(SPI_TX(dummy_data_2_bytes) == SPI_MODULE_FAILURE){
        asm("     ESTOP0");  //Test failed!! Stop!
        for(;;);
    }
    if(SPI_TX(dummy_data_2_bytes+(Uint16)100) == SPI_MODULE_FAILURE){
        asm("     ESTOP0");  //Test failed!! Stop!
                for(;;);
    }
    if(SPI_TX(dummy_data_2_bytes+(Uint16)200) == SPI_MODULE_FAILURE){
        asm("     ESTOP0");  //Test failed!! Stop!
                for(;;);
    }
    if(SPI_TX(dummy_data_2_bytes+(Uint16)300) == SPI_MODULE_FAILURE){
        asm("     ESTOP0");  //Test failed!! Stop!
                for(;;);
    }
    words_sent += 4;



    // Updates dummy data
    dummy_data_2_bytes++;
    if(dummy_data_2_bytes >= 800)  dummy_data_2_bytes = 0;


    if(ADC_RESULTS_BUFFER_SIZE <= resultsIndex)
    {
        resultsIndex = 0;
        bufferFull = 1;
    }



    AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag

    //
    // Check if overflow has occurred
    //
    if(1 == AdcaRegs.ADCINTOVF.bit.ADCINT1)
    {
        AdcaRegs.ADCINTOVFCLR.bit.ADCINT1 = 1; //clear INT1 overflow flag
        AdcaRegs.ADCINTFLGCLR.bit.ADCINT1 = 1; //clear INT1 flag
    }

    PieCtrlRegs.PIEACK.all |= PIEACK_GROUP1;
}


// Triggers when SPI completes 16 transmissions
// 16 is specified in SPI_INIT -> SPIFFRX
interrupt void spia_rx_isr(void){

    // Counter variable
    unsigned char i = 0;

    // First thing to do is to reset the CS line and turn it on again if needed
    GPIO_WritePin(SPI_CS_PIN, 1);
    SPI_FIFO_TX_nr -= SPI_BATCH_NR;
    /*
    if(SPI_FIFO_TX_nr > 100){
        asm("     ESTOP0");  //Test failed!! Stop!
        for(;;);
    }
    */

    // Then reads the data from RXBUFFER
    for(i = 0; i<SPI_BATCH_NR; i++){
        r_data[i] = SPI_RX();
    }

    // Also updates internal state machine
    if(r_data[0] == SPI_ESP_FAILURE)  SPI_global_status = SPI_MODULE_FAILURE;

    if(SPI_FIFO_TX_nr >= SPI_BATCH_NR){
        // If there are more than x words
        SpiaRegs.SPITXBUF = SPI_DATA;
        for(i = 0; i<SPI_BATCH_NR; i++){
            SpiaRegs.SPITXBUF = SPI_TX_deQueue();
        }
        GPIO_WritePin(SPI_CS_PIN, 0);
    }
    else{
        // Signals no transaction occuring
        SPI_transaction_occuring = 0;
    }

    // End of interrupt stuff
    SpiaRegs.SPIFFRX.bit.RXFFOVFCLR=1;  // Clear Overflow flag
    SpiaRegs.SPIFFRX.bit.RXFFINTCLR=1;
    SpiaRegs.SPIFFTX.bit.TXFFINTCLR = 1;  // Clear Interrupt flag
    PieCtrlRegs.PIEACK.all|=0x20;       // Issue PIE ack
}

// Shouldn't trigger
// Issue error when it does
interrupt void spia_tx_isr(void){
    asm("     ESTOP0");  //Test failed!! Stop!
    for (;;);
}


// Public functions
void INTERRUPTS_INIT_VARIABLES(void){

    for(resultsIndex = 0; resultsIndex < ADC_RESULTS_BUFFER_SIZE; resultsIndex++)
    {
        ADC_CHA_RESULTS[resultsIndex] = 0;
    }

    for(resultsIndex=0; resultsIndex<4; resultsIndex++){
        r_data[resultsIndex] = 0;
    }

    resultsIndex = 0;
    bufferFull = 0;

}
