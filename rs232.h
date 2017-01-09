#ifndef RS232_H
#define RS232_H

#include "sprinkio.h"

void write_rs232(uint8_t data);
void write_rs232_s(const char *s);
void write_rs232_byte(uint8_t b);
void write_rs232_int(uint8_t num);
void write_rs232_time(time_t *t);
void write_rs232_date(date_t *d);
void write_rs232_zone(zone_t *z);
void write_rs232_settings(settings_t *s);

#endif
