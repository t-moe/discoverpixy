#ifndef PIXY_HELPER_H
#define PIXY_HELPER_H

#include <stdbool.h>
#include <stdint.h>
#include "filesystem.h"

/**
 * Receives a fullsized frame from pixy and display's it on the display with the topleft corner at (x,y)
 * @param x The x-Coordinate of the top left corner
 * @param y The y-Coordinate of the top left corner
 * @return 0 on success, otherwise the errorcode from pixy
 */
int pixy_render_full_frame(uint16_t x, uint16_t y);

/**
 * Receives a cropped frame from pixy and display's it on the display with the topleft corner at (x,y)
 * @param x The x-Coordinate of the top left corner to draw the image
 * @param y The y-Coordinate of the top left corner to draw the image
 * @param xoffset The x-Coordinate on the pixy image from where on you want the frame data
 * @param yoffset The y-Coordinate on the pixy image from where on you want the frame data
 * @param width The width of the image recorded from pixy
 * @param height The height of the image recorded from pixy
 * @return 0 on success, otherwise the errorcode from pixy
 */
int pixy_render_cropped_frame(uint16_t x, uint16_t y, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height);

/**
 * Receives a fullsized frame from pixy and saves it to the given file in the 24bit (b,g,a) format.
 * Use this method to write the bitmap-data part of a windows bitmap (.bmp).
 * This method will neither open nor close the passed file.
 * @param handle The file to write the data to. The file must be open and it should be seeked to the right position.
 * @return 0 on success, otherwise the errorcode from pixy
 */
int pixy_save_full_frame(FILE_HANDLE* handle);

/**
 * Receives a cropped frame from pixy and saves it to the given file in the 24bit (b,g,a) format.
 * @param handle The file to write the data to. The file must be open and it should be seeked to the right position.
 * @param xoffset The x-Coordinate on the pixy image from where on you want the frame data
 * @param yoffset The y-Coordinate on the pixy image from where on you want the frame data
 * @param width The width of the image recorded from pixy
 * @param height The height of the image recorded from pixy
 * @return 0 on success, otherwise the errorcode from pixy
 */
int pixy_save_cropped_frame(FILE_HANDLE* handle, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height);

/**
 * Sets the color signature  to the color in the selected region of the frame
 * @param signum the color signature number (1..7)
 * @param xoffset The x-Coordinate of the topleft point of the region
 * @param yoffset The y-Coordinate of the topleft point of the region
 * @param width The width of the region
 * @param height The height of the region
 * @return 0 on success, otherwise the errorcode from pixy
 */
int pixy_cc_set_region(uint8_t signum, uint16_t xoffset, uint16_t yoffset, uint16_t width, uint16_t height);

#endif /* PIXY_HELPER_H */
