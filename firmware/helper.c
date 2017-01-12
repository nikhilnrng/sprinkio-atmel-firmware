#include "sprinkio.h"

/**
 * Transform decimal value to BCD
 */
uint8_t dec_to_bcd(uint8_t val) {
  return val + 6 * (val / 10);
}

/**
 * Transform BCD value to decimal
 */
uint8_t bcd_to_dec(uint8_t val) {
  return val - 6 * (val >> 4);
}

/**
 * Return true if a number is a power of 2
 */
bool is_power_of_two(uint8_t val) {
  while(((val % 2) == 0) && val > 1) {
    val /= 2;
  }
  return (val == 1);
}

/**
 * Return the number of positions a bit is shifted left
 */
uint8_t bit_field_to_dec(int8_t bf) {
  uint8_t cnt = 0;
  while (bf != 0) {
    bf = bf >> 1; 
    if (++cnt == 8) return 0;
  }
  return cnt - 1;
}