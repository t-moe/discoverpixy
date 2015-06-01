#include "tft.h"
#include "touch.h"
#include "button.h"
#include "numupdown.h"
#include <stdio.h> //for sprintf
#include <stddef.h> //for offsetof macro
#include <stdlib.h> //for abs

/* The idea is as follows:
 * When the user add's a numupdown we create two buttons, one with a plus and one with a minus sign in it
 * When the user presses one of the buttons we check and increase the value and execute the provided user callback
 */


#define BASE_COLOR RGB(90,90,90) //Background color for the whole element

//Callback which is called when the user presses the "plus" button
void button_up_cb(void* button)
{
	//Get the pointer to the numupdown: subtract the offset of the buttonUp member in the struct from the button pointer
	NUMUPDOWN_STRUCT* element = button-offsetof(NUMUPDOWN_STRUCT,buttonUp);

	if(element->value<element->max) { //old value lies below the maximum
		element->value++; //let's increase the value
		gui_numupdown_update(element); //and redraw everything
		if(element->callback!=NULL) { //the user provided a callback
			element->callback(element,element->value); //Call the user callback with the new value
		}
	}
}

//Callback which is called when the user presses the "minus" button
void button_down_cb(void* button)
{
	//Get the pointer to the numupdown: subtract the offset of the buttonDown member in the struct from the button pointer
	NUMUPDOWN_STRUCT* element = button-offsetof(NUMUPDOWN_STRUCT,buttonDown);

	if(element->value>element->min) { //old value lies above the minimum
		element->value--; //let's decrease the value
		gui_numupdown_update(element); //and redraw everything
		if(element->callback!=NULL) { //the user provided a callback
			element->callback(element,element->value); //Call the user callback with the new value
		}
	}
}

//Method to calculate the number of characters needed to print the provided number in decimal notation (with optional sign)
static uint8_t calc_text_width(int16_t val) {
	uint8_t width = 1 + (val<0); //1 if positive, 2 if negative (to let space for sign)	
	val=abs(val); //Make the number positive
	while(val>=10) { //while we have two or more digits
		val/=10; //remove one digit
		width++; //add one character
	}
	return width;
}


bool gui_numupdown_add(NUMUPDOWN_STRUCT* numupdown)
{
	 if(touch_have_empty(2)) //Check if the touch module can handle two additional areas
	 {
		if(numupdown->min > numupdown->max) { //min is bigger than max?
			return false; //invalid parameter
		}

		if(numupdown->value < numupdown->min) { //value is smaller than min?
			numupdown->value = numupdown->min; //normalize value
		} else if(numupdown->value > numupdown->max) { //value is bigger than max?
			numupdown->value = numupdown->max;  //normalize value
		}
		
		 uint8_t tw1 = calc_text_width(numupdown->max); //Calculate character width to render maximum value
		 uint8_t tw2 = calc_text_width(numupdown->min); //Calculate character width to render minimum value
		 if(tw2 > tw1) tw1 = tw2; //ensure tw1 contains the larger number of the two
	     uint8_t width= tft_font_width(0)*(tw1+1); //Calculate width of the number area

	     //Add "minus" button to the left side of the number area
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

		 //Add "plus" button to the right side of the number area
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

		 //Draw background and label of the number area
		 tft_fill_rectangle(numupdown->buttonDown.base.x2+2,numupdown->y,numupdown->buttonDown.base.x2+width,numupdown->buttonUp.base.y2,BASE_COLOR);
		 tft_print_formatted(numupdown->buttonDown.base.x2+2+tft_font_width(0)/2,numupdown->y+tft_font_height(0)/2,numupdown->fgcolor,BASE_COLOR,0,"%*d",tw1,numupdown->value);
		
		 return true;
	 }

	 return false; //not enough touch areas left
 }
 
void gui_numupdown_remove(NUMUPDOWN_STRUCT* numupdown)
{
	 //remove the two buttons, we have no other allocated resources
	 gui_button_remove(&numupdown->buttonUp);
	 gui_button_remove(&numupdown->buttonDown);
}


void gui_numupdown_redraw(NUMUPDOWN_STRUCT* numupdown)
{
	//redraw the two buttons
	gui_button_redraw(&numupdown->buttonUp);
	gui_button_redraw(&numupdown->buttonDown);

	//call update method which will take care of the number-area rendering
	gui_numupdown_update(numupdown);
}

void gui_numupdown_update(NUMUPDOWN_STRUCT* numupdown)
{
	 //Calculate the number area width again (see above)
	 uint8_t tw1 = calc_text_width(numupdown->max);
	 uint8_t tw2 = calc_text_width(numupdown->min);
	 if(tw2 > tw1) tw1 = tw2;
	 uint8_t width= tft_font_width(0)*(tw1+1);

	 //Draw background and label of the number area
	 tft_fill_rectangle(numupdown->buttonDown.base.x2+2,numupdown->y,numupdown->buttonDown.base.x2+width,numupdown->buttonUp.base.y2,BASE_COLOR);
	 tft_print_formatted(numupdown->buttonDown.base.x2+2+tft_font_width(0)/2,numupdown->y+tft_font_height(0)/2,numupdown->fgcolor,BASE_COLOR,0,"%*d",tw1,numupdown->value);
}


