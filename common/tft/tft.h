#ifndef TFT_H
#define TFT_H

/**
 * @defgroup tft TFT
 * The TFT Modul provides access to the display
 */


/**
 * @addtogroup tft
 */
/*@{*/


#include<stdbool.h>
#include<stdint.h>

/**
 * Creates a 16bit color from 8bit * 3 colors (r,g,b)
 * @return
 */
#define RGB(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))

#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define WHITE 0xF7BE
#define BLACK RGB(0,0,0)

/**
 * Creates a 16bit color from a 24bit hex rgb color code
 * @return
 */
#define HEX(h) (RGB(((h)>>16),((h)>>8),(h)))

/**
 * Transparent color
 * @return
 */
#define TRANSPARENT ((uint16_t)0x80C2)

/**
 * Initializes the display. Call this method before using any tft_* functions
 * @return true on success
 */
bool tft_init();

/**
 * Clears the entire display with the given color. Overpaints everything which was there before.
 * @param color The 16-bit color to clear the display with.
 */
void tft_clear(uint16_t color);

/**
 * Draws a line onto the display. The pixels specified by start/end point are inclusive!
 * @param x1 The x-Coordinate of the start-point
 * @param y1 The y-Coordinate of the start-point
 * @param x2 The x-Coordinate of the end-point
 * @param y2 The y-Coordinate of the end-point
 * @param color The 16-bit color to draw the line with
 */
void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

/**
 * Draws a pixel onto the display.
 * @param x The x-Coordinate of the pixel
 * @param y The y-Coordinate of the pixel
 * @param The 16-bit color to draw the pixel with
 */
void tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color);

/**
 * Draws the outline of a rectangle onto the display. The outline is one pixel wide and goes through the specified start and endpoint.
 * @param x1 The x-Coordinate of the start-point
 * @param y1 The y-Coordinate of the start-point
 * @param x2 The x-Coordinate of the end-point
 * @param y2 The y-Coordinate of the end-point
 * @param The 16-bit color to draw the pixel with
 */
void tft_draw_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);

/**
 * Draws a filled rectangle onto the display. The start,end points are inclusive
 * @param x1 The x-Coordinate of the start-point
 * @param y1 The y-Coordinate of the start-point
 * @param x2 The x-Coordinate of the end-point
 * @param y2 The y-Coordinate of the end-point
 * @param The 16-bit color to draw the pixel with
 */
void tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);

/**
 * Draws a bitmap onto the display without scaling/cropping. The bitmap must be provided as an array of 16-bit colors
 * @param x The x-coordinate of the top-left corner to draw the bitmap at
 * @param y The y-coordinate of the top-left corner to draw the bitmap at
 * @param width The width of the bitmap in pixels
 * @param height The height of the bitmap in pixels
 * @param dat A pointer to a uint16_t array containing the colors for each pixel. Starting in the topleft and going from left to right, line by line.
 */
void tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat);

/**
 * Draws the outline of a circle onto the display
 * @param x The x-Coordinate of the center point
 * @param y The y-Coordinate of the center point
 * @param r The Radius in Pixels
 * @param color The 16-Bit color to draw the circle with
 */
void tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);

/**
 * Queries the number of available fonts
 * @return
 */
uint8_t tft_num_fonts();

/**
 * Get the height of a font
 * @param fontnum The number of the font, from 0 .. (num_fonts -1)
 * @return The height in pixel
 */
uint8_t tft_font_height(uint8_t fontnum);

/**
 * Get the width of a font
 * @param fontnum The number of the font, from 0 .. (num_fonts -1)
 * @return The width in pixel
 */
uint8_t tft_font_width(uint8_t fontnum);

/**
 * Prints a unformatted/preformatted string onto the display
 * @param x The x-Coordinate of the Top-Left corner where the text should be drawn
 * @param y The y-Coordinate of the Top-Left corner where the text should be drawn
 * @param color The 16-bit foreground color of the text
 * @param bgcolor The 16-bit background color of the text. You may pass TRANSPARENT as Color
 * @param font The Fontnum to use for drawing
 * @param text The text to draw
 */
void tft_print_line(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* text);

/**
 * Prints a formatted text (like printf) onto the display
 * @param x The x-Coordinate of the Top-Left corner where the text should be drawn
 * @param y The y-Coordinate of the Top-Left corner where the text should be drawn
 * @param color The 16-bit foreground color of the text
 * @param bgcolor The 16-bit background color of the text. You may pass TRANSPARENT as Color
 * @param font The Fontnum to use for drawing
 * @param format The format string (like printf)
 */
void tft_print_formatted(uint16_t x, uint16_t y, uint16_t color, uint16_t bgcolor, uint8_t font, const char* format, ...);

/*@}*/

#endif /* TFT_H */
