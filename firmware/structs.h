#ifndef STRUCTS_H
#define STRUCTS_H

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
} time_t;

typedef struct {
  uint8_t day_of_week; // current day of week
  uint8_t day;
  uint8_t month;
  uint8_t year;
} date_t;

typedef struct {
  uint8_t num; // zone number bit field
  uint8_t days_of_week; // days of week bit field
  uint8_t duration; // decimal duration
  bool moisture_sensor; // moisture sensor available
  time_t start_time; // zone start time
  time_t end_time; // zone end time
} zone_t;

typedef bool mode_t;

typedef struct {
  zone_t zones[8]; // 8 individual zone settings
  mode_t mode; // water saver mode boolean
} settings_t;

typedef struct {
  uint8_t zones; // zone status bit field
  uint8_t zones_updated; // zone status updated bit field
  uint8_t state; // system state
  time_t time; // current time
  date_t date; // current date
  settings_t settings; // system settings
} system_t;

#endif
