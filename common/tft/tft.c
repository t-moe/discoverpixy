#include "tft.h"
#include "ll_tft.h"

//it might seems pointless to forward all the functions but we might also introduce functions which have some logic here

bool tft_init() {
	return ll_tft_init();

}

void tft_clear(uint16_t color) {
	ll_tft_clear(color);
}

void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	ll_tft_draw_line(x1,y1,x2,y2,color);
}


void tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color) {
	ll_tft_draw_pixel(x,y,color);
}

void tft_draw_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color) {
	//could be implemented with 4 lines instead of introducing a ll func
	ll_tft_draw_rectangle(x1,y1,x2,y2,color);
}

void tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color) {
	ll_tft_fill_rectangle(x1,y1,x2,y2,color);
}

void tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint8_t* dat) {
	ll_tft_draw_bitmap_unscaled(x,y,width,height,dat);
}
