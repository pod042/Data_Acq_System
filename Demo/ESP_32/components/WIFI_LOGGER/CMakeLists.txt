idf_component_register(SRCS "WIFI_LOGGER.c"
                    INCLUDE_DIRS "include"
                    REQUIRES nvs_flash
                    REQUIRES DATA_PARSER
                    REQUIRES SPI
                    REQUIRES esp_timer)
