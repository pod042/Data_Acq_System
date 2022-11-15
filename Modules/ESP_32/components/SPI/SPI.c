/*  
    Component: SPI
    Author: Rafael J. Scholtz
    Date: 25/09/2022
    Contents: Functions to initialize primary SPI module
*/

// --- Includes --- //
#include <stdio.h>
#include "SPI.h"


// --- Defines --- //
#define DATA_LIM 160

// --- Global variables --- //

// Task handle
TaskHandle_t SPI_talk_task_handle = NULL;

// Spinlock test
portMUX_TYPE spinlock_buffers_ready;

// SPI status message
SPI_ESP_RESPONSE ESP_STATUS = SPI_ESP_OK;

// Tag for debug
static const char *TAG = "SPI";

// rx and tx spi buffers - note: the +1 is meant for a header word to be implemented
uint16_t rx_data[SPI_BATCH_SIZE+1];
uint16_t tx_data[SPI_BATCH_SIZE+1];

// Global test receiver buffers
//uint16_t buffer1[300];
//uint16_t buffer2[300];
//uint16_t buffer3[300];
//uint16_t buffer4[300];

esp_err_t ret; // Error check return variable

//  -> SPI configuration structures

// Necessary handle
spi_device_handle_t SPI_HANDLE;

// Utilizing VSPI (SPI3)
spi_bus_config_t SPI_BUS_CONFIG={
        .miso_io_num = MISO,
        .mosi_io_num = MOSI,
        .sclk_io_num = SCLK,
        .quadwp_io_num = -1,
        .quadhd_io_num = -1,
        .max_transfer_sz = 64 // Bytes
};

// Control test variables
unsigned int sample_nr = 0;
unsigned int current_byte_nr = 0;
unsigned char buffer_nr = 0;
volatile bool buffer_access_flag = 0;
volatile unsigned char count = 0;
volatile unsigned char buffer1_ix = 0;
volatile unsigned char buffer2_ix = 0;
volatile unsigned char buffer3_ix = 0;
volatile unsigned char buffer4_ix = 0;
volatile unsigned char i = 0;
volatile unsigned char j = 0;
unsigned int * buffer4_ix_ptr = NULL;

// Secondary SPI config interface
spi_device_interface_config_t TI_DSP={
    .mode = 1,
    .clock_speed_hz = (uint32_t)20*1000*1000,
    .queue_size = 8,
    .command_bits = 0,
    .address_bits = 0,
    .dummy_bits = 0,
    .input_delay_ns = 0,
    .spics_io_num = -1
};

// Data format struct
spi_transaction_t spi_data = {
        .length = 16*(SPI_BATCH_SIZE+1),
        .rxlength = 16*(SPI_BATCH_SIZE+1),
        .tx_buffer = &tx_data,
        .rx_buffer = &rx_data
};

// --- Public functions --- //

void SPI_Initialize(void){
    // Configures CS pin for reading
    gpio_set_direction(CS, GPIO_MODE_INPUT);
    gpio_set_pull_mode(CS, GPIO_PULLUP_ENABLE);

    // Initializes the SPI bus
    ret = spi_bus_initialize(VSPI_HOST, &SPI_BUS_CONFIG, 0);
    ESP_ERROR_CHECK(ret);

    // Adds secondary device to bus
    ret = spi_bus_add_device(VSPI_HOST, &TI_DSP, &SPI_HANDLE);
    ESP_ERROR_CHECK(ret);

    spi_device_acquire_bus(SPI_HANDLE, portMAX_DELAY);
    ESP_LOGI(TAG, "SPI freq. limit = %d\n", spi_get_freq_limit(0, 0));
    //printf("SPI freq. limit = %d\n", spi_get_freq_limit(0, 0));
    //printf("Starting SPI transaction:\n");

    //memset(rx_data, 0, (SPI_BATCH_SIZE+1)*2);
    //memset(tx_data, 0, (SPI_BATCH_SIZE+1)*2);
}


void IRAM_ATTR SPI_Talk_task(void *arg){
    ESP_LOGI(TAG, "Init SPI Task");


    // Initialization routine

    // Reads CS pin and does nothing while it`s high
    while(gpio_get_level(CS) == 1);

    // Starts the spi through polling
    ret = spi_device_polling_transmit(SPI_HANDLE, &spi_data);

    // Reads header
    uint16_t header = SPI_SWAP_DATA_RX(rx_data[0], 16);
    printf("ESP read header: %d\n", header);

    //while(1);

    if(header != SPI_PROTOCOL){
        // Issues failure
        tx_data[0] = SPI_SWAP_DATA_TX(SPI_ESP_FAILURE, 16);
        printf("tx_data: %d\n", tx_data[0]);
        // Reads CS pin and does nothing while it`s high
        while(gpio_get_level(CS) == 1);
        // Starts the spi through polling
        ret = spi_device_polling_transmit(SPI_HANDLE, &spi_data);

        // Task self deletes
        vTaskDelete(NULL);
    }
    else{
        // Reads sample bit number and saves it in the DP_Table
        unsigned char i = 0;
        for(i=0; i<DP_MAX_SAMPLE_VARIABLE_NUMBER; i++){
            DP_BUFFER_ID_TABLE[i] = SPI_SWAP_DATA_RX(rx_data[i+1], 16);
        }
    }

    // Notifies WL task and waits back for response
    xTaskNotifyGive(WL_TCP_client_task_handle);
    // Wait time: 10 seconds - If timeout occurs, return error
    if(ulTaskNotifyTake(pdTRUE, 10*1000/portTICK_PERIOD_MS) == 0){
        // Connection failed -> issues failure
        tx_data[0] = SPI_SWAP_DATA_TX(SPI_ESP_FAILURE, 16);
        printf("tx_data: %d\n", tx_data[0]);
        // Reads CS pin and does nothing while it`s high
        while(gpio_get_level(CS) == 1);
        // Starts the spi through polling
        ret = spi_device_polling_transmit(SPI_HANDLE, &spi_data);

        // Task self deletes
        vTaskDelete(NULL);
    }

    // Connection successful -> issues ESP_READY message
    tx_data[0] = SPI_SWAP_DATA_TX(SPI_ESP_READY, 16);
    printf("tx_data: %d\n", tx_data[0]);
    // Reads CS pin and does nothing while it`s high
    while(gpio_get_level(CS) == 1);
    // Starts the spi through polling
    ret = spi_device_polling_transmit(SPI_HANDLE, &spi_data);

    // 2 ticks delay
    tx_data[0] = SPI_SWAP_DATA_TX(ESP_STATUS, 16);
    for(volatile unsigned char i=0; i<2; i++);
    //while(gpio_get_level(CS) == 0);
    //ESP_LOGI(TAG, "Init success");
    // DEBUG WHILE
    //while(1);

    // Infinite loop
    while(1){
        // Checks for end of test length
        //if(*buffer4_ix_ptr >= DATA_LIM)    break;

        // Reads CS pin and does nothing while it`s high
        while(gpio_get_level(CS) == 1);

        // Starts the spi through polling
        
        ret = spi_device_polling_transmit(SPI_HANDLE, &spi_data);
        tx_data[0] = SPI_SWAP_DATA_TX(ESP_STATUS, 16);
        //ESP_ERROR_CHECK(ret);

        // Swaps data bits
        //uint16_t swapped_data[SPI_BATCH_SIZE];
        //for(unsigned char i=0; i<SPI_BATCH_SIZE; i++){
        //    swapped_data[i] = SPI_SWAP_DATA_RX(rx_data[i+1], 16);
        //}

        // Updates internal state machine based on header
        /* ...to do... */

        // Writes data to wifi tx buffer
        
        if(DP_write_to_buffer(&rx_data[1], SPI_BATCH_SIZE*2) >= DP_MIN_BUFFER_SIZE){
            //asm("NOP");
            memcpy(DATA_PAYLOAD_BUFFER2, DATA_PAYLOAD_BUFFER1, DP_current_buffer1_size);
            DP_current_buffer2_size = DP_current_buffer1_size;
            xTaskNotifyGive(WL_TCP_client_task_handle);
            DP_current_buffer1_size = 0;
            //ESP_LOGI(TAG, "Filled buffer with %d bytes\n", DP_current_buffer_size);
            //vTaskResume(WL_TCP_client_task_handle);
            //ESP_LOGI(TAG, "Filled buffer");
            //DP_current_buffer_size = 0;
        }

        for(volatile unsigned char i=0; i<2; i++);
        

        //taskENTER_CRITICAL(&spinlock_buffers_ready);
        //buffer_access_flag = 0;
        // Saves data - better method
        
        /*
        for(i=1; i<=SPI_BATCH_SIZE; i++){

            // Flag starts at 0
            done_writing = 0;
            current_byte_nr++;
            do
            {
                switch (DP_BUFFER_ID_TABLE[buffer_nr])
                {
                // Just update buffer_nr
                case NOT_USED:
                    buffer_nr = (buffer_nr+1) % NUM_OF_BUFFERS;
                    break;

                // If 8 bits or 16 bits
                case SAMPLE_16_BITS:
                case SAMPLE_8_BITS:
                    // buffer_ix is within DP_buffer structure
                    sample_nr = DP_buffer_ptrs[buffer_nr]->buffer_ix;
                    DP_buffer_ptrs[buffer_nr]->SAMPLE_BUFFER[DP_buffer_ptrs[buffer_nr]->buffer_ix] = SPI_SWAP_DATA_RX(rx_data[i], 16);

                    // Resets buffer_ix when needed
                    DP_buffer_ptrs[buffer_nr]->buffer_ix = (DP_buffer_ptrs[buffer_nr]->buffer_ix + 1) % DP_MAX_SAMPLE_BUFFER_SIZE;

                    // Updates buffer_nr
                    buffer_nr = (buffer_nr+1) % NUM_OF_BUFFERS;
                    done_writing = 1; // Signals end of write
                    break;
                default:
                    break;
                }
            } while (!done_writing);
        }
        */
        
        // Check if wifi task is suspended
        //eTaskState wifi_state;
        //if(eTaskGetState(WL_TCP_client_task_handle) == )

        // If got enough data, resumes wifi task
        //if(current_byte_nr >= DATA_PAYLOAD_BUFFER_SIZE-4) vTaskResume(WL_TCP_client_task_handle);
        //current_byte_nr = 0;
        
        // Notify core 1 if ready to serialize
        //if(*buffer4_ix_ptr >= DP_MAX_SAMPLE_BUFFER_SIZE-20)  xTaskNotifyGive(WL_TCP_client_task_handle);


        //buffer_nr = (buffer_nr+1) % (NUM_OF_BUFFERS+1); // Updates buffer_nr one more time
    }
    
    /* --- Test level code --- */

    // Notify core 1
    //xTaskNotifyGive(WL_TCP_client_task_handle);

    while(1);
    // Prints the data later
    // Buffer 1
    /*
    printf("Buffer 1 - sample nr. x sample value\n");
    for(count = 0; count < DATA_LIM; count++){
        printf("%d  x  %d\n", count, DP_buffer_ptrs[0]->SAMPLE_BUFFER[count]);
    }

    // Buffer 2
    printf("Buffer 2 - sample nr. x sample value\n");
    for(count = 0; count < DATA_LIM; count++){
        printf("%d  x  %d\n", count, DP_buffer_ptrs[1]->SAMPLE_BUFFER[count]);
    }

    // Buffer 3
    printf("Buffer 3 - sample nr. x sample value\n");
    for(count = 0; count < DATA_LIM; count++){
        printf("%d  x  %d\n", count, DP_buffer_ptrs[2]->SAMPLE_BUFFER[count]);
    }

    // Buffer 4
    printf("Buffer 4 - sample nr. x sample value\n");
    for(count = 0; count < DATA_LIM; count++){
        printf("%d  x  %d\n", count, DP_buffer_ptrs[3]->SAMPLE_BUFFER[count]);
    }
    */
    printf("End of test\n");

    // Endless loop
    while(1);
}
