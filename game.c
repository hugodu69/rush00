#include "rush_header.h"

void timer() {
	flash_led(D3);
	_delay_ms(500);
	flash_led(D3);
	_delay_ms(500);
	flash_led(D3);
	_delay_ms(500);
	flash_led(D3);
	_delay_ms(500);
	turn_on_led(D3);
}

void launch_game() {
	// timer();
	write_one_byte_data(0x01);
}
