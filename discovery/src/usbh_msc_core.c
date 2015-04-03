
#include "usbh_msc_core.h"
#include "usbh_core.h"
#include "pixy.h"


static USBH_Status USBH_MSC_InterfaceInit  (USB_OTG_CORE_HANDLE *pdev ,
                                            void *phost);

static void USBH_MSC_InterfaceDeInit  (USB_OTG_CORE_HANDLE *pdev , 
                                       void *phost);

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                            void *phost);

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                         void *phost);


extern USB_OTG_CORE_HANDLE USB_OTG_Core;


USBH_Class_cb_TypeDef  USBH_MSC_cb = 
{
  USBH_MSC_InterfaceInit,
  USBH_MSC_InterfaceDeInit,
  USBH_MSC_ClassRequest,
  USBH_MSC_Handle,
};


typedef struct
{
  uint8_t              hc_num_in;
  uint8_t              hc_num_out;
  uint8_t              MSBulkOutEp;
  uint8_t              MSBulkInEp;
  uint16_t             MSBulkInEpSize;
  uint16_t             MSBulkOutEpSize;
}
MSC_Machine_TypeDef;
MSC_Machine_TypeDef MSC_Machine;

enum {init,running,down}state;


static USBH_Status USBH_MSC_InterfaceInit ( USB_OTG_CORE_HANDLE *pdev, 
                                        void *phost)
{	 
  USBH_HOST *pphost = phost;


  
  if((pphost->device_prop.Itf_Desc[1].bInterfaceClass == 255) &&
     (pphost->device_prop.Itf_Desc[1].bInterfaceProtocol == 0) &&
     (pphost->device_prop.Itf_Desc[1].bInterfaceSubClass == 1))
  {
    if(pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80)
    {
    	MSC_Machine.MSBulkInEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
    	MSC_Machine.MSBulkInEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
    }
    else
    {
    	MSC_Machine.MSBulkOutEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
    	MSC_Machine.MSBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
    }
    
    if(pphost->device_prop.Ep_Desc[0][1].bEndpointAddress & 0x80)
    {
    	MSC_Machine.MSBulkInEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
    	MSC_Machine.MSBulkInEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
    }
    else
    {
    	MSC_Machine.MSBulkOutEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
    	MSC_Machine.MSBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
    }
    
    MSC_Machine.hc_num_out = USBH_Alloc_Channel(pdev,MSC_Machine.MSBulkOutEp);
    MSC_Machine.hc_num_in = USBH_Alloc_Channel(pdev,MSC_Machine.MSBulkInEp);
    
    USBH_Open_Channel  (pdev,
    					MSC_Machine.hc_num_out,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        MSC_Machine.MSBulkOutEpSize);  
    
    USBH_Open_Channel  (pdev,
    					MSC_Machine.hc_num_in,
                        pphost->device_prop.address,
                        pphost->device_prop.speed,
                        EP_TYPE_BULK,
                        MSC_Machine.MSBulkInEpSize);    
    
  }
  
  else
  {
    pphost->usr_cb->USBH_USR_DeviceNotSupported(); 
  }

  return USBH_OK ;
 
}

void USBH_MSC_InterfaceDeInit ( USB_OTG_CORE_HANDLE *pdev,
                                void *phost)
{	
  if(state==running) {
	pixy_close();
	state=down;
  }

  if ( MSC_Machine.hc_num_out)
  {
    USB_OTG_HC_Halt(pdev, MSC_Machine.hc_num_out);
    USBH_Free_Channel  (pdev, MSC_Machine.hc_num_out);
    MSC_Machine.hc_num_out = 0; 
  }
   
  if ( MSC_Machine.hc_num_in)
  {
    USB_OTG_HC_Halt(pdev, MSC_Machine.hc_num_in);
    USBH_Free_Channel  (pdev, MSC_Machine.hc_num_in);
    MSC_Machine.hc_num_in = 0;  
  }
}

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE *pdev , 
                                        void *phost)
{   
  
  USBH_Status status = USBH_OK ;
  state=init;
  
  return status; 
}

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE *pdev , 
                                   void   *phost)
{
  USBH_HOST *pphost = phost;
    
  USBH_Status status = USBH_BUSY;

  if(HCD_IsDeviceConnected(pdev))
  {
    switch(state)
    {
    case init:
    	state = running;
    	USB_OTG_BSP_mDelay(3000); //let the pixy led flashing pass
    	pixy_init();
    	break;
    case running:
		pixy_service();
		int appliStatus = pphost->usr_cb->USBH_USR_MSC_Application();
		if(appliStatus == 0)
		{
		  state=running; //stay here
		}
		else if (appliStatus == 1)
		{
		  status =  USBH_APPLY_DEINIT;
		  pixy_close();
		  state=down;
		}
    	break;
    case down:
    	break;
    default:
    	break;
    }
  }
  return status;

}


volatile uint32_t cnt;
volatile uint32_t cnt_int;
void USBH_LL_systick() {
	cnt++;
	cnt_int++;
}


void USBH_LL_setTimer() {
	cnt=0;
}

uint32_t USBH_LL_getTimer() {
	return cnt;
}


int USBH_LL_open() {
	return 0; //ok
}

int USBH_LL_close() {
	return 0;
}

int USBH_LL_send(const uint8_t *data, uint32_t len, uint16_t timeoutMs) {
	USB_OTG_CORE_HANDLE *pdev = &USB_OTG_Core;
	
	USBH_BulkSendData (pdev,
                         (uint8_t*)data,
                         len ,
                         MSC_Machine.hc_num_out);

	URB_STATE state;
	cnt_int=0; //reset timer
	if(timeoutMs==0) timeoutMs=10000; //Force 10s timeout (testwise)

	while((state=HCD_GetURB_State(pdev , MSC_Machine.hc_num_out)) == URB_IDLE &&
	      (timeoutMs==0 || cnt_int < timeoutMs));

	if(state!=URB_DONE) {
		if(timeoutMs>0 && cnt_int>=timeoutMs) {
			STM_EVAL_LEDOn(LED3);
			return -7; //timeout (error code like with libusb

		}
		return -1;
	}
	return len;
}

int USBH_LL_receive(uint8_t *data, uint32_t len, uint16_t timeoutMs) {

	USB_OTG_CORE_HANDLE *pdev = &USB_OTG_Core;
	
	USBH_BulkReceiveData (pdev,
                         data,
                         len ,
                         MSC_Machine.hc_num_in);

	URB_STATE state;
	cnt_int=0; //reset timer
	if(timeoutMs==0) timeoutMs=10000; //Force 10s timeout (testwise)

	while((state=HCD_GetURB_State(pdev , MSC_Machine.hc_num_in)) == URB_IDLE &&
	      (timeoutMs==0 || cnt_int < timeoutMs));

	if(state!=URB_DONE) {
		if(timeoutMs>0 && cnt_int>=timeoutMs) {
			STM_EVAL_LEDOn(LED3);
			return -7; //timeout (error code like with libusb
		}
		return -1;
	}
	return len;
}
