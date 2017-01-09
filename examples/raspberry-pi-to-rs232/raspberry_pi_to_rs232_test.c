#include "../../sprinkio.h"

int main (void) {
  // enable global interrupts
  sei();
  
  // enable usart connections
  usart_init();
  
  // echo test data
  while (1) {
    uint8_t data = read_raspberry_pi();
    if (data != USART_NO_DATA) {
      write_rs232(data);
    }
  }
  
  return 0;
}
