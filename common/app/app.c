#include "app.h"
#include "tft.h"
#include "system.h"
#include "pixy.h"
#include <stdio.h>

void app_init() {
	system_init();
	tft_init();

	pixy_init();
		
	//only testwise
	tft_clear(WHITE);	
	tft_draw_pixel(0,0,GREEN);
	tft_draw_pixel(320-1,240-1,GREEN);
	tft_draw_line(10,10,30,40,RGB(0xFF,0,0xFF));
	tft_draw_pixel(100,100,RED);
	tft_draw_rectangle(120,120,20,30,BLUE);
	tft_fill_rectangle(200,120,30,30,GREEN);
	

}



int colorind;
const uint32_t colors [] = {0xFF0000, 0x00FF00,0x0000FF,0xFFFF00,0x00FFFF,0xFF00FF,0xFFFFFF,0x000000};
const int num_colors = sizeof(colors)/sizeof(uint32_t);

//app event loop
void app_process() {
	pixy_service();

	if(colorind==0) {
        	pixy_led_set_max_current(5);
        }

        int32_t response;
        int return_value;
        return_value = pixy_command("led_set", INT32(colors[colorind++]), END_OUT_ARGS,  &response, END_IN_ARGS);
        colorind%=num_colors;
        
	system_delay(500);
}
