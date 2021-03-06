/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/touch/screen_calibrate.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-06-01	timolang@gmail.com	06227da	Added calibrate screen (WIP). fixed bug in emulator drawing.
* 2015-06-01	timolang@gmail.com	eb573bc	Finalized calibration. Fixed a bug in screen module.
*
**************************************************************************************************************************************/

#include "screen.h"

/**
 * @addtogroup touch
 */
/*@{*/


/**
 * @defgroup calibrate Calibrate (Screen)
 * The calibrate screen for the touch module
 */
/*@{*/


/**
 * Returns a pointer to the calibrate screen
 * \sa gui_screen_navigate
 * @return
 */
SCREEN_STRUCT* get_screen_calibrate();

/*@}*/
/*@}*/


//TODO: Move this define to a common accessible, but private header file (they are used by screen_calibrate.c and touch.c)
#define CCENTER 20  //Pixel Distance from Sides for Calibration Cross
#define CLENGTH 10	//Length of the Calibration Cross Lines
#define CBEGIN (CCENTER-CLENGTH/2)
#define CEND (CCENTER + CLENGTH/2)
#define DWIDTH 320 //TODO: move define to tft module or make a function out of it
#define DHEIGHT 240 //TODO: move define to tft module or make a function out of it
