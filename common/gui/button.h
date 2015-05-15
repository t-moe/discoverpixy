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
 * The Button Gui-Element
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
typedef void (*BUTTON_CALLBACK)(void *button);


/**
 * Structure to configure the Button
 */
typedef struct {
	TOUCH_AREA_STRUCT base; //!< Basic geometry of the button. You only need to set the x1, y1, x2, y2 members of this struct.
	uint16_t bgcolor; //!< The 16-bit background color of the button
	BUTTON_CALLBACK callback; //!< Callback which is executed when the button is pressed
	uint16_t txtcolor; //!< The 16-bit text color
	uint8_t font; //!< The number of the font to use
	const char *text; //!< The label of the button
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

/*
bool guiAddBitmapButton(BITMAPBUTTON_STRUCT* button);
void guiRemoveBitmapButton(BITMAPBUTTON_STRUCT* button);
void guiRedrawBitmapButton(BITMAPBUTTON_STRUCT* button);
*/


/*
typedef struct {
	TOUCH_AREA_STRUCT base;
	unsigned int bgcolor;
	BUTTON_CALLBACK callback; //Callback
	unsigned char imgwidth;
	unsigned char imgheight;
	char* filename;
} BITMAPBUTTON_STRUCT;
*/
//Notice that the first 3 Members are Equal, so it's possible to cast it to a BUTTON_STRUCT even if it's a BITMAPBUTTON_STRUCT (when changeing only the first 3 Members).



/*@}*/

#endif /* BUTTON_H */
