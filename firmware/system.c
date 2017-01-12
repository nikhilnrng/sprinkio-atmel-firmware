#include "sprinkio.h"

extern system_t sys;

/**
 * Initialize Sprinkio system
 */
void system_init(void) {
  sei(); // enable interrupts
  usart_init(); // enable usart communication
  settings_init(); // initialize settings
  zone_controller_init(); // initialize zone controller
  sys.zones = ZONE_NONE_BF; // set all zones off
  sys.state = STATE_IDLE; // set idle state
  sys.zones_updated = 0;
  // TODO: add system initialization for zone_updated
}

/**
 * Run Sprinkio system loop
 */
void run_system_loop(void) {
  write_rs232_s("Main system loop entered\r\n");
  while (1) {
    switch(sys.state) {
      case STATE_IDLE: system_idle_loop(); break;
      case STATE_SETUP: system_setup_loop(); break;
      case STATE_ZONE: system_zone_loop(); break;
      case STATE_CHECK: system_check_loop(); break;
    }
  }
}

/**
 * Check for sys commands, update time/zones, and run zones
 */
void system_idle_loop(void) {
  system_read(); // read serial system commands
  get_time(&sys.time); // update time
  get_date(&sys.date); // update date
  update_zones(); // update zone status
  run_zones(sys.zones); // run zones based on system zone status
}

/**
 * Update Sprinkio settings and return to idle loop
 */
void system_setup_loop(void) {
  write_rs232_s("System setup loop entered\r\n");
  run_setup_loop(); 
  sys.state = STATE_IDLE;
}

/**
 * Update zone status through manual serial commands and return to idle loop
 */
void system_zone_loop(void) {
  write_rs232_s("System zone loop entered\r\n");
  run_zones_manual();
  sys.state = STATE_IDLE;
}

/**
 * Send system sensor and zone status and return to idle loop
 */
void system_check_loop(void) {
  write_rs232_s("System check loop entered\r\n");
  // TODO: send system check information 
  sys.state = STATE_IDLE;
}

/**
 * Read serial commands and update system state
 */
void system_read(void) {
  switch(read_raspberry_pi()) {
    case CMD_SETUP_MODE: sys.state = (uint8_t) STATE_SETUP; break;
    case CMD_SENSOR_STATUS: sys.state = (uint8_t) STATE_CHECK; break;
    case CMD_UPDATE_ZONE: sys.state = (uint8_t) STATE_ZONE; break;
    case CMD_NO_ACTION: break;
  }
}
