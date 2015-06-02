#include "screen_guitest.h"
#include "button.h"
#include "tft.h"
#include "checkbox.h"
#include "numupdown.h"

static BUTTON_STRUCT b_back;
static TOUCH_AREA_STRUCT a_area;
static CHECKBOX_STRUCT c_cbox;
static NUMUPDOWN_STRUCT n_updown;

static void checkboxCB(void *checkbox, bool checked) {
        printf("Checkbox %s\n",(checked?"checked":"unchecked"));
}

static void b_back_cb(void* button) {
        gui_screen_back();
}

static void n_updown_cb(void* numupdown, int16_t value) {
        printf("New NumUpDown Value %d\n",value);
}

static void touchCB(void* touchArea, TOUCH_ACTION triggeredAction) {
        
        switch(triggeredAction) {
                case PEN_DOWN:
                        printf("action PEN_DOWN\n");
                break;
                case PEN_UP:
                        printf("action PEN_UP\n");
                break;
                case PEN_MOVE:
                        printf("action PEN_MOVE\n");
                break;
                case PEN_ENTER:
                        printf("action PEN_ENTER\n");
                break;
                case PEN_LEAVE:
                        printf("action PEN_LEAVE\n");
                break;
                default:        
                printf("action %s\n",triggeredAction);
                break;
        }
}

static void enter(void* screen) {
	tft_clear(HEX(0xA6FD9A));
	
	//Back button
	b_back.base.x1=10; //Start X of Button
        b_back.base.y1=10; //Start Y of Button
        b_back.base.x2=AUTO; //b_back.base.x1+160; //Auto Calculate X2 with String Width
        b_back.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_back.txtcolor=WHITE; //Set foreground color
        b_back.bgcolor=HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_back.font=0; //Select Font
        b_back.text="Back"; //Set Text (For formatted strings take sprintf)
        b_back.callback=b_back_cb; //Call b_back_cb as Callback
        gui_button_add(&b_back); //Register Button (and run the callback from now on)


	//tft test
	tft_draw_pixel(0,0,BLACK);
	tft_draw_pixel(319,239,BLACK);
	tft_draw_pixel(10,210,BLUE);
        tft_draw_pixel(12,210,BLUE);
        tft_draw_rectangle(40,100,60,235,BLUE);
        tft_fill_rectangle(100,215,200,225,GREEN);
        tft_draw_line(10,50,310,225,RGB(0xFF,0,0xFF));
        tft_draw_circle(10,10,100, RED);
        tft_print_line(30, 130, RED, BLUE, 0, "Hallo");
	


	//Area test
	a_area.hookedActions = PEN_DOWN | PEN_UP | PEN_MOVE | PEN_ENTER | PEN_LEAVE;
        a_area.x1 = 130;
        a_area.y1 = 30;
        a_area.x2 = 200;
        a_area.y2 = 60;
        a_area.callback = touchCB;
        touch_register_area(&a_area);



	 //Checkbox test
        c_cbox.base.x1=220;
        c_cbox.base.y1=45;
        c_cbox.base.x2=c_cbox.base.x1+16;
        c_cbox.base.y2=c_cbox.base.y1+16;
        c_cbox.fgcolor = GREEN;
        c_cbox.checked = true;
        c_cbox.callback = checkboxCB;
        gui_checkbox_add(&c_cbox);
	

	//Num up down test
	n_updown.x=200;
	n_updown.y=120;
	n_updown.fgcolor=RED;
	n_updown.value = -3;
	n_updown.max=11;
	n_updown.min =-5;
	n_updown.callback=n_updown_cb;
	gui_numupdown_add(&n_updown);

}

static void leave(void* screen) {
	gui_button_remove(&b_back);
	gui_checkbox_remove(&c_cbox);
	gui_numupdown_remove(&n_updown);
	touch_unregister_area(&a_area);
}

static void update(void* screen) {
	//gui_button_redraw(&b_back); //only needed if button is overdrawn by others
	//.... for the other elements as well
}


static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_guitest() {
	return &screen;
}


