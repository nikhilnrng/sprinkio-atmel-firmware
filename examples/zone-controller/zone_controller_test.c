#include "../../sprinkio.h"

system_t sys;

int main (void) {
  int i;
  
  // initialize zone controller
  zone_controller_init();
  
  // decrement from zone 7 to zone 1
  for (i = 7; i >= 0; i--) {
    uint8_t zone = BIT(i);
    run_zones(zone);
    _delay_ms(100);
  }
  
  // increment from zone 1 to zone 7
  for (i = 1; i < 7; i++) {
    uint8_t zone = BIT(i);
    run_zones(zone);
    _delay_ms(100);
  }
  
  // all zones on
  set_all_zones();
  _delay_ms(100);
  
  // all zones off
  clear_all_zones();
  _delay_ms(100);
 
  return 0;
}