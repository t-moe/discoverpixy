/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          discovery/src/ll_system.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	9a1d12a	Refactored discovery, to use new project structure. Almost ready.
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
* 2015-04-25	timolang@gmail.com	0858b0d	Fixed some bugs when receiving large data.
* 2015-04-27	aaron@duckpond.ch	0b61f21	Fixed misplacement of prototypes in ll_tft.h and implemented a propper init function.
* 2015-06-02	timolang@gmail.com	16bfdad	Removed conflicting led usage from usb code.
*
**************************************************************************************************************************************/

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

void ll_system_process()
{
    USBH_Process(&USB_OTG_Core, &USB_Host);
}

void ll_system_delay(uint32_t msec)
{
    USB_OTG_BSP_mDelay(msec);
}

void ll_system_toggle_led()
{
    //no led's free :(
}
