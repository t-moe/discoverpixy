#include <stdint.h>


#ifdef __cplusplus
extern "C" {
#endif

	bool ll_tft_init();
	void ll_tft_draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);

#ifdef __cplusplus
}
#endif

