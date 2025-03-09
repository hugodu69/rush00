#include "rush_header.h"

volatile Role role = WAITING;

void setup_button_interrupt() {										// interruption SW1 (PD2)
    EICRA |= (1 << ISC01);      									// trigger on falling edge for INT0
    EIMSK |= (1 << INT0);       									// Activate INT0

    SREG |= (1 << SREG_I);  
}

ISR(INT0_vect) {													// interruption SW1 (PD2)
	if (!TEST_PIN(BUTTON1)) {
    	_delay_ms(30);
		flash_led(D4);
		if (role == MASTER) {
			// write_one_byte_data(0x01);
			read_one_byte_data();
		} else if (role == SLAVE) {
			// PORTD &= ~(1 << PD2); // Pull PD2 low (active LOW signal)
    		// _delay_ms(10);        // Give master time to catch the signal
    		// PORTD |= (1 << PD2);  // Reset line HIGH
		}
	}
}

ISR(TWI_vect) {														// interruption slave receive data, Table 12-1 : 25, 2-wire Serial Interface
    uint8_t status = TWSR & MASK_WITHOUT_LAST_3;

    switch (status) {

        // Slave Receiver cases
        case TW_SR_SLA_ACK:											// SLA+W received, ACK sent
            // flash_led(D4);
            turn_on_led_2_light();
            break;
        case TW_SR_DATA_ACK:										// data received, ACK sent
            if (role == WAITING) {
                role = SLAVE;
            }
            received_data = TWDR;                                   // store received data
            break;
        case TW_SR_STOP:											// STOP or REPEATED START
            break;
        case TW_SR_DATA_NACK:										// data received but slave answered NACK (no ACK an means end of communication or error)
            break;

        // Slave Transmitter cases
        case TW_ST_SLA_ACK:											// SLA+R received, ACK sent - First byte to be transmitted
            flash_led(D4);
            TWDR = 0x10;											// send first data byte
            break;
        case TW_ST_DATA_ACK:										// data sent, ACK received - Additional bytes can be sent
            TWDR = 0x10;											// send next data byte
            break;
        case TW_ST_DATA_NACK:										// data sent, NACK received - No more data requested
            break;													// master doesn't want more data
        case TW_ST_LAST_DATA:										// last data byte sent, ACK received
			break;													// All data sent

        default:
            break;
    }

    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA) | (1 << TWIE);	// Clear interrupt flag and enable next transfer
}