/* 
 * pixy_control.h
 */

#ifndef _CONTROL_H_
#define _CONTROL_H_

#include<stdint.h>

void int_init(void);
uint16_t pixy_PID_Y(uint16_t x, uint16_t w);
uint16_t pixy_PID_X(uint16_t x, uint16_t w);

#endif
