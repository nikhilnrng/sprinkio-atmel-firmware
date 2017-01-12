#include "sprinkio.h"

/**
 * Write a byte to raspberry pi over serial connection
 */
void write_raspberry_pi(uint8_t data) {
  USART_SELECT_EN();
  RASPBERRY_PI_COMM_EN();
  usart_write(data);
}

/**
 * Read a byte from raspberry pi over serial connection
 */
uint8_t read_raspberry_pi() {
  return usart_read();
}
