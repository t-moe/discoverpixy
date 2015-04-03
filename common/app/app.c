#include "app.h"
#include "tft.h"
#include <stdio.h>

void app_init() {
	tft_init();
	
	tft_draw_line(10,10,30,40,0b11111);

}

void app_process() {
///	printf("hello world\n");	

}
