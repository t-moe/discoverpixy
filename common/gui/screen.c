#include "screen.h"


static SCREEN_STRUCT* screen_list = NULL;
static SCREEN_STRUCT* screen_current = NULL;


SCREEN_STRUCT* gui_screen_get_current() {
	return screen_current;
}

void gui_screen_update() {
	if(screen_current!=NULL) {
		screen_current->on_update(screen_current);	
	}
}



bool gui_screen_navigate(SCREEN_STRUCT* screen) {
	if(screen==NULL) return false;
	screen->next = NULL;
	if(screen_current!=NULL) {
		screen_current->on_leave(screen_current);
		screen_current->next = screen;
	} else {
		screen_list=screen;
	}
	screen->on_enter(screen);
	screen_current = screen;	
	return true;
}



bool gui_screen_back() {
	if(screen_list==NULL) return false;
	SCREEN_STRUCT* current = screen_list;
	SCREEN_STRUCT* last = NULL;
	while(current->next != NULL) {
		last = current;
		current = current->next;
	}
	if(last==NULL) return false; //There's only a single screen.
	if(current!=screen_current) return false; //List corrupted?
	current->on_leave(current);
	last->next=NULL;
	last->on_enter(last);
	screen_current=last;	
	return true;
}







