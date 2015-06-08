/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          emulator/qt/ll_touch.cpp
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-27	timolang@gmail.com	259d446	Added touch support to emulator. Implemented basic touch function.
* 2015-06-01	timolang@gmail.com	eb573bc	Finalized calibration. Fixed a bug in screen module.
*
**************************************************************************************************************************************/


extern "C" {
#include "ll_touch.h"
#include "touch.h"
}

bool ll_touch_init()
{
    touch_set_value_convert_mode(false); //tell the touch module that we don't need calibration or value conversion
    return true;
}
