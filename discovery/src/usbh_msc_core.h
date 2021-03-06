/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          discovery/src/usbh_msc_core.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	21dd1e2	Starting to integrate usb branch. Optimized Makefiles
*
**************************************************************************************************************************************/


/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_MSC_CORE_H
#define __USBH_MSC_CORE_H

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"
#include "usbh_stdreq.h"
#include "usb_bsp.h"
#include "usbh_ioreq.h"
#include "usbh_hcs.h"


extern USBH_Class_cb_TypeDef  USBH_MSC_cb;

void USBH_LL_systick();
int USBH_LL_open();
int USBH_LL_close();
int USBH_LL_send(const uint8_t* data, uint32_t len, uint16_t timeoutMs);
int USBH_LL_receive(uint8_t* data, uint32_t len, uint16_t timeoutMs);
void USBH_LL_setTimer();
uint32_t USBH_LL_getTimer();


#endif  /* __USBH_MSC_CORE_H */
