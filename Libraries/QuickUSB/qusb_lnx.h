/*=============================================================================

Title        : qusb_lnx.h
Description  : QuickUSB Linux Driver Header
Notes        : None
History      :

Copyright  2010 Bitwise Systems.  All rights reserved.
This software contains confidential information and trade secrets of
Bitwise Systems and is protected by United States and international
copyright laws.  Use, disclosure, or reproduction is prohibited without
the prior express written permission of Bitwise Systems, except as agreed
in the QuickUSB Plug-In Module license agreement.

Use, duplication or disclosure by the U.S. Government is subject to
restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, 
Suite E, Goleta, CA  93117.

Bitwise Systems
6489 Calle Real, Suite E
Goleta, CA  93117
Voice: (805) 683-6469
Fax  : (805) 683-4833
Web  : www.bitwisesys.com
email: support@bitwisesys.com

$History: qusb_lnx.h $

*****************  Version 01  *****************
User: Anh Nguyen  Date: 08/09/10   Time: 12:00p
Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbLinux/Driver

=============================================================================*/
#include "QuickUsbVersion.h"


#define MAX_MMAP_SIZE (256 * 1024)


//
// Driver error status, returned from IOCTL_EZUSB_GET_LAST_ERROR.  This
// replaces NTSTATUS codes which are undefined in the DLL.  These values
// are stored in the DEVICE EXTENSION DriverStatus member.
//
enum
{
   DriverErrorNone               = 0x0000,
   DriverErrorInvalidParameter   = 0x0001,
   DriverErrorMemory             = 0x0002,
   DriverErrorURB                = 0x0003,
   DriverErrorTimeout            = 0x0004,
   DriverErrorBulk               = 0x0005,
   DriverErrorAsync              = 0x0006,
   DriverErrorPipe               = 0x0007,
   DriverErrorBusy               = 0x0008
};


// Structs
#define MAX_STRING_LENGTH 128
typedef struct QUSB_GET_STRING_DESCRIPTOR {
   UCHAR Index;
   USHORT LanguageID;
   UCHAR Data[MAX_STRING_LENGTH];
   USHORT Length;
} STRING_DESCRIPTOR;

#define MAX_DATA_LENGTH 64
typedef struct QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL {
   // transfer direction (0=host to device, 1=device to host)
   UCHAR Direction;

   // request type (1=class, 2=vendor)
   UCHAR RequestType;

   // recipient (0=device,1=interface,2=endpoint,3=other)
   UCHAR Recepient;
   //
   // see the USB Specification for an explanation of the
   // following paramaters.
   //
   UCHAR RequestTypeReservedBits;
   UCHAR Request;
   USHORT Value;
   USHORT Index;
   ULONG Length;
   PVOID Data;
} VENDOR_OR_CLASS_REQUEST_CONTROL;

/*
struct QUSB_DRIVER_VERSION {
   BOOLEAN DeviceFound;
   WORD MajorVersion;
   WORD MinorVersion;
   WORD BuildVersion;
   UCHAR QusbDriverNum;
};
*/

typedef struct QUSB_GET_DRIVER_ERROR {
   ULONG  UsbError;
   USHORT DriverError;
   ULONG  DriverState;
} GET_DRIVER_ERROR, * PGET_DRIVER_ERROR;

///////////////////////////////////////////////////////////
//
// control structures for bulk and interrupt data transfers
// The async context is passed to the async ioCompletion
// routine.
//
///////////////////////////////////////////////////////////
typedef struct QUSB_BULK_TRANSFER_CONTROL
{
   ULONG PipeNum;
   PVOID *Data;
   ULONG Length;
} BULK_TRANSFER_CONTROL, *PBULK_TRANSFER_CONTROL;

typedef struct QUSB_ASYNC {
   unsigned char transaction;
   unsigned char immediate;
} QUSB_ASYNC, *PQUSB_ASYNC;


///////////////////////////////////////////////////////
//
//              IOCTL Definitions
//
// User mode applications wishing to send IOCTLs to a kernel mode driver
// must use this file to set up the correct type of IOCTL code permissions.
//
///////////////////////////////////////////////////////

//
// Set the base of the IOCTL control codes.  This is somewhat of an
// arbitrary base number, so you can change this if you want unique
// IOCTL codes.  You should consult "Documentation/ioctl-number.txt"
// for valid ranges of IOCTL index codes before you choose a base
// index number.
//

#define Ezusb_IOCTL_INDEX  0xBB


#define IOCTL_Ezusb_GET_PIPE_INFO                _IOR( Ezusb_IOCTL_INDEX,  0, int )

#define IOCTL_Ezusb_GET_DEVICE_DESCRIPTOR        _IOR( Ezusb_IOCTL_INDEX,  1, int )

#define IOCTL_Ezusb_GET_CONFIGURATION_DESCRIPTOR _IOR( Ezusb_IOCTL_INDEX,  2, int )

#define IOCTL_Ezusb_BULK_OR_INTERRUPT_WRITE      _IOR( Ezusb_IOCTL_INDEX,  3, int )

#define IOCTL_Ezusb_BULK_OR_INTERRUPT_READ       _IOR( Ezusb_IOCTL_INDEX,  4, int )

#define IOCTL_Ezusb_VENDOR_REQUEST               _IOR( Ezusb_IOCTL_INDEX,  5, int )

#define IOCTL_Ezusb_GET_CURRENT_CONFIG           _IOR( Ezusb_IOCTL_INDEX,  6, int )

#define IOCTL_Ezusb_ANCHOR_DOWNLOAD              _IOR( Ezusb_IOCTL_INDEX,  7, int )

#define IOCTL_Ezusb_CYCLE                        _IOR( Ezusb_IOCTL_INDEX,  11, int )

#define IOCTL_Ezusb_RESET                        _IOR( Ezusb_IOCTL_INDEX,  12, int )

#define IOCTL_Ezusb_RESETPIPE                    _IOR( Ezusb_IOCTL_INDEX,  13, int )

#define IOCTL_Ezusb_ABORTPIPE                    _IOR( Ezusb_IOCTL_INDEX,  15, int )

#define IOCTL_Ezusb_SETINTERFACE                 _IOR( Ezusb_IOCTL_INDEX,  16, int )

#define IOCTL_Ezusb_GET_STRING_DESCRIPTOR        _IOR( Ezusb_IOCTL_INDEX,  17, int )


//
// Perform an IN transfer over the specified bulk or interrupt pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to read from
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer to hold data read from the device.  
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes read
// 
#define IOCTL_EZUSB_BULK_READ                    _IOR( Ezusb_IOCTL_INDEX,  19, int )

//
// Perform an OUT transfer over the specified bulk or interrupt pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to write to
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer of data to write to the device
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes written
// 
#define IOCTL_EZUSB_BULK_WRITE                   _IOR( Ezusb_IOCTL_INDEX,  20, int )

//
// The following IOCTL's are defined as using METHOD_DIRECT_IN buffering.
// This means that the output buffer is directly mapped into system
// space and probed for read access by the driver.  This means that it is
// brought into memory if it happens to be paged out to disk.  Even though
// the buffer is only probed for read access, it is safe (probably) to
// write to it as well.  This read/write capability is used for the loopback
// IOCTL's
// 

// TODO Insert Loopback IOCTL's

//
// Retrieve the current USB frame number from the Host Controller
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PULONG to hold current frame number
// nOutputBufferSize: sizeof(PULONG)
// 
#define IOCTL_EZUSB_GET_CURRENT_FRAME_NUMBER     _IOR( Ezusb_IOCTL_INDEX,  21, int )


//
// Performs a vendor or class specific control transfer to EP0.  The contents of
// the input parameter determine the type of request.  See the USB spec
// for more information on class and vendor control transfers.
//
// lpInBuffer: PVENDOR_OR_CLASS_REQUEST_CONTROL
// nInBufferSize: sizeof(VENDOR_OR_CLASS_REQUEST_CONTROL)
// lpOutBuffer: pointer to a buffer if the request involves a data transfer
// nOutputBufferSize: size of the transfer buffer (corresponds to the wLength
//    field of the USB setup packet)
// 
#define IOCTL_EZUSB_VENDOR_OR_CLASS_REQUEST      _IOR( Ezusb_IOCTL_INDEX,  22, int )

//
// Retrieves the actual USBD_STATUS and Driver codes for the most recently
// failed URB.
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PULONG to hold the URB status
// nOutputBufferSize: sizeof(ULONG)
// 

#define IOCTL_EZUSB_GET_LAST_ERROR               _IOR( Ezusb_IOCTL_INDEX,  23, int )

//
// Reads from the specified ISO endpoint. (USB IN Transfer)
//
// lpInBuffer: ISO_TRANSFER_CONTROL
// nInBufferSize: sizeof(ISO_TRANSFER_CONTROL)
// lpOutBuffer: buffer to hold data read from the device
// nOutputBufferSize: size of the read buffer.
//
// 
// 

#define IOCTL_EZUSB_ISO_READ                     _IOR( Ezusb_IOCTL_INDEX,  25, int )

//
// Writes to the specified ISO endpoint. (USB OUT Transfer)
//
// lpInBuffer: ISO_TRANSFER_CONTROL
// nInBufferSize: sizeof(ISO_TRANSFER_CONTROL)
// lpOutBuffer: buffer to hold data to write to the device
// nOutputBufferSize: size of the write buffer.
//
// 
// 

#define IOCTL_EZUSB_ISO_WRITE                    _IOR( Ezusb_IOCTL_INDEX,  26, int )

//
// Performs and Anchor Download.
//
// lpInBuffer: PANCHOR_DOWNLOAD_CONTROL
// nInBufferSize: sizeof(ANCHOR_DOWNLOAD_CONTROL)
// lpOutBuffer: pointer to a buffer of data to download to the device
// nOutputBufferSize: size of the transfer buffer
// 
#define IOCTL_EZUSB_ANCHOR_DOWNLOAD              _IOR( Ezusb_IOCTL_INDEX,  27, int )

//
// Returns driver version information
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PEZUSB_DRIVER_VERSION
// nOutputBufferSize: sizeof(EZUSB_DRIVER_VERSION)
// 
#define IOCTL_EZUSB_GET_DRIVER_VERSION           _IOR( Ezusb_IOCTL_INDEX,  29, int )

#define IOCTL_EZUSB_START_ISO_STREAM             _IOR( Ezusb_IOCTL_INDEX,  30, int )

#define IOCTL_EZUSB_STOP_ISO_STREAM              _IOR( Ezusb_IOCTL_INDEX,  31, int )

#define IOCTL_EZUSB_READ_ISO_BUFFER              _IOR( Ezusb_IOCTL_INDEX,  32, int )

#define IOCTL_EZUSB_SET_FEATURE                  _IOR( Ezusb_IOCTL_INDEX,  33, int )

#define IOCTL_EZUSB_SET_TIMEOUT                  _IOR( Ezusb_IOCTL_INDEX,  34, int )
//
// Perform an Asynchronous IN transfer over the specified bulk pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to read from
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer to hold data read from the device.  
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes read
// 
#define IOCTL_EZUSB_ASYNC_BULK_READ              _IOR( Ezusb_IOCTL_INDEX,  35, int )
//
// Perform an Asynchronous OUT transfer over the specified bulk pipe.
//
// lpInBuffer: BULK_TRANSFER_CONTROL stucture specifying the pipe number to write to
// nInBufferSize: sizeof(BULK_TRANSFER_CONTROL)
// lpOutBuffer: Buffer of data to write to the device
// nOutputBufferSize: size of lpOutBuffer.  This parameter determines
//    the size of the USB transfer.
// lpBytesReturned: actual number of bytes written
// 
#define IOCTL_EZUSB_ASYNC_BULK_WRITE             _IOR( Ezusb_IOCTL_INDEX,  36, int )
//
// This IOCTL is sent to test the end of an asynchronous transfer.  This
// is an event set by the asynchronous ioCompletion routine and carries
// the bulk timeout setting.
#define IOCTL_EZUSB_ASYNC_WAIT                   _IOR( Ezusb_IOCTL_INDEX,  37, int )

