#include <stdio.h>
#include <string.h>
#include "grupoBateo.h"
#include "grupoMedicion.h"
#include "comunicacionWiFi.h"
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "freertos/task.h"
#include "driver/uart.h"
#include "freertos/queue.h"
#include "esp_log.h"
#include "freertos/timers.h"

#define pin_1 12
#define pin_2 14
#define pin_3 27
#define pin_4 19
#define pin_5 18
#define pin_6 5
#define pulso 22
#define led 2

#define pin_tx GPIO_NUM_17
#define pin_rx GPIO_NUM_16
#define uart_num UART_NUM_2     //UART_NUM_2  UART_NUM_0
#define BUFE_SIZE 1024 * 2
#define TASK_MEMORY 1024 *3 
#define TSTACK_SIZE 2500
const char *TAG3 = "main";

TimerHandle_t xTimers;
bool comunicacion = false;
int intervalo = 50;
int timerId = 1;
uint32_t distanRecorridaFinal = 0;
uint8_t estado = 0, valorP = 0, ledLevel = 0, pot = 0, distanciaRecorrida = 0;
float alineacion2 = 0,  peralte2 = 0, nivelIzquierdo2 = 0, nivelDerecho2 = 0;
int16_t distancia2 = 0, idtrabajo2 = 0, tipoMedicion2 = 0;
char valor[20];
static QueueHandle_t uart_queue;

void sendMessage(float valorPot, int sensor){
    char cadena [30];
    char buffer[30];
    snprintf(buffer, sizeof(buffer),"%4.1f", valorPot);
    
    if (strcmp(valor, "gb") == 0)
    {
        switch (sensor)
        {
        case 1:
            strcpy(cadena, "page2.NmGrIzEx.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            break;

        case 2:
            strcpy(cadena, "page2.NmGrIzIn.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            break;

        case 3:
            strcpy(cadena, "page2.NmGrDeIn.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            break;

        case 4:
            strcpy(cadena, "page2.NmGrDeEx.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
        default:
            break;
        }
        uart_write_bytes(uart_num, cadena, strlen(cadena));
    }
    if (strcmp(valor, "gm") == 0)
    {
        switch (sensor)
        {
        case 1:
            strcpy(cadena, "page1.NmAlineacion.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "mm\"\xFF\xFF\xFF");
            //ESP_LOGI(TAG3,"alineacion:%s",cadena);
            break;

        case 2:
            strcpy(cadena, "page1.NmPeralte.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "mm\"\xFF\xFF\xFF");
            break;

        case 3:
            strcpy(cadena, "page1.NmNivelIz.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "mm\"\xFF\xFF\xFF");
            break;

        case 4:
            
            strcpy(cadena, "page1.NmNivelDe.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "mm\"\xFF\xFF\xFF");
            break;

        case 10:
            
            strcpy(cadena, "page1.NmDistancia.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "m\"\xFF\xFF\xFF");
        default:
            break;
        }
        uart_write_bytes(uart_num, cadena, strlen(cadena));
    }
    
}

static void uart_task(void *pvParameters)
{
    uart_event_t event;
    char *data = (char *)malloc(BUFE_SIZE);

    while (1)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY))
        {
            bzero(data, BUFE_SIZE);
            switch (event.type)
            {
            case UART_DATA:
                uart_read_bytes(uart_num, data, event.size, pdMS_TO_TICKS(100));
                // uart_write_bytes(uart_num,(const char *)data,event.size);
                // printf("numero de bits de transmicion: %d\n\n", event.size);
                uart_flush(uart_num);
                strcpy(valor, data);
                //free(data);
                // data = NULL;
                ESP_LOGI(TAG3,"Se recibio el dato:%s", data);
                break;
            case UART_BUFFER_FULL:
                ESP_LOGI(TAG3, "ring buffer full");
                // If buffer full happened, you should consider increasing your buffer size
                // As an example, we directly flush the rx buffer here in order to read more data.
                uart_flush_input(uart_num);
                xQueueReset(uart_queue);
                break;
            case UART_BREAK:
                ESP_LOGI(TAG3, "uart rx break");
                break;
            //Event of UART parity check error
            case UART_PARITY_ERR:
                ESP_LOGI(TAG3, "uart parity error");
                break;
            //Event of UART frame error
            case UART_FRAME_ERR:
                ESP_LOGI(TAG3, "uart frame error");
                break;
            //UART_PATTERN_DET
            //Event of UART RX break detected
            default:
                break;
            }
        }
    }
    free(data);
    data = NULL;
    vTaskDelete(NULL);
}

static void init_uart(void)
{
    uart_config_t uart_config = {
        .baud_rate = 9600, // 115200
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        ////.rx_flow_ctrl_thresh = ,
        .source_clk = UART_SCLK_APB,
    };
    uart_param_config(uart_num, &uart_config);
    // uart_set_pin(uart_num, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); //configurar para tx0 y rx0
    uart_set_pin(uart_num, pin_tx, pin_rx,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, BUFE_SIZE, BUFE_SIZE, 5, &uart_queue, 0);
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);
    // ESP_LOGI(TAG,"inicio de comunicacion serial completa");
}

void vTimerCallback(TimerHandle_t pxTimer)
{
    // void mqttIniciar(void);
}
        
void setTimer()
{
    xTimers = xTimerCreate("Timer",                    // Just a text name, not used by the kernel.
                              (pdMS_TO_TICKS(intervalo)), // The timer period in ticks.
                              pdTRUE,                     // The timers will auto-reload themselves when they expire.
                              (void *)timerId,            // Assign each timer a unique id equal to its array index.
                              vTimerCallback              // Each timer calls the same callback when it expires.
    );

    if (xTimers == NULL)
    {
        // The timer was not created.
    }
    else
    {

        if (xTimerStart(xTimers, 0) != pdPASS)
        {
            // The timer could not be set into the Active state.
        }
    }
}
void isr_handler(void *args)
{
    distanciaRecorrida++;
    if (distanciaRecorrida == 15)
    {
        
        comunicacion = true;
        gpio_set_level(led,1);
        distanciaRecorrida = 0;
        distanRecorridaFinal++;
        distancia2 = distanRecorridaFinal;
    }
}

void initIrs(void){
    gpio_config_t pGIOConfig;
    pGIOConfig.pin_bit_mask = (1ULL << pulso);       /*!< GPIO pin: set with bit mask, each bit maps to a GPIO */
    pGIOConfig.mode = GPIO_MODE_INPUT;               /*!< GPIO mode: set input/output mode                     */
    pGIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;     /*!< GPIO pull-up                                         */
    pGIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE; /*!< GPIO pull-down                                       */
    pGIOConfig.intr_type = GPIO_INTR_NEGEDGE; 

    gpio_config(&pGIOConfig);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(pulso, isr_handler, NULL);
    //ESP_LOGI(TAG3,"Se configuro la interrupcion");
}

void eraser(){
    strcpy(valor, "gm");
    ESP_LOGI(TAG3,"valor es: %s", valor);
}
void app_main(void)
{
    initIrs();
    init_uart();
    configPin(pin_1, pin_2, pin_3);
    configPinMedicion(pin_4, pin_5, pin_6);
    configAdc();
    configAdc2();
    // setTimer();
    wifi_init_sta();
    configurarWiFi();
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_DEF_OUTPUT); 
    
    while (1)
    {
        if (strcmp(valor, "R1") == 0)
        {
            tipoMedicion2 = 1;
            eraser();
            ESP_LOGI(TAG3,"Se recibio R1 %d", tipoMedicion2);
        }
        if (strcmp(valor, "R2") == 0)
        {
            tipoMedicion2 = 2;
            eraser();
            ESP_LOGI(TAG3,"Se recibio R2 %d", tipoMedicion2);
        }
        if (strcmp(valor, "R3") == 0)
        {
            tipoMedicion2 = 3;
            eraser();
            ESP_LOGI(TAG3,"Se recibio R3 %d", tipoMedicion2);
        }
        
        if(strcmp(valor, "cn") == 0){
            char dato[10];
            char cadena[30];
            ESP_LOGI(TAG3, "Entro a configurar WiFi y MQTT");
            mqttIniciar();   
            // bzero(valor, sizeof(valor));
            
            strcpy(valor, "");
            strcpy(dato, "Conectado");
            strcpy(cadena, "page0.BtnConectar.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, dato);
            strcat(cadena, "\"\xFF\xFF\xFF");
            buscarIdTrabajo();
            uart_write_bytes(uart_num,cadena, strlen(cadena));
        }
        if (comunicacion == true)
        {
            comunicacion = false;
            // idtrabajo2 = 16;
            enviarData( alineacion2, peralte2, nivelIzquierdo2, nivelDerecho2, distancia2, idtrabajo2, tipoMedicion2);
            gpio_set_level(led,0);
            ESP_LOGI(TAG3, "envia mensaje de valor de recorrido");
        }
        
        if (strcmp(valor, "gb") == 0)
        {
            estado = 0;
            pot += 1;
            if (pot == 5)
            {
                pot = 1;
            }
            sendMessage(leerTransmisor(pot), pot);
        }
        if (strcmp(valor, "gm") == 0)
        {
            pot += 1;
            estado = 1;
            if (pot == 5)
            {
                pot = 1;
                sendMessage((float)distanRecorridaFinal, 10);    
            }    
            sendMessage(leerTransmisorMedicion(pot), pot);
            switch (pot)
            {
            case 1:
                alineacion2 = leerTransmisorMedicion(pot);
                break;
            case 2:
                peralte2 = leerTransmisorMedicion(pot);
                break;
            case 3:
                nivelIzquierdo2 = leerTransmisorMedicion(pot);
                break;
            case 4:
                nivelDerecho2 = leerTransmisorMedicion(pot);
                break;        
            default:
                break;
            }
        }    
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
