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
	tft_draw_pixel(10,210,BLUE);
	tft_draw_pixel(12,210,BLUE);
	tft_draw_rectangle(40,210,60,235,BLUE);
	tft_fill_rectangle(100,215,200,225,GREEN);
	tft_draw_line(10,215,310,225,RGB(0xFF,0,0xFF));
}



void pixy_led_test();
void pixy_frame_test();

//app event loop
void app_process() {
	pixy_service(); //send/receive event data

	//Code for tests see below
	pixy_led_test();	
	pixy_frame_test();
	        
	//system_delay(500);
}


//-----------------------------------------------------------------------------------------------------------------

int colorind;
const uint32_t colors [] = {0xFF0000, 0x00FF00,0x0000FF,0xFFFF00,0x00FFFF,0xFF00FF,0xFFFFFF,0x000000};
const int num_colors = sizeof(colors)/sizeof(uint32_t);

void pixy_led_test() {
        if(colorind==0) {
                pixy_led_set_max_current(5);
        }

        int32_t response;
        int return_value;
        return_value = pixy_command("led_set", INT32(colors[colorind++]), END_OUT_ARGS,  &response, END_IN_ARGS);
        colorind%=num_colors;
}

//----------------------------------------------------------------------------------------------------------------------------

int renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);


void pixy_frame_test() {

 uint8_t* videodata;
      int32_t response;
      int32_t fourccc;
      int8_t renderflags;
      uint16_t xwidth;
      uint16_t ywidth;
      uint32_t size;


      int return_value = pixy_command("cam_getFrame",  // String id for remote procedure
                                      INT8(0x21),     // mode
                                      INT16(0),        // xoffset
                                      INT16(0),         // yoffset
                                      INT16(320),       // width
                                      INT16(200),       // height
                                      END_OUT_ARGS,              // separator
                                      &response,      // pointer to mem address for return value
                                      &fourccc,
                                      &renderflags,
                                      &xwidth,
                                      &ywidth,
                                      &size,
                                      &videodata,        // pointer to mem address for returned frame
                                      END_IN_ARGS);


      if(return_value==0) {
            return_value = renderBA81(renderflags,xwidth,ywidth,size,videodata);
      }
}



inline void interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b)
{
    if (y&1)
    {
        if (x&1)
        {
            *r = *pixel;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
        }
        else
        {
            *r = (*(pixel-1)+*(pixel+1))>>1;
            *g = *pixel;
            *b = (*(pixel-width)+*(pixel+width))>>1;
        }
    }
    else
    {
        if (x&1)
        {
            *r = (*(pixel-width)+*(pixel+width))>>1;
            *g = *pixel;
            *b = (*(pixel-1)+*(pixel+1))>>1;
        }
        else
        {
            *r = (*(pixel-width-1)+*(pixel-width+1)+*(pixel+width-1)+*(pixel+width+1))>>2;
            *g = (*(pixel-1)+*(pixel+1)+*(pixel+width)+*(pixel-width))>>2;
            *b = *pixel;
        }
    }

}



int renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame)
{
    uint16_t x, y;
    uint8_t r, g, b;


    // skip first line
    frame += width;

    // don't render top and bottom rows, and left and rightmost columns because of color
    // interpolation
    //uint32_t decodedimage[(width-2)*(height-2)];
    uint16_t decodedimage[(width-2)*(height-2)];
    
    uint16_t* line = decodedimage;
    for (y=1; y<height-1; y++)
    {
        //line = (unsigned int *)img.scanLine(y-1);
        frame++;
        for (x=1; x<width-1; x++, frame++)
        {
            interpolateBayer(width, x, y, frame, &r, &g, &b);
            //*line++ = (0xff<<24) | (r<<16) | (g<<8) | (b<<0);
            *line++ = RGB(r,g,b);
        }
        frame++;
    }

    tft_draw_bitmap_unscaled(0,0,width-2,height-2,decodedimage); 
   
    return 0;
}

