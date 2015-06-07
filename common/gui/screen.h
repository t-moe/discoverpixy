#ifndef SCREEN_H
#define SCREEN_H

#include <stdio.h>
#include <stdbool.h>

/**
 * @addtogroup gui
 */
/*@{*/

/**
 * @defgroup screen Screen
 * The Screen Submodule provides an api to navigate between different "screens" on the UI.
 * Each screen must provide an enter, update and a leave method; which will be called from this module at the right time. 
 * The implemented screens of the application are documented in the \ref screens module.
 */
/*@}*/

/**
 * @addtogroup screen
 */
/*@{*/

/**
 * Prototype for Event Listeners (called when the screen is entered, left or should be updated)
 * @param screen The pointer to the SCREEN_STRUCT where the event occurred
 */
typedef void (*SCREEN_CALLBACK)(void* screen);

/**
 * Structure to configure the Screen
 */
typedef struct SCREEN_S{
	SCREEN_CALLBACK on_enter; //!< The Callback which is called when the screen is entered. Add/Register all UI-Elements here
	SCREEN_CALLBACK on_leave; //!< The Callback which is called when the screen is left. Remove/Unregister all UI-Elements here
	SCREEN_CALLBACK on_update; //!< The Callback which is called repeatedly when the screen should be updated. Update/Redraw all UI-Elements here

	struct SCREEN_S* next; //!< Used internally. do not modify, do not initialize
} SCREEN_STRUCT;



/**
 * Navigate to the given screen as soon as the app enters the main loop again (and gui_screen_update() is called)
 * It's safe to call this method from an interrupt
 * @note Do not pass a screen which is already in your history of screens!
 * @param screen A Pointer to the preinitialized SCREEN_STRUCT
 * @return true on success
 */
bool gui_screen_navigate(SCREEN_STRUCT* screen);

/**
 * Navigate one screen back as soon as the app enters the main loop again.
 * It's safe to call this method from an interrupt
 * @return true on success
 */
bool gui_screen_back();


/**
 * Returns the currently active screen
 * @return A Pointer to the active SCREEN_STRUCT
 */
SCREEN_STRUCT* gui_screen_get_current();

//Updates/switches the screens. Call this from the app main loop, as fast as you can.
/**
 * Updates the current screen. Switches the screen if gui_screen_navigate() or gui_screen_back() have been called since the last call to this method.
 * This method should be called repeatedly from the main loop (e.g. app_process())
 */
void gui_screen_update();

/*@}*/

#endif /* SCREEN_H */
