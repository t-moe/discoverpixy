#include <QThread>
#include <QApplication>

extern "C" {
#include "ll_system.h"
}

bool ll_system_init() {
    return true; //nothing to initialize here, apart from the stuff which is done in the main method.
}

void ll_system_delay(uint32_t msec) {
        QThread::msleep(msec); //Let the app_process() Thread sleep
}

void ll_system_process() {
        QApplication::processEvents(); //Process pending qt events
        QThread::msleep(1); //Sleep for 1ms, to keep the cpu load down
}

void ll_system_toggle_led() {
    //No led emulated :(
}
