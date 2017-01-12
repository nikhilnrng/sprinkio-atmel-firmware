#include "sprinkio.h"

const uint8_t days_per_month [] PROGMEM = {31,28,31,30,31,30,31,31,30,31,30,31};

static uint8_t write_clock(uint8_t *buf, uint8_t *data) {
  return i2c_io(CLOCK_ADDR, buf, 1, data, 1, NULL, 0);
}

static uint8_t read_clock(uint8_t *buf, uint8_t *store) {
  return i2c_io(CLOCK_ADDR, buf, 1, NULL, 0, store, 1);
}

static uint32_t hours_to_seconds(time_t *time) {
  return ((time->hour * 3600) + (time->minute * 60) + time->second);
}

/**
 * Calculate number of days since 01/01/2001
 */
static uint16_t date_to_days(date_t *date) {
  uint8_t i;
  uint16_t days = date->day;
  // calculate days this year
  for (i = 1; i < date->month; i++) {
    days += pgm_read_byte(days_per_month + i - 1);
  }
  // account for leap year this year
  if ((date->month > 2) && (date->year % 4 == 0)) {
    days++;
  }
  // return sum of days this year and previous years
  return days + (365 * date->year) + ((date->year + 3) / 4 - 1);
}

/**
 * Return time valid boolean
 */
bool is_time_valid(time_t *time) {
	return (time->second >= 0 && time->second <= 59 &&
    time->minute >= 0 && time->minute <= 59 &&
    time->hour >= 0 && time->hour <= 23);
}

/**
 * Return date valid boolean
 */
bool is_date_valid(date_t *date) {
  return (date->day >= 1 && date->day <= 31 &&
    date->month >= 1 && date->month <= 12 &&
    date->year >= 0 && date->year <= 99 &&
    date->day <= pgm_read_byte(days_per_month + date->month - 1));
}

/**
 * Return true if t1 is greater than t2
 */
bool compare_time(time_t *t1, time_t *t2) {
  return (hours_to_seconds(t1) > hours_to_seconds(t2));
}

/**
 * Add minutes to t1 and update t2 with new time
 */
void add_minutes(time_t *t1, uint8_t min, time_t *t2) {
  // calculate new time
  uint8_t nminute = (t1->minute + (min % 60)) % 60;
  uint8_t nhour = (t1->hour + (t1->minute + (min % 60)) / 60) % 24;

  // update t2
  t2->second = t1->second; // does not change
  t2->minute = nminute;
  t2->hour = nhour;
}

/**
 * Return day of week in decimal
 */
static uint8_t get_day_of_week(date_t *date) {
  uint16_t day = date_to_days(date);
  return (day + 6) % 7;
}

bool get_time(time_t *time) {
	uint8_t h, m, s;

	// store time registers
  uint8_t hreg = CLOCK_HOUR_REG;
  uint8_t mreg = CLOCK_MINUTE_REG;
  uint8_t sreg = CLOCK_SECOND_REG;

  // read time
  if (read_clock(&hreg, &h)) return false;
  if (read_clock(&mreg, &m)) return false;
  if (read_clock(&sreg, &s)) return false;
  
  // convert time to dec
  time->hour = bcd_to_dec(h);
  time->minute = bcd_to_dec(m);
  time->second = bcd_to_dec(s);

  return true;
}

bool get_date(date_t *date) {
	uint8_t dow, d, m, y;

	// store date registers
  uint8_t dowreg = CLOCK_DAYS_OF_WEEK_REG;
  uint8_t dreg = CLOCK_DAYS_REG;
  uint8_t mreg = CLOCK_MONTHS_REG;
  uint8_t yreg = CLOCK_YEARS_REG;

  // read date
  if (read_clock(&dowreg, &dow)) return false;
  if (read_clock(&dreg, &d)) return false;
  if (read_clock(&mreg, &m)) return false;
  if (read_clock(&yreg, &y)) return false;

  // convert date to dec
  date->day_of_week = bcd_to_dec(dow);
  date->day = bcd_to_dec(d);
  date->month = bcd_to_dec(m);
  date->year = bcd_to_dec(y);

  return true;
}

bool set_time(time_t *time) {
	if (!is_time_valid(time)) return false; 

	// store time registers
  uint8_t hreg = CLOCK_HOUR_REG;
  uint8_t mreg = CLOCK_MINUTE_REG;
  uint8_t sreg = CLOCK_SECOND_REG;

  // convert time to bcd
  uint8_t h = dec_to_bcd(time->hour);
  uint8_t m = dec_to_bcd(time->minute);
  uint8_t s = dec_to_bcd(time->second);
  
  // write time
  if (write_clock(&hreg, &h)) return false;
  if (write_clock(&mreg, &m)) return false;
  if (write_clock(&sreg, &s)) return false;
  
  return true;
}

bool set_date(date_t *date) {
  if (!is_date_valid(date)) return false;

  // store date registers
  uint8_t dowreg = CLOCK_DAYS_OF_WEEK_REG;
  uint8_t dreg = CLOCK_DAYS_REG;
  uint8_t mreg = CLOCK_MONTHS_REG;
  uint8_t yreg = CLOCK_YEARS_REG;

  // convert date to bcd
  uint8_t dow = dec_to_bcd(get_day_of_week(date));
  uint8_t d = dec_to_bcd(date->day);
  uint8_t m = dec_to_bcd(date->month);
  uint8_t y = dec_to_bcd(date->year);
  
  // write date
  if (write_clock(&dowreg, &dow)) return false;
  if (write_clock(&dreg, &d)) return false;
  if (write_clock(&mreg, &m)) return false;
  if (write_clock(&yreg, &y)) return false;

  return true;
}
