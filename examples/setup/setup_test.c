#include "../../sprinkio.h"

int main (void) {

  // enable global interrupts
  sei();
  
  // enable usart connections
  usart_init();

  // run setup loop
  run_setup_loop();

  return 0;
}