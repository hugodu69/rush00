#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include "headers.h"


#define F_CPU 16000000UL // CPU clock
#define TWI_FREQ 100000UL // 100kHz I2C
#define SLAVE_ADDRESS 0x10 // Slave address




// -----------------------------------------------------------------------------------------------------------
// MASTER

void TWI_init_master() {
    TWSR = 0x00;										// set prescaler
    TWBR = ((F_CPU / TWI_FREQ) - 16) / 2;				// set SCL frequency, formula from datasheet
}

void TWI_start() {
    TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT);	// send Start condition
    while (!(TWCR & (1 << TWINT)));						// wait for TWINT flag to be set
}

void TWI_write(uint8_t data) {
    TWDR = data;										// load data into TWDR register
    TWCR = (1 << TWEN) | (1 << TWINT);					// start transmission
    while (!(TWCR & (1 << TWINT)));						// wait for TWINT flag to be set
}

void TWI_stop() {
    TWCR = (1 << TWSTO) | (1 << TWEN) | (1 << TWINT); 	// send STOP condition
	while (TWCR & (1 << TWSTO));						// wait for STOP condition to be executed
}

void send_one_byte_data(uint8_t data) {
    TWI_start();
    TWI_write((SLAVE_ADDRESS << 1) | TW_WRITE);			// Send Slave address with Write bit
    TWI_write(data);									// Send data byte
    TWI_stop();
}

int main() {
    TWI_init_master();
    while (1) {
        sendData(0x01);									// Send 1-byte data
        _delay_ms(1000);
    }
}




// -----------------------------------------------------------------------------------------------------------
// SLAVE


volatile uint8_t received_data = 0;

void TWI_init_slave(void) {
    TWAR = (SLAVE_ADDRESS << 1);
    
    TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);		// Enable TWI, enable ACK, and enable TWI interrupt
    
    sei();												// Enable global interrupts
}

ISR(TWI_vect) {											// Table 12-1 : 25, 2-wire Serial Interface
    if ((TWSR & 0xF8) == TW_SR_DATA_ACK) {				// Check if this is a data receive event (TWDR has valid data)
		// blink led
    	SET_ELEM(LED1);
    	_delay_ms(100);
    	CLEAR_ELEM(LED1);
    }
    
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);	// Clear interrupt flag and enable next transfer
}

int main() {
    MODE_OUTPUT(LED1);									// Initialize LED
	CLEAR_ELEM(LED1);									// ensure led is initially off
    TWI_init_slave();
    while (1);
}