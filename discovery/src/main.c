/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          discovery/src/main.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	21dd1e2	Starting to integrate usb branch. Optimized Makefiles
* 2015-04-03	timolang@gmail.com	9a1d12a	Refactored discovery, to use new project structure. Almost ready.
* 2015-04-25	timolang@gmail.com	416883c	Pixy&Usb work with the new folder structure now.
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
*
**************************************************************************************************************************************/

#include "app.h"

int main(void)
{
    app_init();

    while (1) {
        app_process();
    }

}
