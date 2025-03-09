#include "rush_header.h"

void get_role() {
	MODE_INPUT(BUTTON1);
	int end = FALSE;
	while(!end) {
		if (role == SLAVE) {
			end = TRUE;
			MODE_OUTPUT(BUTTON1);
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
		_delay_ms(50);
	}
}