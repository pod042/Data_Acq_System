/*
    Random tests on the ESP32 DOITV1 platform
    Current objective: Integrate wi-fi driver and SPI transmissions
        1. Develop wi-fi transmission task
            1.1. Create component components with the required functions (based on the code from simple_data_logger)
        2. Control task by blocking and realesing it in the SPI task
            2.1. Task control can be done by creating tasks that only run once, blocking / realesing ...

    Note to self: 1. continue the integration from WL_send_payload and unsuspend release point
    then test for unlimited samples. 2. do this until it works; 3. then research about automated programming tests 
*/

// --- Includes --- //

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stddef.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "WIFI_LOGGER.h"
#include "SPI.h"
//#include "esp_log.h"

// --- Defines --- //

#define LED_OUT 2

// --- Global variables --- //



// --- Task declarations --- //
// static void WL_TCP_client_task(void *pvParameters);
//static void SPI_Talk_task(void *arg);


void app_main(void)
{

    //Allow other core to finish initialization
    vTaskDelay(pdMS_TO_TICKS(100));

    // Configura LED
    gpio_reset_pin(LED_OUT);
    /* Set the GPIO as a push/pull output */
    gpio_set_direction(LED_OUT, GPIO_MODE_OUTPUT);

    

    // Blink led twice
    gpio_set_level(2, 1);
    vTaskDelay(500/portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(500/portTICK_PERIOD_MS);
    gpio_set_level(2, 1);
    vTaskDelay(500/portTICK_PERIOD_MS);
    gpio_set_level(2, 0);
    vTaskDelay(500/portTICK_PERIOD_MS);

    // Initializes SPI dependencies
    SPI_Initialize();
    
    // Initializes WL dependencies
    WL_Initialize();

    // Initializes DP dependencies
    //DP_initialize();

    vTaskDelay(500/portTICK_PERIOD_MS);


    /* --- Tasks creation --- */

    // Wifi logger -> CPU 0
    xTaskCreatePinnedToCore(WL_TCP_client_task, "TCP_client", 3000, NULL, 10, &WL_TCP_client_task_handle, 0);

    // SPI coordinator -> CPU 1
    xTaskCreatePinnedToCore(SPI_Talk_task, "SPI", 3000, NULL, 20, &SPI_talk_task_handle, 1);

    
}