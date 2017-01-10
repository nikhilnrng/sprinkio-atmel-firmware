#include "sprinkio.h"

/**
 * Initialize I2C
 */
void i2c_init() {
  TWSR = 0; // Set prescalar for 1
  TWBR = BDIV; // Set bit rate register
}

/**
 * Write or read to an I2C device
 *
 * @param device_addr - I2C address of the device
 * @param *ap - pointer to address buffer
 * @param an - number of bytes to write (usually 1)
 * @param *wp - pointer to write data buffer
 * @param wn - number of bytes to write
 * @param *rp - pointer to read data buffer
 * @param rn - number of bytes to read
 * @return status - 0 if no errors
 */
uint8_t i2c_io(uint8_t device_addr, uint8_t *ap, uint16_t an, 
               uint8_t *wp, uint16_t wn, uint8_t *rp, uint16_t rn) {
  
  uint8_t status, send_stop, wrote, start_stat;

  status = 0;
  wrote = 0;
  send_stop = 0;

  if (an > 0 || wn > 0) {
    wrote = 1;
    send_stop = 1;

    I2C_REQ_START(); // send start condition
    while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
    status = TWSR & 0xf8;
    if (status != 0x08) // check that START was sent OK
      return(status);

    TWDR = device_addr & 0xfe; // load device address and R/W = 0;
    I2C_TX_START(); // start transmission
    while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
    status = TWSR & 0xf8;
    if (status != 0x18) { // check that SLA+W was sent OK
      if (status == 0x20) // check for NAK
        goto nakstop; // send STOP condition
      return(status); // otherwise just return the status
    }

    // write "an" data bytes to the slave device
    while (an-- > 0) {
      TWDR = *ap++; // put next data byte in TWDR
      I2C_TX_START(); // start transmission
      while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
      status = TWSR & 0xf8;
      if (status != 0x28) { // check that data was sent OK
        if (status == 0x30) // check for NAK
          goto nakstop; // send STOP condition
        return(status); // otherwise just return the status
      }
    }

    // write "wn" data bytes to the slave device
    while (wn-- > 0) {
      TWDR = *wp++; // put next data byte in TWDR
      I2C_TX_START(); // start transmission
      while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
      status = TWSR & 0xf8;
      if (status != 0x28) { // check that data was sent OK
        if (status == 0x30) // check for NAK
          goto nakstop; // send STOP condition
        return(status); // otherwise just return the status
      }
    }

    status = 0; // set status value to successful
  }

  if (rn > 0) {
    send_stop = 1;

    // set the status value to check for depending on whether this is a
    // START or repeated START
    start_stat = (wrote) ? 0x10 : 0x08;

    // put TWI into Master Receive mode by sending a START, which could
    // be a repeated START condition if we just finished writing.
    I2C_REQ_START(); // send start (or repeated start) condition
    while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
    status = TWSR & 0xf8;
    if (status != start_stat) // check that START or repeated START sent OK
      return(status);

    TWDR = device_addr  | 0x01; // load device address and R/W = 1;
    I2C_TX_START();  // send address+r
    while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
    status = TWSR & 0xf8;
    if (status != 0x40) { // check that SLA+R was sent OK
      if (status == 0x48) // check for NAK
        goto nakstop;
      return(status);
    }

    // read all but the last of n bytes from the slave device in this loop
    rn--;
    while (rn-- > 0) {
      I2C_RX_ACK_START(); // read byte and send ACK
      while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
      status = TWSR & 0xf8;
      if (status != 0x50) // check that data received OK
        return(status);
      *rp++ = TWDR; // read the data
    }

    // read the last byte
    I2C_RX_START();  // read last byte with NOT ACK sent
    while (!(TWCR & (1 << TWINT))); // wait for TWINT to be set
    status = TWSR & 0xf8;
    if (status != 0x58) // check that data received OK
      return(status);
    *rp++ = TWDR; // read the data

    status = 0; // set status value to successful
  }
  
  nakstop: // come here to send STOP after a NAK
  if (send_stop)
    I2C_REQ_STOP();  // send STOP condition

  return(status);
}