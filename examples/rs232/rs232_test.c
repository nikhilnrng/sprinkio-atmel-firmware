#include "../../sprinkio.h"

char str[] = "Hello World!\r\n";
uint8_t b = 0b10110010;

int main (void) {
  // enable global interrupts
  sei();
  
  // enable usart connections
  usart_init();

  // transmit hello world
  write_rs232_s(str);

  // print byte
  write_rs232_byte(byte);

  _delay_ms(1000);
  
  return 0;
}
