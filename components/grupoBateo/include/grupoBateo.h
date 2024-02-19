#include "esp_err.h"
#include "driver/gpio.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
//#include "esp_adc/adc_oneshot.h"
#include "driver/adc.h"
//#include "esp_adc_cal.h"
#include "esp_log.h"

void configPin(uint8_t pin_1, uint8_t pin_2, uint8_t pin_3);
void configAdc(void);
float leerTransmisor(int8_t sensor);
void activarMultiplexor1(int8_t sensor);