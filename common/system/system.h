/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/system/system.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
* 2015-05-11	timolang@gmail.com	08d9fe0	More work on doxygen module structure
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
*
**************************************************************************************************************************************/

#ifndef SYSTEM_H
#define SYSTEM_H

#include <stdbool.h>
#include <stdint.h>

/**
 * @defgroup system System
 * The System Module provides access to delay functions, leds and provides a system init function
 */
/*@{*/


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
