#ifndef CHECKBOX_H
#define CHECKBOX_H

#include "touch.h"

/**
 * @addtogroup gui
 */
/*@{*/

/**
 * @defgroup checkbox Checkbox
 * The Checkbox Gui-Element is a clickable, rectangular box with an optional tickmark inside of it.
 * When the checkbox is pressed and released it's tick state changes and you will be notified via the provided callback.
 */
/*@}*/

/**
 * @addtogroup checkbox
 */
/*@{*/

/**
 * Prototype for Event Listeners (called when the checkbox state has changed)
 * \note You should NOT execute long running things in this callback nor should you update the gui. But you can call gui_screen_navigate() for instance.
 * @param checkbox The pointer to the CHECKBOX_STRUCT where to corresponding Checkbox has changed the state
 * @param checked A boolean which indicates whether the checkbox is now checked or not.
 */
typedef void (*CHECKBOX_CALLBACK)(void *checkbox, bool checked);

/**
 * Structure to configure the Checkbox
 */
typedef struct {
	TOUCH_AREA_STRUCT base; //!<  Basic geometry of the Checkbox. You only need to set the x1, y1, x2, y2 members of this struct.
	uint16_t fgcolor; //!< The 16-bit color of the tickmark
	bool checked; //!< A boolean which indicates whether or not the checkbox is currently checked.
	CHECKBOX_CALLBACK callback; //!< Callback which is executed when the checkbox changes state
} CHECKBOX_STRUCT;

/**
 * Adds a checkbox. Your Callback will be called from now on, if the checkbox changes state
 * @param checkbox A Pointer to the preinitialized CHECKBOX_STRUCT
 * @return true on success
 */
bool gui_checkbox_add(CHECKBOX_STRUCT* checkbox);

/**
 * Removes the checkbox. You will no longer receive events for this checkbox. This function will not overdraw the region where the checkbox was located.
 * @param checkbox A Pointer to the CHECKBOX_STRUCT
 */
void gui_checkbox_remove(CHECKBOX_STRUCT* checkbox);

/**
 * Updates the checkbox. Call this function when you change the state of the checkbox through code.
 * @param checkbox A Pointer to the CHECKBOX_STRUCT
 */
void gui_checkbox_update(CHECKBOX_STRUCT* checkbox);

/**
 * Redraws the checkbox. Call this method if you have to redraw the entire screen or if you want to draw a checkbox on top of an image.
 * @param checkbox A Pointer to the CHECKBOX_STRUCT
 */
void gui_checkbox_redraw(CHECKBOX_STRUCT* checkbox);

#define CHECKBOX_WIN_FG_COLOR RGB(32,161,34)

/*@}*/

#endif /* CHECKBOX_H */
