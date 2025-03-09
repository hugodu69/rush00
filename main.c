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
#define SEND_START_CONDITION			(1<<TWINT) | (1<<TWSTA) | (1<<TWEN)
#define SEND_CONTINUE_TRANSMISSION		(1<<TWINT) | (1<<TWEN)
#define SEND_STOP_CONDITION				(1<<TWINT) | (1<<TWSTO) | (1<<TWEN)

#define MASK_WITHOUT_LAST_3				11111000					// 0xF8

// FUNCTION PROTOTYPES
void TWI_init_master();
void TWI_start();
void TWI_write(uint8_t data);
void TWI_stop();
void send_one_byte_data(uint8_t data);
void setup_button_interrupt();

// GLOBAL VARIABLES

// -----------------------------------------------------------------------------------------------------------
// MASTER

void TWI_init_master() {
    TWSR = TWI_PRESCALE_SET(TWI_PRESCALE_VALUE);					// 22.9.3 : (Status Register) set prescaler
    TWBR = ((F_CPU / TWI_FREQ) - 16) / (2 * TWI_PRESCALE_VALUE);	// 22.9.1 : (Bit Rate Register) set SCL frequency (formula from datasheet, 22.5.2)
}

void TWI_start() {
    TWCR = SEND_START_CONDITION;									// 22.9.2 : (Control Register) send Start condition (22.7.1) ! writting 1 to TWINT clears it (set it to 0)
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the START condition has been transmitted

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from START go to ERROR
    if (status != TW_START && status != TW_REP_START) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_write_addr(uint8_t addr_w) {
    TWDR = addr_w;													// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load SLA_W into TWDR Register. Clear TWINT bit in TWCR to start transmission of address
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the SLA+W has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// Check value of TWI Status Register. Mask prescaler bits. If status different from MT_SLA_ACK go to ERROR
    if (status != TW_MT_SLA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_write(uint8_t data) {
    TWDR = data;													// 22.9.4 : (Data Register) load data into TWDR register
    TWCR = SEND_CONTINUE_TRANSMISSION;								// p225 example code : Load DATA into TWDR Register. Clear TWINT bit in TWCR to start transmission of data
    while (!(TEST(TWCR, TWINT)));									// p225 example code : Wait for TWINT Flag set. This indicates that the DATA has been transmitted, and ACK/NACK has been received

	uint8_t status = TWSR & MASK_WITHOUT_LAST_3;					// p225 example code : Check value of TWI Status Register. Mask prescaler bits. If status different from MT_DATA_ACK go to ERROR
    if (status != TW_MT_DATA_ACK) {
        TWCR = SEND_STOP_CONDITION;
        return;
    }
}

void TWI_stop() {
    TWCR = SEND_STOP_CONDITION;
}

void send_one_byte_data(uint8_t data) {
    TWI_start();
    TWI_write_addr((SLAVE_ADDRESS << 1) | TW_WRITE);				// Send Slave address with Write bit
    TWI_write(data);												// Send data byte
    TWI_stop();
}

void setup_button_interrupt() {
    EICRA |= (1 << ISC01);      									// trigger on falling edge for INT0
    EIMSK |= (1 << INT0);       									// Activate INT0

	MODE_OUTPUT(LED1);

    SREG |= (1 << SREG_I);  
}

int main() {
    TWI_init_master();
	setup_button_interrupt();
    while (1);
}

ISR(INT0_vect)  // Interruption SW1 (PD2)
{
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		TOGGLE_ELEM(LED1);
		send_one_byte_data(0x01);
	}
}




// -----------------------------------------------------------------------------------------------------------
// SLAVE


// volatile uint8_t received_data = 0;

// void TWI_init_slave(void) {
//     TWAR = (SLAVE_ADDRESS << 1);
    
//     TWCR = (1 << TWEN) | (1 << TWEA) | (1 << TWIE);		// Enable TWI, enable ACK, and enable TWI interrupt
    
//     SREG |= (1 << SREG_I);
// }

// ISR(TWI_vect) {											// Table 12-1 : 25, 2-wire Serial Interface
//     if ((TWSR & 0xF8) == TW_SR_DATA_ACK) {				// Check if this is a data receive event (TWDR has valid data)
// 		// blink led
//     	SET_ELEM(LED1);
//     	_delay_ms(100);
//     	CLEAR_ELEM(LED1);
//     }
    
//     TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);	// Clear interrupt flag and enable next transfer
// }

// int main() {
//     MODE_OUTPUT(LED1);									// Initialize LED
// 	CLEAR_ELEM(LED1);									// ensure led is initially off
//     TWI_init_slave();
//     while (1);
// }