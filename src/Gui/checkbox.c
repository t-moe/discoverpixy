#include "../settings.h"
#include "../types.h"
#include "../tft/tft.h"
#include "../touch/touch.h"
#include "checkbox.h"

#define BRIGHTNESS_VAL 2 //How much the Brightness is in/decreased for checkbox shadows (3 -> Add 1/3 off Full Value)
#define ACTIVE_COLOR RGB(251,208,123)
#define BORDER_COLOR RGB(29,82,129)
#define BACKGROUND_COLOR WHITE

void checkboxes_cb(void* touchArea, TOUCH_ACTION triggeredAction)
{
	TOUCH_AREA_STRUCT * area = (TOUCH_AREA_STRUCT*)touchArea;
	CHECKBOX_STRUCT* checkbox = (CHECKBOX_STRUCT*)touchArea;
	switch(triggeredAction)
	{
		case PEN_DOWN:
			area->hookedActions=PEN_UP|PEN_LEAVE;
			tftDrawRectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,ACTIVE_COLOR);
			tftDrawRectangle(checkbox->base.x1+2,checkbox->base.y1+2,checkbox->base.x2-2,checkbox->base.y2-2,ACTIVE_COLOR);
		break;
		case PEN_UP:
			checkbox->checked=!checkbox->checked;
			guiUpdateCheckbox(checkbox);
			if(checkbox->callback!=NULL)
				checkbox->callback(checkbox,checkbox->checked);
		case PEN_LEAVE:
			area->hookedActions=PEN_DOWN;
			tftDrawRectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,BACKGROUND_COLOR);
			tftDrawRectangle(checkbox->base.x1+2,checkbox->base.y1+2,checkbox->base.x2-2,checkbox->base.y2-2,BACKGROUND_COLOR);
		break;
		default:break;
	}
}

 BOOL guiAddCheckbox(CHECKBOX_STRUCT* checkbox)
 {
	if(touchHaveEmpty(1))
	{
		unsigned char size=0;
		checkbox->base.hookedActions=PEN_DOWN;
		checkbox->base.callback = checkboxes_cb;
		if(checkbox->base.x2>checkbox->base.x1)
			size = checkbox->base.x2 - checkbox->base.x1;
		if(checkbox->base.y2>checkbox->base.y1)	
		{
			if((checkbox->base.y2 - checkbox->base.y1)>size)	
				size = checkbox->base.y2 - checkbox->base.y1;
		}
		if((size&0x01))
			size++;
		checkbox->base.x2 = checkbox->base.x1 + size;
		checkbox->base.y2 = checkbox->base.y1 + size;
		guiRedrawCheckbox(checkbox);	
		return touchRegisterArea(&checkbox->base);		
	}
	return FALSE;
 }
 
void guiRedrawCheckbox(CHECKBOX_STRUCT* checkbox)
{
	tftFillRectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,BACKGROUND_COLOR);
	tftDrawRectangle(checkbox->base.x1,checkbox->base.y1,checkbox->base.x2,checkbox->base.y2,BORDER_COLOR);
	if(checkbox->checked)
		guiUpdateCheckbox(checkbox);
}
 void guiRemoveCheckbox(CHECKBOX_STRUCT* checkbox)
 {
	 touchUnregisterArea((TOUCH_AREA_STRUCT*)checkbox);
 }
 void guiUpdateCheckbox(CHECKBOX_STRUCT* checkbox)
 {
	unsigned int c = (checkbox->checked)? checkbox->fgcolor:BACKGROUND_COLOR;
	unsigned int  xcent = checkbox->base.x1+(checkbox->base.x2-checkbox->base.x1)*6/14;
	unsigned int yleft = checkbox->base.y2 - (xcent- checkbox->base.x1) - 1 ;
	unsigned int yright = checkbox->base.y2 - (checkbox->base.x2 - xcent) - 1 ;
	unsigned int ybot = checkbox->base.y2 - 4;
	tftDrawLine(checkbox->base.x1+3,yleft-1,xcent,ybot -1,c);
	tftDrawLine(checkbox->base.x1+3,yleft,xcent,ybot ,c);
	tftDrawLine(checkbox->base.x1+3,yleft+1,xcent,ybot + 1,c);
	xcent++;
	ybot--;
	tftDrawLine(xcent,ybot-1,checkbox->base.x2-3,yright-1,c);
	tftDrawLine(xcent,ybot,checkbox->base.x2-3,yright+0,c);
	tftDrawLine(xcent,ybot+1,checkbox->base.x2-3,yright+1,c);

 }