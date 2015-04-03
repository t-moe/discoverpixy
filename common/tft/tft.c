#include "tft.h"
#include "ll_tft.h"



bool tft_init() {
	return ll_tft_init();

}

void tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color) {
	ll_tft_draw_line(x1,y1,x2,y2,color);
}
