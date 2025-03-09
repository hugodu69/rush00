#ifndef RUSH_HEADER_H
#define RUSH_HEADER_H

#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#include "utils.h"
#include "bitmanip.h"
#include "timer.h"
#include "usart.h"
#include "interrupt.h"

// TWI
// table 22-7 : prescale sets
#define TWI_PRESCALE_VALUE				1							// can be 1, 4, 16, 64
#define TWI_PRESCALE_SET(value)  \
										((value) == 1  ? (0<<TWPS1 | 0<<TWPS0) : \
										 (value) == 4  ? (0<<TWPS1 | 1<<TWPS0) : \
										 (value) == 16 ? (1<<TWPS1 | 0<<TWPS0) : \
										 (value) == 64 ? (1<<TWPS1 | 1<<TWPS0) : 0x00)
// 100kHz I2C
#define TWI_FREQ						100000UL
// Slave address
#define SLAVE_ADDRESS					42							// 22.3.3 : address 0000000 and 1111xxx are reservedm 42 is 0101010
// 22.7.1 : TWCR, Master Transmitter Mode
#define SEND_START_CONDITION			((1<<TWINT) | (1<<TWSTA) | (1<<TWEN))
#define SEND_CONTINUE_TRANSMISSION		((1<<TWINT) | (1<<TWEN))
#define SEND_STOP_CONDITION				((1<<TWINT) | (1<<TWSTO) | (1<<TWEN))

#define MASK_WITHOUT_LAST_3				11111000					// 0xF8

// FUNCTION PROTOTYPES
// twi.c
void TWI_init_master();
void TWI_start();
void TWI_write_addr(uint8_t addr_w);
void TWI_write(uint8_t data);
void TWI_stop();
void send_one_byte_data(uint8_t data);
// main.c
void setup_button_interrupt();
void setup_button_role();
void flash_led_1();

// GLOBAL VARIABLES

#endif