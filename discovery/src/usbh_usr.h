/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          discovery/src/usbh_usr.h
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	21dd1e2	Starting to integrate usb branch. Optimized Makefiles
*
**************************************************************************************************************************************/


/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __USH_USR_H__
#define __USH_USR_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4_discovery.h"
#include "usbh_core.h"
#include <stdio.h>


/* Exported macros -----------------------------------------------------------*/
/* Exported variables --------------------------------------------------------*/
extern  USBH_Usr_cb_TypeDef USR_Callbacks;

/* Exported functions ------------------------------------------------------- */
void USBH_USR_Init(void);
void USBH_USR_DeviceAttached(void);
void USBH_USR_ResetDevice(void);
void USBH_USR_DeviceDisconnected(void);
void USBH_USR_OverCurrentDetected(void);
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed);
void USBH_USR_Device_DescAvailable(void*);
void USBH_USR_DeviceAddressAssigned(void);
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef* cfgDesc,
        USBH_InterfaceDesc_TypeDef* itfDesc,
        USBH_EpDesc_TypeDef* epDesc);
void USBH_USR_Manufacturer_String(void*);
void USBH_USR_Product_String(void*);
void USBH_USR_SerialNum_String(void*);
void USBH_USR_EnumerationDone(void);
USBH_USR_Status USBH_USR_UserInput(void);
int USBH_USR_MSC_Application(void);
void USBH_USR_DeInit(void);
void USBH_USR_DeviceNotSupported(void);
void USBH_USR_UnrecoveredError(void);
#ifdef __cplusplus
}
#endif

#endif /*__USH_USR_H__*/

/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
