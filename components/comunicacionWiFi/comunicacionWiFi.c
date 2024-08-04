#include <stdio.h>
#include "comunicacionWiFi.h"

const char *TAG4 = "comunicacionWiFi";
esp_mqtt_client_handle_t client;
int msg_id;
int numero;
char mensajeTrabajo[10];
// Nuevo manejador de eventos usando las nuevas APIs
static void wifi_event_handler(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data) {
    if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_START) {
        esp_wifi_connect();
    } else if (event_base == WIFI_EVENT && event_id == WIFI_EVENT_STA_DISCONNECTED) {
        esp_wifi_connect();
        ESP_LOGI(TAG4, "retrying to connect to the AP");
    } 
}

// Función para cargar los certificados desde SPIFFS

static void procesarMensaje(char *data, int data_len){

    char mensaje[data_len +1];
    strncpy(mensaje,data,data_len);
    mensaje[data_len] = '\0'; //
    char *delimitador = strchr(mensaje,'/');

    //Encuentra el delimitador
    if(delimitador == NULL){
        ESP_LOGI(TAG4, "No se ha encontrado el delimitador /");
        return;
    }

    // Separa la cadena en 2 partes
    int id_trabajo_len = delimitador - mensaje;
    char id_trabajo[id_trabajo_len + 1];
    strncpy(id_trabajo,mensaje,id_trabajo_len);
    id_trabajo[id_trabajo_len] = '\0';

    //La parte numerica despues del delimitador
    char *numPart = delimitador + 1;
    numero = atoi(numPart);

    //Imprime los resultados
    ESP_LOGI(TAG4, "Mensaje recibido: %s", id_trabajo);
    ESP_LOGI(TAG4, "Numero: %d", numero);
}

void wifi_init_sta(void) {
    // Inicializa el almacenamiento no volátil para la configuración del WiFi
    ESP_ERROR_CHECK(nvs_flash_init());
    // Inicializa la pila TCP/IP
    ESP_ERROR_CHECK(esp_netif_init());
    // Crea el bucle de eventos predeterminado
    ESP_ERROR_CHECK(esp_event_loop_create_default());
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
    
}

void configurarWiFi(){
    // Inicia el WiFi
    ESP_ERROR_CHECK(esp_wifi_start());
    ESP_LOGI(TAG4, "Esta configurando el wifi");
  
}
static void log_error_if_nonzero(const char *message, int error_code)
{
    if (error_code != 0) {
        ESP_LOGE(TAG4, "Last error %s: 0x%x", message, error_code);
    }
}

/*
 * @brief Event handler registered to receive MQTT events
 *
 *  This function is called by the MQTT client event loop.
 *
 * @param handler_args user data registered to the event.
 * @param base Event base for the handler(always MQTT Base in this example).
 * @param event_id The id for the received event.
 * @param event_data The data for the event, esp_mqtt_event_handle_t.
 */
static void mqtt_event_handler(void *handler_args, esp_event_base_t base, int32_t event_id, void *event_data)
{
    ESP_LOGD(TAG4, "Event dispatched from event loop base=%s, event_id=%ld", base, event_id);
    esp_mqtt_event_handle_t event = event_data;
    esp_mqtt_client_handle_t client = event->client;
    
    switch ((esp_mqtt_event_id_t)event_id) {
    case MQTT_EVENT_CONNECTED:
        ESP_LOGI(TAG4, "MQTT_EVENT_CONNECTED");
        msg_id = esp_mqtt_client_subscribe(client, MQTT_TOPIC, 0);
        ESP_LOGI(TAG4, "sent subscribe successful, msg_id=%d", msg_id);
        
        break;
    case MQTT_EVENT_DISCONNECTED:
        ESP_LOGI(TAG4, "MQTT_EVENT_DISCONNECTED");
        esp_mqtt_client_reconnect(client);
        break;

    case MQTT_EVENT_SUBSCRIBED:
        ESP_LOGI(TAG4, "MQTT_EVENT_SUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_UNSUBSCRIBED:
        ESP_LOGI(TAG4, "MQTT_EVENT_UNSUBSCRIBED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_PUBLISHED:
        ESP_LOGI(TAG4, "MQTT_EVENT_PUBLISHED, msg_id=%d", event->msg_id);
        break;
    case MQTT_EVENT_DATA:
        ESP_LOGI(TAG4, "MQTT_EVENT_DATA");
        printf("TOPIC=%.*s\r\n", event->topic_len, event->topic);
        printf("DATA=%.*s\r\n", event->data_len, event->data);
        procesarMensaje(event->data, event->data_len);
        break;
    case MQTT_EVENT_ERROR:
        ESP_LOGI(TAG4, "MQTT_EVENT_ERROR");
        if (event->error_handle->error_type == MQTT_ERROR_TYPE_TCP_TRANSPORT) {
            log_error_if_nonzero("reported from esp-tls", event->error_handle->esp_tls_last_esp_err);
            log_error_if_nonzero("reported from tls stack", event->error_handle->esp_tls_stack_err);
            log_error_if_nonzero("captured as transport's socket errno",  event->error_handle->esp_transport_sock_errno);
            ESP_LOGI(TAG4, "Last errno string (%s)", strerror(event->error_handle->esp_transport_sock_errno));

        }
        break;
    default:
        ESP_LOGI(TAG4, "Other event id:%d", event->event_id);
        break;
    }
}

static void mqtt_app_start(void){
        
    esp_mqtt_client_config_t mqtt_cfg = {
        .broker.address.uri = MQTT_URI,
        .credentials.username = "Bateadora201",      //BateadorasVyO     Bateadora201
        // .credentials.authentication.password = "Trenes_Argentinos",
        .session.keepalive = MQTT_KEEPALIVE_INTERVAL,
        .broker.verification.certificate = (const char *)server_cert_pem_start,
        .credentials.authentication.certificate = (const char *)client_cert_pem_start,
        .credentials.authentication.key = (const char *)client_key_pem_start,
    };
    client = esp_mqtt_client_init(&mqtt_cfg);
    esp_mqtt_client_register_event(client, ESP_EVENT_ANY_ID, mqtt_event_handler, NULL);
    ESP_LOGI(TAG4, "[APP] Free memory: %ld bytes", esp_get_free_heap_size());
    
    esp_mqtt_client_start(client);
}

void mqttIniciar(void){
    ESP_LOGI(TAG4, "[APP] Startup..");
    ESP_LOGI(TAG4, "[APP] Free memory: %ld bytes", esp_get_free_heap_size());
    ESP_LOGI(TAG4, "[APP] IDF version: %s", esp_get_idf_version());

    esp_log_level_set("*", ESP_LOG_INFO);
    esp_log_level_set("MQTT_CLIENT", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT_BASE", ESP_LOG_VERBOSE);
    esp_log_level_set("TRANSPORT", ESP_LOG_VERBOSE);
    esp_log_level_set("OUTBOX", ESP_LOG_VERBOSE);
    mqtt_app_start();        
}
void buscarIdTrabajo(){
    msg_id = esp_mqtt_client_publish(client, MQTT_TOPIC, "trabajo", 0, 1, 0);
    ESP_LOGI(TAG4, "sent publish successful, msg_id=%d", msg_id);
}
void enviarData(float alineacion, float  peralte, float nivel_izquierdo, float nivel_derecho, int distancia, int idtrabajo2, int tipoMedicion){
    char payload[300]; // Increased buffer size for additional data
    idtrabajo2 = numero;
    snprintf(payload, sizeof(payload), "{\"alineacion\":%.2f,\"peralte\":%.2f,\"nivel_izquierdo\":%.2f,\"nivel_derecho\":%.2f,\"distancia\":%d,\"idtrabajo2\":%d,\"tipoMedicion\":\"%d\"}", alineacion, peralte, nivel_izquierdo, nivel_derecho, distancia, idtrabajo2, tipoMedicion);
    esp_mqtt_client_publish(client, "Bateadora", payload, 0, 1, 0);
    ESP_LOGI(TAG4, "sent publish successful, msg_id=%d", msg_id);
    ESP_LOGI(TAG4, "el valor que se envio %s", payload);
}
