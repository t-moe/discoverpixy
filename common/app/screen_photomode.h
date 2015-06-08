/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_photomode.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-15	timolang@gmail.com	27c09ba	Redesigned main menu. Moved stuff from pixytest to a new helper file and to the new "photo mode"-screen.
*
**************************************************************************************************************************************/

#include "screen.h"

/**
 * @addtogroup screens
 */
/*@{*/

/**
 * @defgroup photomode Photo Mode (Screen)
 * The Photo Mode Screen allows taking snapshots of the current pixy cam feed
 */
/*@{*/

/**
 * Returns a pointer to the photomode screen
 * \sa gui_screen_navigate
 * @return
 */
SCREEN_STRUCT* get_screen_photomode();

/*@}*/
/*@}*/
