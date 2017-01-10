#ifndef CLOCK_H
#define CLOCK_H

#include "sprinkio.h"

#define CLOCK_ADDR 0xD0

#define SUNDAY BIT(0)
#define MONDAY BIT(1)
#define TUESDAY BIT(2)
#define WEDNESDAY BIT(3)
#define THURSDAY BIT(4)
#define FRIDAY BIT(5)
#define SATURDAY BIT(6)

#define CLOCK_SECOND_REG 0x00
#define CLOCK_MINUTE_REG 0x01
#define CLOCK_HOUR_REG 0x02
#define CLOCK_DAYS_OF_WEEK_REG 0x03
#define CLOCK_DAYS_REG 0x04
#define CLOCK_MONTHS_REG 0x05
#define CLOCK_YEARS_REG 0x06

bool is_time_valid(time_t *time);
bool is_date_valid(date_t *date);
bool compare_time(time_t *t1, time_t *t2);
void add_minutes(time_t *t1, uint8_t min, time_t *t2);
bool get_time(time_t *time);
bool get_date(date_t *date);
bool set_time(time_t *time);
bool set_date(date_t *date);

#endif
