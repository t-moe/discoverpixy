#include "screen_photomode_save.h"
#include "filesystem.h"
#include "button.h"
#include "tft.h"
#include "touch.h"
#include "pixy.h"
#include "pixy_helper.h"
#include <stdlib.h>
#include <string.h>


static BUTTON_STRUCT b_back; //Button to navigate back
static TOUCH_AREA_STRUCT a_area; //Touch area to select the save-file

//Callback for when the user presses the "back" button
static void b_back_cb(void* button) {
        gui_screen_back();
}

static int num_files_ok; //number of files into which we can write the image (size, flags ok)
static enum {init, error, showlist, picking, saving, done} state; //Current state of the screen state machine
static int fontheight; //The space between one line of text to the next
static int liststart; //The y-Coordinate of the Start of the File-List
static const char* picked_file; //The filename picked by the user, to save the image to

//Linked list structure to save all files which are suitable for saving.
typedef struct FILE_LIST_ENTRY_S{
	char* filename; //Name of the file
	struct FILE_LIST_ENTRY_S* next; //Pointer to the next entry in the list or NULL
} FILE_LIST_ENTRY;

static FILE_LIST_ENTRY* files_ok; //Pointer to the head of the list

//Callback for when the user selects a file to save the image into
static void touchCB(void* touchArea, TOUCH_ACTION triggeredAction) {

	int y = touch_get_last_point().y-liststart; //Calculate the y-Coordinate of the touch point relative to the start of the file-list
	int elem = y/fontheight; //Calculate the file index
	if(elem<0 | elem>= num_files_ok) return; //Check if the file index is valid  (0,1,..,num_files_ok-1)

	//Search for the corresponding entry in the linked list
	FILE_LIST_ENTRY* current_entry = files_ok; //Start walking through the list, starting by the head of the list
	for(int i=0; i<elem; i++) { //Until we have reached the file (index)
		current_entry= current_entry->next; //traverse to the next file
	}

	picked_file = current_entry->filename; //save the picked filename. It will be used by the statemachine in the main loop
	touch_unregister_area(&a_area); //unregister the touch area, we no longer need it. No more interrupts will be fired.
	state=saving; //Change the state of the  statemachine
}

//Text-Lines to show if we have no matching files (num_files_ok==0)
static const char* nomatch_text [] = {
	"Due to limitations of the filesystem",
	"implementation you can only write to",
	"existing files.",
	"",
	"The files need to have a .bmp",
	"extension and must be at least",
	"189410 bytes (185kb) large.",
	"Unfortunately there were no such",
	"files found in the root directory.",
	"",
	"Please create some files and come",
	"back again.",
	NULL
};


//Bitmap header for a 318x198x24bit windows bitmap. data starts at 0x7A (= after this header)
//This header has been taken from a white bitmap saved with gimp.
//Wikipedia has a pretty good description on the header: http://de.wikipedia.org/wiki/Windows_Bitmap
static unsigned char bmpheader_data[0x7A] = {
		  0x42, 0x4d, 0xe2, 0xe3, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x7a, 0x00,
		  0x00, 0x00, 0x6c, 0x00, 0x00, 0x00, 0x3e, 0x01, 0x00, 0x00, 0xc6, 0x00,
		  0x00, 0x00, 0x01, 0x00, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x68, 0xe3,
		  0x02, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x13, 0x0b, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x42, 0x47, 0x52, 0x73, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00,
		  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
		  0x00, 0x00
};

//Callback for when the screen is entered/loaded
static void enter(void* screen) {
	tft_clear(WHITE);


	#define X_OFS 5

	//Back button
	b_back.base.x1=X_OFS; //Start X of Button
	b_back.base.y1=210; //Start Y of Button
	b_back.base.x2=AUTO; //Auto Calculate X2 with String Width
	b_back.base.y2=AUTO; //Auto Calculate Y2 with String Height
	b_back.txtcolor=WHITE; //Set foreground color
	b_back.bgcolor=HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
	b_back.font=0; //Select Font
	b_back.text="Back"; //Set Text (For formatted strings take sprintf)
	b_back.callback=b_back_cb; //Call b_back_cb as Callback
	gui_button_add(&b_back); //Register Button (and run the callback from now on)

	state =init; //Start with the init state
	fontheight = tft_font_height(0)+2; //Save the height of the used font, for fast access
	files_ok = NULL; //initialize the linked list with 0 elements
	num_files_ok = 0; //we have zero! elements
}

//Callback for when the screen should be updated
//This is the main loop of the screen. This method will be called repeatedly
static void update(void* screen) {
	switch(state) {
		case init: //Init State: The user just entered the screen
		{
			DIRECTORY_STRUCT* dir = filesystem_dir_open("."); //open root directory
			if(dir==NULL) { //error while opening root directory
				tft_print_line(X_OFS,5,BLACK,TRANSPARENT,0,"Error accessing Filesystem");
				state=error;
				break;
			}

			bool nomatch= true; //whether or not we have zero files which are suitable for saving

			for(int i=0; i<dir->num_files; i++) { //walk through all files in the directory
					FILE_STRUCT* file = &(dir->files[i]); //Pointer to the current file/subdirectory

					//Ignore directories, archives, hidden files, system files and files we cannot write to
					if(file->fattrib&(F_SYS|F_HID|F_ARC|F_DIR|F_RDO)) continue;

					//ignore files which are not large enough
					if(file->fsize<189410) continue; //size taken from an example bitmap (318x198x24)

					nomatch=false; //at least one file matches
					break;
			}

			if(nomatch) { //not one file is suitable for writing
				int y=5; //y-Coordinate where to start writing the error text
				int i=0;
				while(nomatch_text[i]!=NULL) { //for every line in the big error array
					//Write the line's text and go to the next line
					tft_print_line(X_OFS,y+i*fontheight,BLACK,TRANSPARENT,0,nomatch_text[i]);
					i++;
				}
				state = error;
			} else { //we have a least one suitable file
				state = showlist;
			}

			filesystem_dir_close(dir); //free directory struct
		}
		break;

		case showlist: //Show List State: Where we load and present the suitable file's to the user in a list
		{
			DIRECTORY_STRUCT* dir2 = filesystem_dir_open("."); //Open the directory again
			if(dir2==NULL) return; //Error on opening? This should never happen, since it's handled in the previous state

			int y = 5; //y-Coordinate where to start drawing/writing text/list-elements

			tft_print_line(X_OFS,y,BLACK,TRANSPARENT,0,"Pick a file to save the image to");
			y+=fontheight+5;

			tft_print_line(X_OFS,y,BLUE,TRANSPARENT,0,"Name             Modified          Size");
			y+=fontheight;

			liststart = y; //store the y coordinate of the start of the list away (used in toucharea callback)
			num_files_ok = 0; //we start with 0 matching files

			FILE_LIST_ENTRY* current_entry = NULL; //We start with an empty list
			for(int i=0; i<dir2->num_files && num_files_ok<10; i++) { //go through all the files of the directory, abort if we have 10 matches
					FILE_STRUCT* file = &(dir2->files[i]);

					//Ignore directories, archives, hidden files, system files and files we cannot write to
					if(file->fattrib&(F_SYS|F_HID|F_ARC|F_DIR|F_RDO)) continue;

					//ignore files which are not large enough
					if(file->fsize<189410) continue; //size taken from an example bitmap (318x198x24)

					//Print out filename, modified date,time and file size
					tft_print_formatted(X_OFS,y,BLACK,
								TRANSPARENT,0,"%-16s %02u.%02u.%02u %02u:%02u:%02u %u",
								file->fname,
								file->fdate.day,
								file->fdate.month,
								(file->fdate.year+1980)%100,
								file->ftime.hour,
								file->ftime.min,
								file->ftime.sec*2,
								file->fsize);

					if(current_entry==NULL) { //The list is empty
						current_entry = malloc(sizeof(FILE_LIST_ENTRY)); //create new entry
						files_ok = current_entry; //assign it to the list head
					} else { //there's a least one entry in the list
						current_entry->next = malloc(sizeof(FILE_LIST_ENTRY)); //append entry to previous entry
						current_entry = current_entry->next; //newly created entry is the current now.
					}
					current_entry->next = NULL; //we're at the end of the list (for now)
					current_entry->filename = malloc(strlen(file->fname)+1); //allocate space for the filename + zero-termination
					strcpy(current_entry->filename,file->fname); //copy filename (so that we can close the directory after scanning)

					//since we have found a suitable file we need to increment the position in the list
					num_files_ok++;
					y+=fontheight;
			}

			//Touch area for file-selection (in the list)
			a_area.hookedActions = PEN_UP; //we're only interested in PEN_UP events
			a_area.x1 = X_OFS; //Left border
			a_area.y1 = liststart; //Start where the list started
			a_area.x2 = 320-X_OFS; //Right border
			a_area.y2 = liststart+fontheight*num_files_ok; //stop at the end of the list
			a_area.callback = touchCB; //execute our callback when PEN_UP occurs
			touch_register_area(&a_area); //register the touch area and receive events from now on

			filesystem_dir_close(dir2); //we no longer need the directory struct, since we have our own linked list now

			state=picking;
		}
		break;

		case picking: //Picking State: Where we wait on the users file choice
			pixy_service(); //Handle pending pixy events
			//do nothing and wait on user to pick a file
			break;

		case saving: //Saving State: Where we save the image to the selected file
		{
			FILE_HANDLE* file = filesystem_file_open(picked_file); //try to open the selected file
			if(file==NULL) { //opening the file failed
				tft_print_formatted(X_OFS,190,BLUE,TRANSPARENT,0,"Could not open %s",picked_file);
				state=error;
				break;
			}

			filesystem_file_seek(file,0); //seek to the start of the file (optional?)
			if(filesystem_file_write(file,bmpheader_data,0x7A)!=F_OK) { //Writing the header failed
				tft_print_formatted(X_OFS,190,BLUE,TRANSPARENT,0,"Error while writing to %s",picked_file);
				filesystem_file_close(file);
				state=error;
				break;
			}

			if(pixy_save_full_frame(file)!=0) { //Writing the imagedata failed
				tft_print_formatted(X_OFS,190,BLUE,TRANSPARENT,0,"Error while writing to %s",picked_file);
				filesystem_file_close(file);
				state=error;
				break;
			}

			//if we reach this point, we have written all data out successfully

			filesystem_file_close(file); //close/finalize the file
			tft_print_formatted(X_OFS,190,BLUE,TRANSPARENT,0,"Image saved to %s",picked_file);
			state = done;
		}
		break;

		case error: //Error State: Where we show an error message and leave the user no other choice than to click the backbutton
		case done: //Done State: When saving the file was successful
			pixy_service(); //Handle pending pixy events
			//wait on user to click the back button
			break;

	}
}

//Callback for when the screen is left/unloaded
static void leave(void* screen) {
	gui_button_remove(&b_back); //Remove/Free the back button

	if(state==picking){ //The user left the screen in the "picking"-phase
		touch_unregister_area(&a_area); //remove the touch area (for the list)
	}

	if(state==picking|| state==saving || state==done) { //the user left the screen after we created the linked list
		//Iterate through the linked list and free all resources
		FILE_LIST_ENTRY* current_entry = files_ok; //start with the list head
		while(current_entry!=NULL) { //while we're not at the end
			FILE_LIST_ENTRY* temp = current_entry->next; //save the next pointer because we free the current element on the next line
			free((void*)(current_entry->filename)); //free filename
			free(current_entry); //free element itself
			current_entry= temp; //advance
		}
	}
}

//Declare screen callbacks
static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};

SCREEN_STRUCT* get_screen_photomodesave() {
	return &screen;
}
