#ifndef ATMEGA328P_H
#define ATMEGA328P_H

#include "sprinkio.h"

#define DEVICE "Atmega328p"

// USART interrupt vectors
#define USART_RX_COMPLETE USART_RX_vect
#define USART_TX_COMPLETE USART_TX_vect
#define USART_DATA_REG_EMPTY USART_UDRE_vect

// USART macros
#define USART_TX_START() UCSR0B |= (1 << TXEN0)	// enable TX
#define USART_TX_STOP() UCSR0B &= ~(1 << TXEN0)	// disable TX
#define USART_RX_START() UCSR0B |= (1 << RXEN0)	// enable RX
#define USART_RX_STOP() UCSR0B &= ~(1 << RXEN0)	// disable RX

// USART interrupt macros
#define USART_RX_INT_EN() UCSR0B |= (1 << RXCIE0) // enable RX complete interrupt
#define USART_RX_INT_DIS() UCSR0B &= ~(1 << RXCIE0) // disable RX complete interrupt
#define USART_TX_INT_EN() UCSR0B |= (1 << TXCIE0) // enable TX complete interrupt
#define USART_TX_INT_DIS() UCSR0B &= ~(1 << TXCIE0) // disable TX complete interrupt
#define USART_UDR0_EMPTY_INT_EN() UCSR0B |= (1 << UDRIE0) // enable UDR0 empty interrupt
#define USART_UDR0_EMPTY_INT_DIS() UCSR0B &= ~(1 << UDRIE0) // disable UDR0 empty interrupt 

// Decoder Select Bit macros
#define USART_SELECT_EN() DDRB |= (1 << DDB0) // set PB0 to output
#define RASPBERRY_PI_COMM_EN() PORTB |= (1 << PB0) // set PB0 decoder select bit
#define RS232_COMM_EN() PORTB &= ~(1 << PB0) // clear PB0 decoder select bit

// I2C macros
#define I2C_REQ_START() TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTA) // send start condition
#define I2C_REQ_STOP() TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWSTO) // send stop condition
#define I2C_TX_START() TWCR = (1 << TWINT) | (1 << TWEN) // start transmission
#define I2C_RX_START() TWCR = (1 << TWINT) | (1 << TWEN) // read byte
#define I2C_RX_ACK_START() TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) // read byte and send ACK


// Zone Controller port pin macros
#define SHIFT_REG_EN() DDRC |= (1 << DDC1) | (1 << DDC2) | (1 << DDC3) // set shift reg ports to output
#define SHIFT_REG_CLR() PORTC &= ~((1 << PC1) | (1 << PC2) | (1 << PC3)) // clear shift reg ports

// Zone Controller macros
#define SHIFT_REG_SER_HIGH() PORTC |= (1 << PC1) // set shift reg serial input to high
#define SHIFT_REG_SER_LOW() PORTC &= ~(1 << PC1) // set shift reg serial input to low
#define SHIFT_REG_RCLK_HIGH() PORTC |= (1 << PC2) // set shift reg rclk input to high
#define SHIFT_REG_RCLK_LOW() PORTC &= ~(1 << PC2) // set shift reg rclk input to low
#define SHIFT_REG_SRCLK_HIGH() PORTC |= (1 << PC3) // set shift reg srclk input to high
#define SHIFT_REG_SRCLK_LOW() PORTC &= ~(1 << PC3) // set shift reg srclk input to low

// Moisture Sensor macros
#define MOIST_CLEAR_ADMUX() ADMUX &= 0b00000000;

#endif
