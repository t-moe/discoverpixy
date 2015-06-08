/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_photomode.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-05-15	timolang@gmail.com	27c09ba	Redesigned main menu. Moved stuff from pixytest to a new helper file and to the new "photo mode"-screen.
* 2015-05-16	timolang@gmail.com	62006e0	Documented pixy_helper and implemented/finished photo-mode screens! Snap some shots!
* 2015-06-07	timolang@gmail.com	c87220d	Renamed pixy_helper to pixy_frame. Updated docu of appliaction. added doxygen comments to pixy_{frame,control}.h
*
**************************************************************************************************************************************/

#include "screen_photomode.h"
#include "screen_photomode_save.h"
#include "button.h"
#include "tft.h"
#include "touch.h"
#include "pixy.h"
#include "system.h"
#include "pixy_frame.h"

static bool pixy_connected = false; //Whether or not the pixy cam is currently connected

static BUTTON_STRUCT b_back; //Button to navigate back
static BUTTON_STRUCT b_save; //Button to save the current image
static TOUCH_AREA_STRUCT a_area; //Touch Area, where the frame is drawn. Used to drag the image around
static bool subMenu = false; //Whether or not we left the current screen for a submenu

//Callback for when the user presses the "back" button
static void b_back_cb(void* button)
{
    subMenu = false; //we're not entering a submenu
    gui_screen_back(); //navigate back to the previous screen
}

//Callback for when the user presses the "save" button
static void b_save_cb(void* button)
{
    subMenu = true; //we're entering a submenu
    gui_screen_navigate(get_screen_photomodesave()); //navigate to the save screen
}

static POINT_STRUCT pixy_pos; //The current position of pixy's servos
static POINT_STRUCT old_pos; //The last touch position on the screen

//Callback for when the user drags the image around
static void touchCB(void* touchArea, TOUCH_ACTION triggeredAction)
{
    POINT_STRUCT p = touch_get_last_point(); //get the last touched point

    switch (triggeredAction) {
    case PEN_ENTER:
    case PEN_DOWN:
        old_pos = p; //If the user "newly" enters the touch area, we set the "last" position to the current
        break;

    case PEN_MOVE: { //the user is moving around, he entered the screen a while ago (old_pos is set)
        int16_t deltaX = p.x - old_pos.x; //Calculate x difference between last and current touch
        int16_t deltaY = p.y - old_pos.y; //Calculate y difference between last and current touch
        old_pos = p; //store the current touch point for the next time

        //printf("%d %d\n",deltaX,deltaY);
        if (pixy_connected) {
            //Calculate new servo coordinates. 2 is just a proportional factor
            int16_t new_x = pixy_pos.x + deltaX * 2;
            int16_t new_y = pixy_pos.y - deltaY * 2;

            //check limits
            if (new_x < 0) {
                new_x = 0;
            }

            if (new_x > 1000) {
                new_x = 1000;
            }

            if (new_y < 0) {
                new_y = 0;
            }

            if (new_y > 1000) {
                new_y = 1000;
            }

            //set pixy_pos so that the main routine can send it to the servos
            pixy_pos.x = new_x;
            pixy_pos.y = new_y;
        }
    }
    break;

    case PEN_UP:
    case PEN_LEAVE:
        //printf("Leave/up\n");
        break;

    default:
        break;
    }

}

//Callback for when the screen is entered/loaded
static void enter(void* screen)
{
    tft_clear(WHITE);

    tft_print_line(5, 5, BLACK, TRANSPARENT, 0, "Drag the image around and ");

    //Back button
    b_back.base.x1 = 5; //Start X of Button
    b_back.base.y1 = 19; //Start Y of Button
    b_back.base.x2 = AUTO; //Auto Calculate X2 with String Width
    b_back.base.y2 = AUTO; //Auto Calculate Y2 with String Height
    b_back.txtcolor = WHITE; //Set foreground color
    b_back.bgcolor = HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
    b_back.font = 0; //Select Font
    b_back.text = "Back"; //Set Text (For formatted strings take sprintf)
    b_back.callback = b_back_cb; //Call b_back_cb as Callback
    gui_button_add(&b_back); //Register Button (and run the callback from now on)

    //Save button
    b_save.base.x1 = 190;
    b_save.base.y1 = 3;
    b_save.base.x2 = AUTO;
    b_save.base.y2 = AUTO;
    b_save.txtcolor = WHITE;
    b_save.bgcolor = HEX(0x1010AE);
    b_save.font = 0;
    b_save.text = "Save it!";
    b_save.callback = b_save_cb;
    gui_button_add(&b_save);

    //Frame Coordinates: topleft = (1,40); bottomright = (318,238)
    //Leave a 10px border for the area

    //Area to drag the image around
    a_area.hookedActions =  PEN_DOWN | PEN_MOVE |  PEN_ENTER | PEN_UP | PEN_LEAVE;
    a_area.x1 = 11;
    a_area.y1 = 50;
    a_area.x2 = 308;
    a_area.y2 = 228;
    a_area.callback = touchCB;
    touch_register_area(&a_area);

    //Pixy stuff
    pixy_connected = (pixy_init() == 0); //try to connect to pixy

    if (pixy_connected && !subMenu) { //pixy is connected, but we are not coming from a submenu
        pixy_pos.x = pixy_pos.y = 500; //reset servo positions to center
    }
}

//Callback for when the screen is left/unloaded
static void leave(void* screen)
{
    //remove buttons and touch area.
    gui_button_remove(&b_back);
    gui_button_remove(&b_save);
    touch_unregister_area(&a_area);
}

//Callback for when the screen should be updated
//This is the main loop of the screen. This method will be called repeatedly
static void update(void* screen)
{
    //Note: The only way to detect that pixy has been disconnected is if a command fails. There's no pixy_is_connected method yet :'(

    if (!pixy_connected) { //Pixy not connected
        pixy_close(); //Ensure that all pixy resources are freed (failsafe)

        if (pixy_init() == 0) { //try to connect to pixy
            pixy_connected = true;

            if (!subMenu) { //we're not coming from a submenu
                pixy_pos.x = pixy_pos.y = 500; //reset servo positions to center
            }

            printf("pixy (re)initialized\n");
        }
    }

    if (pixy_connected) { //If we are connected (now)
        pixy_service(); //Handle pending pixy events (e.g. color info retrival)

        pixy_render_full_frame(1, 40); //render the pixy video at point (1,40)

        //set the servo positions to the coordinates form the touch interrupt
        pixy_rcs_set_position(0, pixy_pos.x);
        pixy_rcs_set_position(1, pixy_pos.y);
    }
}

//Declare screen callbacks
static SCREEN_STRUCT screen = {
    enter,
    leave,
    update
};


SCREEN_STRUCT* get_screen_photomode()
{
    return &screen;
}
