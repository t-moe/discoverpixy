/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/gui/button.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-27	timolang@gmail.com	7c9eabc	Added button support.
* 2015-04-27	timolang@gmail.com	cf72baa	Introduced a Screen (sub) module and divided app into multiple screens.
* 2015-05-11	timolang@gmail.com	08d9fe0	More work on doxygen module structure
* 2015-05-12	timolang@gmail.com	1402598	Added doxygen stuff for button module and some minor changes to touch, screen_main and tft module.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
* 2015-05-17	timolang@gmail.com	2d46336	Improved comments in implementation of button, checkbox, numupdown, tft, touch and screen modules/submodules.
*
**************************************************************************************************************************************/

#ifndef BUTTON_H
#define BUTTON_H

#include "touch.h"

/**
 * @defgroup gui Gui
 * The Gui Module
 */
/*@{*/

/**
 * @defgroup button Button
 * The Button Gui-Element is a clickable, rectangular box with a label inside.
 * When it is pressed and released you will be notified via the provided callback.
 */

/*@}*/

/**
 * @addtogroup button
 */
/*@{*/


/**
 * Prototype for Event Listeners (called when the button is pressed)
 * \note You should NOT execute long running things in this callback nor should you update the gui. But you can call gui_screen_navigate() for instance.
 * @param button The pointer to the BUTTON_STRUCT where to corresponding Button was pressed
 */
typedef void (*BUTTON_CALLBACK)(void* button);


/**
 * Structure to configure the Button
 */
typedef struct {
    TOUCH_AREA_STRUCT base; //!< Basic geometry of the button. You only need to set the x1, y1, x2, y2 members of this struct.
    uint16_t bgcolor; //!< The 16-bit background color of the button
    BUTTON_CALLBACK callback; //!< Callback which is executed when the button is pressed
    uint16_t txtcolor; //!< The 16-bit text color
    uint8_t font; //!< The number of the font to use
    const char* text; //!< The label of the button
} BUTTON_STRUCT;


#define AUTO 0  //!< Use this value instead of x2, y2 in the BUTTON_STRUCT to autocalculate the button width/height

/**
 * Adds a button. Your Callback will be called from now on, if the button was pressed
 * @param button A Pointer to the preinitialized BUTTON_STRUCT
 * @return true on success
 */
bool gui_button_add(BUTTON_STRUCT* button);

/**
 * Removes the button. You will no longer receive events for this button. This function will not overdraw the region where the button was located.
 * @param button A Pointer to the BUTTON_STRUCT
 */
void gui_button_remove(BUTTON_STRUCT* button);

/**
 * Redraws the button. Call this method if you have to redraw the entire screen or if you want to draw a button on top of an image.
 * @param button A Pointer to the BUTTON_STRUCT
 */
void gui_button_redraw(BUTTON_STRUCT* button);


/*@}*/

#endif /* BUTTON_H */
