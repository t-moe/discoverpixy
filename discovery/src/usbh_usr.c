#include "usbh_usr.h"
#include <stdbool.h>
#include <string.h>
#include "pixy.h"



USBH_Usr_cb_TypeDef USR_Callbacks =
{
  USBH_USR_Init,
  USBH_USR_DeInit,
  USBH_USR_DeviceAttached,
  USBH_USR_ResetDevice,
  USBH_USR_DeviceDisconnected,
  USBH_USR_OverCurrentDetected,
  USBH_USR_DeviceSpeedDetected,
  USBH_USR_Device_DescAvailable,
  USBH_USR_DeviceAddressAssigned,
  USBH_USR_Configuration_DescAvailable,
  USBH_USR_Manufacturer_String,
  USBH_USR_Product_String,
  USBH_USR_SerialNum_String,
  USBH_USR_EnumerationDone,
  USBH_USR_UserInput,
  USBH_USR_MSC_Application,
  USBH_USR_DeviceNotSupported,
  USBH_USR_UnrecoveredError
};


/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/
bool manufacturer_ok;
bool product_ok;
bool serial_ok;

/**
  * @brief  USBH_USR_Init
  * @param  None
  * @retval None
  */
void USBH_USR_Init(void)
{
}

/**
  * @brief  USBH_USR_DeviceAttached
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAttached(void)
{
	manufacturer_ok=false;
	product_ok= false;
	serial_ok=false;

  
  STM_EVAL_LEDOff(LED5);
  STM_EVAL_LEDOff(LED3);
  STM_EVAL_LEDOn(LED4);
}

/**
  * @brief  USBH_USR_UnrecoveredError
  * @param  None
  * @retval None
  */
void USBH_USR_UnrecoveredError (void)
{
}

/**
  * @brief  USBH_DisconnectEvent
  *         Device disconnect event
  * @param  None
  * @retval Staus
  */
void USBH_USR_DeviceDisconnected (void)
{
  /* Red Led on if the USB Key is removed */
  STM_EVAL_LEDOn(LED5);
  STM_EVAL_LEDOff(LED4);
}

/**
  * @brief  USBH_USR_ResetUSBDevice
  * @param  None
  * @retval None
  */
void USBH_USR_ResetDevice(void)
{
  /* callback for USB-Reset */
}


/**
  * @brief  USBH_USR_DeviceSpeedDetected
  *         Displays the message on LCD for device speed
  * @param  Device speed:
  * @retval None
  */
void USBH_USR_DeviceSpeedDetected(uint8_t DeviceSpeed)
{
}

/**
  * @brief  USBH_USR_Device_DescAvailable
  * @param  device descriptor
  * @retval None
  */
void USBH_USR_Device_DescAvailable(void *DeviceDesc)
{
  /* callback for device descriptor */
}

/**
  * @brief  USBH_USR_DeviceAddressAssigned
  *         USB device is successfully assigned the Address
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceAddressAssigned(void)
{
  /* callback for device successfully assigned the Address */
}

/**
  * @brief  USBH_USR_Conf_Desc
  * @param  Configuration descriptor
  * @retval None
  */
void USBH_USR_Configuration_DescAvailable(USBH_CfgDesc_TypeDef * cfgDesc,
    USBH_InterfaceDesc_TypeDef *itfDesc,
    USBH_EpDesc_TypeDef *epDesc)
{
  /* callback for configuration descriptor */
}

/**
  * @brief  USBH_USR_Manufacturer_String
  * @param  Manufacturer String
  * @retval None
  */
void USBH_USR_Manufacturer_String(void *ManufacturerString)
{
	manufacturer_ok = strcmp((char*)ManufacturerString,"Charmed Labs") == 0;
  /* callback for  Manufacturer String */
}

/**
  * @brief  USBH_USR_Product_String
  * @param  Product String
  * @retval None
  */
void USBH_USR_Product_String(void *ProductString)
{
	product_ok = strcmp((char*)ProductString,"Pixy") == 0;
  /* callback for Product String */
}

/**
  * @brief  USBH_USR_SerialNum_String
  * @param  SerialNum_String
  * @retval None
  */
void USBH_USR_SerialNum_String(void *SerialNumString)
{
	serial_ok = strcmp((char*)SerialNumString,"DEMO 0.0") == 0;
  /* callback for SerialNum_String */
}

/**
  * @brief  EnumerationDone 
  *         User response request is displayed to ask application jump to class
  * @param  None
  * @retval None
  */
void USBH_USR_EnumerationDone(void)
{

} 

/**
  * @brief  USBH_USR_DeviceNotSupported
  *         Device is not supported
  * @param  None
  * @retval None
  */
void USBH_USR_DeviceNotSupported(void)
{
}


/**
  * @brief  USBH_USR_UserInput
  *         User Action for application state entry
  * @param  None
  * @retval USBH_USR_Status : User response for key button
  */
USBH_USR_Status USBH_USR_UserInput(void)
{
  if(product_ok&&manufacturer_ok&&serial_ok) {
	  return USBH_USR_RESP_OK;
  }
  return USBH_USR_NO_RESP;
}

/**
  * @brief  USBH_USR_OverCurrentDetected
  *         Over Current Detected on VBUS
  * @param  None
  * @retval None
  */
void USBH_USR_OverCurrentDetected (void)
{
}

/**
  * @brief  USBH_USR_MSC_Application
  * @param  None
  * @retval Staus
  */

/*int colorind;
const uint32_t colors [] = {0xFF0000, 0x00FF00,0x0000FF,0xFFFF00,0x00FFFF,0xFF00FF,0xFFFFFF,0x000000};
const int num_colors = sizeof(colors)/sizeof(uint32_t);
*/

int USBH_USR_MSC_Application(void)
{

/*	if(colorind==0) {
		pixy_led_set_max_current(5);
	}

	 int32_t response;
	 int return_value;
	 return_value = pixy_command("led_set", INT32(colors[colorind++]), END_OUT_ARGS,  &response, END_IN_ARGS);
	 colorind%=num_colors;
	 USB_OTG_BSP_mDelay(500);
*/

  return 0;
}

/**
  * @brief  USBH_USR_DeInit
  *         Deint User state and associated variables
  * @param  None
  * @retval None
  */
void USBH_USR_DeInit(void)
{

}

/**
  * @}
  */



/******************* (C) COPYRIGHT 2011 STMicroelectronics *****END OF FILE****/
