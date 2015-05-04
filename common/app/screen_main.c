#include "screen_main.h"
#include "screen_guitest.h"
#include "screen_pixytest.h"
#include "button.h"
#include "tft.h"

BUTTON_STRUCT b_guitest;
BUTTON_STRUCT b_pixytest;


static void b_guitest_cb(void* button) {
	gui_screen_navigate(get_screen_guitest());
}

static void b_pixytest_cb(void* button) {
	gui_screen_navigate(get_screen_pixytest());
}


static void enter(void* screen) {
	tft_clear(WHITE);

	//button to reach guitest
	b_guitest.base.x1=25; //Start X of Button
        b_guitest.base.y1=45; //Start Y of Button
        b_guitest.base.x2=AUTO; //b_guitest.base.x1+160; //Auto Calculate X2 with String Width
        b_guitest.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_guitest.txtcolor=WHITE; //Set foreground color
        b_guitest.bgcolor=HEX(0xDE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_guitest.font=0; //Select Font
        b_guitest.text="Gui/Tft Test"; //Set Text (For formatted strings take sprintf)
        b_guitest.callback=b_guitest_cb; //Call b_guitest_cb as Callback
        gui_button_add(&b_guitest); //Register Button (and run the callback from now on)

	//button to reach pixy test
        b_pixytest.base.x1=200; //Start X of Button
        b_pixytest.base.y1=45; //Start Y of Button
        b_pixytest.base.x2=AUTO; //b_pixytest.base.x1+160; //Auto Calculate X2 with String Width
        b_pixytest.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_pixytest.txtcolor=BLUE; //Set foreground color
        b_pixytest.bgcolor=HEX(0x10DE10); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_pixytest.font=0; //Select Font
        b_pixytest.text="Pixy Test"; //Set Text (For formatted strings take sprintf)
        b_pixytest.callback=b_pixytest_cb; //Call b_pixytest_cb as Callback
        gui_button_add(&b_pixytest); //Register Button (and run the callback from now on)



}

static void leave(void* screen) {
	gui_button_remove(&b_guitest);
	gui_button_remove(&b_pixytest);
}

static void update(void* screen) {
	//gui_button_redraw(&b_guitest); //only needed if button is overdrawn by others
}


static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_main() {
	return &screen;
}


