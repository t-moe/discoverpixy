/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_filetest.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-10	timolang@gmail.com	e2bce8f	Added filesystem module, tests and implementation for it in emulator.
* 2015-05-10	timolang@gmail.com	21edc56	Added doxyfile (doxygen) for the common folder. Started with doxygen comments for app and tft module.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
*
**************************************************************************************************************************************/

#include "screen.h"

/**
 * @addtogroup screens
 */
/*@{*/

/**
 * @defgroup filetest Filetest (Screen)
 * The File-Test Screen tests the filesystem module. It read/writes from/to files and shows a bitmap
 */
/*@{*/


/**
 * Returns a pointer to the filetest screen
 * \sa gui_screen_navigate
 * @return
 */
SCREEN_STRUCT* get_screen_filetest();

/*@}*/
/*@}*/
