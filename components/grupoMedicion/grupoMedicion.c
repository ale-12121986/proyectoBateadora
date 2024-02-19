#include <stdio.h>
#include "grupoMedicion.h"

uint8_t pin4 = 0;
uint8_t pin5 = 0;
uint8_t pin6 = 0;

float alpha2 = 0.1;  // Factor de suavizado
float alineacion = 0, peralte = 0, nivelIzquierdo = 0, nivelDerecho = 0;

const char * TAG2 ="grupoMedicion";

void configPinMedicion(uint8_t pin_4, uint8_t pin_5, uint8_t pin_6){
    pin4 = pin_4;
    pin5 = pin_5;
    pin6 = pin_6;

    gpio_reset_pin(pin4);
    gpio_reset_pin(pin5);
    gpio_reset_pin(pin6);

    gpio_set_direction(pin4, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(pin5, GPIO_MODE_DEF_OUTPUT);
    gpio_set_direction(pin6, GPIO_MODE_DEF_OUTPUT);
    
    //ESP_LOGI(TAG,"configPin");
}

void configAdc2(void){
    adc1_config_channel_atten(ADC1_CHANNEL_7, ADC_ATTEN_DB_11);
    adc1_config_width(ADC_WIDTH_BIT_12);
    
}

void activarMultiplexor2(int8_t sensor){
    switch (sensor)
    {
    case 1:
        //ESP_LOGI(TAG2,"Aineacion");
        gpio_set_level(pin4,0);
        gpio_set_level(pin5,0);
        gpio_set_level(pin6,0);
        break;
    case 2:
        //ESP_LOGI(TAG2,"Peralte");
        gpio_set_level(pin4,1);
        gpio_set_level(pin5,0);
        gpio_set_level(pin6,0);
        break;
    case 3:
        //ESP_LOGI(TAG2,"Nivel Izquierdo");
        gpio_set_level(pin4,0);
        gpio_set_level(pin5,1);
        gpio_set_level(pin6,0);
        break;
    case 4:
        //ESP_LOGI(TAG2,"nivel Derecho");
        gpio_set_level(pin4,1);
        gpio_set_level(pin5,1);
        gpio_set_level(pin6,0);
        break;
    default:
        break;
    }
}

float leerTransmisorMedicion(int8_t sensor){
    float salida_filtrada = 0;
    activarMultiplexor2(sensor);
    vTaskDelay(3 / portTICK_PERIOD_MS);
    float valor = adc1_get_raw(ADC1_CHANNEL_7);
    switch (sensor)
    {
    case 1:
        //alineacion = alpha2 * valor + (1 - alpha2) * alineacion;
        alineacion = valor;
        salida_filtrada = alineacion;
        break;
    
    case 2:
        //peralte = alpha2 * valor + (1 - alpha2) * peralte;
        peralte = valor;
        salida_filtrada = peralte;  
        break;

    case 3:
        //nivelIzquierdo = alpha2 * valor + (1 - alpha2) * nivelIzquierdo;
        nivelIzquierdo = valor;
        salida_filtrada = nivelIzquierdo;  
        break;

    case 4:
        //nivelDerecho = alpha2 * valor + (1 - alpha2) * nivelDerecho;
        nivelDerecho = valor;
        salida_filtrada = nivelDerecho;
        break;
    default:
        break;
    }
    

    return salida_filtrada;
}