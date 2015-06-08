/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/system/system.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
*
**************************************************************************************************************************************/

#include "system.h"
#include "ll_system.h"


bool system_init()
{
    return ll_system_init();
}

void system_delay(uint32_t msec)
{
    ll_system_delay(msec);
}

void system_process()
{
    ll_system_process();
}

void system_toggle_led()
{
    ll_system_toggle_led();
}
