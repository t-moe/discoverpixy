#include "tft.h"
#include "ll_tft.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "filesystem.h"

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

void tft_print_formatted(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* format, ...) {
	static char buffer[256]; //not sure if that's the best solution. It would propbably better to implement putchar and use vprintf
	va_list args;
	va_start (args, format);
	vsprintf(buffer,format,args);
	tft_print_line(x,y,color,bgcolor,font,buffer);
	va_end(args);
}

bool tft_draw_bitmap_file_unscaled(uint16_t x, uint16_t y, const char* filename) {
	//Copied and modified from: http://stackoverflow.com/a/17040962/2606757

	FILE_HANDLE* file = filesystem_file_open(filename);
	if(file==NULL) {
		return false;
	}

	unsigned char info[54];
	if(filesystem_file_read(file,info,54)!=F_OK) {
		filesystem_file_close(file);
		return false;
	}

	// extract image height and width from header
	uint32_t width = *(uint32_t*)&info[18]; //width in pixel
	uint32_t height = *(uint32_t*)&info[22]; //height in pixel
	uint16_t depth = *(uint16_t*)&info[28]; //bit's per pixel (color depth)
	depth/=8; //we want the number of bytes per pixel

	filesystem_file_seek(file,*(uint32_t*)&info[10]); //seek to the place where img data begins

	uint32_t row_padded = (width*depth + 3) & (~3); //row size aligned to 4 bytes

	unsigned char data [row_padded];

	for(int i = 0; i < height; i++)
	{
		filesystem_file_read(file,data,row_padded);
		for(int j = 0; j < width*depth; j += depth)
		{
			unsigned char a,r,g,b;
			if(depth==4) {
				a = data[j];
				r = data[j+1];
				g = data[j+2];
				b = data[j+3];
			} else if (depth==3) {
				a = 255;
				r = data[j+2];
				g = data[j+1];
				b = data[j];
			}

			if(a!=0) {
				tft_draw_pixel(x+j/depth,y+height-1-i,RGB(r,g,b));
			}
		}
	}

	filesystem_file_close(file);

	return true;

}
