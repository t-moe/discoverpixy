/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/gui/button.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-27	timolang@gmail.com	7c9eabc	Added button support.
* 2015-05-17	timolang@gmail.com	2d46336	Improved comments in implementation of button, checkbox, numupdown, tft, touch and screen modules/submodules.
*
**************************************************************************************************************************************/

#include "tft.h"
#include "touch.h"
#include "button.h"
#include <string.h>

/* The Idea is as follows:
 * When the user add's a button we create a touch area for that region and wait for PEN_DOWN events.
 * Once the user puts the pen down in this area we'll redraw the button with different shadows (feedback)
 * and we'll now wait on PEN_UP or PEN_LEAVE events.
 * If the user takes the pen away while in the area (PEN_UP), we call the provided user callback
 * Otherwise (PEN_LEAVE) we only restore the initial shadows
 */

/* Possible improvements:
 * Move the button by 1 pixel while he is pressed, to create a "full 3d" experience
 * Add events for the case when the button is pressed for a long time, without release
 */

//Method to calculate the shadow colors used to create the "3d" effect
void calculate_shadows(uint16_t bgcolor, uint16_t* light_shadow, uint16_t* dark_shadow)
{
#define BRIGHTNESS_VAL 3 //How much the Brightness is in/decreased for button shadows (3 -> Add/Subtract 1/3 off Full Value)

    uint16_t c_light, c_dark; //c_light and c_dark will be filled with a lighter and a darker color as the background color (for the shadows)
    uint8_t r, g, b;

    //separate the channels of the 16-bit rgb565 color
    r = (bgcolor & 0xF800) >> 11;
    g = (bgcolor & 0x07E0) >> 5;
    b = (bgcolor & 0x001F) >> 0;

    //For the light shadow color:
    if ((r + 0x1F / BRIGHTNESS_VAL)  > 0x1F) { //Adding one third would exceed the maximum of the red channel
        c_light = 0xF800;    //Use full red
    } else { //adding one third to the red channel is fine
        c_light = (r + 0x1F / BRIGHTNESS_VAL) << 11;    //Use same red as in the background, but add one third
    }

    if ((g + 0x3F / BRIGHTNESS_VAL)  > 0x3F) { //same for the green channel
        c_light |= 0x07E0;
    } else {
        c_light |= (g + 0x3F / BRIGHTNESS_VAL) << 5;
    }

    if ((b + 0x1F / BRIGHTNESS_VAL)  > 0x1F) { //and the blue channel
        c_light |= 0x0018;
    } else {
        c_light |= (b + 0x1F / BRIGHTNESS_VAL) << 0;
    }

    //For the dark shadow color
    if (r  > (0x1F / BRIGHTNESS_VAL)) { //Subtracting one third would NOT exceed the minimum of the red channel
        c_dark = (r - 0x1F / BRIGHTNESS_VAL) << 11;    //Use same red as in the background, but subtract one third
    } else { //Subtracting one third would give us a number below zero
        c_dark = 0x0000;    //use no red channel
    }

    if (g > (0x3F / BRIGHTNESS_VAL)) { //Same for the green channel
        c_dark |= (g - 0x3F / BRIGHTNESS_VAL) << 5;
    }

    if (b > (0x1F / BRIGHTNESS_VAL)) { //and the blue channel
        c_dark |= (b - 0x1F / BRIGHTNESS_VAL) << 0;
    }

    //Assign the calculated shadows to out parameters
    if (light_shadow != NULL) {
        *light_shadow = c_light;
    }

    if (dark_shadow != NULL) {
        *dark_shadow = c_dark;
    }

}

//Callback which is called when the user touches the touch-area we created for the button
void buttons_cb(void* touchArea, TOUCH_ACTION triggeredAction)
{
    TOUCH_AREA_STRUCT* area = (TOUCH_AREA_STRUCT*)touchArea;
    BUTTON_STRUCT* button = (BUTTON_STRUCT*)touchArea;

    uint16_t c_light, c_dark; //c_light and c_dark will be filled with a lighter and a darker color as the background color (for the shadows)
    calculate_shadows(button->bgcolor, &c_light, &c_dark);

    switch (triggeredAction) {
    case PEN_DOWN: //If the user touches the area for the "first time"
        area->hookedActions = PEN_UP | PEN_LEAVE; //for the future we only want PEN_UP and PEN_LEAVE events

        //Draw shadows
        tft_draw_line(button->base.x1 + 1, button->base.y1, button->base.x2 - 1, button->base.y1, c_dark); //North
        tft_draw_line(button->base.x1, button->base.y1 + 1, button->base.x1, button->base.y2 - 1, c_dark); //West
        tft_draw_line(button->base.x1 + 1, button->base.y2, button->base.x2 - 1, button->base.y2, c_light); //South
        tft_draw_line(button->base.x2, button->base.y1 + 1, button->base.x2, button->base.y2 - 1, c_light); //East
        break;

    case PEN_UP: //If the user took the pen away, while in the area (=button pressed!)
    case PEN_LEAVE: //or the user "slided out" of the area
        area->hookedActions = PEN_DOWN; //for the future we only want PEN_DOWN events

        //Draw inverse shadows
        tft_draw_line(button->base.x1 + 1, button->base.y1, button->base.x2 - 1, button->base.y1, c_light); //North
        tft_draw_line(button->base.x1, button->base.y1 + 1, button->base.x1, button->base.y2 - 1, c_light); //West
        tft_draw_line(button->base.x1 + 1, button->base.y2, button->base.x2 - 1, button->base.y2, c_dark); //South
        tft_draw_line(button->base.x2, button->base.y1 + 1, button->base.x2, button->base.y2 - 1, c_dark); //East

        if (triggeredAction == PEN_UP && button->callback != NULL) { //If the button got "pressed" instead of left, and the user provided a callback
            button->callback(button);    //execute the user callback
        }

        break;

    default:
        break;
    }
}


bool gui_button_add(BUTTON_STRUCT* button)
{
    if (touch_have_empty(1)) { //Check if the touch module can handle one additional area
        //Calculate width and height of the button text
        unsigned int strwidth = tft_font_width(button->font) * strlen(button->text);
        unsigned char strheight = tft_font_height(button->font);

        button->base.hookedActions = PEN_DOWN; //At first we are interested in PEN_DOWN events
        button->base.callback = buttons_cb; //Use our own callback for the touch area events

        if (button->base.x2 == AUTO) { //The user wants us to calculate the button width automatically
            //Use string width + half of a character width as button width
            button->base.x2 = button->base.x1 - 1 + strwidth + (tft_font_width(button->font) / 2);
        } else if ((button->base.x2 - button->base.x1 + 1) < (strwidth + 2)) { //the provided width is too small to fit the entire text
            return false; //report error
        }

        if (button->base.y2 == AUTO) { //The user wants us to calculate the button height automatically
            //Use one and a half character heights as button height
            button->base.y2 = button->base.y1 - 1 + strheight + (strheight / 2);
        } else if ((button->base.y2 - button->base.y1 + 1) < (strheight + 2)) { //the provided height is too small to fit the text
            return  false;
        }

        gui_button_redraw(button); //call the redraw method, which will take care of drawing the entire button
        return touch_register_area(&button->base);	//Register the touch area and receive events for this button, from now on
    }

    return false; //no more touch areas left
}

void gui_button_redraw(BUTTON_STRUCT* button)
{
    //Calculate text dimensions and shadow colors
    unsigned int strwidth = tft_font_width(button->font) * strlen(button->text);
    unsigned char strheight = tft_font_height(button->font);
    uint16_t c_light, c_dark;
    calculate_shadows(button->bgcolor, &c_light, &c_dark);

    //Draw the background and the 4 lines (shadow colors)
    tft_fill_rectangle(button->base.x1 + 1, button->base.y1 + 1, button->base.x2 - 1, button->base.y2 - 1, button->bgcolor);
    tft_draw_line(button->base.x1 + 1, button->base.y1, button->base.x2 - 1, button->base.y1, c_light); //North
    tft_draw_line(button->base.x1, button->base.y1 + 1, button->base.x1, button->base.y2 - 1, c_light); //West
    tft_draw_line(button->base.x1 + 1, button->base.y2, button->base.x2 - 1, button->base.y2, c_dark); //South
    tft_draw_line(button->base.x2, button->base.y1 + 1, button->base.x2, button->base.y2 - 1, c_dark); //East

    //Draw the text
    tft_print_line(button->base.x1 + (button->base.x2 - button->base.x1 + 1 - strwidth) / 2, button->base.y1 + (button->base.y2 - button->base.y1 + 1 - strheight) / 2, button->txtcolor, button->bgcolor, button->font, button->text);

}

void gui_button_remove(BUTTON_STRUCT* button)
{
    //We only need to unregister the touch area, as we have not allocated anything else
    touch_unregister_area((TOUCH_AREA_STRUCT*)button);
}
