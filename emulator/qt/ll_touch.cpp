
extern "C" {
#include "ll_touch.h"
#include "touch.h"
}

bool ll_touch_init() {
    touch_set_value_convert_mode(false); //tell the touch module that we don't need calibration or value conversion
    return true;
}
