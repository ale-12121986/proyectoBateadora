#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp_adc/adc_oneshot.h"
#include "driver/adc.h"
#include "esp_log.h"

void configPinMedicion(uint8_t pin_4, uint8_t pin_5, uint8_t pin_6);
void configAdc2(void);
float leerTransmisorMedicion(int8_t sensor);
void activarMultiplexor2(int8_t sensor);