/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/lowlevel/ll_tft.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	51089aa	Refactored Project Structure for use with emulator
* 2015-04-03	timolang@gmail.com	1f2af9f	Added more tft functions to common and emulator. Fixed eventloop.
* 2015-04-03	timolang@gmail.com	9a1d12a	Refactored discovery, to use new project structure. Almost ready.
* 2015-04-03	timolang@gmail.com	1aa9194	Fixed Drawing of rects in Emulator. Got frames from pixy to emulator. Slooooow.
* 2015-04-27	aaron@duckpond.ch	aed90ef	Drawcircle added (emulator)
* 2015-04-27	timolang@gmail.com	e249fb2	Added font support
* 2015-04-27	aaron@duckpond.ch	f0a6c3b	Implemented init functions for gpio, fsmc and display
* 2015-04-27	aaron@duckpond.ch	0b61f21	Fixed misplacement of prototypes in ll_tft.h and implemented a propper init function.
* 2015-05-15	timolang@gmail.com	9a16865	Added doxgen comments to filesyste, checkbox, numupdown and screen module. And some minor changes to the other modules.
*
**************************************************************************************************************************************/

#include <stdint.h>
#include <stdbool.h>


/**
 * @addtogroup lowlevel
 */
/*@{*/

/**
 * @defgroup ll_tft TFT (LowLevel)
 * Low level functions for the \ref tft module
 */
/*@}*/

/**
 * @addtogroup ll_tft
 */
/*@{*/

// init functions
bool ll_tft_init();

// draw functions
void ll_tft_clear(uint16_t color);
void ll_tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ll_tft_draw_pixel(uint16_t x, uint16_t y, uint16_t color);
void ll_tft_draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ll_tft_fill_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void ll_tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat);
void ll_tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);


uint8_t ll_tft_num_fonts();
uint8_t ll_tft_font_height(uint8_t fontnum);
uint8_t ll_tft_font_width(uint8_t fontnum);
void ll_tft_draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, char c);

/*@}*/
