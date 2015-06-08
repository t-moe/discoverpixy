/**************************************************************************************************************************************
* Project:       discoverpixy
* Website:       https://github.com/t-moe/discoverpixy
* Authors:       Aaron Schmocker, Timo Lang
* Institution:   BFH Bern University of Applied Sciences
* File:          discovery/src/usbh_msc_core.c
*
* Version History:
* Date			Autor Email			SHA		Changes
* 2015-04-03	timolang@gmail.com	21dd1e2	Starting to integrate usb branch. Optimized Makefiles
* 2015-04-25	timolang@gmail.com	416883c	Pixy&Usb work with the new folder structure now.
* 2015-04-25	timolang@gmail.com	3d1e4b2	Simplified code a bit. Emulator does not work stable when replugging pixy.
*
**************************************************************************************************************************************/


#include "usbh_msc_core.h"
#include "usbh_core.h"


static USBH_Status USBH_MSC_InterfaceInit(USB_OTG_CORE_HANDLE* pdev ,
        void* phost);

static void USBH_MSC_InterfaceDeInit(USB_OTG_CORE_HANDLE* pdev ,
                                     void* phost);

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE* pdev ,
                                   void* phost);

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE* pdev ,
        void* phost);


extern USB_OTG_CORE_HANDLE USB_OTG_Core;
extern USBH_HOST USB_Host;



USBH_Class_cb_TypeDef  USBH_MSC_cb = {
    USBH_MSC_InterfaceInit,
    USBH_MSC_InterfaceDeInit,
    USBH_MSC_ClassRequest,
    USBH_MSC_Handle,
};


typedef struct {
    uint8_t              hc_num_in;
    uint8_t              hc_num_out;
    uint8_t              MSBulkOutEp;
    uint8_t              MSBulkInEp;
    uint16_t             MSBulkInEpSize;
    uint16_t             MSBulkOutEpSize;
}
MSC_Machine_TypeDef;
MSC_Machine_TypeDef MSC_Machine;


static USBH_Status USBH_MSC_InterfaceInit(USB_OTG_CORE_HANDLE* pdev,
        void* phost)
{
    USBH_HOST* pphost = phost;



    if ((pphost->device_prop.Itf_Desc[1].bInterfaceClass == 255) &&
            (pphost->device_prop.Itf_Desc[1].bInterfaceProtocol == 0) &&
            (pphost->device_prop.Itf_Desc[1].bInterfaceSubClass == 1)) {
        if (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress & 0x80) {
            MSC_Machine.MSBulkInEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
            MSC_Machine.MSBulkInEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
        } else {
            MSC_Machine.MSBulkOutEp = (pphost->device_prop.Ep_Desc[0][0].bEndpointAddress);
            MSC_Machine.MSBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][0].wMaxPacketSize;
        }

        if (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress & 0x80) {
            MSC_Machine.MSBulkInEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
            MSC_Machine.MSBulkInEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
        } else {
            MSC_Machine.MSBulkOutEp = (pphost->device_prop.Ep_Desc[0][1].bEndpointAddress);
            MSC_Machine.MSBulkOutEpSize  = pphost->device_prop.Ep_Desc[0][1].wMaxPacketSize;
        }

        MSC_Machine.hc_num_out = USBH_Alloc_Channel(pdev, MSC_Machine.MSBulkOutEp);
        MSC_Machine.hc_num_in = USBH_Alloc_Channel(pdev, MSC_Machine.MSBulkInEp);

        USBH_Open_Channel(pdev,
                          MSC_Machine.hc_num_out,
                          pphost->device_prop.address,
                          pphost->device_prop.speed,
                          EP_TYPE_BULK,
                          MSC_Machine.MSBulkOutEpSize);

        USBH_Open_Channel(pdev,
                          MSC_Machine.hc_num_in,
                          pphost->device_prop.address,
                          pphost->device_prop.speed,
                          EP_TYPE_BULK,
                          MSC_Machine.MSBulkInEpSize);

    }

    else {
        pphost->usr_cb->USBH_USR_DeviceNotSupported();
    }

    return USBH_OK ;

}

void USBH_MSC_InterfaceDeInit(USB_OTG_CORE_HANDLE* pdev,
                              void* phost)
{
    if (MSC_Machine.hc_num_out) {
        USB_OTG_HC_Halt(pdev, MSC_Machine.hc_num_out);
        USBH_Free_Channel(pdev, MSC_Machine.hc_num_out);
        MSC_Machine.hc_num_out = 0;
    }

    if (MSC_Machine.hc_num_in) {
        USB_OTG_HC_Halt(pdev, MSC_Machine.hc_num_in);
        USBH_Free_Channel(pdev, MSC_Machine.hc_num_in);
        MSC_Machine.hc_num_in = 0;
    }
}

static USBH_Status USBH_MSC_ClassRequest(USB_OTG_CORE_HANDLE* pdev ,
        void* phost)
{

    USBH_Status status = USBH_OK ;
    return status;
}

static USBH_Status USBH_MSC_Handle(USB_OTG_CORE_HANDLE* pdev ,
                                   void*   phost)
{
    USBH_HOST* pphost = phost;

    USBH_Status status = USBH_BUSY;

    if (HCD_IsDeviceConnected(pdev)) {
        int appliStatus = pphost->usr_cb->USBH_USR_MSC_Application();

        if (appliStatus != 0) {
            status =  USBH_APPLY_DEINIT;
        }
    }

    return status;

}


volatile uint32_t cnt;
volatile uint32_t cnt_int;
void USBH_LL_systick()
{
    cnt++;
    cnt_int++;
}


void USBH_LL_setTimer()
{
    cnt = 0;
}

uint32_t USBH_LL_getTimer()
{
    return cnt;
}


int USBH_LL_open()
{
    int timeoutDetect = 100;
    int timeoutStartup = 3000;
    cnt_int = 0; //reset timer

    while (USB_Host.gState != HOST_CLASS && cnt_int < timeoutDetect) {
        USBH_Process(&USB_OTG_Core, &USB_Host);
    }

    if (USB_Host.gState != HOST_CLASS) {
        return -5; // = LIBUSB_ERROR_NOT_FOUND
    }


    cnt_int = 0;

    while (cnt_int < timeoutStartup) { //let pixy's led flashing pass
        USBH_Process(&USB_OTG_Core, &USB_Host);
    }

    return 0; //ok
}

int USBH_LL_close()
{
    USBH_Process(&USB_OTG_Core, &USB_Host);
    return 0;
}

int USBH_LL_send(const uint8_t* data, uint32_t len, uint16_t timeoutMs)
{
    USB_OTG_CORE_HANDLE* pdev = &USB_OTG_Core;

    if (!HCD_IsDeviceConnected(pdev)) {
        return -1;
    }

    USBH_BulkSendData(pdev,
                      (uint8_t*)data,
                      len ,
                      MSC_Machine.hc_num_out);

    URB_STATE state;
    cnt_int = 0; //reset timer

    if (timeoutMs == 0) {
        timeoutMs = 1000;    //Force 1s timeout (testwise)
    }

    while ((state = HCD_GetURB_State(pdev , MSC_Machine.hc_num_out)) == URB_IDLE &&
            (timeoutMs == 0 || cnt_int < timeoutMs));

    if (state != URB_DONE) {
        if (timeoutMs > 0 && cnt_int >= timeoutMs) {
            STM_EVAL_LEDOn(LED3);
            return -7; //timeout (error code like with libusb)

        }

        return -1;
    }

    return len;
}

int USBH_LL_receive(uint8_t* data, uint32_t len, uint16_t timeoutMs)
{

    USB_OTG_CORE_HANDLE* pdev = &USB_OTG_Core;

    if (!HCD_IsDeviceConnected(pdev)) {
        return -1;
    }

    USBH_BulkReceiveData(pdev,
                         data,
                         len ,
                         MSC_Machine.hc_num_in);

    URB_STATE state;
    cnt_int = 0; //reset timer

    if (timeoutMs == 0) {
        timeoutMs = 1000;    //Force 1s timeout (testwise)
    }

    while ((state = HCD_GetURB_State(pdev , MSC_Machine.hc_num_in)) == URB_IDLE &&
            (timeoutMs == 0 || cnt_int < timeoutMs));

    if (state != URB_DONE) {
        if (timeoutMs > 0 && cnt_int >= timeoutMs) {
            STM_EVAL_LEDOn(LED3);
            return -7; //timeout (error code like with libusb)
        }

        return -1;
    }

    return len;
}
