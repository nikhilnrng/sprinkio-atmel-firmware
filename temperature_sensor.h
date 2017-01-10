#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#include "sprinkio.h"

// void temperature_sensor_init();
void read_temp(uint16_t* temp,uint16_t* humidity,uint8_t* check_sum);
int get_pulse();

#endif