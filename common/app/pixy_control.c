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

// PID tuning factors
#define REG_PID_KP      1
#define REG_PID_KI      1
#define REG_PID_KD      1
#define REG_PID_TA      1

#define REG_PI_KP       1
#define REG_PI_KI       1
#define REG_PI_TA       1

void int_init(void){  
    // TODO Init ports and outputs if needed.
}

// PI controller
uint16_t pixy_PI(uint16_t x, uint16_t w)
{
    uint16_t        y = 0;
    float           e = 0;
    static float    esum;

    // Calculate controller offset
    e = x - w;

    //----PID-control--------------------------------

    // Integrate and check boundaries if necassary
    esum = esum + e;
    //esum = (esum < -400) ? -400 : esum;
    //esum = (esum > 400)  ?  400 : esum;

    // PI controller equation
    y = ( REG_PI_KP * e ) + ( REG_PI_KI * REG_PI_TA * esum);

    //-----------------------------------------------

    return y;
}

// PID controller
uint16_t control_SpeedPID(uint16_t x, uint16_t w)
{
    float e;
    static float esum;
    static float ealt;
    uint16_t y;

    // Calculate controller offset
    e = x - w;
    
    //----PID-control--------------------------------
    
    // Integrate and check boundaries if necessary
    esum = esum + e;
    //esum = (esum > 50)? 50 : esum;
    //esum = (esum < -50)? -50 : esum;

    // PID controller equation
    y = REG_PID_KP * e + REG_PID_KI * REG_PID_TA * esum + REG_PID_KD * (e - ealt)/REG_PID_TA;
    
    //-----------------------------------------------

    // save old value
    ealt = e;

    return y;
}

