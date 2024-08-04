#include <stdio.h>
#include "moduloRele.h"

const char * TAG7 ="grupoBateo";
uint8_t grupo_1;
uint8_t grupo_2;
uint8_t grupo_3; 
uint8_t grupo_4;

void confPinRele(uint8_t grupo1, uint8_t grupo2, uint8_t grupo3, uint8_t grupo4){
    grupo_1 = grupo1;
    grupo_2 = grupo2;
    grupo_3 = grupo3;
    grupo_4 = grupo4;
    gpio_num_t gpio_pins[] = {grupo_1, grupo_2, grupo_3, grupo_4};
    // Configurar cada pin como salida usando gpio_config_t
    for (int i = 0; i < sizeof(gpio_pins)/sizeof(gpio_pins[0]); i++) {
        gpio_config_t pGIOConfig;
        pGIOConfig.pin_bit_mask = (1ULL << gpio_pins[i]);
        pGIOConfig.mode = GPIO_MODE_OUTPUT;
        pGIOConfig.pull_up_en = GPIO_PULLUP_DISABLE;
        pGIOConfig.pull_down_en = GPIO_PULLDOWN_DISABLE;
        pGIOConfig.intr_type = GPIO_INTR_DISABLE;
        
        gpio_config(&pGIOConfig);
    }
}
void activarGrupoBateo(int8_t sensor){
    switch (sensor)
    {
    case 1:
        ESP_LOGI(TAG7,"grupo izquierdo externoc");
        gpio_set_level(grupo_1,1);
        gpio_set_level(grupo_2,0);
        gpio_set_level(grupo_3,0);
        gpio_set_level(grupo_4,0);
        break;
    case 2:
        ESP_LOGI(TAG7,"grupo izquierdo internoC");
        gpio_set_level(grupo_1,0);
        gpio_set_level(grupo_2,1);
        gpio_set_level(grupo_3,0);
        gpio_set_level(grupo_4,0);
        break;
    case 3:
        ESP_LOGI(TAG7,"grupo derecho internoC");
        gpio_set_level(grupo_1,0);
        gpio_set_level(grupo_2,0);
        gpio_set_level(grupo_3,1);
        gpio_set_level(grupo_4,0);
        break;
    case 4:
        ESP_LOGI(TAG7,"grupo derecho externoC");
        gpio_set_level(grupo_1,0);
        gpio_set_level(grupo_2,0);
        gpio_set_level(grupo_3,0);
        gpio_set_level(grupo_4,1);
        break;
    case 5:
        ESP_LOGI(TAG7,"grupo cerrado");
        gpio_set_level(grupo_1,0);
        gpio_set_level(grupo_2,0);
        gpio_set_level(grupo_3,0);
        gpio_set_level(grupo_4,0);
        break;
    default:
        break;
    }
}
