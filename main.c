#include "rush_header.h"

// -----------------------------------------------------------------------------------------------------------
// MASTER



void setup_button_interrupt() {										// Interruption SW1 (PD2)
    EICRA |= (1 << ISC01);      									// trigger on falling edge for INT0
    EIMSK |= (1 << INT0);       									// Activate INT0

	MODE_OUTPUT(LED1);

    SREG |= (1 << SREG_I);  
}

void setup_button_role() {											// Interruption SW2 (PD4)
    PCICR |= (1 << PCIE2);											// Activate PCINT[23:16] (PORTD)
    PCMSK2 |= (1 << PCINT20);										// Activate l'interrupt on PD4

	MODE_OUTPUT(LED1);

    SREG |= (1 << SREG_I);  
}

void flash_led_1() {
    SET_ELEM(LED1);
    _delay_ms(100);
    CLEAR_ELEM(LED1);
}

int main() {
    TWI_init_master();
	setup_button_interrupt();
	setup_button_role();
    while (1);
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