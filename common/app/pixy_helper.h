#ifndef PIXY_HELPER_H
#define PIXY_HELPER_H

#include <stdbool.h>
#include <stdint.h>

int pixy_render_full_frame(uint16_t x, uint16_t y);
int pixy_render_cropped_frame(uint16_t x, uint16_t y, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height);

#endif /* PIXY_HELPER_H */
