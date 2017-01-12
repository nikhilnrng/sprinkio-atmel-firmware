#include "sprinkio.h"

extern system_t sys;
extern settings_t settings;

bool setup_system_time_flag;
bool setup_system_date_flag;

/**
 * Initialize system settings from EEPROM
 */
void settings_init() {
  write_rs232_s("Loading settings from EEPROM...\r\n");
  // read settings block from EEPROM
  while (!eeprom_is_ready()); // wait for EEPROM to be ready
  cli(); // turn off interrupts
  eeprom_read_block((void*) &sys.settings, (const void*) &settings,
    sizeof(sys.settings));
  sei(); // initialize interrupts
  write_rs232_s("Read settings from EEPROM...\r\n");
  write_rs232_settings(&sys.settings);
  write_rs232_s("\r\n");
}

/**
 * System setup loop run when setup mode command is received
 */
void run_setup_loop() {
  write_rs232_s("Setup loop running...\r\n");
  setup_system_time_flag = false;
  setup_system_date_flag = false;
  // initialize setting holders
  time_t time; date_t date; zone_t zone; mode_t mode;
  bool run_setup = true;
  setup_write((uint8_t) CMD_SETUP_START);
  while(run_setup) {
    switch(setup_read()) {
      case CMD_SETUP_TIME: setup_system_time_flag = true; if(!setup_time(&time)) run_setup = false; break;
      case CMD_SETUP_DATE: setup_system_date_flag = true; if(!setup_date(&date)) run_setup = false; break;
      case CMD_SETUP_ZONE: if(!setup_zone(&zone)) run_setup = false; break;
      case CMD_SETUP_WATER_MODE: if(!setup_mode(&mode)) run_setup = false; break;
      case CMD_SETUP_END: run_setup = false; break;
      default: setup_write((uint8_t) CMD_SETUP_ERROR); run_setup = false;
    }
  }
  setup_write((uint8_t) CMD_SETUP_END);
  // wait for write buffer to empty
  while(!is_write_buf_empty());
  write_rs232_s("Setup loop completed...\r\n");
}

/**
 * Setup current time loop
 */
bool setup_time(time_t *time) {
  write_rs232_s("Setup time loop running...\r\n");
  bool run_setup_time = true;
  setup_write((uint8_t) CMD_TIME_START);
  while(run_setup_time) {
    switch(setup_read()) {
      case CMD_TIME_SECOND: time->second = setup_read(); break;
      case CMD_TIME_MINUTE: time->minute = setup_read(); break;
      case CMD_TIME_HOUR: time->hour = setup_read(); break;
      case CMD_TIME_END: run_setup_time = false; break;
      default: setup_write((uint8_t) CMD_TIME_ERROR); return false;
    }
  }
  if(set_time(time) && setup_system_time_flag) setup_write((uint8_t) CMD_TIME_VALID);
  else setup_write((uint8_t) CMD_TIME_INVALID);
  setup_write((uint8_t) CMD_TIME_END);
  write_rs232_s("Setup time loop completed...\r\n");
  return true;
}

/**
 * Setup current date loop
 */
bool setup_date(date_t *date) {
  write_rs232_s("Setup date loop running...\r\n");
  bool run_setup_date = true;
  setup_write((uint8_t) CMD_DATE_START);
  while(run_setup_date) {
    switch(setup_read()) {
      case CMD_DATE_DAY: date->day = setup_read(); break;
      case CMD_DATE_MONTH: date->month = setup_read(); break;
      case CMD_DATE_YEAR: date->year = setup_read(); break;
      case CMD_DATE_END: run_setup_date = false; break;
      default: setup_write((uint8_t) CMD_DATE_ERROR); return false;
    }
  }
  if(set_date(date) && setup_system_date_flag) setup_write((uint8_t) CMD_DATE_VALID);
  else setup_write((uint8_t) CMD_DATE_INVALID);
  setup_write((uint8_t) CMD_DATE_END);
  write_rs232_s("Setup date loop completed...\r\n");
  return true;
}

/**
 * Setup or update zone loop
 */
bool setup_zone(zone_t *zone) {
  write_rs232_s("Setup zone loop running...\r\n");
  bool run_setup_zone = true;
  setup_write((uint8_t) CMD_ZONE_START);
  while(run_setup_zone) {
    switch(setup_read()) {
      case CMD_ZONE_NUM: write_rs232_s("Waiting for zone number...\r\n"); zone->num = setup_read(); break;
      case CMD_ZONE_DAYS_OF_WEEK: write_rs232_s("Waiting for days of week...\r\n"); zone->days_of_week = setup_read(); break;
      case CMD_ZONE_DURATION: write_rs232_s("Waiting for duration...\r\n"); zone->duration = setup_read(); break;
      case CMD_ZONE_MOISTURE_SENSOR: write_rs232_s("Waiting for moisture sensor...\r\n"); zone->moisture_sensor = setup_read(); break;
      case CMD_ZONE_TIME: write_rs232_s("Waiting for zone number...\r\n"); setup_time(&(zone->start_time)); break;
      case CMD_ZONE_END: write_rs232_s("Zone setup end command received...\r\n"); run_setup_zone = false; break;
      default: setup_write((uint8_t) CMD_ZONE_ERROR); return false;
    }
  }
  if(store_zone(zone)) { setup_write((uint8_t) CMD_ZONE_VALID); write_rs232_zone(zone);}
  else setup_write((uint8_t) CMD_ZONE_INVALID);
  setup_write((uint8_t) CMD_ZONE_END);
  write_rs232_s("Setup zone loop completed...\r\n");
  return true;
}

/**
 * Setup water saving mode loop
 */
bool setup_mode(mode_t *mode) {
  bool run_setup_mode = true;
  setup_write((uint8_t) CMD_MODE_START);
  while(run_setup_mode) {
    switch(setup_read()) {
      case CMD_MODE_WATER_SAVE: *mode = setup_read(); break;
      case CMD_MODE_END: run_setup_mode = false; break;
      default: setup_write((uint8_t) CMD_MODE_ERROR); return false;
    }
  }
  if(is_mode_valid(mode)) {
    setup_write((uint8_t) CMD_MODE_VALID);
    configure_mode(mode);
  } else setup_write((uint8_t) CMD_MODE_INVALID);
  setup_write((uint8_t) CMD_MODE_END);
  return true;
}

bool is_mode_valid(mode_t* mode) {
  return (* mode == true || * mode == false);
}

void configure_mode() {
  // TODO: configure mode
}

void setup_write(uint8_t data) {
  write_raspberry_pi(data);
}

uint8_t setup_read() {
  // wait for data to be received
  while(is_read_buf_empty());
  return read_raspberry_pi();
}