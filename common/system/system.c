#include "system.h"
#include "ll_system.h"


bool system_init() {
	return ll_system_init();
}

void system_delay(uint32_t msec) {
	ll_system_delay(msec);
}

void system_process() {
	ll_system_process();
}

void system_toggle_led() {
	ll_system_toggle_led();
}
