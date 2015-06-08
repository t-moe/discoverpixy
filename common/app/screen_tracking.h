/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_tracking.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-16	timolang@gmail.com	e46314b	Added Tracking Screen and implemented "Reference Tracking" and "Color Region Selection"
*
**************************************************************************************************************************************/

#include "screen.h"

/**
 * @addtogroup screens
 */
/*@{*/

/**
 * @defgroup tracking Tracking (Screen)
 * The Tracking-Screen shows the object-tracking and allows some configuration
 */
/*@{*/


/**
 * Enum which contains the available tracking implementations
 */
enum Tracking_Implementation {
    OUR_TRACKING,     //!< Our own tracking PID implementation
    REFERENCE_TRACKING//!< Pixy's internal tracking implementation
};

/**
 * Sets the current Mode/Tracking Implementation. Call this before using the screen obtained by get_screen_tracking()
 * @param impl The new mode
 */
void tracking_set_mode(enum Tracking_Implementation impl);

/**
 * Returns a pointer to the tracking screen
 * \sa gui_screen_navigate
 * @return
 */
SCREEN_STRUCT* get_screen_tracking();

/*@}*/
/*@}*/
