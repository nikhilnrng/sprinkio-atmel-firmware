#include "sprinkio.h"

// static variables
uint8_t usart_tx_buffer[USART_TX_BUFFER_SIZE];
volatile uint8_t usart_tx_head = 0;
volatile uint8_t usart_tx_tail = 0;
uint8_t usart_rx_buffer[USART_RX_BUFFER_SIZE];
volatile uint8_t usart_rx_head = 0;
volatile uint8_t usart_rx_tail = 0;

/**
 * Initialize the USART Tx/Rx ports
 */
void usart_init() {
  // set baud rate
  UBRR0 = UBRR;
  
  // enable rx and tx
  USART_TX_START();
  USART_RX_START();
  
  // enable interrupt when byte receive complete
  USART_RX_INT_EN();
}

/**
 * Write a byte to the Tx buffer
 */
void usart_write(uint8_t data) {
  // calculate next head
  uint8_t next_head = usart_tx_head + 1;

  // reset if next head is size of buffer
  if (next_head == USART_TX_BUFFER_SIZE) {
    next_head = 0;
  }
  
  // wait until there is space in the buffer
  while (next_head == usart_tx_tail);
  
  // write to the buffer at current head
  usart_tx_buffer[usart_tx_head] = data;
  
  // update head index to next head
  usart_tx_head = next_head;
  
  // enable interrupt to begin data streaming
  USART_UDR0_EMPTY_INT_EN();
}

/**
 * Check if write buffer is empty
 */
bool is_write_buf_empty() {
  return (usart_tx_head == usart_tx_tail);
}

/**
 * UDR0 register empty interrupt handler 
 */
ISR(USART_DATA_REG_EMPTY) {
  uint8_t tail = usart_tx_tail;
  
  // load data to UDR0
  UDR0 = usart_tx_buffer[tail];
  
  // increment tail
  tail++;
  if (tail == USART_TX_BUFFER_SIZE) {
    tail = 0;
  }
  usart_tx_tail = tail;
  
  // disable interrupt to stop data streaming
  if (tail == usart_tx_head) {
    USART_UDR0_EMPTY_INT_DIS();
  }
}

/**
 * Read a byte from the Rx buffer
 */
uint8_t usart_read() {
  uint8_t tail = usart_rx_tail;
  if (usart_rx_head == tail) {
    // no data received
    return USART_NO_DATA;
  } else {
    // load data from Rx buffer
    uint8_t data = usart_rx_buffer[tail];
    
    // increment tail
    tail++;
    if (tail == USART_RX_BUFFER_SIZE) {
      tail = 0;
    }
    usart_rx_tail = tail;

    return data;
  }
}

/**
* Check if read buffer is empty
*/
bool is_read_buf_empty() {
  return (usart_rx_head == usart_rx_tail);
}

/**
 * USART byte Rx complete interrupt handler 
 * TODO ADD SWITCH FOR IMPORTANT STATES
 */
ISR(USART_RX_COMPLETE) {
  uint8_t data = UDR0;
  uint8_t next_head = usart_rx_head + 1;
  
  // reset if next head is size of buffer
  if (next_head == USART_RX_BUFFER_SIZE) {
    next_head = 0;
  }
  
  // drop data if there is no space in the buffer
  if (next_head != usart_rx_tail) {
    // write data to the buffer if it is not full
    usart_rx_buffer[usart_rx_head] = data;
    
    // increment buffer head
    usart_rx_head = next_head;
  } else {
    // TODO handle overflow occurrence  
  }
}
