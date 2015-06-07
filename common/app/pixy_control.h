/* 
 * pixy_control.h
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include<stdint.h>

void int_init(void);
int16_t pixy_PID_Y(int16_t x, int16_t w);
int16_t pixy_PID_X(int16_t x, int16_t w);

#endif
