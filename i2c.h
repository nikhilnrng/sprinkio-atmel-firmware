#ifndef I2C_H
#define I2C_H

#include "sprinkio.h"

#ifndef FOSC
#define FOSC 7372800 // clock frequency
#endif

#define BDIV ((FOSC / 100000 - 16) / 2 + 1) // I2C BAUD rate

// prototypes
void i2c_init();
uint8_t i2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an, 
               uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn);

#endif