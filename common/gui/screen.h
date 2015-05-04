#ifndef SCREEN_H
#define SCREEN_H


#include <stdio.h>
#include <stdbool.h>

typedef void (*SCREEN_CALLBACK)(void* screen);  //!< Function pointer used...

typedef struct SCREEN_S{
	SCREEN_CALLBACK on_enter;
	SCREEN_CALLBACK on_leave;
	SCREEN_CALLBACK on_update;


	struct SCREEN_S* next; //Used internally. do not modify

} SCREEN_STRUCT;


//Navigate to the given string as soon as the app enters the main loop again. Method can be called from an interrupt
bool gui_screen_navigate(SCREEN_STRUCT* screen);

//Navigate one screen back as soon as the app enters the main loop again.  Method can be called from an interrupt
bool gui_screen_back();

//Returns the current active screen
SCREEN_STRUCT* gui_screen_get_current();

//Updates/switches the screens. Call this from the app main loop, as fast as you can.
void gui_screen_update();

#endif /* SCREEN_H */
