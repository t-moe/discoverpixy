#include "app.h"



#include "usb_hcd_int.h"
#include "usbh_usr.h"
#include "usbh_core.h"
#include "usbh_msc_core.h"


extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;

int main(void)
{ 
  app_init();

  while (1)
  {
    USBH_Process(&USB_OTG_Core, &USB_Host);
    app_process();
  }
  
}
