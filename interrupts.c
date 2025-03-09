#include "rush_header.h"

volatile Role role = WAITING;

void setup_button_interrupt() {										// interruption SW1 (PD2)
    EICRA |= (1 << ISC01);      									// trigger on falling edge for INT0
    EIMSK |= (1 << INT0);       									// Activate INT0

    SREG |= (1 << SREG_I);  
}

ISR(INT0_vect) {													// interruption SW1 (PD2) send data
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		flash_led(D4);
		get_role();
		if (role == MASTER) {
			send_one_byte_data(0x01);
		}
	}
}

ISR(TWI_vect) {														// interruption slave receive data, Table 12-1 : 25, 2-wire Serial Interface
    uint8_t status = TWSR & MASK_WITHOUT_LAST_3;

    switch (status) {
        case TW_SR_SLA_ACK:											// SLA+W received
            break;

        case TW_SR_DATA_ACK:										// data received
            flash_led(D4);
            if (role == WAITING) {
                role = SLAVE;
            }
            break;

        case TW_SR_STOP:											// STOP or REPEATED START
            break;

        case TW_SR_DATA_NACK:										// data received but slave answered NACK (no ACK an means end of communication or error)
            break;

        default:
            break;
    }

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);	// Clear interrupt flag and enable next transfer
}