#include <stdio.h>
#include "grupoBateo.h"


uint8_t pin1 = 0;
uint8_t pin2 = 0;
uint8_t pin3 = 0;

float alpha = 0.5;  // Factor de suavizado

uint8_t pos = 0;

const char * TAG ="grupoBateo";
float grupoBateoIzquieroExterno = 0, grupoBateoIzquieroInterno = 0, grupoBateoDerechoInterno = 0, grupoBateoDerechoExterno = 0;
void configPin(uint8_t pin_1, uint8_t pin_2, uint8_t pin_3){
    pin1 = pin_1;
    pin2 = pin_2;
    pin3 = pin_3;

    gpio_reset_pin(pin1);
    gpio_reset_pin(pin2);
    gpio_reset_pin(pin3);

    gpio_set_direction(pin1, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(pin2, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(pin3, GPIO_MODE_DEF_OUTPUT);
    
    ESP_LOGI(TAG,"configPin");
}

void configAdc(void){
    adc1_config_channel_atten(ADC1_CHANNEL_4, ADC_ATTEN_DB_11 );
    adc1_config_width(ADC_WIDTH_BIT_12);
    
}

void activarMultiplexor1(int8_t sensor){
    switch (sensor)
    {
    case 1:
        //ESP_LOGI(TAG,"grupo izquierdo externo");
        gpio_set_level(pin1,0);
        gpio_set_level(pin2,0);
        gpio_set_level(pin3,0);
        break;
    case 2:
        //ESP_LOGI(TAG,"grupo izquierdo interno");
        gpio_set_level(pin1,1);
        gpio_set_level(pin2,0);
        gpio_set_level(pin3,0);
        break;
    case 3:
       // ESP_LOGI(TAG,"grupo derecho interno");
        gpio_set_level(pin1,0);
        gpio_set_level(pin2,1);
        gpio_set_level(pin3,0);
        break;
    case 4:
        //ESP_LOGI(TAG,"grupo derecho externo");
        gpio_set_level(pin1,1);
        gpio_set_level(pin2,1);
        gpio_set_level(pin3,0);
        break;
    default:
        break;
    }
}

float map2(float adc, float in_min, float in_max, float out_min,float out_max){
    return (adc - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

float leerTransmisor(int8_t sensor){
    float salida_filtrada = 0;
    activarMultiplexor1(sensor);
    vTaskDelay(3 / portTICK_PERIOD_MS);
    float valor = adc1_get_raw(ADC1_CHANNEL_4);
    switch (sensor)
    {
    case 1:
        grupoBateoIzquieroExterno = alpha * valor + (1 - alpha) * grupoBateoIzquieroExterno;
        salida_filtrada = map2(grupoBateoIzquieroExterno, 0, 4096, -10, 10);
        break;
    
    case 2:
        grupoBateoIzquieroInterno = alpha * valor + (1 - alpha) * grupoBateoIzquieroInterno;
        salida_filtrada = map2(grupoBateoIzquieroInterno, 0, 4096, -10, 10);
        break;

    case 3:
        grupoBateoDerechoInterno = alpha * valor + (1 - alpha) * grupoBateoDerechoInterno;
        salida_filtrada = map2(grupoBateoDerechoInterno, 0, 4096, -10, 10);
        break;

    case 4:
        grupoBateoDerechoExterno = alpha * valor + (1 - alpha) * grupoBateoDerechoExterno;
        salida_filtrada = map2(grupoBateoDerechoExterno, 0, 4096, -10, 10);
        break;
    default:
        break;
    }
    
    return salida_filtrada;
}

