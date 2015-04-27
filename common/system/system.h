#include <stdbool.h>
#include <stdint.h>

bool system_init();
void system_delay(uint32_t msec);
void system_process();
void system_toggle_led();
