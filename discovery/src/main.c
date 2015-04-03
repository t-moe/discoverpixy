#include "app.h"

int main(void)
{ 
  app_init();

  while (1)
  {
    app_process();
    //USBH_Process(&USB_OTG_Core, &USB_Host);
  }
  
}
