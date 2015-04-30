#include "tft.h"
#include "touch.h"
#include "button.h"
#include "numupdown.h"
#include <stdio.h> //for sprintf
#include <stddef.h> //for offsetof macro
#include <stdlib.h> //for abs


#define BASE_COLOR RGB(90,90,90)
void button_up_cb(void* button)
{
	NUMUPDOWN_STRUCT* element = button-offsetof(NUMUPDOWN_STRUCT,buttonUp);
	if(element->value<element->max) {
		element->value++;
		gui_numupdown_update(element);		
		if(element->callback!=NULL) {
			element->callback(element,element->value);
		}
	}
}

void button_down_cb(void* button)
{
	NUMUPDOWN_STRUCT* element = button-offsetof(NUMUPDOWN_STRUCT,buttonDown);
	if(element->value>element->min) {
		element->value--;
		gui_numupdown_update(element);		
		if(element->callback!=NULL) {
			element->callback(element,element->value);
		}
	}
}


static uint8_t calc_text_width(int16_t val) {
	uint8_t width = 1 + (val<0); //1 if positive, 2 if negative (to let space for sign)	
	val=abs(val);
	while(val>=10) {
		val/=10;
		width++;
	}
	return width;
}


bool gui_numupdown_add(NUMUPDOWN_STRUCT* numupdown)
{
	 if(touch_have_empty(2)) //We require 2 TouchAreas (for Buttons)
	 {
		if(numupdown->min > numupdown->max) return false;

		if(numupdown->value > numupdown->max) {
			numupdown->value = numupdown->max;
		}
		if(numupdown->value < numupdown->min) {
			numupdown->value = numupdown->min;
		} else if(numupdown->value > numupdown->max) {
			numupdown->value = numupdown->max;
		}
		
		 uint8_t tw1 = calc_text_width(numupdown->max);
		 uint8_t tw2 = calc_text_width(numupdown->min);
		 if(tw2 > tw1) tw1 = tw2;
	         uint8_t width= tft_font_width(0)*(tw1+1);	

		 numupdown->buttonDown.base.x1=numupdown->x;
		 numupdown->buttonDown.base.y1=numupdown->y;
		 numupdown->buttonDown.base.x2=AUTO;
		 numupdown->buttonDown.base.y2=numupdown->y+tft_font_height(0)*2;
		 numupdown->buttonDown.text="-";
		 numupdown->buttonDown.font=0;
		 numupdown->buttonDown.bgcolor=BASE_COLOR;
		 numupdown->buttonDown.txtcolor=WHITE;
		 numupdown->buttonDown.callback = button_down_cb;
		 gui_button_add(&numupdown->buttonDown);
		 numupdown->buttonUp.base.x1=numupdown->buttonDown.base.x2+width+2;
		 numupdown->buttonUp.base.y1=numupdown->y;
		 numupdown->buttonUp.base.x2=AUTO;
		 numupdown->buttonUp.base.y2=numupdown->y +tft_font_height(0)*2;
		 numupdown->buttonUp.text="+";
		 numupdown->buttonUp.font=0;
		 numupdown->buttonUp.bgcolor=BASE_COLOR;
		 numupdown->buttonUp.txtcolor=WHITE;
		 numupdown->buttonUp.callback = button_up_cb;
		 gui_button_add(&numupdown->buttonUp);

		 tft_fill_rectangle(numupdown->buttonDown.base.x2+2,numupdown->y,numupdown->buttonDown.base.x2+width,numupdown->buttonUp.base.y2,BASE_COLOR);
		 tft_print_formatted(numupdown->buttonDown.base.x2+2+tft_font_width(0)/2,numupdown->y+tft_font_height(0)/2,numupdown->fgcolor,BASE_COLOR,0,"%*d",tw1,numupdown->value);
		
		 return true;
	 }
	 return false;
 }
 
void gui_numupdown_remove(NUMUPDOWN_STRUCT* numupdown)
 {
	 gui_button_remove(&numupdown->buttonUp);
	 gui_button_remove(&numupdown->buttonDown);
 }

void gui_numupdown_update(NUMUPDOWN_STRUCT* numupdown)
 {
	
	 
	 uint8_t tw1 = calc_text_width(numupdown->max);
	 uint8_t tw2 = calc_text_width(numupdown->min);
	 if(tw2 > tw1) tw1 = tw2;
	 uint8_t width= tft_font_width(0)*(tw1+1);

	 tft_fill_rectangle(numupdown->buttonDown.base.x2+2,numupdown->y,numupdown->buttonDown.base.x2+width,numupdown->buttonUp.base.y2,BASE_COLOR);
	 tft_print_formatted(numupdown->buttonDown.base.x2+2+tft_font_width(0)/2,numupdown->y+tft_font_height(0)/2,numupdown->fgcolor,BASE_COLOR,0,"%*d",tw1,numupdown->value);
 }

 void gui_numupdown_redraw(NUMUPDOWN_STRUCT* numupdown)
 {
	gui_button_redraw(&numupdown->buttonUp);
	gui_button_redraw(&numupdown->buttonDown);
	gui_numupdown_update(numupdown);	 
 }
