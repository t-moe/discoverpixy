#include "tft.h"
#include "ll_tft.h"
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include "filesystem.h"

/* The idea is as follows:
 * Most of the tft_* functions can be forwarded to the lowlevel implementation.
 * The exceptions are commented below.
 * Make sure to have a look at the doxygen comments for the lowlevel functions and for the tft_* functions
 */

/* Possible improvements:
 * For formatted printing implement putchar, instead of writing into a buffer and drawing that buffer afterwards
 */

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

//Print line can be done with multiple calls to draw_char
void tft_print_line(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* text) {
	if(font>=ll_tft_num_fonts()) return; //invalid font index
	for(int i=0; i<strlen(text); i++) { //for each char in the line
		ll_tft_draw_char(x,y,color,bgcolor, font, text[i]); //draw the char
		x+=ll_tft_font_width(font);	//and increase the x position
	}
}

//Printing a formatted line can be done by printing the line in a buffer using "sprintf" and then calling print_line
void tft_print_formatted(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* format, ...) {
	static char buffer[128]; //buffer to save the formatted text into

	//Since we have variable arguments, we need to forward them. We have to use vsprintf instead of sprintf for that.
	va_list args;
	va_start (args, format); //start the varg-list
	vsprintf(buffer,format,args); //let vsprintf render the formatted string
	tft_print_line(x,y,color,bgcolor,font,buffer); //print the string as normal text
	va_end(args); //end the varg-list
}

bool tft_draw_bitmap_file_unscaled(uint16_t x, uint16_t y, const char* filename) {
	//This method reads a .bmp file from the filesystem and tries to draw it.
	//Note: The bmp implementation is not complete, it has some limitations and it makes assumptions. See doxygen comment for this method.
	//Source Copied and adapted from: http://stackoverflow.com/a/17040962/2606757

	FILE_HANDLE* file = filesystem_file_open(filename); //try to open the file
	if(file==NULL) { //file opening failed
		return false;
	}

	unsigned char info[54];
	if(filesystem_file_read(file,info,54)!=F_OK) { //try to read the 54 byte header
		filesystem_file_close(file);
		return false; //reading the header failed
	}

	// extract image height and width from header
	uint32_t width = *(uint32_t*)&info[18]; //width in pixel
	uint32_t height = *(uint32_t*)&info[22]; //height in pixel
	uint16_t depth = *(uint16_t*)&info[28]; //bit's per pixel (color depth)
	depth/=8; //we want the number of bytes per pixel

	filesystem_file_seek(file,*(uint32_t*)&info[10]); //seek to the place where img data begins

	uint32_t row_padded = (width*depth + 3) & (~3); //row size must be aligned to 4 bytes

	unsigned char data [row_padded]; //allocate space for one row (incl. padding)

	for(int i = 0; i < height; i++) //for each row
	{
		filesystem_file_read(file,data,row_padded); //read row into buffer
		for(int j = 0; j < width*depth; j += depth) //for each pixel
		{
			unsigned char a,r,g,b;
			if(depth==4) { //a,r,g,b 8bit each
				a = data[j];
				r = data[j+1];
				g = data[j+2];
				b = data[j+3];
			} else if (depth==3) { // b,g,r, 8bit each
				a = 255;
				r = data[j+2];
				g = data[j+1];
				b = data[j];
			}

			if(a!=0) {
				//bmp's are stored "bottom-up", so we start drawing at the bottom
				tft_draw_pixel(x+j/depth,y+height-1-i,RGB(r,g,b));
			}
		}
	}

	filesystem_file_close(file);

	return true;

}
