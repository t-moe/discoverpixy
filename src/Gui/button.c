#include "../settings.h"
#include "../types.h"
#include "../tft/tft.h"
#include "../touch/touch.h"
#include "button.h"
#include <string.h>

#define BRIGHTNESS_VAL 3 //How much the Brightness is in/decreased for button shadows (3 -> Add 1/3 off Full Value)

void buttons_cb(void* touchArea, TOUCH_ACTION triggeredAction)
//Method shared between normal Buttons and Bitmap Buttons-> Look at comment in headerfile for explanation.
{
	TOUCH_AREA_STRUCT * area = (TOUCH_AREA_STRUCT*)touchArea;
	BUTTON_STRUCT* button = (BUTTON_STRUCT*)touchArea; 
	unsigned int c1,c2;
	unsigned char r,g,b;
	r=(button->bgcolor&0xF800)>>11;
	g=(button->bgcolor&0x07E0)>>5;
	b=(button->bgcolor&0x001F)>>0;
	if((r + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c1=0xF800;
	else
		c1=(r+0x1F/BRIGHTNESS_VAL)<<11;
	if((g + 0x3F/BRIGHTNESS_VAL)  >0x3F)
		c1|=0x07E0;
	else
		c1|=(g+0x3F/BRIGHTNESS_VAL)<<5;
	if((b + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c1|=0x0018;
	else
		c1|=(b+0x1F/BRIGHTNESS_VAL)<<0;
	if(r  > (0x1F/BRIGHTNESS_VAL))
		c2=(r-0x1F/BRIGHTNESS_VAL)<<11;
	else
		c2=0x0000;
	if(g > (0x3F/BRIGHTNESS_VAL))
		c2|=(g-0x3F/BRIGHTNESS_VAL)<<5;
	if(b > (0x1F/BRIGHTNESS_VAL))
		c2|=(b-0x1F/BRIGHTNESS_VAL)<<0;
	switch(triggeredAction)
	{
		case PEN_DOWN:
			area->hookedActions=PEN_UP|PEN_LEAVE;
			tftDrawLine(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c2); //Nord
			tftDrawLine(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c2);//West
			tftDrawLine(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c1); //Süd
			tftDrawLine(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c1);  //Ost	
		break;
		case PEN_UP:
		case PEN_LEAVE:
			area->hookedActions=PEN_DOWN;
			tftDrawLine(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c1); //Nord
			tftDrawLine(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c1);//West
			tftDrawLine(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c2); //Süd
			tftDrawLine(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c2);  //Ost
			if(triggeredAction==PEN_UP && button->callback!=NULL)
				button->callback(button);
		break;
		default:break;
	}
}

	
BOOL guiAddButton(BUTTON_STRUCT* button)//Registers a button (fill Struct first). Return FALSE if no more Space in the Pointertable (-->Change NUM_AREAS).
{
	if(touchHaveEmpty(1))
	{
		unsigned int strwidth=FLASH_CHAR_ARR_READ(&button->font[0])*strlen(button->text);
		unsigned char strheight=FLASH_CHAR_ARR_READ(&button->font[1]);
		button->base.hookedActions=PEN_DOWN;
		button->base.callback = buttons_cb;
		if(button->base.x2==AUTO)
			button->base.x2= button->base.x1 -1 + strwidth+(FLASH_CHAR_ARR_READ(&button->font[0])/2);
		else if((button->base.x2-button->base.x1+1)<(strwidth+2))
			return FALSE;
	
		if(button->base.y2==AUTO)
			button->base.y2=button->base.y1 -1 +strheight+(strheight/2);
		else if((button->base.y2-button->base.y1+1)<(strheight+2))
			return  FALSE;
		guiRedrawButton(button);	
		return touchRegisterArea(&button->base);		
	}
	return FALSE;
}

void guiRedrawButton(BUTTON_STRUCT* button)
{
	unsigned int strwidth=FLASH_CHAR_ARR_READ(&button->font[0])*strlen(button->text);
	unsigned char strheight=FLASH_CHAR_ARR_READ(&button->font[1]);
	unsigned char r,g,b;
	unsigned int c;
	r=(button->bgcolor&0xF800)>>11;
	g=(button->bgcolor&0x07E0)>>5;
	b=(button->bgcolor&0x001F)>>0;
	tftFillRectangle(button->base.x1+1,button->base.y1+1,button->base.x2-1,button->base.y2-1,button->bgcolor);
	if((r + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c=0xF800;
	else
		c=(r+0x1F/BRIGHTNESS_VAL)<<11;
	if((g + 0x3F/BRIGHTNESS_VAL)  >0x3F)
		c|=0x07E0;
	else 
		c|=(g+0x3F/BRIGHTNESS_VAL)<<5;
	if((b + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c|=0x0018;
	else
		c|=(b+0x1F/BRIGHTNESS_VAL)<<0;
	tftDrawLine(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c); //Nord
	tftDrawLine(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c);//West
	if(r  > (0x1F/BRIGHTNESS_VAL))
		c=(r-0x1F/BRIGHTNESS_VAL)<<11;
	else
		c=0x0000;
	if(g > (0x3F/BRIGHTNESS_VAL))
		c|=(g-0x3F/BRIGHTNESS_VAL)<<5;
	if(b > (0x1F/BRIGHTNESS_VAL))
		c|=(b-0x1F/BRIGHTNESS_VAL)<<0;
	tftDrawLine(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c); //Süd
	tftDrawLine(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c);  //Ost
	tftPrint(button->base.x1+(button->base.x2-button->base.x1+1-strwidth)/2,button->base.y1+(button->base.y2-button->base.y1+1-strheight)/2,button->txtcolor,button->bgcolor,button->font,button->text);	
		
}
void guiRemoveButton(BUTTON_STRUCT* button)
{
	touchUnregisterArea((TOUCH_AREA_STRUCT*)button);
}


BOOL guiAddBitmapButton (BITMAPBUTTON_STRUCT* button)
{
	if(touchHaveEmpty(1))
	{
		button->base.hookedActions=PEN_DOWN;
		button->base.callback = buttons_cb;
		if(button->base.x2==AUTO)
			button->base.x2= button->base.x1 -1 + button->imgwidth + button->imgwidth/4;
		else if((button->base.x2-button->base.x1+1)<(button->imgwidth+2))
			return FALSE;
			
		if(button->base.y2==AUTO)
			button->base.y2=button->base.y1 -1 +button->imgheight + button->imgheight/4;
		else if((button->base.y2-button->base.y1+1)<(button->imgheight+2))
			return  FALSE;
		guiRedrawBitmapButton(button);	
		return touchRegisterArea(&button->base);		
	}
	return FALSE;
}

void guiRedrawBitmapButton(BITMAPBUTTON_STRUCT* button)
{
	
	unsigned char r,g,b;
	unsigned int c;
	r=(button->bgcolor&0xF800)>>11;
	g=(button->bgcolor&0x07E0)>>5;
	b=(button->bgcolor&0x001F)>>0;
	tftFillRectangle(button->base.x1+1,button->base.y1+1,button->base.x2-1,button->base.y2-1,button->bgcolor);
	if((r + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c=0xF800;
	else
		c=(r+0x1F/BRIGHTNESS_VAL)<<11;
	if((g + 0x3F/BRIGHTNESS_VAL)  >0x3F)
		c|=0x07E0;
	else 
		c|=(g+0x3F/BRIGHTNESS_VAL)<<5;
	if((b + 0x1F/BRIGHTNESS_VAL)  >0x1F)
		c|=0x0018;
	else
		c|=(b+0x1F/BRIGHTNESS_VAL)<<0;
	tftDrawLine(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c); //Nord
	tftDrawLine(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c);//West
	if(r  > (0x1F/BRIGHTNESS_VAL))
		c=(r-0x1F/BRIGHTNESS_VAL)<<11;
	else
		c=0x0000;
	if(g > (0x3F/BRIGHTNESS_VAL))
		c|=(g-0x3F/BRIGHTNESS_VAL)<<5;
	if(b > (0x1F/BRIGHTNESS_VAL))
		c|=(b-0x1F/BRIGHTNESS_VAL)<<0;
	tftDrawLine(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c); //Süd
	tftDrawLine(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c);  //Ost
	tftDrawBitmapUnscaledStreamedRaw(button->base.x1+(button->base.x2-button->base.x1+1-button->imgwidth)/2,button->base.y1+(button->base.y2-button->base.y1+1-button->imgheight)/2,button->imgwidth,button->imgheight,button->filename);	
}
void guiRemoveBitmapButton(BITMAPBUTTON_STRUCT* button)
{
	touchUnregisterArea((TOUCH_AREA_STRUCT*)button);
}
