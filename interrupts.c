#include "rush_header.h"

ISR(INT0_vect) {													// Interruption SW1 (PD2)
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		flash_led_1();
		send_one_byte_data(0x01);
	}
}
ISR(PCINT2_vect) {													// Interruption SW2 (PD4)
    _delay_ms(50);
	if (!TEST_PIN(BUTTON1)) {
		flash_led_1();
		send_one_byte_data(0x01);
	}
}