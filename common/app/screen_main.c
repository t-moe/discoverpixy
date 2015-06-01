#include "screen_main.h"
#include "screen_guitest.h"
#include "screen_pixytest.h"
#include "screen_filetest.h"
#include "screen_photomode.h"
#include "screen_tracking.h"
#include "button.h"
#include "tft.h"
#include "filesystem.h"

BUTTON_STRUCT b_guitest;
BUTTON_STRUCT b_pixytest;
BUTTON_STRUCT b_filetest;

BUTTON_STRUCT b_our_tracking;
BUTTON_STRUCT b_ref_tracking;
BUTTON_STRUCT b_photo_mode;


static void b_our_tracking_cb(void* button) {
	tracking_set_mode(OUR_TRACKING);
	gui_screen_navigate(get_screen_tracking());
}

static void b_ref_tracking_cb(void* button) {
	tracking_set_mode(REFERENCE_TRACKING);
	gui_screen_navigate(get_screen_tracking());
}

static void b_photo_mode_cb(void* button) {
	gui_screen_navigate(get_screen_photomode());
}

static void b_guitest_cb(void* button) {
	gui_screen_navigate(get_screen_guitest());
}

static void b_filetest_cb(void* button) {
	gui_screen_navigate(get_screen_filetest());
}

static void b_pixytest_cb(void* button) {
	gui_screen_navigate(get_screen_pixytest());
}


static void enter(void* screen) {
	tft_clear(WHITE);

	//Heading
	tft_print_line(10,10,BLUE,TRANSPARENT,1,"Discoverpixy");
	tft_draw_line(0,40,319,40,BLACK);

    #define X_TAB 			90
	#define BUTTON_SPACING 	7

	//First line of buttons
	#define Y_FIRST 		60
	tft_print_line(10,Y_FIRST,BLACK,TRANSPARENT,0,"Tracking:");

	b_our_tracking.base.x1=X_TAB; //Start X of Button
	b_our_tracking.base.y1=Y_FIRST-3; //Start Y of Button
	b_our_tracking.base.x2=AUTO; //Auto Calculate X2 with String Width
	b_our_tracking.base.y2=AUTO; //Auto Calculate Y2 with String Height
	b_our_tracking.txtcolor=WHITE; //Set foreground color
	b_our_tracking.bgcolor=HEX(0xE30535); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
	b_our_tracking.font=0; //Select Font
	b_our_tracking.text="Our Tracking"; //Set Text (For formatted strings take sprintf)
	b_our_tracking.callback=b_our_tracking_cb; //Call b_our_tracking when the button get's pressed
	gui_button_add(&b_our_tracking); //Register Button (and run the callback from now on)


	b_ref_tracking.base.x1=b_our_tracking.base.x2+BUTTON_SPACING;
	b_ref_tracking.base.y1=Y_FIRST-3;
	b_ref_tracking.base.x2=AUTO;
	b_ref_tracking.base.y2=AUTO;
	b_ref_tracking.txtcolor=WHITE;
	b_ref_tracking.bgcolor=HEX(0xFF2151);
	b_ref_tracking.font=0;
	b_ref_tracking.text="Reference Tracking";
	b_ref_tracking.callback=b_ref_tracking_cb;
	gui_button_add(&b_ref_tracking);

	//Second line of buttons
	#define Y_SECOND Y_FIRST+25
	tft_print_line(10,Y_SECOND,BLACK,TRANSPARENT,0,"Photo mode:");

	b_photo_mode.base.x1=X_TAB;
	b_photo_mode.base.y1=Y_SECOND-3;
	b_photo_mode.base.x2=AUTO;
	b_photo_mode.base.y2=AUTO;
	b_photo_mode.txtcolor=WHITE;
	b_photo_mode.bgcolor=HEX(0x21B1FF);
	b_photo_mode.font=0;
	b_photo_mode.text="Photo Mode";
	b_photo_mode.callback=b_photo_mode_cb;
	gui_button_add(&b_photo_mode);


	//Third line of buttons
	#define Y_THIRD Y_SECOND+25
	tft_print_line(10,Y_THIRD,BLACK,TRANSPARENT,0,"Tests:");

	b_guitest.base.x1=X_TAB;
	b_guitest.base.y1=Y_THIRD-3;
	b_guitest.base.x2=AUTO;
	b_guitest.base.y2=AUTO;
	b_guitest.txtcolor=BLACK;
	b_guitest.bgcolor=HEX(0x00FA21);
	b_guitest.font=0;
	b_guitest.text="Gui Test";
	b_guitest.callback=b_guitest_cb;
	gui_button_add(&b_guitest);


	b_pixytest.base.x1=b_guitest.base.x2+BUTTON_SPACING;
	b_pixytest.base.y1=Y_THIRD-3;
	b_pixytest.base.x2=AUTO;
	b_pixytest.base.y2=AUTO;
	b_pixytest.txtcolor=BLACK;
	b_pixytest.bgcolor=HEX(0x00FA96);
	b_pixytest.font=0;
	b_pixytest.text="Pixy Test";
	b_pixytest.callback=b_pixytest_cb;
	gui_button_add(&b_pixytest);


	b_filetest.base.x1=b_pixytest.base.x2+BUTTON_SPACING;
	b_filetest.base.y1=Y_THIRD-3;
	b_filetest.base.x2=AUTO;
	b_filetest.base.y2=AUTO;
	b_filetest.txtcolor=BLACK;
	b_filetest.bgcolor=HEX(0x00FAC4);
	b_filetest.font=0;
	b_filetest.text="File Test";
	b_filetest.callback=b_filetest_cb;
	gui_button_add(&b_filetest);


	//Bottom line
	tft_draw_line(0,145,319,145,BLACK);
	tft_print_line(10,150,BLUE,TRANSPARENT,0,"Powered by");
	tft_draw_bitmap_file_unscaled(10,165,"pixy_small.bmp");
	tft_draw_bitmap_file_unscaled(165,165,"stm_small.bmp");

}

static void leave(void* screen) {
	gui_button_remove(&b_our_tracking);
	gui_button_remove(&b_ref_tracking);
	gui_button_remove(&b_photo_mode);
	gui_button_remove(&b_guitest);
	gui_button_remove(&b_pixytest);
	gui_button_remove(&b_filetest);
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


