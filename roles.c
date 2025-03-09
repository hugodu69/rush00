#include "rush_header.h"

void get_role() {
	MODE_INPUT(BUTTON1);
	int end = FALSE;
	while(!end) {
		if (role != WAITING) {
			end = TRUE;
			continue;
		}
		if (!IS_PIN_SET(BUTTON1)) {
			continue;
 		}
		turn_on_led(D1);
		end = TRUE;
		role = MASTER;
		twi_stop_slave();
		twi_init_master();
	}
}