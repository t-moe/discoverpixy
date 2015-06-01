#include "screen.h"

/* The idea is as follows:
 * We only call screen callbacks from the gui_screen_update() method, which is called from the applications main loop.
 * Instructions to switch the screen will be delayed until the gui_screen_update() method is called again.
 * This makes it safe to change the screen from an touch interrupt (e.g. button callback)
 */

static SCREEN_STRUCT* screen_list = NULL; //Head of the linked list which stores the screen history.
static SCREEN_STRUCT* screen_current = NULL; //Pointer to the current screen (= tail of the list)
static volatile SCREEN_STRUCT* screen_goto = NULL; //Screen we should navigate to once we enter the gui_screen_update() method again

SCREEN_STRUCT* gui_screen_get_current() {
	return screen_current;
}


void gui_screen_update() {
  	if(screen_goto!=NULL) { //we received the task to switch the screen
		SCREEN_STRUCT* go = (SCREEN_STRUCT*) screen_goto; //Backup volatile variable
		screen_goto=NULL; //reset the "goto instruction", since we're processing it now
		if(go->next!=NULL) { //The screen is not the last in the list, so we're going back
		    if(go->next!=screen_current) { //this condition should always be false
		    	return; //list corrupted?
		    }
			screen_current->on_leave(screen_current); //let the current screen free/unregister it's resources
       		go->next=NULL; //remove the current screen from the list
		} else { //we're going forward (to a new screen)
			if(screen_current!=NULL) { //this is not the first screen
					screen_current->on_leave(screen_current); //let the current screen free/unregister it's resources
					screen_current->next = go; //append the new screen to the end of the list
			} else { //first screen ever seen
					screen_list=go; //set the new screen as list-head
			}
		}
		go->on_enter(go); //let the new screen allocate/register it's resources
		screen_current = go; //the new screen is now the current screen. Transition done
	}		

	if(screen_current!=NULL) { //A screen has been set
		screen_current->on_update(screen_current); //Update current screen	
	}
}



bool gui_screen_navigate(SCREEN_STRUCT* screen) {
	if(screen==NULL || screen==screen_current || screen==screen_goto) { //invalid argument passed
		return false;
	}
	screen->next = NULL; //this will become the new tail of the list, so the next pointer must be NULL
	screen_goto=screen; //"send message" to main loop, to switch the screen
	return true;
}

bool gui_screen_back() {
	if(screen_list==NULL) { //the list head is emtpy, nothing to go back to
		return false;
	}
	SCREEN_STRUCT* current = screen_list;
	SCREEN_STRUCT* last = NULL;
	//Find second last element in list
	while(current->next != NULL) {
		last = current;
		current = current->next;
	}
	if(last==NULL) return false; //There's only a single screen, there's no going back here
	if(current!=screen_current) return false; //The last entry in the list is not the current screen. List corrupted?
	screen_goto=last; //"send message" to main loop, to switch the screen
	return true;	
}
