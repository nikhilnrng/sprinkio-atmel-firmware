#include "../../sprinkio.h"

uint8_t EEMEM testInteger = 30;

int main() {

  uint8_t SRAMchar = eeprom_read_byte(&testInteger);
  _delay_ms(1000); // allow time for data transfer

  sei(); // enable global interrupts
  usart_init(); // enable usart connections
  write_rs232_s("\r\nRandom Variable: ");
  write_rs232_int(SRAMchar);
  _delay_ms(1000);

  return 0;
}