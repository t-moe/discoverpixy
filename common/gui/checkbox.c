#include "tft.h"
#include "touch.h"
#include "checkbox.h"
#include <stdio.h>

/* The idea is as follows:
 * When the user creates a checkbox we create a touch area for that region and wait for PEN_DOWN events.
 * Once the user puts the pen down in this area we'll redraw the checkbox with different shadows (feedback)
 * and we'll now wait on PEN_UP or PEN_LEAVE events.
 * If the user takes the pen away while in the area (PEN_UP), we toggle the checkbox and we call the provided user callback
 * Otherwise (PEN_LEAVE) we only restore the initial shadows
 */


#define ACTIVE_COLOR RGB(251,208,123) //shadow color (inside of border)
#define BORDER_COLOR RGB(29,82,129) //1px border color
#define BACKGROUND_COLOR WHITE //Background color

//Callback which is called when the user touches the touch-area we created for the checkbox
void checkboxes_cb(void* touchArea, TOUCH_ACTION triggeredAction)
{
	TOUCH_AREA_STRUCT * area = (TOUCH_AREA_STRUCT*)touchArea;
	CHECKBOX_STRUCT* checkbox = (CHECKBOX_STRUCT*)touchArea;
	switch(triggeredAction)
	{
		case PEN_DOWN:  //If the user touches the area for the "first time"
			area->hookedActions=PEN_UP|PEN_LEAVE; //for the future we only want PEN_UP and PEN_LEAVE events

			//Draw active shadows
			tft_draw_rectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,ACTIVE_COLOR);
			tft_draw_rectangle(checkbox->base.x1+2,checkbox->base.y1+2,checkbox->base.x2-2,checkbox->base.y2-2,ACTIVE_COLOR);
		break;
		case PEN_UP: //If the user took the pen away, while in the area (=toggle checkbox!)
			checkbox->checked=!checkbox->checked; //Toggle checkbox state
			gui_checkbox_update(checkbox); //redraw/overdraw tickmark
			if(checkbox->callback!=NULL) { //The user provided a callback
				checkbox->callback(checkbox,checkbox->checked); //Call the provided callback with the new checked state
			}
			// no break statement here!
		case PEN_LEAVE: //if the user "slided out" of the area
			area->hookedActions=PEN_DOWN;  //for the future we only want PEN_DOWN events

			//Draw inactive shadows
			tft_draw_rectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,BACKGROUND_COLOR);
			tft_draw_rectangle(checkbox->base.x1+2,checkbox->base.y1+2,checkbox->base.x2-2,checkbox->base.y2-2,BACKGROUND_COLOR);
		break;
		default:break;
	}
}

bool gui_checkbox_add(CHECKBOX_STRUCT* checkbox)
{
	if(touch_have_empty(1)) //Check if the touch module can handle one additional area
	{
		unsigned char size=0;
		checkbox->base.hookedActions=PEN_DOWN; //At first we are interested in PEN_DOWN events
		checkbox->base.callback = checkboxes_cb; //Use our own callback for the touch area events

		//Check the size of the checkbox
		if(checkbox->base.x2>checkbox->base.x1)
			size = checkbox->base.x2 - checkbox->base.x1; //use width a as size
		if(checkbox->base.y2>checkbox->base.y1)	
		{
			if((checkbox->base.y2 - checkbox->base.y1)>size)  //height is larger than size
				size = checkbox->base.y2 - checkbox->base.y1; //use height as size
		}
		if(size==0) { //no size found (maybe swap x2 and x1 or y2 and y1 ?)
			return false; //signal error
		}
		if((size&0x01)) //the size is an odd number
			size++; //make size an even number

		//Correct x2,y2 so that the checkbox is quadratic
		checkbox->base.x2 = checkbox->base.x1 + size;
		checkbox->base.y2 = checkbox->base.y1 + size;

		gui_checkbox_redraw(checkbox);//Call redraw method, which will take care of the drawing of the entire checkbox

		return touch_register_area(&checkbox->base); //Register the touch area and receive events for this checkbox, from now on
	}

	return false; //no more touch areas left
}
 
void gui_checkbox_redraw(CHECKBOX_STRUCT* checkbox)
{
	//Draw background and border
	tft_fill_rectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,BACKGROUND_COLOR);
	tft_draw_rectangle(checkbox->base.x1,checkbox->base.y1,checkbox->base.x2,checkbox->base.y2,BORDER_COLOR);

	if(checkbox->checked) { //checkbox is currently checked
		gui_checkbox_update(checkbox); //Call update method which will draw the tickmark
	}
}

void gui_checkbox_remove(CHECKBOX_STRUCT* checkbox)
{
	//We only need to unregister the touch area, as we have not allocated anything else
	 touch_unregister_area((TOUCH_AREA_STRUCT*)checkbox);
}

void gui_checkbox_update(CHECKBOX_STRUCT* checkbox)
{
	unsigned int c = (checkbox->checked)? checkbox->fgcolor:BACKGROUND_COLOR; //color to use for the tickmark

	//helper points inside the checkbox
	unsigned int xcent = checkbox->base.x1+(checkbox->base.x2-checkbox->base.x1)*6/14;
	unsigned int yleft = checkbox->base.y2 - (xcent- checkbox->base.x1) - 1 ;
	unsigned int yright = checkbox->base.y2 - (checkbox->base.x2 - xcent) - 1 ;
	unsigned int ybot = checkbox->base.y2 - 4;

	//Draw tickmark as a 3pixel wide line
	tft_draw_line(checkbox->base.x1+3,yleft-1,xcent,ybot -1,c);
	tft_draw_line(checkbox->base.x1+3,yleft,xcent,ybot ,c);
	tft_draw_line(checkbox->base.x1+3,yleft+1,xcent,ybot + 1,c);
	xcent++;
	ybot--;
	tft_draw_line(xcent,ybot-1,checkbox->base.x2-3,yright-1,c);
	tft_draw_line(xcent,ybot,checkbox->base.x2-3,yright+0,c);
	tft_draw_line(xcent,ybot+1,checkbox->base.x2-3,yright+1,c);
}
