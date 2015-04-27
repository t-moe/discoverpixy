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

bool gui_screen_navigate(SCREEN_STRUCT* screen);
bool gui_screen_back();
SCREEN_STRUCT* gui_screen_get_current();
void gui_screen_update();

#endif /* SCREEN_H */
