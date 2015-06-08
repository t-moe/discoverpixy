/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          emulator/qt/ll_system.cpp
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
* 2015-05-09	timolang@gmail.com	c652b6b	Improved Emulator Gui
* 2015-06-07	timolang@gmail.com	4b5768c	Improved Comments in whole emulator. Finalized emulator section in docu.
*
**************************************************************************************************************************************/

#include <QThread>
#include <QApplication>

extern "C" {
#include "ll_system.h"
}

bool ll_system_init()
{
    return true; //nothing to initialize here, apart from the stuff which is done in the main method.
}

void ll_system_delay(uint32_t msec)
{
    QThread::msleep(msec); //Let the app_process() Thread sleep
}

void ll_system_process()
{
    QApplication::processEvents(); //Process pending qt events
    QThread::msleep(1); //Sleep for 1ms, to keep the cpu load down
}

void ll_system_toggle_led()
{
    //No led emulated :(
}
