#ifndef _EZUSB_DEFINED
#define _EZUSB_DEFINED

#if DRIVER==WindowsDriver

#ifdef __cplusplus
extern "C" {
#endif

typedef struct _THREAD_CONTROL {
   HANDLE hDevice;
   DWORD Ioctl;
   PVOID InBuffer;
   DWORD InBufferSize;
   PVOID OutBuffer;
   DWORD OutBufferSize;
   DWORD BytesReturned;
   HANDLE completionEvent;
   BOOL  status;
} THREAD_CONTROL, *PTHREAD_CONTROL;

#define GET_CONFIG_DESCRIPTOR_LENGTH(pv)    ((pUsb_Configuration_Descriptor)pv)->wTotalLength

typedef struct __usb_Dev_Descriptor__ {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT bcdUSB;
    UCHAR bDeviceClass;
    UCHAR bDeviceSubClass;
    UCHAR bDeviceProtocol;
    UCHAR bMaxPacketSize0;
    USHORT idVendor;
    USHORT idProduct;
    USHORT bcdDevice;
    UCHAR iManufacturer;
    UCHAR iProduct;
    UCHAR iSerialNumber;
    UCHAR bNumConfigurations;
} __usb_Dev_Descriptor__, Usb_Device_Descriptor, *pUsb_Device_Descriptor;

typedef struct __usb_Config_Descriptor__ {
    UCHAR bLength;
    UCHAR bDescriptorType;
    USHORT wTotalLength;
    UCHAR bNumInterfaces;
    UCHAR bConfigurationValue;
    UCHAR iConfiguration;
    UCHAR bmAttributes;
    UCHAR MaxPower;
} Usb_Configuration_Descriptor, *pUsb_Configuration_Descriptor;

/*
typedef struct _USB_INTERFACE_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bInterfaceNumber;
    UCHAR bAlternateSetting;
    UCHAR bNumEndpoints;
    UCHAR bInterfaceClass;
    UCHAR bInterfaceSubClass;
    UCHAR bInterfaceProtocol;
    UCHAR iInterface;
} Usb_Interface_Descriptor, *pUsb_Interface_Descriptor;

typedef struct _USB_ENDPOINT_DESCRIPTOR {
    UCHAR bLength;
    UCHAR bDescriptorType;
    UCHAR bEndpointAddress;
    UCHAR bmAttributes;
    USHORT wMaxPacketSize;
    UCHAR bInterval;
} Usb_Endpoint_Descriptor, *pUsb_Endpoint_Descriptor;
*/

BOOLEAN bOpenDriver (HANDLE * phDeviceHandle, PCHAR devname);

//
// Stuff copied fro USBDI.H
//
typedef PVOID USBD_PIPE_HANDLE;
typedef PVOID USBD_CONFIGURATION_HANDLE;
typedef PVOID USBD_INTERFACE_HANDLE;

typedef enum _USBD_PIPE_TYPE {
    UsbdPipeTypeControl,
    UsbdPipeTypeIsochronous,
    UsbdPipeTypeBulk,
    UsbdPipeTypeInterrupt
} USBD_PIPE_TYPE;

typedef struct _USBD_PIPE_INFORMATION {
    //
    // OUTPUT
    // These fields are filled in by USBD
    //
    USHORT MaximumPacketSize;  // Maximum packet size for this pipe
    UCHAR EndpointAddress;     // 8 bit USB endpoint address (includes direction)
                               // taken from endpoint descriptor
    UCHAR Interval;            // Polling interval in ms if interrupt pipe 
    
    USBD_PIPE_TYPE PipeType;   // PipeType identifies type of transfer valid for this pipe
    USBD_PIPE_HANDLE PipeHandle;
    
    //
    // INPUT
    // These fields are filled in by the client driver
    //
    ULONG MaximumTransferSize; // Maximum size for a single request
                               // in bytes.
    ULONG PipeFlags;                                   
} USBD_PIPE_INFORMATION, *PUSBD_PIPE_INFORMATION;


typedef struct _USBD_INTERFACE_INFORMATION {
    USHORT Length;       // Length of this structure, including
                         // all pipe information structures that
                         // follow.
    //
    // INPUT
    //
    // Interface number and Alternate setting this
    // structure is associated with
    //
    UCHAR InterfaceNumber;
    UCHAR AlternateSetting;
    
    //
    // OUTPUT
    // These fields are filled in by USBD
    //
    UCHAR Class;
    UCHAR SubClass;
    UCHAR Protocol;
    UCHAR Reserved;
    
    USBD_INTERFACE_HANDLE InterfaceHandle;
    ULONG NumberOfPipes; 

    //
    // INPUT/OUPUT
    // see PIPE_INFORMATION
    USBD_PIPE_INFORMATION Pipes[1];

} USBD_INTERFACE_INFORMATION, *PUSBD_INTERFACE_INFORMATION;

typedef struct _USBD_ISO_PACKET_DESCRIPTOR {
    ULONG Offset;       // INPUT Offset of the packet from the begining of the
                        // buffer.
    ULONG Length;       // OUTPUT length of data received (for in).
                        // OUTPUT 0 for OUT.
    LONG Status; // status code for this packet.     
} USBD_ISO_PACKET_DESCRIPTOR, *PUSBD_ISO_PACKET_DESCRIPTOR;


#ifdef __cplusplus
}
#endif

#endif // #if DRIVER==WindowsDriver

#endif // #ifndef _EZUSB_DEFINED
