#include "rush_header.h"

void get_role() {
	int end = FALSE;
	while(!end) {
		_delay_ms(50);
		if (role == SLAVE) {
			end = TRUE;
			// MODE_OUTPUT(BUTTON1);
			continue;
		}
		if (!IS_PIN_SET(BUTTON1)) {
			continue;
 		}
		end = TRUE;
		turn_on_led(D1);
		role = MASTER;
		twi_stop_slave();
		twi_init_master();
		write_one_byte_data(0x01);
	}
}