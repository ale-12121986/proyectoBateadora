idf_component_register(SRCS "comunicacionWiFi.c"
                    PRIV_REQUIRES mqtt
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_wifi" nvs_flash spiffs 
                    EMBED_TXTFILES ESP32_1.crt ESP32_1.key rootCA.crt
                    )
                    
