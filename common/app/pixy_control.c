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
