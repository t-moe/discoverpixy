#include <QThread>
extern "C" {
#include "ll_system.h"
}

bool ll_system_init() {
    return true;
}

void ll_system_delay(uint32_t msec) {
        QThread::msleep(msec);
}

