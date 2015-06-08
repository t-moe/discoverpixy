/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          common/app/pixy_control.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-06-02	aaron@duckpond.ch	e018a75	Implemented basic pi and pid controller
* 2015-06-06	aaron@duckpond.ch	8c264c2	Comment refactoring, updated PID values
* 2015-06-06	aaron@duckpond.ch	a04cda9	Refactured comments and implemented a bugfix for the PID controller
* 2015-06-07	aaron@duckpond.ch	802d3df	Fixed pid controller and refactored code
* 2015-06-07	aaron@duckpond.ch	3d98ca9	Minor changes
* 2015-06-07	timolang@gmail.com	c87220d	Renamed pixy_helper to pixy_frame. Updated docu of appliaction. added doxygen comments to pixy_{frame,control}.h
*
**************************************************************************************************************************************/

/*
 * pixy_control.c
 *
 * Notation
 * --------
 *
 * x    :   Sollwert (Führgrösse)
 * w    :   Istwert (Reglergrösse)
 * esum :   Integralteil
 * e    :   Regelabweichung
 * y    :   Stellgrösse
 *
 *
 */
#include<pixy_control.h>
#include<stdint.h>

// PID tuning factors
#define REG_PID_KP      (0.5f)
#define REG_PID_KI      (0.001f)
#define REG_PID_KD      (0.001f)
#define REG_PID_TA      (0.01f)


// PID controller implementatoin for the y-axis
int16_t pixy_PID_Y(int16_t x, int16_t w)
{
    float e = 0;
    static float esum = 0;
    static float eold = 0;
    float y = 0;

    e = (float)(x - w);                         // calculate the controller offset

    //----PID-control-------------------------------------------------------------------------
    esum = esum + e;                            // add e to the current sum

    y += REG_PID_KP * e;                        // add the proportional part to the output
    y += REG_PID_KI * REG_PID_TA * esum;        // add the integral part to the output
    y += REG_PID_KD * (e - eold) / REG_PID_TA;  // add the differential part to the output
    //----------------------------------------------------------------------------------------

    eold = e;                                   // save the previous value

    return (int16_t)y;
}

// PID controller implementation for the x-axis
int16_t pixy_PID_X(int16_t x, int16_t w)
{
    float e = 0;
    static float esum = 0;
    static float eold = 0;
    float y = 0;

    e = (float)(x - w);                         // calculate the controller offset

    //----PID-control-------------------------------------------------------------------------
    esum = esum + e;                            // add e to the current sum

    y += REG_PID_KP * e;                        // add the proportional part to the output
    y += REG_PID_KI * REG_PID_TA * esum;        // add the integral part to the output
    y += REG_PID_KD * (e - eold) / REG_PID_TA;  // add the differential part to the output
    //----------------------------------------------------------------------------------------

    eold = e;                                   // save the previous value

    return (int16_t)y;
}
