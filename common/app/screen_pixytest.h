/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_pixytest.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-27	timolang@gmail.com	cf72baa	Introduced a Screen (sub) module and divided app into multiple screens.
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
 * @defgroup pixytest Pixytest (Screen)
 * The Pixy-Test Screen tests the pixy module.
 */
/*@{*/

/**
 * Returns a pointer to the pixytest screen
 * \sa gui_screen_navigate
 * @return
 */
SCREEN_STRUCT* get_screen_pixytest();

/*@}*/
/*@}*/
