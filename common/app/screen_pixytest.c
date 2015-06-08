/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/screen_pixytest.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-27	timolang@gmail.com	cf72baa	Introduced a Screen (sub) module and divided app into multiple screens.
* 2015-05-02	timolang@gmail.com	3281616	Added some more touch functions. Improved pixy test. Drag the Image around!
* 2015-05-09	timolang@gmail.com	0b5173e	Added reference tracking.
* 2015-05-15	timolang@gmail.com	27c09ba	Redesigned main menu. Moved stuff from pixytest to a new helper file and to the new "photo mode"-screen.
* 2015-05-25	timolang@gmail.com	6a61769	Reimplemented pixytest screen. Added a lot of Test-Buttons.
* 2015-06-01	aaron@duckpond.ch	caa7b5c	Added IRQ for user button, fixed some touchproblems.
* 2015-06-03	timolang@gmail.com	74aa186	Made pixy_test screen working again. Had to disable pixy_service. Recalibrated initial touch values.
* 2015-06-07	timolang@gmail.com	c87220d	Renamed pixy_helper to pixy_frame. Updated docu of appliaction. added doxygen comments to pixy_{frame,control}.h
*
**************************************************************************************************************************************/

#include "screen_pixytest.h"
#include "button.h"
#include "numupdown.h"
#include "tft.h"
#include "touch.h"
#include "pixy.h"
#include "system.h"
#include "pixy_frame.h"

static volatile enum {detecting, idle, update_servos, update_ledcolor, update_ledcurrent} state; //Current state of the screen state machine

static BUTTON_STRUCT b_back;

static BUTTON_STRUCT b_servos_center;
static BUTTON_STRUCT b_servos_topleft;
static BUTTON_STRUCT b_servos_topright;
static BUTTON_STRUCT b_servos_bottomleft;
static BUTTON_STRUCT b_servos_bottomright;
static uint16_t servo_x;
static uint16_t servo_y;

static BUTTON_STRUCT b_led_off;
static BUTTON_STRUCT b_led_white;
static BUTTON_STRUCT b_led_red;
static BUTTON_STRUCT b_led_green;
static BUTTON_STRUCT b_led_blue;
static uint32_t led_color;

static uint32_t led_maxcurrent;
static NUMUPDOWN_STRUCT n_led_powerlimit;


static void b_back_cb(void* button)
{
    gui_screen_back();
}

static void b_servos_center_cb(void* button)
{
    if (state == idle) {
        servo_x = 500;
        servo_y = 500;
        state = update_servos;
    }
}

static void b_servos_topleft_cb(void* button)
{
    if (state == idle) {
        servo_x = 0;
        servo_y = 0;
        state = update_servos;
    }
}

static void b_servos_topright_cb(void* button)
{
    if (state == idle) {
        servo_x = 1000;
        servo_y = 0;
        state = update_servos;
    }
}

static void b_servos_bottomleft_cb(void* button)
{
    if (state == idle) {
        servo_x = 0;
        servo_y = 1000;
        state = update_servos;
    }
}

static void b_servos_bottomright_cb(void* button)
{
    if (state == idle) {
        servo_x = 1000;
        servo_y = 1000;
        state = update_servos;
    }
}

static void b_led_off_cb(void* button)
{
    if (state == idle) {
        led_color = 0x000000;
        state = update_ledcolor;
    }
}

static void b_led_white_cb(void* button)
{
    if (state == idle) {
        led_color = 0xFFFFFF;
        state = update_ledcolor;
    }
}

static void b_led_red_cb(void* button)
{
    if (state == idle) {
        led_color = 0xFF0000;
        state = update_ledcolor;
    }
}

static void b_led_green_cb(void* button)
{
    if (state == idle) {
        led_color = 0x00FF00;
        state = update_ledcolor;
    }
}

static void b_led_blue_cb(void* button)
{
    if (state == idle) {
        led_color = 0x0000FF;
        state = update_ledcolor;
    }
}

static void n_led_powerlimit_cb(void* numupdown, int16_t value)
{
    if (state == idle) {
        led_maxcurrent = value;
        state = update_ledcurrent;
    }
}

static void enter(void* screen)
{
    tft_clear(WHITE);

    //Back button
    b_back.base.x1 = 10; //Start X of Button
    b_back.base.y1 = 210; //Start Y of Button
    b_back.base.x2 = AUTO; //Auto Calculate X2 with String Width
    b_back.base.y2 = AUTO; //Auto Calculate Y2 with String Height
    b_back.txtcolor = WHITE; //Set foreground color
    b_back.bgcolor = HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
    b_back.font = 0; //Select Font
    b_back.text = "Back"; //Set Text (For formatted strings take sprintf)
    b_back.callback = b_back_cb; //Call b_back_cb as Callback
    gui_button_add(&b_back); //Register Button (and run the callback from now on)


    //Servo stuff
#define SERVO_BUTTON_Y 10
#define SERVO_BUTTON_SPACING 5
    tft_print_line(5, SERVO_BUTTON_Y, BLACK, TRANSPARENT, 0, "Servos:");

    b_servos_center.base.x1 = 55;
    b_servos_center.base.y1 = SERVO_BUTTON_Y - 3;
    b_servos_center.base.x2 = AUTO;
    b_servos_center.base.y2 = AUTO;
    b_servos_center.txtcolor = WHITE;
    b_servos_center.bgcolor = HEX(0xAE1010);
    b_servos_center.font = 0;
    b_servos_center.text = "Center";
    b_servos_center.callback = b_servos_center_cb;
    gui_button_add(&b_servos_center);

    b_servos_topleft.base.x1 = b_servos_center.base.x2 + SERVO_BUTTON_SPACING;
    b_servos_topleft.base.y1 = SERVO_BUTTON_Y - 3;
    b_servos_topleft.base.x2 = AUTO;
    b_servos_topleft.base.y2 = AUTO;
    b_servos_topleft.txtcolor = WHITE;
    b_servos_topleft.bgcolor = HEX(0xAE1010);
    b_servos_topleft.font = 0;
    b_servos_topleft.text = "ToLe";
    b_servos_topleft.callback = b_servos_topleft_cb;
    gui_button_add(&b_servos_topleft);

    b_servos_topright.base.x1 = b_servos_topleft.base.x2 + SERVO_BUTTON_SPACING;
    b_servos_topright.base.y1 = SERVO_BUTTON_Y - 3;
    b_servos_topright.base.x2 = AUTO;
    b_servos_topright.base.y2 = AUTO;
    b_servos_topright.txtcolor = WHITE;
    b_servos_topright.bgcolor = HEX(0xAE1010);
    b_servos_topright.font = 0;
    b_servos_topright.text = "ToRi";
    b_servos_topright.callback = b_servos_topright_cb;
    gui_button_add(&b_servos_topright);

    b_servos_bottomleft.base.x1 = b_servos_topright.base.x2 + SERVO_BUTTON_SPACING;
    b_servos_bottomleft.base.y1 = SERVO_BUTTON_Y - 3;
    b_servos_bottomleft.base.x2 = AUTO;
    b_servos_bottomleft.base.y2 = AUTO;
    b_servos_bottomleft.txtcolor = WHITE;
    b_servos_bottomleft.bgcolor = HEX(0xAE1010);
    b_servos_bottomleft.font = 0;
    b_servos_bottomleft.text = "BoLe";
    b_servos_bottomleft.callback = b_servos_bottomleft_cb;
    gui_button_add(&b_servos_bottomleft);

    b_servos_bottomright.base.x1 = b_servos_bottomleft.base.x2 + SERVO_BUTTON_SPACING;
    b_servos_bottomright.base.y1 = SERVO_BUTTON_Y - 3;
    b_servos_bottomright.base.x2 = AUTO;
    b_servos_bottomright.base.y2 = AUTO;
    b_servos_bottomright.txtcolor = WHITE;
    b_servos_bottomright.bgcolor = HEX(0xAE1010);
    b_servos_bottomright.font = 0;
    b_servos_bottomright.text = "BoRi";
    b_servos_bottomright.callback = b_servos_bottomright_cb;
    gui_button_add(&b_servos_bottomright);

    //Led Color stuff
#define LED_COLOR_BUTTON_Y 35
#define LED_COLOR_BUTTON_SPACING 5
    tft_print_line(5, LED_COLOR_BUTTON_Y, BLACK, TRANSPARENT, 0, "Led Color:");

    b_led_off.base.x1 = 85;
    b_led_off.base.y1 = LED_COLOR_BUTTON_Y - 3;
    b_led_off.base.x2 = AUTO;
    b_led_off.base.y2 = AUTO;
    b_led_off.txtcolor = WHITE;
    b_led_off.bgcolor = BLACK;
    b_led_off.font = 0;
    b_led_off.text = "Off";
    b_led_off.callback = b_led_off_cb;
    gui_button_add(&b_led_off);

    b_led_white.base.x1 = b_led_off.base.x2 + LED_COLOR_BUTTON_SPACING;
    b_led_white.base.y1 = LED_COLOR_BUTTON_Y - 3;
    b_led_white.base.x2 = AUTO;
    b_led_white.base.y2 = AUTO;
    b_led_white.txtcolor = BLACK;
    b_led_white.bgcolor = HEX(0xEEEEEE);
    b_led_white.font = 0;
    b_led_white.text = "White";
    b_led_white.callback = b_led_white_cb;
    gui_button_add(&b_led_white);

    b_led_red.base.x1 = b_led_white.base.x2 + LED_COLOR_BUTTON_SPACING;
    b_led_red.base.y1 = LED_COLOR_BUTTON_Y - 3;
    b_led_red.base.x2 = AUTO;
    b_led_red.base.y2 = AUTO;
    b_led_red.txtcolor = WHITE;
    b_led_red.bgcolor = HEX(0xEE0000);
    b_led_red.font = 0;
    b_led_red.text = "Red";
    b_led_red.callback = b_led_red_cb;
    gui_button_add(&b_led_red);

    b_led_green.base.x1 = b_led_red.base.x2 + LED_COLOR_BUTTON_SPACING;
    b_led_green.base.y1 = LED_COLOR_BUTTON_Y - 3;
    b_led_green.base.x2 = AUTO;
    b_led_green.base.y2 = AUTO;
    b_led_green.txtcolor = WHITE;
    b_led_green.bgcolor = HEX(0x00EE00);
    b_led_green.font = 0;
    b_led_green.text = "Green";
    b_led_green.callback = b_led_green_cb;
    gui_button_add(&b_led_green);

    b_led_blue.base.x1 = b_led_green.base.x2 + LED_COLOR_BUTTON_SPACING;
    b_led_blue.base.y1 = LED_COLOR_BUTTON_Y - 3;
    b_led_blue.base.x2 = AUTO;
    b_led_blue.base.y2 = AUTO;
    b_led_blue.txtcolor = WHITE;
    b_led_blue.bgcolor = HEX(0x0000EE);
    b_led_blue.font = 0;
    b_led_blue.text = "Blue";
    b_led_blue.callback = b_led_blue_cb;
    gui_button_add(&b_led_blue);

    //Led MaxPower stuff
#define LED_POWER_BUTTON_Y 70
    tft_print_line(5, LED_POWER_BUTTON_Y, BLACK, TRANSPARENT, 0, "Led Maximum Current:");

    //Num up down test
    n_led_powerlimit.x = 160;
    n_led_powerlimit.y = LED_POWER_BUTTON_Y - 7;
    n_led_powerlimit.fgcolor = WHITE;
    n_led_powerlimit.value = 10;
    n_led_powerlimit.max = 40;
    n_led_powerlimit.min = 0;
    n_led_powerlimit.callback = n_led_powerlimit_cb;
    gui_numupdown_add(&n_led_powerlimit);



    state = detecting;
}

static void leave(void* screen)
{
    gui_button_remove(&b_back);
    gui_button_remove(&b_servos_center);
    gui_button_remove(&b_servos_topleft);
    gui_button_remove(&b_servos_topright);
    gui_button_remove(&b_servos_bottomleft);
    gui_button_remove(&b_servos_bottomright);
    gui_button_remove(&b_led_off);
    gui_button_remove(&b_led_white);
    gui_button_remove(&b_led_red);
    gui_button_remove(&b_led_green);
    gui_button_remove(&b_led_blue);
    gui_numupdown_remove(&n_led_powerlimit);

}


static void update(void* screen)
{
    switch (state) {
    case detecting: //Detecting State: Where we try to connect to the pixy
        if (pixy_init() == 0) { //Pixy connection ok
            int32_t response;
            int return_value;
            return_value = pixy_command("stop", END_OUT_ARGS,  &response, END_IN_ARGS);
            pixy_led_set_max_current(10);

            state = idle; //Go to next state
        }

        break;

    case idle:
        pixy_service();
        break;

    case update_servos:
        pixy_rcs_set_position(0, servo_x);
        pixy_rcs_set_position(1, servo_y);
        state = idle;
        break;

    case update_ledcolor: {
        int32_t response;
        int return_value;
        return_value = pixy_command("led_set", INT32(led_color), END_OUT_ARGS,  &response, END_IN_ARGS);
        state = idle;
    }
    break;

    case update_ledcurrent:
        pixy_led_set_max_current(led_maxcurrent);
        state = idle;
        break;
    }

}


static SCREEN_STRUCT screen = {
    enter,
    leave,
    update
};


SCREEN_STRUCT* get_screen_pixytest()
{
    return &screen;
}
