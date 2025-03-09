#include "rush_header.h"

#define BLINK_PRESCALE_VALUE				1024		            			        // can be 1, 8, 64, 256, 1024
#define BLINK_DUTY_CYCLE					10

void flash_led(int bit) {
	SET(DDRB, bit);
    SET(PORTB, bit);
    _delay_ms(100);
    CLEAR(PORTB, bit);
}
void turn_on_led(int bit) {
	SET(DDRB, bit);
    SET(PORTB, bit);
}
void turn_on_led_2_light() {
    uint8_t duty = 10;
    // Set PB1 (OC1A) as output
    DDRB |= (1 << PB1);

    // Configure Timer1 for Fast PWM, 8-bit mode
    TCCR1A = (1 << COM1A1) | (1 << WGM10); // Clear OC1A on compare match, Fast PWM
    TCCR1B = (1 << WGM12) | (1 << CS11);   // Prescaler 8

    // Set duty cycle (0-255, where 255 = fully ON, 0 = fully OFF)
    OCR1A = duty;
}
void turn_off_led(int bit) {
	SET(DDRB, bit);
    CLEAR(PORTB, bit);
}
void blink_led_2(int period) {
	MODE_OUTPUT(LED2);
    SET(TCCR1A, WGM11);							                						// Table 16-4 : set timer in Fast PWM (Pulse With Modulation) mode with TOP = ICR1 (Mode 14)
	SET(TCCR1B, WGM12);
    SET(TCCR1B, WGM13);
	SET(TCCR1A, COM1A1);						                						// Table 16-2 : non-inverting mode, the LED will be ON for DUTY_CYCLE% of the time (CLEAR OC1A on compare match, SET OC1A at BOTTOM)
    ICR1 = TIME_MS(period, BLINK_PRESCALE_VALUE);		              			  		// Table 16-4 : set the period (compare TOP value)
    OCR1A = TIME_MS(PERCENT(BLINK_DUTY_CYCLE, period), BLINK_PRESCALE_VALUE);			// 16.9.3 : set the duty cycle to DUTY_CYCLE% of the time on channel A -> OC1A (alternate function of PORTB1, aka LED2) is cleared when TCNT1 (the counter value) equals OCR1A
    TCCR1B |= (PRESCALE_SET(BLINK_PRESCALE_VALUE));	                					// start the timer with the prescaler
}

int main() {
	MODE_INPUT(BUTTON1);	// set as input
	SET_ELEM(BUTTON1);		// pull-up resistor on

	twi_init_slave();
	get_role();
	setup_button_interrupt();
	// launch_game();
    while (1);
}
