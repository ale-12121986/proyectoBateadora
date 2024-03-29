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
int intervalo = 50;
int timerId = 1;
uint8_t pot = 0, distanciaRecorrida = 0;
uint32_t distanRecorridaFinal = 0;
uint8_t estado = 0;
uint8_t valorP = 0;
uint8_t ledLevel = 0;
char valor[20];
static QueueHandle_t uart_queue;

    
void sendMessage(float valorPot, int sensor)
{
    char cadena [30];
    char buffer[30];
    snprintf(buffer, sizeof(buffer), "%4.1f", valorPot);
    //sprintf(buffer, "%4.1f", valorPot);
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
                // ESP_LOGI(TAG3,"Se envio el dato:%s", data);
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
    //uart_set_pin(uart_num, 1, 3, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE); //configurar para tx0 y rx0
    uart_set_pin(uart_num, pin_tx, pin_rx,UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    uart_driver_install(uart_num, BUFE_SIZE, BUFE_SIZE, 5, &uart_queue, 0);
    xTaskCreate(uart_task, "uart_task", TASK_MEMORY, NULL, 5, NULL);

    // ESP_LOGI(TAG,"inicio de comunicacion serial completa");
}


void vTimerCallback(TimerHandle_t pxTimer)
{

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
        distanciaRecorrida = 0;
        distanRecorridaFinal++;
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

void app_main(void)
{
    initIrs();
    init_uart();
    configPin(pin_1, pin_2, pin_3);
    configPinMedicion(pin_4, pin_5, pin_6);
    configAdc();
    configAdc2();
    // setTimer();
    configurarWiFi();
    gpio_reset_pin(led);
    gpio_set_direction(led, GPIO_MODE_DEF_OUTPUT); 
    // gpio_reset_pin(pulso);
    //gpio_set_direction(pulso, GPIO_MODE_DEF_INPUT);
    void configurar();
    while (1)
    {
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
        }    
        vTaskDelay(50 / portTICK_PERIOD_MS);
    }
}
