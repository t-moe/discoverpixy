/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/pixy_control.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-06-02	aaron@duckpond.ch	e018a75	Implemented basic pi and pid controller
* 2015-06-06	aaron@duckpond.ch	8c264c2	Comment refactoring, updated PID values
* 2015-06-06	aaron@duckpond.ch	a04cda9	Refactured comments and implemented a bugfix for the PID controller
* 2015-06-07	aaron@duckpond.ch	802d3df	Fixed pid controller and refactored code
* 2015-06-07	timolang@gmail.com	c87220d	Renamed pixy_helper to pixy_frame. Updated docu of appliaction. added doxygen comments to pixy_{frame,control}.h
*
**************************************************************************************************************************************/

#ifndef PIXY_CONTROL_H_
#define PIXY_CONTROL_H_

#include<stdint.h>

/**
 * @addtogroup app
 */
/*@{*/

/**
 * @defgroup pixy_control Pixy Control Helper
 * A collection of helper functions that contain PID Control code used for object tracking.
 */
/*@}*/


/**
 * @addtogroup pixy_control
 */
/*@{*/

/**
 * Execute one step of PID regulation for the Y-servo.
 * @param x desired value (e.g. current y-position of the biggest block)
 * @param w actual value (e.g desired y-position)
 * @return The offset which needs to be added to the Y-Servo position
 */
int16_t pixy_PID_Y(int16_t x, int16_t w);

/**
 * Execute one step of PID regulation for the X-servo.
 * @param x desired value (e.g. current x-position of the biggest block)
 * @param w actual value (e.g desired x-position)
 * @return The offset which needs to be added to the X-Servo position
 */
int16_t pixy_PID_X(int16_t x, int16_t w);

/*@}*/

#endif /* PIXY_CONTROL_H_ */
