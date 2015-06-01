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
void ll_tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color);
void ll_tft_draw_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);
void ll_tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);
void ll_tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t *dat);
void ll_tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);


uint8_t ll_tft_num_fonts();
uint8_t ll_tft_font_height(uint8_t fontnum);
uint8_t ll_tft_font_width(uint8_t fontnum);
void ll_tft_draw_char(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, char c);

/*@}*/


