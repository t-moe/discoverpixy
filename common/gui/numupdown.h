#ifndef NUMUPDOWN_H
#define NUMUPDOWN_H

#include "button.h"

/**
 * @addtogroup gui
 */
/*@{*/

/**
 * @defgroup numupdown NummericUpDown
 * The NummericUpDown Gui Element
 */
/*@}*/

/**
 * @addtogroup numupdown
 */
/*@{*/

/**
 * Prototype for Event Listeners (called when the NummericUpDown value has changed)
 * \note You should NOT execute long running things in this callback nor should you update the gui. But you can call gui_screen_navigate() for instance.
 * @param numupdown The pointer to the NUMUPDOWN_STRUCT where to corresponding NummericUpDown has changed it's value
 * @param value The new value of the NummericUpDown
 */
typedef void (*NUMUPDOWN_CALLBACK)(void *numupdown, int16_t value);

/**
 * Structure to configure the NummericUpDown
 */
typedef struct {
	uint16_t x; //!< The x-Coordinate of the Top-Left Starting Point.
	uint16_t y; //!< The y-Coordinate of the Top-Left Starting Point.
	uint16_t fgcolor; //!< The 16-bit color of the value-text
	int16_t value; //!< The current/default value
	int16_t min; //!< The minimum possible value (inclusive)
	int16_t max; //!< The maximum possible value (inclusive)
	NUMUPDOWN_CALLBACK callback; //!< Callback which is executed when the value changes
	
	BUTTON_STRUCT buttonUp; //!< For internal use, don't change, don't initialize
	BUTTON_STRUCT buttonDown; //!< For internal use, don't change, don't initialize
} NUMUPDOWN_STRUCT;

/**
 * Adds a NummericUpDown. Your Callback will be called from now on, if the numupdown's value changes
 * @param numupdown A Pointer to the preinitialized NUMUPDOWN_STRUCT
 * @return true on success
 */
bool gui_numupdown_add(NUMUPDOWN_STRUCT* numupdown);

/**
 * Removes the NummericUpDown. You will no longer receive events for this numupdown. This function will not overdraw the region where the numupdown was located.
 * @param numupdown A Pointer to the NUMUPDOWN_STRUCT
 */
void gui_numupdown_remove(NUMUPDOWN_STRUCT* numupdown);

/**
 * Updates the NummericUpDown. Call this function when you change the value/min/max of the numupdown through code.
 * @param numupdown A Pointer to the NUMUPDOWN_STRUCT
 */
void gui_numupdown_update(NUMUPDOWN_STRUCT* numupdown);

/**
 * Redraws the NummericUpDown. Call this method if you have to redraw the entire screen or if you want to draw a numupdown on top of an image.
 * @param numupdown A Pointer to the NUMUPDOWN_STRUCT
 */
void gui_numupdown_redraw(NUMUPDOWN_STRUCT* numupdown);

/*@}*/

#endif /* NUMUPDOWN_H */
