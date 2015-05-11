#ifndef NUMUPDOWN_H
#define NUMUPDOWN_H

/**
 * @addtogroup gui
 */
/*@{*/

/**
 * @defgroup numupdown NummericUpDown
 * The NummericUpDown Gui Element
 */
/*@{*/


#include "button.h"

typedef void (*NUMUPDOWN_CALLBACK)(void *numupdown, int16_t value);	//!< Function pointer used...
typedef struct {
	uint16_t x;
	uint16_t y;
	uint16_t fgcolor;
	int16_t value;
	int16_t min;
	int16_t max;
	NUMUPDOWN_CALLBACK callback; //Callback
	
	//Internally used:
	BUTTON_STRUCT buttonUp;
	BUTTON_STRUCT buttonDown;
} NUMUPDOWN_STRUCT;


bool gui_numupdown_add(NUMUPDOWN_STRUCT* numupdown);
void gui_numupdown_remove(NUMUPDOWN_STRUCT* numupdown);
void gui_numupdown_update(NUMUPDOWN_STRUCT* numupdown);
void gui_numupdown_redraw(NUMUPDOWN_STRUCT* numupdown);

/*@}@}*/

#endif /* NUMUPDOWN_H */
