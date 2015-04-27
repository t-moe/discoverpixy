#include<stdbool.h>
#include<stdint.h>


#define RGB(r,g,b) ((((r) & 0xF8) << 8) | (((g) & 0xFC) << 3) | (((b) & 0xF8) >> 3))
#define RED RGB(255,0,0)
#define GREEN RGB(0,255,0)
#define BLUE RGB(0,0,255)
#define WHITE RGB(255,255,255)
#define BLACK RGB(0,0,0)
#define HEX(h) (RGB(((h)>>16),((h)>>8),(h)))


bool tft_init();
void tft_clear(uint16_t color);
void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void tft_draw_pixel(uint16_t x,uint16_t y,uint16_t color);
void tft_draw_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);
void tft_fill_rectangle(uint16_t x1,uint16_t y1,uint16_t x2,uint16_t y2, uint16_t color);
void tft_draw_bitmap_unscaled(uint16_t x, uint16_t y, uint16_t width, uint16_t height, const uint16_t* dat);
void tft_draw_circle(uint16_t x, uint16_t y, uint16_t r, uint16_t color);
