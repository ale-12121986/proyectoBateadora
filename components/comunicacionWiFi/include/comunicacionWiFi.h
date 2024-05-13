
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "mqtt_client.h"

// #define WIFI_SSID "Telecentro-93e3_0_1"
// #define WIFI_PASS "Alex+18996365"

#define WIFI_SSID "motorola edge 20 pro_8815"
#define WIFI_PASS "Ale-18996365"

#define MQTT_URI "mqtt://tu_broker_mqtt"
#define MQTT_TOPIC "Bateadora/201"

static const char *TAG4 = "comunicacionWiFi";



//void func(void);
void configurarWiFi(void);
void mqttIniciar(void);
void wifi_init_sta(void);
// void enviarData(void);