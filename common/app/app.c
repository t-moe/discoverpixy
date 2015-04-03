#include "app.h"
#include "tft.h"
#include "system.h"
#include <stdio.h>

void app_init() {
	system_init();
	tft_init();

	
	//only testwise
	tft_clear(WHITE);	
	tft_draw_pixel(0,0,GREEN);
	tft_draw_pixel(320-1,240-1,GREEN);
	tft_draw_line(10,10,30,40,RGB(0xFF,0,0xFF));
	tft_draw_pixel(100,100,RED);
	tft_draw_rectangle(120,120,20,30,BLUE);
	tft_fill_rectangle(200,120,30,30,GREEN);
	

}


//app event loop
void app_process() {
///	printf("hello world\n");	

}
