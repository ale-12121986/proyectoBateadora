#include <stdio.h>
#include "comunicacionWiFi.h"

void func(void)
{

}
// static void disconnect_hanfler(void *arg,esp_event_base_t event_base, int32_t event_id, void *event_data)
// { 
//     stop_webserver(*server);
//     *server = NULL;
// }

// static void connect_handler(void *arg, esp_event_base_t event_base, int32_t event_id, void *event_data)
// {
//     httpd_handle_t *server = (httpd_handle_t *)arg;
//     if (*server == NULL)
//     {
//         *server = start_webserver():
//     }
    
// }

// void configurarWiFi{
//     static httpd_handle_t server = NULL;
//     ESP_ERROR_CHECK(nvs_flash_init());
//     ESP_ERROR_CHECK(esp_netif_init());
//     ESP_ERROR_CHECK(esp_event_loop_create_default());
//     ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &connect_handler, &server));
//     ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, WIFI_EVENT_STA_DISCONNECTED, &disconnect_handler, &server));
//     ESP_ERROR_CHECK(example_connect());
// }