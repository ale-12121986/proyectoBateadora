
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"

#define WIFI_SSID "Telecentro-93e3_0_1"
#define WIFI_PASS "Alex+18996365"


static const char *TAG4 = "wifi_station";
//void func(void);
void configurarWiFi(void);
