#include <unistd.h>
#include <stdio.h>
#include "usblink.h"
#include "pixy.h"
#include "debuglog.h"

extern "C" {
  extern int USBH_LL_open();
  extern int USBH_LL_close();
  extern int USBH_LL_send(const uint8_t *data, uint32_t len, uint16_t timeoutMs);
  extern int USBH_LL_receive(uint8_t *data, uint32_t len, uint16_t timeoutMs);
  extern void USBH_LL_setTimer();
  extern uint32_t USBH_LL_getTimer();
}


USBLink::USBLink()
{
  m_blockSize = 64;
  m_flags = LINK_FLAG_ERROR_CORRECTED;
}

USBLink::~USBLink()
{
    USBH_LL_close();
}

int USBLink::open()
{
    return USBH_LL_open();
}

int USBLink::send(const uint8_t *data, uint32_t len, uint16_t timeoutMs)
{
    return USBH_LL_send(data,len,timeoutMs);
}

int USBLink::receive(uint8_t *data, uint32_t len, uint16_t timeoutMs)
{
    return USBH_LL_receive(data,len,timeoutMs);
}

void USBLink::setTimer()
{
    USBH_LL_setTimer();
}

uint32_t USBLink::getTimer()
{
  return USBH_LL_getTimer();
}

