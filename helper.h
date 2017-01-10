#ifndef HELPER_H
#define HELPER_H

// bit field and masking macros
#define BIT(n) (1 << n) // create value with bit n set
#define BIT_SHIFT(a,n) (a << n) // shift a by n
#define BIT_MASK(len) (BIT(len) - 1) // create a bitmask len bits long
#define BIT_SET(x,mask) (x |= mask) // set mask bits in x
#define BIT_CLEAR(x,mask) (x &= ~mask) // clear mask bits in x
#define BIT_FLIP(x,mask) (x ^= mask) // flip mask bits in x
#define BIT_EQ(x,mask) ((x & mask) == mask) // test whether only x bits set in mask
#define BIT_IS_SET(x,mask) ((x & mask) != 0) // test whether all x bits set in mask 
#define BIT_IS_CLEAR(x,mask) ((x & mask) == 0) // test whether no x bits set in mask

uint8_t dec_to_bcd(uint8_t val);
uint8_t bcd_to_dec(uint8_t val);
bool is_power_of_two(uint8_t val);
uint8_t bit_field_to_dec(int8_t bf);

#endif
