#include "tft.h"
#include "touch.h"
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
			tft_draw_line(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c2); //Nord
			tft_draw_line(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c2);//West
			tft_draw_line(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c1); //Süd
			tft_draw_line(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c1);  //Ost	
		break;
		case PEN_UP:
		case PEN_LEAVE:
			area->hookedActions=PEN_DOWN;
			tft_draw_line(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c1); //Nord
			tft_draw_line(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c1);//West
			tft_draw_line(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c2); //Süd
			tft_draw_line(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c2);  //Ost
			if(triggeredAction==PEN_UP && button->callback!=NULL)
				button->callback(button);
		break;
		default:break;
	}
}

	
bool gui_button_add(BUTTON_STRUCT* button)//Registers a button (fill Struct first). Return false if no more Space in the Pointertable (-->Change NUM_AREAS).
{
	if(touch_have_empty(1))
	{
		unsigned int strwidth=tft_font_width(button->font)*strlen(button->text);
		unsigned char strheight=tft_font_height(button->font);
		button->base.hookedActions=PEN_DOWN;
		button->base.callback = buttons_cb;
		if(button->base.x2==AUTO)
			button->base.x2= button->base.x1 -1 + strwidth+(tft_font_width(button->font)/2);
		else if((button->base.x2-button->base.x1+1)<(strwidth+2))
			return false;
	
		if(button->base.y2==AUTO)
			button->base.y2=button->base.y1 -1 +strheight+(strheight/2);
		else if((button->base.y2-button->base.y1+1)<(strheight+2))
			return  false;
		gui_button_redraw(button);	
		return touch_register_area(&button->base);		
	}
	return false;
}

void gui_button_redraw(BUTTON_STRUCT* button)
{
     	unsigned int strwidth=tft_font_width(button->font)*strlen(button->text);
        unsigned char strheight=tft_font_height(button->font);
	unsigned char r,g,b;
	unsigned int c;
	r=(button->bgcolor&0xF800)>>11;
	g=(button->bgcolor&0x07E0)>>5;
	b=(button->bgcolor&0x001F)>>0;
	tft_fill_rectangle(button->base.x1+1,button->base.y1+1,button->base.x2-1,button->base.y2-1,button->bgcolor);
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
	tft_draw_line(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c); //Nord
	tft_draw_line(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c);//West
	if(r  > (0x1F/BRIGHTNESS_VAL))
		c=(r-0x1F/BRIGHTNESS_VAL)<<11;
	else
		c=0x0000;
	if(g > (0x3F/BRIGHTNESS_VAL))
		c|=(g-0x3F/BRIGHTNESS_VAL)<<5;
	if(b > (0x1F/BRIGHTNESS_VAL))
		c|=(b-0x1F/BRIGHTNESS_VAL)<<0;
	tft_draw_line(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c); //Süd
	tft_draw_line(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c);  //Ost
	tft_print_line(button->base.x1+(button->base.x2-button->base.x1+1-strwidth)/2,button->base.y1+(button->base.y2-button->base.y1+1-strheight)/2,button->txtcolor,button->bgcolor,button->font,button->text);	
		
}
void gui_button_remove(BUTTON_STRUCT* button)
{
	touch_unregister_area((TOUCH_AREA_STRUCT*)button);
}

/*
bool guiAddBitmapButton (BITMAPBUTTON_STRUCT* button)
{
	if(touchHaveEmpty(1))
	{
		button->base.hookedActions=PEN_DOWN;
		button->base.callback = buttons_cb;
		if(button->base.x2==AUTO)
			button->base.x2= button->base.x1 -1 + button->imgwidth + button->imgwidth/4;
		else if((button->base.x2-button->base.x1+1)<(button->imgwidth+2))
			return false;
			
		if(button->base.y2==AUTO)
			button->base.y2=button->base.y1 -1 +button->imgheight + button->imgheight/4;
		else if((button->base.y2-button->base.y1+1)<(button->imgheight+2))
			return  false;
		guiRedrawBitmapButton(button);	
		return touchRegisterArea(&button->base);		
	}
	return false;
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
	tft_draw_line(button->base.x1+1,button->base.y1,button->base.x2-1,button->base.y1,c); //Nord
	tft_draw_line(button->base.x1,button->base.y1+1,button->base.x1,button->base.y2-1,c);//West
	if(r  > (0x1F/BRIGHTNESS_VAL))
		c=(r-0x1F/BRIGHTNESS_VAL)<<11;
	else
		c=0x0000;
	if(g > (0x3F/BRIGHTNESS_VAL))
		c|=(g-0x3F/BRIGHTNESS_VAL)<<5;
	if(b > (0x1F/BRIGHTNESS_VAL))
		c|=(b-0x1F/BRIGHTNESS_VAL)<<0;
	tft_draw_line(button->base.x1+1,button->base.y2,button->base.x2-1,button->base.y2,c); //Süd
	tft_draw_line(button->base.x2,button->base.y1+1,button->base.x2,button->base.y2-1,c);  //Ost
	tftDrawBitmapUnscaledStreamedRaw(button->base.x1+(button->base.x2-button->base.x1+1-button->imgwidth)/2,button->base.y1+(button->base.y2-button->base.y1+1-button->imgheight)/2,button->imgwidth,button->imgheight,button->filename);	
}
void guiRemoveBitmapButton(BITMAPBUTTON_STRUCT* button)
{
	touchUnregisterArea((TOUCH_AREA_STRUCT*)button);
}

*/
