#include "screen_tracking.h"
#include "pixy_control.h"
#include "button.h"
#include "checkbox.h"
#include "tft.h"
#include "touch.h"
#include "pixy.h"
#include "system.h"
#include "pixy_helper.h"

static BUTTON_STRUCT b_back; //Button to navigate back
static BUTTON_STRUCT b_select; //Button to start the color region selection
static CHECKBOX_STRUCT c_frame_toggle; //Checkbox to toggle video data on/off
static TOUCH_AREA_STRUCT a_area; //Touch area for the color region selection

//Callback for when the user presses the "back" button
static void b_back_cb(void* button) {
        gui_screen_back(); //navigate back to the previous screen
}

static volatile bool frame_visible = false; //Whether or not the video data should be displayed
static void c_frame_toggle_cb(void *checkbox, bool checked) {
	frame_visible=checked; //Set the visibility of the frame to the checked state of the checkbox
	//Frame will be drawn in the main loop below
}

static enum {detecting, init, tracking, preselecting, abortselecting, selecting, selected, error} state; //Current state of the screen state machine

static POINT_STRUCT point1; //First point of the rectangle selected by the user (color region selection)
static POINT_STRUCT point2; //End point of the rectangle selected by the user (color region selection)
static bool point1_valid; //Whether or not we have a valid first point

//Callback for when the user presses the "select color" button
static void b_select_cb(void* button) {
	if(state==selecting) { //we're currently selecting a color region
		state = abortselecting; //Abort selecting!!
	} else if (state==tracking) { //we're currently watching the tracking
		state = preselecting; //start selecting
	}
}

//Video Region properties
//The camera records with 320*200px, but we need to keep a 1px border because of color interpolation (bayer format)
#define FRAME_START_X 1 //x-Coordinate of the top-left point of the frame rectangle on display
#define FRAME_START_Y 41 //y-Coordinate of the top-left point of the frame rectangle on display
#define FRAME_WIDTH 318 //Width of the video frame
#define FRAME_HEIGHT 198 //Height of the video frame
#define FRAME_END_X FRAME_START_X +FRAME_WIDTH-1 //x-Coordinate of the bottom-right point of the frame rectangle
#define FRAME_END_Y FRAME_START_Y +FRAME_HEIGHT-1 //y-Coordinate of the bottom-right point of the frame rectangle

//Callback for when the user touches the frame area to select a color region.
//Note: It doesn't matter in which direction the user draws the rectangle, we'll normalize the coordinates later
static void touchCB(void* touchArea, TOUCH_ACTION triggeredAction) {
	POINT_STRUCT p = touch_get_last_point();
	switch(triggeredAction) {
		case PEN_DOWN: //The user just put down the pen
			point1.x = p.x-FRAME_START_X; //Calculate x-Coordinate relative to frame start
			point1.y = p.y-FRAME_START_Y; //Calculate y-Coordinate relative to frame start
			point1_valid= true; //The point1 is now valid
			break;
		case PEN_UP: //The user took the pen away
			if(point1_valid) { //only execute if point1 is valid
				point2.x = p.x-FRAME_START_X;  //Calculate x-Coordinate relative to frame start
				point2.y = p.y-FRAME_START_Y; //Calculate y-Coordinate relative to frame start
				state = selected;
			}
			break;
	}
}

//Prototype for tracking start/stop methods
typedef void (*TRACKING_VOID_CALLBACK)(void* tracking_config);
//Prototype for tracking update method
typedef void (*TRACKING_BLOCK_CALLBACK)(void* tracking_config, struct Block* blocks, int num_blocks );

//Structure to save callbacks and settings of a tracking implementation
typedef struct {
	TRACKING_VOID_CALLBACK start;
	TRACKING_VOID_CALLBACK stop;
	TRACKING_BLOCK_CALLBACK update;
} TRACKING_CONFIG_STRUCT;

//Methods for our tracking implementation ahead
static int16_t servo_x = 0;
static int16_t servo_y = 0;

//Method/Callback to start our tracking
void tracking_our_start(void* tracking_config) {
	//Activate pixy's data send program
	int32_t response;
	int return_value;

    servo_x = servo_y = 500;                // set a default value of 500
    pixy_rcs_set_position(0, servo_x);      //  
    pixy_rcs_set_position(1, servo_y);      //

	return_value = pixy_command("runprog", INT8(0), END_OUT_ARGS,  &response, END_IN_ARGS);
}

//Method/Callback to stop our tracking
void tracking_our_stop(void* tracking_config) {
	//Stop pixy's data send programm
	int32_t response;
	int return_value;
	return_value = pixy_command("stop", END_OUT_ARGS,  &response, END_IN_ARGS);
}

//Method/Callback to calculate one step of our tracking
void tracking_our_update(void* tracking_config, struct Block* blocks, int num_blocks) {

    if(num_blocks <= 0){                    // Check if there are blocks available
        return;                             // When there are none, do nothing
    }
    
    uint16_t x = blocks[0].x;               // Get x coordinate of the biggest object
    uint16_t y = blocks[0].y;               // Get y coordinate of the biggest object

    int16_t xset = 0;                       
    int16_t yset = 0;

    xset = (servo_x + pixy_PID_X((FRAME_WIDTH / 2), x));    // calculate the PID output for x
    yset = (servo_y - pixy_PID_Y((FRAME_HEIGHT / 2), y));   // calculate the PID output for y
    
    xset = (xset < 0) ? 0 : xset;           // x lower boundary check
    xset = (xset > 1000) ? 1000 : xset;     // x upper boundary check
    
    yset = (yset < 0) ? 0 : yset;           // y lower boundary check
    yset = (yset > 1000) ? 1000 : yset;     // y upper boundary check

    servo_x = xset;                         // update the global, static variable for x
    servo_y = yset;                         // update the global, statuc variable for y

    pixy_rcs_set_position(0, servo_x);      // set the new x position
    pixy_rcs_set_position(1, servo_y);      // set the new y position
}

//Variable which stores all the callbacks and settings for our tracking implementation
static TRACKING_CONFIG_STRUCT tracking_our = {
		tracking_our_start,
		tracking_our_stop,
		tracking_our_update
};

//Methods for reference tracking implementation ahead

//Method/Callback to start reference tracking
void tracking_reference_start(void* tracking_config) {
	//Run reference tracking
	int32_t response;
	int return_value;
	return_value = pixy_command("runprog", INT8(2), END_OUT_ARGS,  &response, END_IN_ARGS);
}

//Method/Callback to stop reference tracking
void tracking_reference_stop(void* tracking_config) {
	//Stop reference tracking
	int32_t response;
	int return_value;
	return_value = pixy_command("stop", END_OUT_ARGS,  &response, END_IN_ARGS);
}

//Method/Callback to calculate one step of the reference tracking
void tracking_reference_update(void* tracking_config, struct Block* blocks, int num_blocks) {
	//Nothing to do here. Pixy does it all.
}

//Variable which stores all the callbacks and settings for the reference tracking implementation
static TRACKING_CONFIG_STRUCT tracking_reference = {
		tracking_reference_start,
		tracking_reference_stop,
		tracking_reference_update
};

//Pointer to the currently active tracking implementation. See also tracking_set_mode
static TRACKING_CONFIG_STRUCT* tracking_current;

//Method to set the current tracking implementation. This function is exported and should be called before getting the screen
void tracking_set_mode(enum Tracking_Implementation impl) {
	//Depending on the enum value let tracking_current point to a different setting/callback structure
	switch(impl) {
		case OUR_TRACKING:
			tracking_current = &tracking_our;
			break;
		case REFERENCE_TRACKING:
			tracking_current = &tracking_reference;
			break;
		default:
			tracking_current=NULL;
			break;
	}
}

//Callback for when the screen is entered/loaded
static void enter(void* screen) {
	tft_clear(WHITE);
	
	//"Back" button
	b_back.base.x1=5; //Start X of Button
	b_back.base.y1=5; //Start Y of Button
	b_back.base.x2=AUTO; //Auto Calculate X2 with String Width
	b_back.base.y2=AUTO; //Auto Calculate Y2 with String Height
	b_back.txtcolor=WHITE; //Set foreground color
	b_back.bgcolor=HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
	b_back.font=0; //Select Font
	b_back.text="Back"; //Set Text (For formatted strings take sprintf)
	b_back.callback=b_back_cb; //Call b_back_cb as Callback
	gui_button_add(&b_back); //Register Button (and run the callback from now on)


	//"Select color" button
	b_select.base.x1=150;
	b_select.base.y1=5;
	b_select.base.x2=AUTO;
	b_select.base.y2=AUTO;
	b_select.txtcolor=WHITE;
	b_select.bgcolor=HEX(0xAE1010);
	b_select.font=0;
	b_select.text="Select Color";
	b_select.callback=b_select_cb;
	gui_button_add(&b_select);

	//"Frame visible" checkbox
	c_frame_toggle.base.x1 = 50;
	c_frame_toggle.base.x2 = 50+16;
	c_frame_toggle.base.y1 = 5;
	c_frame_toggle.base.y2 = 5+16;
	c_frame_toggle.checked = frame_visible;
	c_frame_toggle.fgcolor = CHECKBOX_WIN_FG_COLOR;
	c_frame_toggle.callback = c_frame_toggle_cb;
	gui_checkbox_add(&c_frame_toggle);
	tft_print_line(73,8,BLACK,TRANSPARENT,0,"Show Video");


	//Area to select a "color region"
	a_area.hookedActions =  PEN_DOWN | PEN_UP;
	a_area.x1 = FRAME_START_X;
	a_area.y1 = FRAME_START_Y;
	a_area.x2 = FRAME_END_X;
	a_area.y2 = FRAME_END_Y;
	a_area.callback = touchCB;
    //Do not register it here, we do that later

	if(tracking_current==NULL) {
		state = error;
	} else {
		state = detecting; //Start with the detecting state
	}
}

//Callback for when the screen is left/unloaded
static void leave(void* screen) {
	//Remove buttons and checkbox
	gui_button_remove(&b_back);
	gui_button_remove(&b_select);
	gui_checkbox_remove(&c_frame_toggle);

	if(state==selecting) { //the user left the screen in the "selecting" phase
		touch_unregister_area(&a_area); //remove the touch area
	}

	if(state==tracking) { //the user left the screen in the "tracking" phase
		tracking_current->stop(tracking_current); //stop tracking
		pixy_led_set_RGB(0,0,0);
	}
}

//Callback for when the screen should be updated
//This is the main loop of the screen. This method will be called repeatedly
static void update(void* screen) {
	switch(state) {
		case detecting: //Detecting State: Where we try to connect to the pixy
			if(pixy_init()==0) { //Pixy connection ok
				state = init; //Go to next state
			}
			break;

		case init: //Init State: Where we start the tracking
			tracking_current->start(tracking_current);
			state=tracking;
		break;

		case tracking: //Tracking state: Where we render the frame and the tracked objects
			pixy_service(); //Receive events (e.g. block-data) from pixy

			if(pixy_blocks_are_new()) { //There are new blocks available
				if(frame_visible) { //If the user want's us to draw the video data
					pixy_render_full_frame(FRAME_START_X,FRAME_START_Y);
				} else { //the user want's a colored background
					tft_fill_rectangle(FRAME_START_X,FRAME_START_Y,FRAME_END_X,FRAME_END_Y,RGB(200,200,200));
				}

				#define BLOCK_BUFFER_SIZE 5 //The maximum amount of blocks that we want to receive
				struct Block blocks[BLOCK_BUFFER_SIZE]; //Storage to receive blocks from pixy
				int blocks_received= pixy_get_blocks(BLOCK_BUFFER_SIZE,blocks); //Try to receive up to BLOCK_BUFFER_SIZE Blocks from pixy

				if(blocks_received>=0) { //block receiving ok
					tracking_current->update(tracking_current,blocks,blocks_received); //apply tracking

					//Draw blocks
					for(int i=0; i<blocks_received; i++) { //for each received block
						struct Block* block = &(blocks[i]);
						//block.x and block.y are the center coordinates of the object relative to the camera origin.
						uint16_t x = block->x-1+FRAME_START_X -block->width/2; //Calculate x-Coordinate on the display
						uint16_t y = block->y-1+FRAME_START_Y -block->height/2; //Calculate y-Coordinate on the display
						tft_draw_rectangle(x,y,x+block->width-1, y+block->height-1,WHITE); //Draw a white rectangle
					}
				}
			}
			break;

		case preselecting: //Pre-Selecting State: Where we set up the color region selection
		{
			tracking_current->stop(tracking_current); //Stop tracking

			pixy_render_full_frame(FRAME_START_X,FRAME_START_Y); //Render one frame

			touch_register_area(&a_area); //Register touch area and receive events from now on
			point1_valid=false; //we start with an invalid point1

			b_select.text="Abort"; //Change the button text to "Abort"
			gui_button_redraw(&b_select); //redraw button

			state = selecting; //The user can now select a region
		}
		break;

		case selected: //Selected State: Where we send the users selection to pixy
		{
			//Ensure that (x1,y1) represent the top-left point and (x2,y2) the bottom-right.
			unsigned int tmp;
			if(point1.x > point2.x){
				tmp = point1.x;
				point1.x = point2.x;
				point2.x = tmp;
			}

			if(point1.y > point2.y){
				tmp = point1.y;
				point1.y = point2.y;
				point2.y = tmp;
			}
			//Send pixy the selected region
			pixy_cc_set_region(1,point1.x,point1.y,point2.x-point1.x,point2.y-point1.y);
		}
		//no break here: We want the following code to be executed as well

		case abortselecting: //Abort-Selecting State: Where we deinitialize the stuff we used for region selection
		{
			touch_unregister_area(&a_area); //Remove the touch area. We'll no longer receive touch events

			b_select.text="Select Color"; //Change the button text back to "Select Color"
			gui_button_redraw(&b_select); //redraw button

			tracking_current->start(tracking_current); //Start tracking again
			state=tracking;
		}
		break;

		case selecting: //Selecting State: Where we wait on the user to select a color region
			pixy_service(); //receive pixy events
			//wait on user to select the image area
			break;

		case error: //Error State: Where we show an error message and leave the user no other choice than to click the backbutton
			//wait on user to click the back button
			break;
	}
}

//Declare screen callbacks
static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_tracking() {
	return &screen;
}
