//
//
// SUPPORT DISCONTINUED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
//
//
//
#include "../settings.h"
#include "../types.h"
#include "../tft/tft.h"
#include "../touch/touch.h"
#include "button.h"
#include "numupdown.h"

#define BASE_COLOR RGB(90,90,90)
void button_up_cb(void* button)
{
	
}
void button_down_cb(void* button)
{
	
}


 BOOL guiAddNumUpDown(NUMUPDOWN_STRUCT* numupdown)
 {
	 if(touchHaveEmpty(2)) //We require 2 TouchAreas (for Buttons)
	 {
		 unsigned char width =31;
		 if(numupdown->max>=10) width+=16;
		 else if(numupdown->max>=100) width+=16*2;
		 numupdown->buttonUp.base.x1=numupdown->x;
		 numupdown->buttonUp.base.y1=numupdown->y;
		 numupdown->buttonUp.base.x2=numupdown->x+width;
		 numupdown->buttonUp.base.y2=AUTO;
		 numupdown->buttonUp.text="+";
		 numupdown->buttonUp.font=BigFont;
		 numupdown->buttonUp.bgcolor=BASE_COLOR;
		 numupdown->buttonUp.txtcolor=WHITE;
		 numupdown->buttonUp.callback = button_up_cb;
		 guiAddButton(&numupdown->buttonUp);
		 numupdown->buttonDown.base.x1=numupdown->x;
		 numupdown->buttonDown.base.y1=numupdown->buttonUp.base.y2+31;
		 numupdown->buttonDown.base.x2=numupdown->x+width;
		 numupdown->buttonDown.base.y2=AUTO;
		 numupdown->buttonDown.text="-";
		 numupdown->buttonDown.font=BigFont;
		 numupdown->buttonDown.bgcolor=BASE_COLOR;
		 numupdown->buttonDown.txtcolor=WHITE;
		 numupdown->buttonDown.callback = button_down_cb;
		 guiAddButton(&numupdown->buttonDown);
		 tftFillRectangle(numupdown->x,numupdown->buttonUp.base.y2+1,numupdown->x+width,numupdown->buttonDown.base.y1-1,BASE_COLOR);
		 tftPrintf_P(numupdown->x,numupdown->buttonUp.base.y2+7,numupdown->fgcolor,BASE_COLOR,BigFont,PSTR("%03u"),numupdown->value);
	 }
	 return FALSE;
	/*if(touchHaveEmptyAreaPlace())
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
		size/=DISTANCE_BORDER;	
			
		tftFillRectangle(checkbox->base.x1+1,checkbox->base.y1+1,checkbox->base.x2-1,checkbox->base.y2-1,BACKGROUND_COLOR);
		tftDrawRectangle(checkbox->base.x1,checkbox->base.y1,checkbox->base.x2,checkbox->base.y2,BORDER_COLOR);
		if(checkbox->checked)
			guiUpdateCheckbox(checkbox);
		return touchRegisterArea(&checkbox->base);		
	}
	return FALSE;*/
 }
 void guiRemoveNumUpDown(NUMUPDOWN_STRUCT* numupdown)
 {
	 guiRemoveButton(&numupdown->buttonUp);
	 guiRemoveButton(&numupdown->buttonDown);
 }
 void guiUpdateNumUpDown(NUMUPDOWN_STRUCT* numupdown)
 {
	 tftPrintf_P(numupdown->x,numupdown->buttonUp.base.y2+7,numupdown->fgcolor,BASE_COLOR,BigFont,PSTR("%03u"),numupdown->value);

 }
 void guiRedrawNumUpDown(NUMUPDOWN_STRUCT* numupdown)
 {
	 
 }
