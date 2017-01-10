#include "sprinkio.h"

// initialize global system variable to 0
system_t sys;

// store settings EEMEM location
settings_t EEMEM settings;
// settings_t EEMEM settings = {{{0}}}; // necessary to prime EEPROM to empty settings

int main(void) {

  // read zone settings from eeprom
  while (!eeprom_is_ready());
  cli(); // turn off interrupts
  eeprom_read_block(&sys.settings, &settings, sizeof(sys.settings));
  sei();

  // initialize system
  system_init();

  // run system loop
  run_system_loop(); 
  
  return 0;
}
