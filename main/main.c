#include <stdio.h>
#include <string.h>
#include "grupoBateo.h"
#include "grupoMedicion.h"
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
#define pin_4 18
#define pin_5 19
#define pin_6 5
#define pulso 22

#define pin_tx GPIO_NUM_17
#define pin_rx GPIO_NUM_16
#define uart_num UART_NUM_2
#define BUFE_SIZE 1024 * 4
#define TASK_MEMORY 1024 *3 
#define TSTACK_SIZE 2500
const char *TAG3 = "main";

TimerHandle_t xTimers;
int intervalo = 150;
int timerId = 1;
uint8_t pot = 0;
float distanciaRecorrida = 0.00, distanRecorridaFinal = 0.00;
uint8_t estado = 0;
char valor[10];
static QueueHandle_t uart_queue;

void sendMessage(float valorPot, int sensor, char *comando)
{
    char *cadena = (char *)malloc(30);
    char *buffer = (char *)malloc(30);
    sprintf(buffer, "%4.1f", valorPot);
    if (strcmp(valor, "gb") == 0)
    {
        switch (sensor)
        {
        case 1:
            strcpy(cadena, "page2.NmGrIzEx.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 2:
            strcpy(cadena, "page2.NmGrIzIn.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 3:
            strcpy(cadena, "page2.NmGrDeIn.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 4:
            strcpy(cadena, "page2.NmGrDeEx.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
        default:
            break;
        }
    }
    if (strcmp(valor, "gm") == 0)
    {
        switch (sensor)
        {
        case 1:
            strcpy(cadena, "page1.NmAlineacion.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 2:
            strcpy(cadena, "page1.NmPeralte.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 3:
            strcpy(cadena, "page1.NmNivelIz.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 4:
            strcpy(cadena, "page1.NmNivelDe.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
            break;

        case 10:
            strcpy(cadena, "page1.NmDistancia.txt=\""); // copia las cadenas de caracteres
            strcat(cadena, buffer);
            strcat(cadena, "\"\xFF\xFF\xFF");
            uart_write_bytes(uart_num, cadena, strlen(cadena));
        default:
            break;
        }
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
                // ESP_LOGI(TAG3,"Se envio el dato:%s", data);
                break;

            default:
                break;
            }
        }
    }
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

void taskDistancia(void *pvParameters)
{
    gpio_reset_pin(pulso);
    gpio_set_direction(pulso, GPIO_MODE_DEF_INPUT);
    int valorP = gpio_get_level(pulso);
    while (1)
    {
        valorP = gpio_get_level(pulso);
        if (estado ==1)
        {
            if (valorP == 0)
            {
                while (valorP == 0)
                {
                    valorP = gpio_get_level(pulso);
                }
                distanciaRecorrida++;
                if (distanciaRecorrida == 15)
                {
                distanciaRecorrida = 0;
                distanRecorridaFinal++;
                sendMessage(distanRecorridaFinal, 10, valor);
                }
            }
        
         }
        vTaskDelay(10);
    }
}

void vTimerCallback(TimerHandle_t pxTimer)
{
    if (strcmp(valor, "gb") == 0)
        {
            pot += 1;
            if (pot == 5)
            {
                pot = 1;
            }
            // vTaskDelay(50 / portTICK_PERIOD_MS);
            sendMessage(leerTransmisor(pot), pot, valor);
        }
        if (strcmp(valor, "gm") == 0)
        {
            pot += 1;
            estado = 1;
            if (pot == 5)
            {
                pot = 1;
            }
            // vTaskDelay(50 / portTICK_PERIOD_MS);
            sendMessage(leerTransmisorMedicion(pot), pot, valor);
        }
}
// void isr_handler(void *args){
//     //if (strcmp(valor, "gm") == 0)
//         //{
//         distanciaRecorrida++;
//         if (distanciaRecorrida == 15)
//         {
//             distanciaRecorrida = 0;
//             distanRecorridaFinal++;
//             sendMessage(distanRecorridaFinal, 10, valor);
//         }
//         //sendMessage(distanRecorridaFinal, 10, valor);
//   //  }
//         //vTaskDelay(10);
//         // sendMessage(distanciaRecorrida, 10, valor);
// }
        
// void initIrs(){
//     gpio_config_t pGPIOConfig;
//     pGPIOConfig.pin_bit_mask = (1ULL << pulso);
//     pGPIOConfig.mode = GPIO_MODE_INPUT;
//     pGPIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
//     pGPIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
//     pGPIOConfig.intr_type = GPIO_INTR_NEGEDGE;
//     gpio_config(&pGPIOConfig);
//     gpio_install_isr_service(0);
//     gpio_isr_handler_add(pulso, isr_handler, NULL);
//     ESP_LOGI(TAG3,"Se configuro interrupcion");
// }
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

void crearTask()
{
    static uint8_t ucParameterToPass;
    TaskHandle_t xHandle = NULL;
    xTaskCreate(taskDistancia,
                "taskDistancia",
                TSTACK_SIZE,
                &ucParameterToPass,
                2,
                &xHandle);
}

void app_main(void)
{
    
    init_uart();
    crearTask();
    configPin(pin_1, pin_2, pin_3);
    configPinMedicion(pin_4, pin_5, pin_6);
    configAdc();
    configAdc2();
    setTimer();
    //initIrs(); 
    //while (1)
    //{

    //}
}
