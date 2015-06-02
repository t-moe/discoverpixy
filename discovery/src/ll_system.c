#include "ll_system.h"
#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include <stdio.h>

#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"

USB_OTG_CORE_HANDLE          USB_OTG_Core;
USBH_HOST                    USB_Host;
RCC_ClocksTypeDef RCC_Clocks;

void SysTick_Handler(void)
{
  USBH_LL_systick();
}

void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}

void OTG_FS_IRQHandler(void)
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

bool ll_system_init(void)
{ 
  /* Initialize LEDS */
  STM_EVAL_LEDInit(LED3);
  STM_EVAL_LEDInit(LED4);
  //LED5 and LED6 can not be used because of pin conflict. See docu
  
  STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO);
 
       
  /* SysTick end of count event each 1ms */
  RCC_GetClocksFreq(&RCC_Clocks); //we run at 168mhz :)
  SysTick_Config(RCC_Clocks.HCLK_Frequency / 1000);
  
  /* Init Host Library */
  USBH_Init(&USB_OTG_Core, USB_OTG_FS_CORE_ID, &USB_Host, &USBH_MSC_cb, &USR_Callbacks);
  
  return true;
}

void ll_system_process() {
	USBH_Process(&USB_OTG_Core, &USB_Host);
}

void ll_system_delay(uint32_t msec) {
	USB_OTG_BSP_mDelay(msec);
}

void ll_system_toggle_led() {	
    //no led's free :(
}
