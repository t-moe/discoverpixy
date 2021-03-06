/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/pixy_frame.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-06-07	timolang@gmail.com	c87220d	Renamed pixy_helper to pixy_frame. Updated docu of appliaction. added doxygen comments to pixy_{frame,control}.h
*
**************************************************************************************************************************************/

#ifndef PIXY_FRAME_H
#define PIXY_FRAME_H

#include <stdbool.h>
#include <stdint.h>
#include "filesystem.h"


/**
 * @addtogroup app
 */
/*@{*/

/**
 * @defgroup pixy_helper Pixy Frame Helper
 * A collection of helper functions that allow receiving and rendering a frame from pixy onto the display.
 * Furthermore you can select a color in a frame, to use for tracking.
 */
/*@}*/


/**
 * @addtogroup pixy_helper
 */
/*@{*/


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

/*@}*/

#endif /* PIXY_FRAME_H */
