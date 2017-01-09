#include "sprinkio.h"

extern system_t sys;
extern settings_t settings;

/**
 * Return true if days of week contains valid days
 */
static bool is_days_of_week_valid(uint8_t dow) {
  return dow > 0; // true for X0000001 to X1111111, X don't care
}

static bool is_zone_num_valid(int8_t zone) {
  return is_power_of_two(zone);
}

/**
 * Return true if current day is set in provided day of week bit field
 */
static bool is_day_of_week_set(uint8_t dow) {
  return (BIT_IS_SET(BIT(sys.date.day_of_week), dow));
}

/**
 * Return true if current time falls between the start and end time
 */
static bool is_time_set(time_t *start, time_t *end) {
  return (compare_time(&sys.time, start) &&
    compare_time(end, &sys.time));
}

/**
 * Return true if received zone information is valid
 */
static bool is_received_zone_valid(zone_t *zone) {
  write_rs232_s("Day of week: ");
  write_rs232_byte(zone->days_of_week);
  write_rs232_s("\r\n");
  write_rs232_s("Zone number: ");
  write_rs232_byte(zone->num);
  write_rs232_s("\r\n");
  write_rs232_s("Zone start time: ");
  write_rs232_time(&zone->start_time);
  write_rs232_s("\r\n");
  write_rs232_s("Zone duration: ");
  write_rs232_int(zone->duration);
  write_rs232_s("\r\n");
  return (is_time_valid(&(zone->start_time)) && // valid start time
    is_zone_num_valid(zone->num) && // valid zone number
    is_days_of_week_valid(zone->days_of_week) && // valid days of week
    zone->duration > 0 && zone->duration < MAX_DURATION); // valid duration
}

/**
 * Initialize zone controller
 */
void zone_controller_init() {
  SHIFT_REG_EN();
  SHIFT_REG_CLR();
}

/**
 * Store zone information
 */
bool store_zone(zone_t *zone) {
  if (!is_received_zone_valid(zone)) {
    write_rs232_s("Invalid zone information received...\r\n");
    return false;
  }

  // compute end time
  add_minutes(&(zone->start_time), zone->duration, &(zone->end_time));

  // if end time wraps around to next day return false
  if (compare_time(&(zone->start_time), &(zone->end_time))) {
    write_rs232_zone(zone);
    write_rs232_s("Invalid zone duration received...\r\n");
    return false;
  }

  // debug
  write_rs232_s("Valid zone received...\r\n");
  write_rs232_zone(zone);

  // store zone settings on RAM
  sys.settings.zones[bit_field_to_dec(zone->num)] = *zone;

  // store zone settings to eeprom
  while (!eeprom_is_ready()); // wait for EEPROM to be ready
  cli(); // turn off interrupts
  eeprom_update_block((const void*) &sys.settings, (void*) &settings,
    sizeof(sys.settings));
  sei(); // initialize interrupts

  // debug
  write_rs232_s("Zone settings stored to EEPROM...\r\n");
  write_rs232_settings(&sys.settings);
  write_rs232_s("\r\n");

  return true;
}

/**
 * Update zone status to ON or OFF depending on current time
 */
void update_zones() {
  uint8_t i;
  for (i = 0; i < MAX_ZONES; i++) {
    uint8_t zone = BIT(i); // zone bit field
    // uint8_t water_saver_mode = sys.settings.mode;
    zone_t *zone_settings = &sys.settings.zones[i]; // pointer to zone settings

    // return if zone will not run on current day
    if (!is_day_of_week_set(zone_settings->days_of_week));

    // turn zone off if time invalid or zone is moist and zone has not been turned off
    // TODO: add moisture sensor functionality
    if (BIT_IS_SET(zone, sys.zones_updated) && 
      (!is_time_set(&(zone_settings->start_time), &(zone_settings->end_time))) /*||
        (is_zone_moist(zone) && zone_settings->moisture_sensor*/) {
      BIT_CLEAR(sys.zones, zone);
      BIT_CLEAR(sys.zones_updated, zone);

      // debug
      write_rs232_s("Zone turned off...\r\n");
    }

    // turn zone on if time is valid and zone has not been turned on
    else if (BIT_IS_CLEAR(zone, sys.zones_updated) &&
      is_time_set(&(zone_settings->start_time), &(zone_settings->end_time))) {
      BIT_SET(sys.zones, zone);
      BIT_SET(sys.zones_updated, zone);

      // debug
      write_rs232_s("Zone turned on...\r\n");
    }
  }
}

/**
 * Run zones based on content of a zones bit field
 */
void run_zones(uint8_t zones) {
  // initialize srclk and rclk to low
  SHIFT_REG_SRCLK_LOW();
  SHIFT_REG_RCLK_LOW();
  uint8_t i;
  for (i = 0; i < MAX_ZONES; i++) {
    uint8_t zone = BIT(i);
    if (BIT_IS_SET(zone, zones)) {
      // pull up data if zone is on
      SHIFT_REG_SER_HIGH();
    } else {
      // pull down data if zone is off
      SHIFT_REG_SER_LOW();
    }
    // cycle srclk
    SHIFT_REG_SRCLK_HIGH();
    SHIFT_REG_SRCLK_LOW();
  }
  SHIFT_REG_RCLK_HIGH();
}

/**
 * Run system zones based on manual command
 * TODO: ADD ERROR CHECKS
 */
void run_zones_manual() {
  write_rs232_s("Update zone waiting...\r\n");
  switch (read_zone_cmd()) {
    case CMD_RUN_ZONE_1: BIT_SET(sys.zones, ZONE_1_BF); break;
    case CMD_RUN_ZONE_2: BIT_SET(sys.zones, ZONE_2_BF); break;
    case CMD_RUN_ZONE_3: BIT_SET(sys.zones, ZONE_3_BF); break;
    case CMD_RUN_ZONE_4: BIT_SET(sys.zones, ZONE_4_BF); break;
    case CMD_RUN_ZONE_5: BIT_SET(sys.zones, ZONE_5_BF); break;
    case CMD_RUN_ZONE_6: BIT_SET(sys.zones, ZONE_6_BF); break;
    case CMD_RUN_ZONE_7: BIT_SET(sys.zones, ZONE_7_BF); break;
    case CMD_RUN_ZONE_8: BIT_SET(sys.zones, ZONE_8_BF); break;
    case CMD_STOP_ZONE_1: BIT_CLEAR(sys.zones, ZONE_1_BF); break;
    case CMD_STOP_ZONE_2: BIT_CLEAR(sys.zones, ZONE_2_BF); break;
    case CMD_STOP_ZONE_3: BIT_CLEAR(sys.zones, ZONE_3_BF); break;
    case CMD_STOP_ZONE_4: BIT_CLEAR(sys.zones, ZONE_4_BF); break;
    case CMD_STOP_ZONE_5: BIT_CLEAR(sys.zones, ZONE_5_BF); break;
    case CMD_STOP_ZONE_6: BIT_CLEAR(sys.zones, ZONE_6_BF); break;
    case CMD_STOP_ZONE_7: BIT_CLEAR(sys.zones, ZONE_7_BF); break;
    case CMD_STOP_ZONE_8: BIT_CLEAR(sys.zones, ZONE_8_BF); break;
  }
  write_rs232_s("Update zone complete\r\n");
}

/**
 * Turn all zones on
 */
void run_all_zones() {
  int8_t zones = (int8_t) ZONE_ALL_BF;
  run_zones(zones);
}

/**
 * Turn all zones off
 */
void stop_all_zones() {
  uint8_t zones = (int8_t) ZONE_NONE_BF;
  run_zones(zones);
}

/**
 * Read in zone command
 */
uint8_t read_zone_cmd() {
  // wait for data to be received
  while(is_read_buf_empty()) {
  };
  return read_raspberry_pi();
}
