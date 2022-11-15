/*  
    Component: WIFI_LOGGER
    Author: Rafael J. Scholtz
    Date: 25/09/2022
    Contents: Functions and tasks to initialize, transmit and coordinate
    wifi communication
*/

#ifndef HEADER_WL
#define HEADER_WL

// --- Includes --- //
#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/portmacro.h"
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
#include "lwip/sockets.h"
#include "lwip/err.h"
#include "lwip/sys.h"

// --- Own components --- //
#include "DATA_PARSER.h"
#include "SPI.h"


// --- Defines --- //

/*  Wi-fi configuration can be changed below, otherwise
    it's taken from menuconfig */

#define EXAMPLE_ESP_WIFI_SSID      CONFIG_ESP_WIFI_SSID
#define EXAMPLE_ESP_WIFI_PASS      CONFIG_ESP_WIFI_PASSWORD
#define EXAMPLE_ESP_WIFI_CHANNEL   CONFIG_ESP_WIFI_CHANNEL
#define EXAMPLE_MAX_STA_CONN       CONFIG_ESP_MAX_STA_CONN

// Same IPV4 is configured in menuconfig and below
#if defined(CONFIG_EXAMPLE_IPV4)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV4_ADDR
#elif defined(CONFIG_EXAMPLE_IPV6)
#define HOST_IP_ADDR CONFIG_EXAMPLE_IPV6_ADDR
#else
#define HOST_IP_ADDR "192.168.4.2"
#endif

#define PORT 9999

// --- Object types --- //
typedef enum
{
    WL_SAMPLE_PARAM = 400,
    WL_SERVER_ACK = 500,
    WL_SERVER_FAILURE = 600
} WL_MESSAGE;

// --- External objects --- //
extern TaskHandle_t WL_TCP_client_task_handle; // Task placed outside main
extern portMUX_TYPE spinlock_serializing_data;
// Flags
extern bool serializing_flag; // Flag for coordinating wifi logging
extern bool suspend_flag;

// --- Public functions --- //

// Function: WL_initialize
// Input: void
// Output: void
// Description: Intializes Wifi Logger dependencies (NVS, SOFT AP and variables)
void WL_Initialize(void);


// Function: WL_send_payload
// Input: socket object
// Output: returns -1 if error occured, otherwise returns the number of bytes sent
// Description: Serializes the existent data buffers and sends them through wifi
// Note: this function currently only supports sending pre-defined structs (buffers)
// int WL_send_payload(int sock);


// Function: WL_TCP_client_task
// Input: NULL
// Output: NULL
// Description: Task responsible for sending buffer data to the server
// it should be unsuspended by SPI_Comm task, and will suspend itself
// when done transmitting stuff, if there isn't anything else to send
void WL_TCP_client_task(void *pvParameters);

#endif