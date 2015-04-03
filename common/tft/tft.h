#include<stdbool.h>
#include<stdint.h>

bool tft_init();
void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
