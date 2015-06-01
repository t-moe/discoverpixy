#include "touch.h"
#include "ll_touch.h"
#include <stdio.h>

/* The idea is as follows:
 * The user can add "touch-areas" which basically represent a rectangles on the screen.
 * Once the user touches such a rectangle with the pen, we forward events to his provided callback.
 * Touch events are provided to us from the low level implementation via touch_add_raw_event().
 * We then need to check which touch areas are effected by that event
 */

/* Possible improvements:
 * Exchange pointer-list "areas" with a linked list. This would ensure that we can always accept new regions
 * Implement calibration stuff, and calculate the real coordinates out of the data provided in touch_add_raw_event()
 */

#define NUM_AREAS 50    //Number of Touch Areas we can manage
TOUCH_AREA_STRUCT* areas[NUM_AREAS] = {NULL}; //list with pointers to all managed touch area's

volatile POINT_STRUCT pos; //the last touch point
volatile TOUCH_STATE oldState=TOUCH_UP; //the last touch state

bool touch_init() {
	return ll_touch_init();
}


bool touch_add_raw_event(uint16_t touchX, uint16_t touchY, TOUCH_STATE state) {
	//Update current and old position/state
	bool penDown = (state==TOUCH_DOWN);
	bool oldPenDown = (oldState==TOUCH_DOWN);
	oldState=state;
	pos.x=touchX;
	pos.y=touchY;

 	if(penDown) //pen is down now
    {
 		//tft_draw_pixel(touchX,touchY,WHITE);
		if(!oldPenDown) //pen wasn't down before (positive edge) => First Touch
		{
			for(int z=0; z < NUM_AREAS; z++) // For every touch area
			{
				//Check if pos is inside area
				if(areas[z]!=NULL  && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
				{
					areas[z]->flags=1; //Save PenInside=1
					if(areas[z]->hookedActions & PEN_DOWN) //The user wants to receive pen down events
						areas[z]->callback(areas[z],PEN_DOWN); //Send event to user callback
				}
			}
		}
		else //Pen was down before => Second, Third event in row
		{
			for(int z=0; z < NUM_AREAS; z++) // For every touch area
			{
				if(areas[z]!=NULL )
				{
					//Check if pos is inside area
					if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
					{
						if(areas[z]->flags==0) //Pen was not inside before (PenInside==0)
						{
							areas[z]->flags=1; //Pen is inside now (PenInside=1)
							if(areas[z]->hookedActions & PEN_ENTER) //The user wants to receive pen enter events
								areas[z]->callback(areas[z],PEN_ENTER);
						}
					}
					else if(areas[z]->flags) //Pos not inside area, but it was before (PenInside==1)
					{
						areas[z]->flags=0; //Pen is no longer inside (PenInside=0)
						if(areas[z]->hookedActions & PEN_LEAVE) //The user wants to receive pen leave events
							areas[z]->callback(areas[z],PEN_LEAVE);
					}
				}
			}
		}
		for(int z=0; z < NUM_AREAS; z++) // For every touch area
		{
			if(areas[z]!=NULL && (areas[z]->hookedActions&PEN_MOVE)) //User want's to receive pen move events
			{
				//Check if pos is inside area
				if(touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2)
				{
					areas[z]->callback(areas[z],PEN_MOVE);
				}
			}
		}
	}
	else //pen is not down now
	{
		if(oldPenDown) //but it was down before (negative edge)
		{
			for(int z=0; z < NUM_AREAS; z++) // For every touch area
			{
				//Check if pos is inside area
				if(areas[z]!=NULL && touchX >= areas[z]->x1 && touchX <= areas[z]->x2 && touchY >= areas[z]->y1 && touchY <= areas[z]->y2 )
				{
					areas[z]->flags=0; //The pen is no longer inside (PenInside = 0);
					if(areas[z]->hookedActions & PEN_UP) //user want's to receive pen up events
						areas[z]->callback(areas[z],PEN_UP);
				}
			}
		}
	}
	return true;
}

bool touch_have_empty(unsigned char num)
{
	//go through pointer array and check for free spaces
	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==NULL) num--; //a free space was found, we need one less
		if(num==0) return true; //enough free spaces found
	}
	return false; //not enough free spaces found
}

bool touch_register_area(TOUCH_AREA_STRUCT* area)
{
	//go through pointer array and check for free space
	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==NULL) //free space found
		{
			area->flags=0; //we start with empty flags (PenInside=0)
			areas[i]=area; //save pointer into list
			return true;
		}
	}
	return false; //no free space found
}

void touch_unregister_area(TOUCH_AREA_STRUCT* area)
{
	if(area==NULL) return;

	//go through pointer array and find the area to remove
	for(unsigned char i=0; i<NUM_AREAS; i++)
	{
		if(areas[i]==area) //area found in pointer array at pos i
		{
			areas[i]=NULL; //set pointer in list to NULL again
			break;
		}
	}
}


POINT_STRUCT touch_get_last_point() {
	return pos;
}


