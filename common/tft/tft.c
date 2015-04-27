#include "tft.h"
#include "ll_tft.h"
#include <string.h>


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

void tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat) {
	ll_tft_draw_bitmap_unscaled(x,y,width,height,dat);
}

void tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color) {
    ll_tft_draw_circle(x, y, r, color);
}

uint8_t tft_num_fonts() {
	return ll_tft_num_fonts();
}

uint8_t tft_font_height(uint8_t fontnum) {
	return ll_tft_font_height(fontnum);
}

uint8_t tft_font_width(uint8_t fontnum) {
	return ll_tft_font_width(fontnum); 
}

void tft_print_line(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* text) {
	if(font>=ll_tft_num_fonts()) return;
	for(int i=0; i<strlen(text); i++) {
		ll_tft_draw_char(x,y,color,bgcolor, font, text[i]);
		x+=ll_tft_font_width(font);	
	}
}

