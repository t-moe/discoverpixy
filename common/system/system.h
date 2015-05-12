#ifndef SYSTEM_H
#define SYSTEM_H

/**
 * @defgroup system System
 * The System Module provides access to delay functions, leds and provides a system init function
 */
/*@{*/


#include <stdbool.h>
#include <stdint.h>

/**
 * Initializes the system. Call this method at the start of your app_init() function and before using any system_* functions
 * @return true on success
 */
bool system_init();

/**
 * Sleeps for a certain amount of time
 * @param msec The number of milliseconds to sleep
 */
void system_delay(uint32_t msec);

/**
 * Executes pending system events (like handling usb, timers etc). Call this somewhere in app_process().
 */
void system_process();

/**
 * Toggles a Status Led. Use this function for debugging or to show activity
 */
void system_toggle_led();

/*@}*/

#endif /* SYSTEM_H */
