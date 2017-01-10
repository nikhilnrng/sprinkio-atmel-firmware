#ifndef USART_H
#define USART_H

#include "sprinkio.h"

#ifndef FOSC
#define FOSC 7372800 // clock frequency
#endif

#define BAUD 9600 // UART0 BAUD rate
#define UBRR (FOSC / 16 / BAUD - 1) // Value of UBRR0 register

#define USART_NO_DATA 0xFF

// receive transmit buffers
#define USART_RX_BUFFER_SIZE 128
#define USART_TX_BUFFER_SIZE 128

// prototypes
void usart_init();
void usart_write(uint8_t ch);
uint8_t usart_read();
bool is_write_buf_empty();
bool is_read_buf_empty();

#endif
