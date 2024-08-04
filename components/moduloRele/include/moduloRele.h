#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/adc.h"
#include "esp_log.h"



void confPinRele(uint8_t grupo1, uint8_t grupo2, uint8_t grupo3, uint8_t grupo4);
void activarGrupoBateo(int8_t sensor);
