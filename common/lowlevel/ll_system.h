/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/lowlevel/ll_system.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	cab8609	Integrated pixy into emulator. Pixy is no longer in the common/libs folder but in emulator/libs and discovery/libs
* 2015-04-03	timolang@gmail.com	9a1d12a	Refactored discovery, to use new project structure. Almost ready.
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
*
**************************************************************************************************************************************/

#include <stdbool.h>
#include <stdint.h>


/**
 * @defgroup lowlevel LowLevel
 * The Low-Level platform abstraction layer
 */
/*@{*/

/**
 * @defgroup ll_system System (LowLevel)
 * Low level functions of the \ref system Module
 */

/*@}*/

/**
 * @addtogroup ll_system
 */
/*@{*/


bool ll_system_init();
void ll_system_delay(uint32_t msec);
void ll_system_process();
void ll_system_toggle_led();

/*@}*/
