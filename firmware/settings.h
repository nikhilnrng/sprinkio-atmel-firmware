#ifndef SETTINGS_H
#define SETTINGS_H

#include "sprinkio.h"

// setup command modes
#define CMD_SETUP_TIME 0x01 // enter setup time mode
#define CMD_SETUP_DATE 0x02 // enter setup date mode
#define CMD_SETUP_ZONE 0x03 // enter setup zone mode
#define CMD_SETUP_WATER_MODE 0x04 // enter setup water mode
#define CMD_SETUP_START 0x05 // enter setup mode started
#define CMD_SETUP_END 0x06 // setup mode ended
#define CMD_SETUP_ERROR 0x07 // unrecognized command

// setup time commands
#define CMD_TIME_SECOND 0x11 // receive seconds
#define CMD_TIME_MINUTE 0x12 // receive minutes
#define CMD_TIME_HOUR 0x13 // receive hours
#define CMD_TIME_START 0x14 // setup time started
#define CMD_TIME_END 0x15 // setup time ended
#define CMD_TIME_VALID 0x16 // time valid
#define CMD_TIME_INVALID 0x17 // time invalid
#define CMD_TIME_ERROR 0x18 // unrecognized command

// setup date commands
#define CMD_DATE_DAY 0x21 // receive day
#define CMD_DATE_MONTH 0x22 // receive month
#define CMD_DATE_YEAR 0x23 // receive year
#define CMD_DATE_START 0x24 // setup date started
#define CMD_DATE_END 0x25 // setup date ended
#define CMD_DATE_VALID 0x26 // date valid
#define CMD_DATE_INVALID 0x27 // date invalid
#define CMD_DATE_ERROR 0x28 // uncrecognized command

// setup zone commands
#define CMD_ZONE_NUM 0x31 // receive zone number
#define CMD_ZONE_DAYS_OF_WEEK 0x32 // receive watering days
#define CMD_ZONE_DURATION 0x33 // receive watering duration
#define CMD_ZONE_MOISTURE_SENSOR 0x34 // receive moisture sensor boolean
#define CMD_ZONE_TIME 0x35 // receive watering start time
#define CMD_ZONE_START 0x36 // setup zone started
#define CMD_ZONE_END 0x37 // setup zone ended
#define CMD_ZONE_VALID 0x38 // zone valid
#define CMD_ZONE_INVALID 0x39 // zone invalid
#define CMD_ZONE_ERROR 0x40 // unrecognized command

// setup mode commands
#define CMD_MODE_WATER_SAVE 0x41 // receive water save mode boolean
#define CMD_MODE_START 0x42 // setup mode started
#define CMD_MODE_END 0x43 // setup mode ended
#define CMD_MODE_VALID 0x44 // mode valid
#define CMD_MODE_INVALID 0x45 // mode invalid
#define CMD_MODE_ERROR 0x46 // unrecognized command

// prototypes
void settings_init();
void run_setup_loop();
bool setup_time(time_t *time);
bool setup_date(date_t *date);
bool setup_zone(zone_t *zone);
bool setup_mode(mode_t *mode);
bool is_mode_valid(mode_t* mode); // TODO: REMOVE
void configure_mode(); // TODO: REMOVE
void setup_write(uint8_t data); // TODO: make static
uint8_t setup_read(); // TODO: make static

#endif
