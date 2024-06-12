idf_component_register(SRCS "comunicacionWiFi.c"
                    PRIV_REQUIRES mqtt
                    INCLUDE_DIRS "include"
                    REQUIRES "esp_wifi" nvs_flash
                    
                    )
                     

