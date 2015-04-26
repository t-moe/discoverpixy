#include "app.h"
#include "tft.h"
#include "system.h"
#include "touch.h"
#include "pixy.h"
#include <stdio.h>
#include <stdlib.h>

bool pixy_connected = false;

TOUCH_AREA_STRUCT a1;


void touchCB(void* touchArea, TOUCH_ACTION triggeredAction) {
	
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

void app_init() {
	system_init();
	tft_init();
	touch_init();

	pixy_connected = (pixy_init()==0); //try to connect to pixy
		
	//only testwise
	tft_clear(WHITE);	
	tft_draw_pixel(10,210,BLUE);
	tft_draw_pixel(12,210,BLUE);
	tft_draw_rectangle(40,210,60,235,BLUE);
	tft_fill_rectangle(100,215,200,225,GREEN);
	tft_draw_line(10,215,310,225,RGB(0xFF,0,0xFF));


	a1.hookedActions = PEN_DOWN | PEN_UP | PEN_MOVE | PEN_ENTER | PEN_LEAVE;
	a1.x1 = 30;
        a1.y1 = 30;
        a1.x2 = 100;
        a1.y2 = 60;
        a1.callback = touchCB;
	touch_register_area(&a1);

	 tft_draw_rectangle(30,30,100,60,BLUE);


}




int pixy_led_test();
int pixy_frame_test();

//app event loop
void app_process() {
	
	system_process(); //Let the system handle it's pending events


	//Note: The only way to detect that pixy has been disconnected is if a command fails. There's no pixy_is_connected method yet :'(

	if(!pixy_connected) { //Pixy not connected
		pixy_close(); //Ensure that all pixy resources are freed (failsafe)
		if(pixy_init()==0) { //try to connect to pixy
			pixy_connected=true;
		}
	}

	if(pixy_connected) {
		pixy_service(); //Send/receive event data from/to pixy failed

		//Code for tests see below
		if(pixy_led_test()!=0) {
			pixy_connected=false;
		}
			
		/*if(pixy_frame_test()!=0) {
			 pixy_connected=false;
	        }*/
		system_delay(500);
	}
}


//-----------------------------------------------------------------------------------------------------------------

int colorind;
const uint32_t colors [] = {0xFF0000, 0x00FF00,0x0000FF,0xFFFF00,0x00FFFF,0xFF00FF,0xFFFFFF,0x000000};
const int num_colors = sizeof(colors)/sizeof(uint32_t);

int pixy_led_test() {
        if(colorind==0) {
                pixy_led_set_max_current(5);
        }

        int32_t response;
        int return_value;
        return_value = pixy_command("led_set", INT32(colors[colorind++]), END_OUT_ARGS,  &response, END_IN_ARGS);
        colorind%=num_colors;

	if(return_value!=0) {
		colorind=0; //reset color ind, to start at zero when plugging pixy in again
	}

	return return_value;
}

//----------------------------------------------------------------------------------------------------------------------------

int renderBA81(uint8_t renderFlags, uint16_t width, uint16_t height, uint32_t frameLen, uint8_t *frame);


int pixy_frame_test() {

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

      return return_value;
}



void interpolateBayer(uint16_t width, uint16_t x, uint16_t y, uint8_t *pixel, uint8_t* r, uint8_t* g, uint8_t* b)
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
    uint16_t* decodedimage = malloc(sizeof(uint16_t)*(width-2)*(height-2));
   
    if(decodedimage==NULL) { //not enough free space to decode image in memory
        //decode & render image pixel by pixel
	uint16_t* line = decodedimage;
    	for (y=1; y<height-1; y++)
        {
                frame++;
                for (x=1; x<width-1; x++, frame++)
                {
                    interpolateBayer(width, x, y, frame, &r, &g, &b);
                    tft_draw_pixel(x-1,y-1,RGB(r,g,b));
                }
                frame++;
         }
    } else { //enough space  
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
	  
	    free(decodedimage); 
    }
    
    return 0;
}

