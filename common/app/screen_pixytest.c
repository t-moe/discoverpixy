#include "screen_pixytest.h"
#include "button.h"
#include "tft.h"
#include "touch.h"
#include "pixy.h"
#include <stdlib.h>
#include "system.h"

static volatile bool pixy_connected = false;


static BUTTON_STRUCT b_back;
static BUTTON_STRUCT b_runstop;
static TOUCH_AREA_STRUCT a_area;

static void b_back_cb(void* button) {
        gui_screen_back();
}


static volatile bool pixy_running = false;
static bool old_pixy_running= false;
static void b_runstop_cb(void* button) {
	pixy_running=!pixy_running;	
}




static POINT_STRUCT pixy_pos;
static POINT_STRUCT old_pos;
static void touchCB(void* touchArea, TOUCH_ACTION triggeredAction) {
	POINT_STRUCT p = touch_get_last_point();
	switch(triggeredAction) {
		case PEN_ENTER:
                case PEN_DOWN:
			old_pos = p;
                break;
                case PEN_MOVE:
		{
			int16_t deltaX = p.x - old_pos.x;
			int16_t deltaY = p.y - old_pos.y;
			old_pos=p;
			printf("%d %d\n",deltaX,deltaY);
			if(pixy_connected) {
				int16_t new_x = pixy_pos.x+deltaX*2;
				int16_t new_y = pixy_pos.y-deltaY*2;
				if(new_x<0) new_x=0;
				if(new_x>1000) new_x=1000;
				if(new_y<0) new_y=0;
				if(new_y>1000) new_y=1000;
				pixy_pos.x = new_x;
				pixy_pos.y= new_y;
			}
		}
                break;
		case PEN_UP:
		case PEN_LEAVE:
			printf("Leave/up\n");
		default: break;
        }

	
}


static void enter(void* screen) {
	tft_clear(WHITE);
	
	//Back button
	b_back.base.x1=10; //Start X of Button
        b_back.base.y1=210; //Start Y of Button
        b_back.base.x2=AUTO; //b_back.base.x1+160; //Auto Calculate X2 with String Width
        b_back.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_back.txtcolor=WHITE; //Set foreground color
        b_back.bgcolor=HEX(0xAE1010); //Set background color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_back.font=0; //Select Font
        b_back.text="Back"; //Set Text (For formatted strings take sprintf)
        b_back.callback=b_back_cb; //Call b_back_cb as Callback
        gui_button_add(&b_back); //Register Button (and run the callback from now on)


	//Back button
        b_runstop.base.x1=60; //Start X of Button
        b_runstop.base.y1=210; //Start Y of Button
        b_runstop.base.x2=AUTO; //b_runstop.base.x1+160; //Auto Calculate X2 with String Width
        b_runstop.base.y2=AUTO; //Auto Calculate Y2 with String Height
        b_runstop.txtcolor=WHITE; //Set foreground color
        b_runstop.bgcolor=HEX(0xAE1010); //Set runstopground color (Don't take 255 or 0 on at least one channel, to make shadows possible)
        b_runstop.font=0; //Select Font
        b_runstop.text="Run/Stop"; //Set Text (For formatted strings take sprintf)
        b_runstop.callback=b_runstop_cb; //Call b_runstop_cb as Callrunstop
        gui_button_add(&b_runstop); //Register Button (and run the callrunstop from now on)


	//Area test
        a_area.hookedActions =  PEN_DOWN | PEN_MOVE |  PEN_ENTER | PEN_UP | PEN_LEAVE;
        a_area.x1 = 0;
        a_area.y1 = 0;
        a_area.x2 = 317;
        a_area.y2 = 197;
        a_area.callback = touchCB;
        touch_register_area(&a_area);



	//Pixy stuff
        pixy_connected = (pixy_init()==0); //try to connect to pixy
	if(pixy_connected) {
		pixy_pos.x=pixy_pos.y=500;
	}
}

static void leave(void* screen) {
	gui_button_remove(&b_back);
	gui_button_remove(&b_runstop);
	touch_unregister_area(&a_area);
}

int pixy_led_test();
int pixy_frame_test();


static void update(void* screen) {

  //Note: The only way to detect that pixy has been disconnected is if a command fails. There's no pixy_is_connected method yet :'(

        if(!pixy_connected) { //Pixy not connected
                pixy_close(); //Ensure that all pixy resources are freed (failsafe)
                if(pixy_init()==0) { //try to connect to pixy
                        pixy_connected=true;
			pixy_pos.x=pixy_pos.y=500;
			printf("pixy reinitialized\n");
                }
        }

        if(pixy_connected) {
                pixy_service(); //Send/receive event data from/to pixy failed


                if(pixy_frame_test()!=0) {
                         pixy_connected=false;
                }

		/*if(pixy_led_test()!=0) {
			pixy_connected=false;
		}*/

		if(!pixy_running) {	
			pixy_rcs_set_position(0,pixy_pos.x);
			pixy_rcs_set_position(1,pixy_pos.y);
		}

		if(pixy_running!=old_pixy_running) {	
			old_pixy_running=pixy_running;
			if(pixy_running) { //start tracking

				int32_t response;
				int return_value;
				return_value = pixy_command("runprog", INT8(2), END_OUT_ARGS,  &response, END_IN_ARGS);

			} else { //stop tracking
				int32_t response;
				int return_value;
				return_value = pixy_command("stop", END_OUT_ARGS,  &response, END_IN_ARGS);
			}
		}
	
                //system_delay(500);
        }
}


static SCREEN_STRUCT screen = {
	enter,
	leave,
	update
};


SCREEN_STRUCT* get_screen_pixytest() {
	return &screen;
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
