#ifndef __USBLINK_H__
#define __USBLINK_H__

#include "link.h"

class USBLink : public Link
{
public:
    USBLink();
    ~USBLink();

    int open();
    virtual int send(const uint8_t *data, uint32_t len, uint16_t timeoutMs);
    virtual int receive(uint8_t *data, uint32_t len, uint16_t timeoutMs);
    virtual void setTimer();
    virtual uint32_t getTimer();
};

#endif

