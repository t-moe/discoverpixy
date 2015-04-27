#include "touch.h"
#include "ll_touch.h"
#include <stdio.h>

#define NUM_AREAS 50    //Number of Structs Reserved in Memory for TouchAreas (e.g Buttons)
TOUCH_AREA_STRUCT* areas[NUM_AREAS] = {NULL};

volatile int touchY=0; //Last Y Coordinate in pixels
volatile int touchX=0; //Last X Coordinate in pixels
volatile TOUCH_STATE oldState=TOUCH_UP;

bool touch_init() {
	return ll_touch_init();
}


bool touch_add_raw_event(uint16_t x, uint16_t y, TOUCH_STATE state) {
 	bool penDown = (state==TOUCH_DOWN);
        bool oldPenDown = (oldState==TOUCH_DOWN);
        oldState=state;
	uint16_t touchX = x;
	uint16_t touchY = y;
 	if(penDown)
        {
        //      tftDrawPixel(touchX,touchY,WHITE);
                if(!oldPenDown) //First Touch
                {
                        for(int z=0; z < NUM_AREAS; z++) // For every touch area
                        {
                                if(areas[z]!=NULL  && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
                                {
                                        areas[z]->flags=1; //PenInside=1
                                        if(areas[z]->hookedActions & PEN_DOWN)
                                                areas[z]->callback(areas[z],PEN_DOWN);
                                }
                        }
                }
                else //Second, Third
                {
                        for(int z=0; z < NUM_AREAS; z++) // For every touch area
                        {
                                if(areas[z]!=NULL )
                                {
                                        if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
                                        {
                                                if(areas[z]->flags==0)  //PenInside ==0
                                                {
                                                        areas[z]->flags=1; //PenInside=1
                                                        if(areas[z]->hookedActions & PEN_ENTER)
                                                                areas[z]->callback(areas[z],PEN_ENTER);
                                                }
                                        }
                                        else if(areas[z]->flags) //PenInside==1
    					{
                                                areas[z]->flags=0; //PenInside=0
                                                if(areas[z]->hookedActions & PEN_LEAVE)
                                                        areas[z]->callback(areas[z],PEN_LEAVE);
                                        }
                                }
                        }
                }
                for(int z=0; z < NUM_AREAS; z++) // For every touch area
                {
                        if(areas[z]!=NULL && areas[z]->hookedActions&PEN_MOVE)
                        {
                                if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
                                {
                                        areas[z]->callback(areas[z],PEN_MOVE);
                                }
                        }
                }                                                                        
        }
        else
        {
                if(oldPenDown) //Was the pen ever down (or was it a too short touch)
                {
                        for(int z=0; z < NUM_AREAS; z++) // For every touch area
                        {
                                if(areas[z]!=NULL && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
                                {
                                        areas[z]->flags=0; //PenInside = 0;
                                        if(areas[z]->hookedActions & PEN_UP)
                                                areas[z]->callback(areas[z],PEN_UP);
                                }
                        }
                }
                touchX=0xFFFF;
                touchY=0xFFFF;
        }

	return true;
}

bool touch_have_empty(unsigned char num)
{
        for(unsigned char i=0; i<NUM_AREAS; i++)
        {
                if(areas[i]==NULL) num--;
                if(num==0) return true;
        }
        return false;

}
bool touch_register_area(TOUCH_AREA_STRUCT* area) //Registers an Area (fill Struct first). Return false if no more Space in the Pointertable (-->Change NUM_AREAS).
{

        for(unsigned char i=0; i<NUM_AREAS; i++)
        {
                if(areas[i]==NULL)
                {
                        area->flags=0;
                        areas[i]=area;
                        return true;
                }
        }
        return false;
}

void touch_unregister_area(TOUCH_AREA_STRUCT* area)//Unregisters an Area
{
        for(unsigned char i=0; i<NUM_AREAS; i++)
        {
                if(areas[i]==area)
                {
                        areas[i]=NULL;
                        break;
                }
        }
}
