#include "rush_header.h"

void get_role() {
	twi_init_slave();
	if (role == WAITING) {
		role = MASTER;
	}
	twi_stop_slave();
    twi_init_master();
}