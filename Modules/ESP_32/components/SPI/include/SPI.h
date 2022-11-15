/*  
    Component: SPI
    Author: Rafael J. Scholtz
    Date: 25/09/2022
    Contents: Functions to initialize primary SPI module
*/

// --- Includes --- //

#ifndef HEADER_SPI
#define HEADER_SPI

#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "esp_log.h"

// Own components
#include "WIFI_LOGGER.h"
#include "DATA_PARSER.h"


// --- Defines --- //

// SPI related
#define MISO 19
#define MOSI 23
#define SCLK 18
#define CS 5
#define SPI_BATCH_SIZE 15

// --- Object types --- //
typedef enum
{
    SPI_DATA = 10,
    SPI_PROTOCOL = 200
} SPI_HEADER;

typedef enum
{
    SPI_ESP_READY = 100,
    SPI_ESP_OK = 200,
    SPI_ESP_FAILURE = 300
} SPI_ESP_RESPONSE;

// --- External objects --- //
extern TaskHandle_t SPI_talk_task_handle; // Task placed outside main
extern volatile bool buffer_access_flag;
extern portMUX_TYPE spinlock_buffers_ready;
extern SPI_ESP_RESPONSE ESP_STATUS;
extern uint16_t tx_data[SPI_BATCH_SIZE+1];

// --- Public functions --- //

// Function: SPI_Initialize
// Input: void
// Output: void
// Description: Initializes the primary SPI moduled tuned to the DSP F28379D specifications
void SPI_Initialize(void);


// Function: SPI_Talk_task
// Input: NULL
// Output: NULL
// Description: Constantly polls for SPI transactions and stores the data
// in RAM buffers
void IRAM_ATTR SPI_Talk_task(void *arg);

#endif