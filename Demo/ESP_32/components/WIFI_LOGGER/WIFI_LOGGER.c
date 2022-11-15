/*  
    Component: WIFI_LOGGER
    Author: Rafael J. Scholtz
    Date: 25/09/2022
    Contents: Functions and tasks to initialize, transmit and coordinate
    wifi communication
*/

// --- Includes --- //
#include "WIFI_LOGGER.h"
#include "esp_timer.h"


// --- Global variables --- //

// Task handle
TaskHandle_t WL_TCP_client_task_handle = NULL;

portMUX_TYPE spinlock_serializing_data;

const unsigned NUM_OF_TRIALS = 100;

// Flags
bool serializing_flag = 0; // Flag for coordinating SPI logging
bool suspend_flag = 0;

// Tag for debug
static const char *TAG = "WIFI_LOGGER";

// Test payload
uint16_t test_payload = 9;

// --- Private function's handles --- //
static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data);
void wifi_init_softap(void);


// --- Public functions --- //

void WL_Initialize(void){
    // Intializes nvs (storage of password and AP name)
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);

    // Initializes wifi in AP mode
    ESP_LOGI(TAG, "ESP_WIFI_MODE_AP");
    wifi_init_softap();
}


void WL_TCP_client_task(void *pvParameters)
{
    // -> Task initialization
    //char rx_buffer[128]; // Buffer to get data from PC (currently not important)
    char host_ip[] = HOST_IP_ADDR; // IPV4 in header file
    int addr_family = 0;
    int ip_protocol = 0;
    ESP_LOGI(TAG, "TCP_client_task init");

    //while(1);

    /*
        Loop structure: two while loops
        - the first for reconfiguring the socket object, in case an error occurs; 
        - the second for actual data transmitting

        This task should never delete itself
    */

    // Loop 1: socket initialization loop
    while (1) {
        // Compile directives for IPV4 / IPV6
        // Code only tested in IPV4
        #if defined(CONFIG_EXAMPLE_IPV4)
                struct sockaddr_in dest_addr;
                dest_addr.sin_addr.s_addr = inet_addr(host_ip);
                dest_addr.sin_family = AF_INET;
                dest_addr.sin_port = htons(PORT);
                addr_family = AF_INET;
                ip_protocol = IPPROTO_IP;
        #elif defined(CONFIG_EXAMPLE_IPV6)
                struct sockaddr_in6 dest_addr = { 0 };
                inet6_aton(host_ip, &dest_addr.sin6_addr);
                dest_addr.sin6_family = AF_INET6;
                dest_addr.sin6_port = htons(PORT);
                dest_addr.sin6_scope_id = esp_netif_get_netif_impl_index(EXAMPLE_INTERFACE);
                addr_family = AF_INET6;
                ip_protocol = IPPROTO_IPV6;
        #elif defined(CONFIG_EXAMPLE_SOCKET_IP_INPUT_STDIN)
                struct sockaddr_storage dest_addr = { 0 };
                ESP_ERROR_CHECK(get_addr_from_stdin(PORT, SOCK_STREAM, &ip_protocol, &addr_family, &dest_addr));
        #endif

        // Socket definition
        int sock =  socket(addr_family, SOCK_STREAM, ip_protocol);
        if (sock < 0) {
            ESP_LOGE(TAG, "Unable to create socket: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Socket created, connecting to %s:%d", host_ip, PORT);

        // Socket connection
        int err = connect(sock, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in6));
        if (err != 0) {
            ESP_LOGE(TAG, "Socket unable to connect: errno %d", errno);
            continue;
        }
        ESP_LOGI(TAG, "Successfully connected");

        // After successful connection, waits for config notification
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        // Sends config. settings
        WL_MESSAGE WL_record_sample_param = WL_SAMPLE_PARAM;
        err = send(sock, &WL_record_sample_param, sizeof(WL_MESSAGE), 0);
        if (err < 0) {
            ESP_STATUS = SPI_ESP_FAILURE;
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
        }
        ESP_LOGI(TAG, "DP table[0] = %d\n", DP_BUFFER_ID_TABLE[0]);
        err = send(sock, DP_BUFFER_ID_TABLE, sizeof(DP_BUFFER_ID_TABLE), 0);
        if (err < 0) {
            ESP_STATUS = SPI_ESP_FAILURE;
            ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
            break;
        }

        // Receives ack
        WL_MESSAGE server_response;
        int len = recv(sock, &server_response, sizeof(server_response), 0);
        if (len < 0) {
            ESP_STATUS = SPI_ESP_FAILURE;
            ESP_LOGE(TAG, "Error occurred during receiving: errno %d", errno);
            break;
        }
        // DEBUG - PRINTS RESPONSE
        ESP_LOGI(TAG, "Server response = %d\n", server_response);

        if(server_response != WL_SERVER_ACK){
            // Updates status flag in SPI and ends task
            ESP_STATUS = SPI_ESP_FAILURE;
            break;
        }
        else{
            // Notifies SPI and continues to infinite loop
            xTaskNotifyGive(SPI_talk_task_handle);
        }



        
        // Loop 2: data transmission loop
        //unsigned char trials = 0;
        //uint64_t start = esp_timer_get_time();
        while (1) {

            /*
             For now always suspends
            // Checks flag for permission to send inside spinlock
            taskENTER_CRITICAL(&suspend_flag_spinlock);
            if(suspend_flag == 0) suspend_flag = 1;
            taskEXIT_CRITICAL(&suspend_flag_spinlock);

            // If buffers weren't ready, suspend itself
            if(suspend_flag == 1)   vTaskSuspend(NULL);
            */
            
            //while(!ulTaskNotifyTake(pdTRUE, 0));
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            //vTaskSuspend(NULL);

            // -> Suspend release point

            // Measures serialization and tx time

            // TIME MEASURING TEST CODE


            // Serializes data, then releases SPI
            // DP_serialize_all();

            //uint64_t end = esp_timer_get_time();

            //printf("Wifi serialization took %llu microseconds\n", (end - start));

            //buffer_access_flag = 1;
            //vTaskResume(SPI_talk_task_handle);
            //serializing_flag = 0;

            // Sends the data
            //ESP_LOGI(TAG, "Wifi buffer size = %d", DATA_PAYLOAD_BUFFER_SIZE);

            

            int err = send(sock, DATA_PAYLOAD_BUFFER2, DP_current_buffer2_size, 0);
            //DP_current_buffer_size = 0;
            //int err = send(sock, &test_payload, 2, 0);
            if (err < 0) {
                ESP_LOGE(TAG, "Error occurred during sending: errno %d", errno);
                //tx_data[0] = SPI_SWAP_DATA_TX(SPI_ESP_FAILURE, 16);
                ESP_STATUS = SPI_ESP_FAILURE;
                break;
            }
            
            /*
            int len = recv(sock, rx_buffer, sizeof(rx_buffer) - 1, 0);
            // Error occurred during receiving
            if (len < 0) {
                ESP_LOGE(TAG, "recv failed: errno %d", errno);
                break;
            }
            // Data received
            else {
                rx_buffer[len] = 0; // Null-terminate whatever we received and treat like a string
                ESP_LOGI(TAG, "Received %d bytes from %s:", len, host_ip);
                ESP_LOGI(TAG, "%s", rx_buffer);
            }
            */

            //trials++;
            //if(trials >= NUM_OF_TRIALS) break;
            //uint64_t end = esp_timer_get_time();

            //printf("Wifi serialization and tx took %llu microseconds\n", (end - start));


            //vTaskDelay(2000 / portTICK_PERIOD_MS);
            //ESP_LOGI(TAG, "After vtaskdelay");
        }

        /*
        uint64_t end = esp_timer_get_time();

        printf("Serial + Wifi tx: %u iterations took %llu miliseconds (%llu microseconds per invocation)\n",
           NUM_OF_TRIALS, (end - start)/1000, (end - start)/NUM_OF_TRIALS);
        //printf("Wifi serialization and tx took %llu microseconds\n", (end - start));
        */
        while(1);

        if (sock != -1) {
            ESP_LOGE(TAG, "Shutting down socket and restarting...");
            shutdown(sock, 0);
            close(sock);
        }
    }
    // Task self deletes if it gets here in case of error
    vTaskDelete(NULL);
}


// --- Private functions --- //

static void wifi_event_handler(void* arg, esp_event_base_t event_base,
                                    int32_t event_id, void* event_data)
{
    if (event_id == WIFI_EVENT_AP_STACONNECTED) {
        wifi_event_ap_staconnected_t* event = (wifi_event_ap_staconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" join, AID=%d",
                 MAC2STR(event->mac), event->aid);
    } else if (event_id == WIFI_EVENT_AP_STADISCONNECTED) {
        wifi_event_ap_stadisconnected_t* event = (wifi_event_ap_stadisconnected_t*) event_data;
        ESP_LOGI(TAG, "station "MACSTR" leave, AID=%d",
                 MAC2STR(event->mac), event->aid);
    }
}

void wifi_init_softap(void)
{
    ESP_ERROR_CHECK(esp_netif_init());
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    esp_netif_create_default_wifi_ap();

    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));

    ESP_ERROR_CHECK(esp_event_handler_instance_register(WIFI_EVENT,
                                                        ESP_EVENT_ANY_ID,
                                                        &wifi_event_handler,
                                                        NULL,
                                                        NULL));

    wifi_config_t wifi_config = {
        .ap = {
            .ssid = EXAMPLE_ESP_WIFI_SSID,
            .ssid_len = strlen(EXAMPLE_ESP_WIFI_SSID),
            .channel = EXAMPLE_ESP_WIFI_CHANNEL,
            .password = EXAMPLE_ESP_WIFI_PASS,
            .max_connection = EXAMPLE_MAX_STA_CONN,
            .authmode = WIFI_AUTH_WPA_WPA2_PSK
        },
    };
    if (strlen(EXAMPLE_ESP_WIFI_PASS) == 0) {
        wifi_config.ap.authmode = WIFI_AUTH_OPEN;
    }

    ESP_ERROR_CHECK(esp_wifi_set_storage(WIFI_STORAGE_RAM) );
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_AP));
    ESP_ERROR_CHECK(esp_wifi_set_config(WIFI_IF_AP, &wifi_config));
    esp_wifi_set_protocol(WIFI_IF_AP, WIFI_PROTOCOL_11N);
    ESP_ERROR_CHECK(esp_wifi_start());

    ESP_LOGI(TAG, "wifi_init_softap finished. SSID:%s password:%s channel:%d",
             EXAMPLE_ESP_WIFI_SSID, EXAMPLE_ESP_WIFI_PASS, EXAMPLE_ESP_WIFI_CHANNEL);
}

