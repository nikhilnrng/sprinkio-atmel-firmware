#ifndef SPRINKIO_H
#define SPRINKIO_H

#include <avr/io.h>
#include <avr/pgmspace.h>
#include <avr/eeprom.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>

#include "helper.h"
#include "atmega328p.h"
#include "structs.h"
#include "usart.h"
#include "i2c.h"
#include "raspberry_pi.h"
#include "rs232.h"
#include "clock.h"
#include "zone_controller.h"
#include "moisture_sensor.h"
#include "temperature_sensor.h"
#include "settings.h"
#include "system.h"

#endif
