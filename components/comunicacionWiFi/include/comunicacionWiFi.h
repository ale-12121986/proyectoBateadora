#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "nvs_flash.h"
#include "esp_log.h"
#include "esp_netif.h"
#include "mqtt_client.h"


//  // En segundos
// #define WIFI_SSID "Telecentro-93e3_0_1"
// #define WIFI_PASS "Alex+18996365"

#define WIFI_SSID "motorola edge 20 pro_8815"
#define WIFI_PASS "Ale-18996365"

#define MQTT_URI "mqtts://192.168.243.8:8883/"     //54.232.196.184   192.168.2.4 
#define MQTT_TOPIC "Bateadora/201"//"Bateadora/201"
#define MQTT_KEEPALIVE_INTERVAL  60 
                                                 

extern const uint8_t client_cert_pem_start[] asm("_binary_client_crt_start");
extern const uint8_t client_cert_pem_end[] asm("_binary_client_crt_end");
extern const uint8_t client_key_pem_start[] asm("_binary_client_key_start");
extern const uint8_t client_key_pem_end[] asm("_binary_client_key_end");
extern const uint8_t server_cert_pem_start[] asm("_binary_ca_crt_start");
extern const uint8_t server_cert_pem_end[] asm("_binary_ca_crt_end");


//void func(void);
void configurarWiFi(void);
void mqttIniciar(void);
void wifi_init_sta(void);
void buscarIdTrabajo(void); 
void enviarData(float alineacion, float  peralte, float nivel_izquierdo, float nivel_derecho, int distancia, int idtrabajo2, int tipoMedicion);