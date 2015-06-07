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
