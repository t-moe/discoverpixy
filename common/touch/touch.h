/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/touch/touch.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-04-27	timolang@gmail.com	259d446	Added touch support to emulator. Implemented basic touch function.
* 2015-04-27	timolang@gmail.com	cf72baa	Introduced a Screen (sub) module and divided app into multiple screens.
* 2015-05-02	timolang@gmail.com	3281616	Added some more touch functions. Improved pixy test. Drag the Image around!
* 2015-05-11	timolang@gmail.com	a175a2f	Added doxygen docu for touch module
* 2015-05-11	timolang@gmail.com	08d9fe0	More work on doxygen module structure
* 2015-05-12	timolang@gmail.com	1402598	Added doxygen stuff for button module and some minor changes to touch, screen_main and tft module.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
* 2015-06-01	timolang@gmail.com	06227da	Added calibrate screen (WIP). fixed bug in emulator drawing.
* 2015-06-01	timolang@gmail.com	eb573bc	Finalized calibration. Fixed a bug in screen module.
*
**************************************************************************************************************************************/

#ifndef TOUCH_H
#define TOUCH_H

#include<stdbool.h>
#include<stdint.h>

/**
 * @defgroup touch Touch
 * The Touch module provides access to the touch controller, and executes a callback if a certain region is touched
 */


/**
 * @addtogroup touch
 */
/*@{*/

/**
Enum to describe the current Touch State. \sa touch_add_raw_event
 */
typedef enum {
    TOUCH_UP, //!< The display is currently not touched
    TOUCH_DOWN //!< The display is currently touched at some point
} TOUCH_STATE ;

/**
 * Enum to describe the hooked actions for which you want to receive events for.
 * You can OR-combine them. \sa touch_register_area
 */
typedef enum {
    NONE = 0x00,    //!< Do not receive any events
    PEN_DOWN = 0x01, //!< Receive an event when the pen goes down inside the region
    PEN_UP = 0x02,  //!< Receive an event when the pen goes up inside the region
    PEN_ENTER = 0x04, //!< Receive an event when the pen enters the region (pen was down before)
    PEN_LEAVE = 0x08, //!< Receive an event when the pen leaves the region (pen was inside region before)
    PEN_MOVE = 0x10 //!< Receive an event when the pen moves inside the region (pen is down)
} TOUCH_ACTION;

/**
 * Prototype for Event Listeners (called for every occurring, hooked action)
 * \note You should NOT execute long running things in this callback nor should you update the gui. But you can call gui_screen_navigate() for instance.
 * @param touchArea The pointer to the TOUCH_AREA_STRUCT in which the event occurred
 * @param triggeredAction The Action which occurred
 */
typedef void (*TOUCH_CALLBACK)(void* touchArea, TOUCH_ACTION triggeredAction);

/**
 * Structure to configure a Touch Area
 */
typedef struct {
    TOUCH_ACTION hookedActions; //!< Actions to listen to
    uint16_t x1; //!< Top Left X-Coordinate of Area
    uint16_t y1; //!< Top Left Y-Coordinate of Area
    uint16_t x2; //!< Bottom Right X-Coordinate of Area
    uint16_t y2; //!< Bottom Right Y-Coordinate of Area
    TOUCH_CALLBACK callback; //!< Callback which is executed when an event occurred in this Area.
    uint8_t flags; //!< For internal use, don't change, don't initialize
} TOUCH_AREA_STRUCT;


/**
 * Struct which represents a 2D point on the display
 */
typedef struct {
    uint16_t x; //!< The X-Coordinate of the point
    uint16_t y; //!< The Y-Coordinate of the point
} POINT_STRUCT;

/**
 * Initializes the Touch Controller.
 * Call this method before using any touch_* functions
 * @return true on success
 */
bool touch_init();

/**
 * Processes a native touch event.
 * Call this function when the pen goes down (\ref TOUCH_DOWN), when it moves (\ref TOUCH_DOWN) and also when it goes up again (\ref TOUCH_UP)!
 * It's safe to call this function from an (SPI)-Interrupt.
 * @param x The x-Coordinate of the touch event
 * @param y The y-Coordinate of the touch event
 * @param state Whether the pen is up or down
 * @return True on success
 */
bool touch_add_raw_event(uint16_t x, uint16_t y, TOUCH_STATE state);

/**
 * Checks whether or not we have memory to manage and track additional \p num TOUCH_AREA_STRUCT%s
 * @param num The number of touch areas you would like to allocate
 * @return True if there's enough memory to allocate num TOUCH_AREAs
 */
bool touch_have_empty(unsigned char num);

/**
 * Registers a new touch Area. You will receive events for this area from now on.
 * @param area A pointer to the configured TOUCH_AREA_STRUCT
 * @return True if everything was successful and the corresponding Touch Area will be monitored from now on
 */
bool touch_register_area(TOUCH_AREA_STRUCT* area);

/**
 * Unregisters a touch area. You will no longer receive events for this area
 * @param area A pointer to the TOUCH_AREA_STRUCT instance
 */
void touch_unregister_area(TOUCH_AREA_STRUCT* area);

/**
 * Gets the last touched point
 * @return The Coordinates of the last touched points
 */
POINT_STRUCT touch_get_last_point();

/**
 * Set's the new calibration values
 * @param xs x offset (to calibration point 1)
 * @param dx x difference (between calibration point 1 and 2)
 * @param ys y offset (to calibration point 1)
 * @param dy y difference (between calibration point 1 and 2)
 */
void touch_set_calibration_values(int xs, int dx, int ys, int dy);


/**
 * Set's the new value convert mode. Per default use_calibration is false.
 * @param use_calibration whether or not the current platform needs display calibration
 */
void touch_set_value_convert_mode(bool use_calibration);

/*@}*/

#endif /* TOUCH_H */
