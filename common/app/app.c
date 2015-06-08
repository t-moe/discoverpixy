/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/app.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-03	timolang@gmail.com	1aa9194	Fixed Drawing of rects in Emulator. Got frames from pixy to emulator. Slooooow.
* 2015-04-25	timolang@gmail.com	416883c	Pixy&Usb work with the new folder structure now.
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
* 2015-04-27	timolang@gmail.com	259d446	Added touch support to emulator. Implemented basic touch function.
* 2015-04-27	aaron@duckpond.ch	aed90ef	Drawcircle added (emulator)
* 2015-04-27	timolang@gmail.com	e249fb2	Added font support
* 2015-04-27	timolang@gmail.com	7c9eabc	Added button support.
* 2015-04-27	timolang@gmail.com	b300ac5	Added Checkbox support
* 2015-04-27	timolang@gmail.com	cf72baa	Introduced a Screen (sub) module and divided app into multiple screens.
* 2015-05-10	timolang@gmail.com	e2bce8f	Added filesystem module, tests and implementation for it in emulator.
* 2015-05-12	aaron@duckpond.ch	aec62d4	Added datasheets, updated touchsupport.
* 2015-05-28	aaron@duckpond.ch	5bda699	implemented functions to get x and y coordinates and a test function
* 2015-05-29	aaron@duckpond.ch	7d2d1a1	Implemented basic sampling and averaging of touch coordinates using timer7
*
**************************************************************************************************************************************/

#include "app.h"
#include "tft.h"
#include "system.h"
#include "touch.h"
#include "screen_main.h"
#include "filesystem.h"

void app_init()
{
    system_init();
    tft_init();
    touch_init();
    filesystem_init();

    gui_screen_navigate(get_screen_main());
}

//app event loop
void app_process()
{

    system_process(); //Let the system handle it's pending events
    gui_screen_update(); //update the currently active screen
}
