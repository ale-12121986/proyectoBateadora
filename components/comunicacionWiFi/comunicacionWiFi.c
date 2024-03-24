#include <stdio.h>
#include "comunicacionWiFi.h"

// Nuevo manejador de eventos usando las nuevas APIs
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG4, "retrying to connect to the AP");
    } 
    // else if (event_base == IP_EVENT && event_id == IP_EVENT_STA_GOT_IP) {
    //     ip_event_got_ip_t* event = (ip_event_got_ip_t*) event_data;
    //     ESP_LOGI(TAG4, "Conectado con éxito!");
    //     ESP_LOGI(TAG4, "Dirección IP asignada: " IPSTR, IP2STR(&event->ip_info.ip));
    // }
}

void wifi_init_sta(void) {
    // Inicializa el almacenamiento no volátil para la configuración del WiFi
    ESP_ERROR_CHECK(nvs_flash_init());
    // Inicializa la pila TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());
    // Registra el manejador de eventos para eventos de WiFi
    ESP_ERROR_CHECK(esp_event_handler_register(WIFI_EVENT, ESP_EVENT_ANY_ID, &wifi_event_handler, NULL));
    // Registra el manejador de eventos para obtener la dirección IP
    ESP_ERROR_CHECK(esp_event_handler_register(IP_EVENT, IP_EVENT_STA_GOT_IP, &wifi_event_handler, NULL));
    // Crea la instancia de netif para la estación WiFi
    esp_netif_create_default_wifi_sta();
    // Inicializa la configuración de WiFi con valores predeterminados
    wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_wifi_init(&cfg));
    // Configura los parámetros de WiFi y los establece
    wifi_config_t wifi_config = {
        .sta = {
            .ssid = WIFI_SSID,
            .password = WIFI_PASS,
            .threshold.authmode = WIFI_AUTH_WPA2_PSK,
        },
    };
    ESP_ERROR_CHECK(esp_wifi_set_mode(WIFI_MODE_STA));
    ESP_ERROR_CHECK(esp_wifi_set_config(ESP_IF_WIFI_STA, &wifi_config));
    // Inicia el WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
}

void configurarWiFi(){
    // Inicializa el subsistema NVS
    ESP_ERROR_CHECK(nvs_flash_init());
    // Inicializa la pila TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());
    // Crea el bucle de eventos predeterminado
    ESP_ERROR_CHECK(esp_event_loop_create_default());
    // Inicializa la configuración de Wi-Fi
    wifi_init_sta();// Inicializa mDNS
    ESP_LOGI(TAG4, "Esta configurando el wifi");
  
}