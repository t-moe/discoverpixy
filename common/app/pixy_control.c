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
#define REG_PID_KP      (0.23f)
#define REG_PID_KI      (1.2f)
#define REG_PID_KD      (0.01f)
#define REG_PID_TA      (0.02f)

void int_init(void){  
    // TODO Init ports and outputs if needed.
}

// PID controller implementatoin for the y-axis
uint16_t pixy_PID_Y(uint16_t x, uint16_t w)
{
    float e;
    static float esum;
    static float ealt;
    uint16_t y;

    // Calculate controller offset
    e = x - w;

    //----PID-control-------------------------------------------------------------------------
    esum = esum + e;                            // add e to the current sum

    esum = (esum > 1000)  ? 1000 : esum;        // check upper boundary and limit size
    esum = (esum < 0) ? 0 : esum;               // check lower boundary and limit size

    // PID controller equation
    y = REG_PID_KP * e + REG_PID_KI * REG_PID_TA * esum + REG_PID_KD * (e - ealt)/REG_PID_TA;
    //----------------------------------------------------------------------------------------

    // save old value
    ealt = e;

    return y;
}

// PID controller implementation for the x-axis
uint16_t pixy_PID_X(uint16_t x, uint16_t w)
{
    float e;
    static float esum;
    static float ealt;
    uint16_t y;

    // Calculate controller offset
    e = x - w;

    //----PID-control-------------------------------------------------------------------------
    esum = esum + e;                            // add e to the current sum

    esum = (esum > 1000)  ? 1000 : esum;        // check upper boundary and limit size
    esum = (esum < 0) ? 0 : esum;               // check lower boundary and limit size

    // PID controller equation
    y = REG_PID_KP * e + REG_PID_KI * REG_PID_TA * esum + REG_PID_KD * (e - ealt)/REG_PID_TA;
    //----------------------------------------------------------------------------------------

    // save old value
    ealt = e;

    return y;
}
