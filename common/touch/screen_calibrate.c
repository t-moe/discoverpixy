#include "screen_calibrate.h"
#include "tft.h"
#include "touch.h"


extern volatile bool calibration; //from touch.c


static void enter(void* screen) {
	tft_clear(BLACK);
}

static void leave(void* screen) {

}

static void update(void* screen) {
	int x1,y1,x2,y2,dx,dy;


	tft_print_line(50,50,WHITE,BLACK,1,"Calibration:");
	tft_print_line(50,120,WHITE,BLACK,0,"Hit the markers exactly!");
	//-----------------First Point--------------------
	tft_draw_line(CCENTER,CBEGIN,CCENTER,CEND,WHITE);	//Draw Cross
	tft_draw_line(CBEGIN,CCENTER,CEND,CCENTER,WHITE);	//Draw Cross
	calibration=1; //TouchX + TouchY Values will not be converted to Pixels
	while(calibration);	//Wait on PenUp
	POINT_STRUCT p1 = touch_get_last_point();
	x1=p1.x;
	y1=p1.y;
	tft_fill_rectangle(CBEGIN,CBEGIN,CEND,CEND,BLACK); //Clear Cross

	//-----------------Second Point-------------------
	tft_draw_line(DWIDTH-CCENTER,DHEIGHT-CBEGIN,DWIDTH-CCENTER,DHEIGHT-CEND,WHITE);
	tft_draw_line(DWIDTH-CBEGIN,DHEIGHT-CCENTER,DWIDTH-CEND,DHEIGHT-CCENTER,WHITE);
	calibration=1;
	while(calibration);
	POINT_STRUCT p2 = touch_get_last_point();
	x2=p2.x;
	y2=p2.y;
	tft_fill_rectangle(DWIDTH-CBEGIN,DHEIGHT-CBEGIN,DWIDTH-CEND,DHEIGHT-CEND,BLACK);

	//-----------------Third Point--------------------
	tft_draw_line(CCENTER,DHEIGHT-CBEGIN,CCENTER,DHEIGHT-CEND,WHITE);
	tft_draw_line(CBEGIN,DHEIGHT-CCENTER,CEND,DHEIGHT-CCENTER,WHITE);
		calibration=1;
	while(calibration);
	POINT_STRUCT p3 = touch_get_last_point();
	x1+=p3.x; //Add(!) values. We'll build the average later
	y2+=p3.y;
	tft_fill_rectangle(CBEGIN,DHEIGHT-CBEGIN,CEND,DHEIGHT-CEND,BLACK);

	//------------------4. Point---------------------
	tft_draw_line(DWIDTH-CCENTER,CBEGIN,DWIDTH-CCENTER,CEND,WHITE);
	tft_draw_line(DWIDTH-CBEGIN,CCENTER,DWIDTH-CEND,CCENTER,WHITE);
	calibration=1;
	while(calibration);
	POINT_STRUCT p4 = touch_get_last_point();
	x2+=p4.x;
	y1+=p4.y;
	tft_fill_rectangle(DWIDTH-CBEGIN,CBEGIN,DWIDTH-CEND,CEND,BLACK);
	//-------------------Calculation---------------------
	x1++; //Add 1 and divide by 2 later = +0.5 (for correct rounding)
	y1++;
	x2++;
	y2++;
	x1>>=1; //Divide by 2
	y1>>=1;
	x2>>=1;
	y2>>=1;
	dx = (x2-x1);	//Build the Difference
	dy = (y2-y1);

	touch_set_calibration_values(x1,dx,y1,dy);
	tft_print_line(50,120,WHITE,BLACK,0,"Calibration Done. Press anywhere");

	calibration=1;
	while(calibration);
	gui_screen_back();

}


static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_calibrate() {
	return &screen;
}


