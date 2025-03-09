#include "rush_header.h"

volatile Role role = WAITING;

void setup_button_interrupt() {										// interruption SW1 (PD2)
    EICRA |= (1 << ISC01);      									// trigger on falling edge for INT0
    EIMSK |= (1 << INT0);       									// Activate INT0

	MODE_OUTPUT(LED1);

    SREG |= (1 << SREG_I);  
}

void setup_button_role() {											// interruption SW2 (PD4)
    PCICR |= (1 << PCIE2);											// Activate PCINT[23:16] (PORTD)
    PCMSK2 |= (1 << PCINT20);										// Activate l'interrupt on PD4

	MODE_OUTPUT(LED2);

    SREG |= (1 << SREG_I);  
}

ISR(INT0_vect) {													// interruption SW1 (PD2) send data
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		flash_led(D1);
		send_one_byte_data(0x01);
	}
}
ISR(PCINT2_vect) {													// interruption SW2 (PD4) select role
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		flash_led(D2);
		get_role();
	}
}

ISR(TWI_vect) {														// interruption slave receive data, Table 12-1 : 25, 2-wire Serial Interface
    if ((TWSR & 0xF8) == TW_SR_DATA_ACK) {							// Check if this is a data receive event (TWDR has valid data)
		flash_led(D1);
		if (role == WAITING) {
			role = SLAVE;
		}
    }

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);	// Clear interrupt flag and enable next transfer
}