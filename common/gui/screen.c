#include "screen.h"


static SCREEN_STRUCT* screen_list = NULL;
static SCREEN_STRUCT* screen_current = NULL;
static volatile SCREEN_STRUCT* screen_goto = NULL;

SCREEN_STRUCT* gui_screen_get_current() {
	return screen_current;
}

void gui_screen_update() {
  	if(screen_goto!=NULL) { //we received the task to switch the screen
		SCREEN_STRUCT* go = (SCREEN_STRUCT*) screen_goto; //Backup volatile variable
		screen_goto=NULL;
		if(go->next!=NULL) { //we're going back
		        if(go->next!=screen_current) return; //list corrupted?
			screen_current->on_leave(screen_current);
       			go->next=NULL;
		} else { //we're going forward
			if(screen_current!=NULL) { //this is not the first screen
                		screen_current->on_leave(screen_current);
                		screen_current->next = go;
        		} else { //first screen ever seen
                		screen_list=go;
       			}
		}
        	go->on_enter(go);
       		screen_current =go;
	}		

	if(screen_current!=NULL) { //A screen has been set
		screen_current->on_update(screen_current); //Update current screen	
	}
}



bool gui_screen_navigate(SCREEN_STRUCT* screen) {
	if(screen==NULL) return false;
	screen->next = NULL;
	screen_goto=screen; //send message to main loop, to switch the screen
	return true;
}



bool gui_screen_back() {
	if(screen_list==NULL) return false;
	SCREEN_STRUCT* current = screen_list;
	SCREEN_STRUCT* last = NULL;
	//Find second last element in list
	while(current->next != NULL) {
		last = current;
		current = current->next;
	}
	if(last==NULL) return false; //There's only a single screen.
	if(current!=screen_current) return false; //List corrupted?
	screen_goto=last; //send message to main loop, to switch the screen
	return true;	
}







