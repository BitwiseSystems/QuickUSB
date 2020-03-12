/*=============================================================================
 Title        : ioctl.h
 Description  : IOCTL Definitions
    User mode applications wishing to send IOCTLs to a kernel mode driver
    must use this file to set up the correct type of IOCTL code permissions.
 Notes        : 
 History      : 

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

=============================================================================*/
#ifndef __QUICKUSB_DRIVER_IOCTL_HEADER__
#define __QUICKUSB_DRIVER_IOCTL_HEADER__

#if defined(_WIN32)
    //#include <devioctl.h>

    #ifndef _BYTE_DEFINED
    #define _BYTE_DEFINED
    typedef unsigned char BYTE;
    #endif // !_BYTE_DEFINED

    #ifndef _WORD_DEFINED
    #define _WORD_DEFINED
    typedef unsigned short WORD;
    #endif // !_WORD_DEFINED

#elif defined(__linux__)
    //#include <asm/io.h>
#endif

//
// NOTE: IOCTL's defined using METHOD_DIRECT_IN buffering means that the 
// output buffer is directly mapped into system space and probed for read 
// access by the driver.  This means that it is brought into memory if it 
// happens to be paged out to disk.  Even though the buffer is only probed 
// for read access, it is safe (probably) to write to it as well.
// 

//
// Define the structures that will be used by the IOCTL 
// interface to the driver
//

#define MAX_STRING_DESCRIPTOR_OUT_LENGTH    128   // In bytes
#define MAX_STRING_DESCRIPTOR_OUT_WCHARS    64    // In wide characters

typedef struct _QUSB_GET_STRING_DESCRIPTOR { // Linux
   UCHAR Index;
   USHORT LanguageID;
   UCHAR Data[MAX_STRING_DESCRIPTOR_OUT_LENGTH];
   USHORT Length;
} QUSB_GET_STRING_DESCRIPTOR, *PQUSB_GET_STRING_DESCRIPTOR;

typedef struct _QUSB_GET_STRING_DESCRIPTOR_IN
{
   UCHAR    Index;
   USHORT   LanguageId;
} QUSB_GET_STRING_DESCRIPTOR_IN, *PQUSB_GET_STRING_DESCRIPTOR_IN;

typedef struct _QUSB_GET_STRING_DESCRIPTOR_OUT
{
   UCHAR    bLength;
   UCHAR    bDescriptorType;
   WCHAR    bString[MAX_STRING_DESCRIPTOR_OUT_WCHARS];
} QUSB_GET_STRING_DESCRIPTOR_OUT, *PQUSB_GET_STRING_DESCRIPTOR_OUT;

typedef struct _QUSB_DRIVER_VERSION
{
    BOOLEAN  DeviceFound;        // Legacy field
    WORD     MajorVersion;
    WORD     MinorVersion;
    WORD     BuildVersion;
    UCHAR    QusbDriverNum;      // Legacy field
    UCHAR    CyAPIDriverNum;     // Legacy field
} QUSB_DRIVER_VERSION, *PQUSB_DRIVER_VERSION;

typedef struct _GET_PIPE_INFO_IN
{
   UCHAR    PipeIndex;
} GET_PIPE_INFO_IN, *PGET_PIPE_INFO_IN;


// QUERY_DEVICE_INFO structure
typedef struct _QUICKUSB_DEVICE_INFO {
	const DWORD Id[2];

	const WORD VendorID;
	const WORD ProductID;
	const WORD DeviceID;

	const WORD BootloaderVendorID;
	const WORD BootloaderProductID;
	const WORD BootloaderDeviceID;

	const UCHAR ProductClass;
	const UCHAR ProductSubClass;
	const UCHAR ProductArchitecture;

	const UCHAR FirmwareIoModel;
	const UCHAR FirmwareVersionMajor;
	const UCHAR FirmwareVersionMinor;
	const UCHAR FirmwareVersionBuild;

    const UCHAR Reserved[37];
} QUICKUSB_DEVICE_INFO, *PQUICKUSB_DEVICE_INFO;

///////////////////////////////////////////////////////////
//
// control structure for sending vendor or class specific requests
// to the control endpoint.
//
///////////////////////////////////////////////////////////
typedef struct _QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL
{
   // transfer direction (0=host to device, 1=device to host)
   UCHAR direction;

   // request type (1=class, 2=vendor)
   UCHAR requestType;

   // recipient (0=device,1=interface,2=endpoint,3=other)
   UCHAR recepient;

   //
   // see the USB Specification for an explanation of the
   // following paramaters.
   //
   UCHAR requestTypeReservedBits;
   UCHAR request;
   USHORT value;
   USHORT index;

#if defined(__linux__)
   //ULONG length;
   int length;//ints on Linux are always 32-bits despite processor arch
   PVOID data;
#endif
} QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL, *PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL;


typedef struct _QUSB_BULK_TRANSFER_CONTROL
{
   ULONG pipeNum;
#if defined(__linux__)
   PVOID *data;
   ULONG length;
#endif
} QUSB_BULK_TRANSFER_CONTROL, *PQUSB_BULK_TRANSFER_CONTROL;


#ifndef USBD_STATUS
#define USBD_STATUS ULONG
#endif

typedef struct _QUSB_GET_DRIVER_ERROR {
#if defined(_WIN32)
   USBD_STATUS UsbdStatus;         // Legacy name: ULONG LastFailedUrbStatus (USBD_STATUS is a ULONG)
   USHORT      QusbDriverError;    // Legacy name: USHORT DriverStatus
   ULONG       DriverState;        // Unused
#else
   ULONG  UsbError;
   USHORT DriverError;
   ULONG  DriverState;
#endif
} QUSB_GET_DRIVER_ERROR, *PQUSB_GET_DRIVER_ERROR;



//
// Set the base of the IOCTL control codes.  This is somewhat of an
// arbitrary base number, so you can change this if you want unique
// IOCTL codes.  You should consult the Windows NT DDK for valid ranges
// of IOCTL index codes before you choose a base index number.
//
#if defined(_WIN32)
    #define QUSB_IOCTL_INDEX  0x0800
    #define QUSB_IOCTL(index, buffering) CTL_CODE(FILE_DEVICE_UNKNOWN, QUSB_IOCTL_INDEX + index, buffering, FILE_ANY_ACCESS)
#elif defined(__linux__)
    #define QUSB_IOCTL_INDEX  0xBB
    #define QUSB_IOCTL(index, buffering) _IOR(QUSB_IOCTL_INDEX, index, int)
#endif

#define IOCTL_QUSB_GET_PIPE_INFO                QUSB_IOCTL(0, METHOD_BUFFERED)

#define IOCTL_QUSB_GET_DEVICE_DESCRIPTOR        QUSB_IOCTL(1, METHOD_BUFFERED)

#define IOCTL_QUSB_GET_CONFIGURATION_DESCRIPTOR QUSB_IOCTL(2, METHOD_BUFFERED)
/*
// No interrupt implementation is currently defined
#define IOCTL_QUSB_BULK_OR_INTERRUPT_WRITE      QUSB_IOCTL(3, METHOD_BUFFERED)

// No interrupt implementation is currently defined
#define IOCTL_QUSB_BULK_OR_INTERRUPT_READ       QUSB_IOCTL(4, METHOD_BUFFERED)

// Unused.  Use IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST instead.
#define IOCTL_QUSB_VENDOR_REQUEST               QUSB_IOCTL(5, METHOD_BUFFERED)

// Un-implemented in WDM driver.
#define IOCTL_QUSB_GET_CURRENT_CONFIG           QUSB_IOCTL(6, METHOD_BUFFERED)
*/

// BUGFIX: This control code was incorrectly defined twice in the last
// WDM driver.  This is the first definition so therefore the second
// definition takes precedence.  Do not use this control code and
// leave it commented out.
//#define IOCTL_QUSB_ANCHOR_DOWNLOAD            QUSB_IOCTL(7, METHOD_BUFFERED)

#define IOCTL_QUSB_CYCLE                        QUSB_IOCTL(11, METHOD_IN_DIRECT)

#define IOCTL_QUSB_RESET                        QUSB_IOCTL(12, METHOD_IN_DIRECT)

#define IOCTL_QUSB_RESETPIPE                    QUSB_IOCTL(13, METHOD_IN_DIRECT)

#define IOCTL_QUSB_ABORTPIPE                    QUSB_IOCTL(15, METHOD_IN_DIRECT)
/*
// WDM-specific implementation.
#define IOCTL_QUSB_SETINTERFACE                 QUSB_IOCTL(16, METHOD_BUFFERED)
*/
#define IOCTL_QUSB_GET_STRING_DESCRIPTOR        QUSB_IOCTL(17, METHOD_BUFFERED)


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
#define IOCTL_QUSB_BULK_READ                    QUSB_IOCTL(19, METHOD_OUT_DIRECT)

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
#define IOCTL_QUSB_BULK_WRITE                   QUSB_IOCTL(20, METHOD_IN_DIRECT)

//
// Retrieve the current USB frame number from the Host Controller
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PULONG to hold current frame number
// nOutputBufferSize: sizeof(PULONG)
// 
// No isochronous implementation is currently defined.
/*
#define IOCTL_QUSB_GET_CURRENT_FRAME_NUMBER     QUSB_IOCTL(21, METHOD_BUFFERED)

*/

//
// Performs a vendor or class specific control transfer to EP0.  The contents of
// the input parameter determine the type of request.  See the USB spec
// for more information on class and vendor control transfers.
//
// lpInBuffer: PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL
// nInBufferSize: sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL)
// lpOutBuffer: pointer to a buffer if the request involves a data transfer
// nOutputBufferSize: size of the transfer buffer (corresponds to the wLength
//    field of the USB setup packet)
// 
#define IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST      QUSB_IOCTL(22, METHOD_IN_DIRECT)

//
// Retrieves the actual USBD_STATUS and Driver codes for the most recently
// failed URB.
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PULONG to hold the URB status
// nOutputBufferSize: sizeof(ULONG)
// 

#define IOCTL_QUSB_GET_LAST_ERROR               QUSB_IOCTL(23, METHOD_BUFFERED)
/*
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

// No isochronous implementation is currently defined.
#define IOCTL_QUSB_ISO_READ                     QUSB_IOCTL(25, METHOD_OUT_DIRECT)

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

// No isochronous implementation is currently defined.
#define IOCTL_QUSB_ISO_WRITE                    QUSB_IOCTL(26, METHOD_IN_DIRECT)

//
// Performs and Anchor Download.
//
// lpInBuffer: PANCHOR_DOWNLOAD_CONTROL
// nInBufferSize: sizeof(ANCHOR_DOWNLOAD_CONTROL)
// lpOutBuffer: pointer to a buffer of data to download to the device
// nOutputBufferSize: size of the transfer buffer
// 
// Unused.  Use IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST with request 0xA0 instead.
#define IOCTL_QUSB_ANCHOR_DOWNLOAD              QUSB_IOCTL(27, METHOD_IN_DIRECT)
*/
//
// Returns driver version information
//
// lpInBuffer: NULL
// nInBufferSize: 0
// lpOutBuffer: PQUSB_DRIVER_VERSION
// nOutputBufferSize: sizeof(QUSB_DRIVER_VERSION)
// 
#define IOCTL_QUSB_GET_DRIVER_VERSION           QUSB_IOCTL(29, METHOD_BUFFERED)
/*
// No isochronous implementation is currently defined.
#define IOCTL_QUSB_START_ISO_STREAM             QUSB_IOCTL(30, METHOD_BUFFERED)

// No isochronous implementation is currently defined.
#define IOCTL_QUSB_STOP_ISO_STREAM              QUSB_IOCTL(31, METHOD_BUFFERED)

// No isochronous implementation is currently defined.
#define IOCTL_QUSB_READ_ISO_BUFFER              QUSB_IOCTL(32, METHOD_OUT_DIRECT)

// WDM-driver specific implementation.
#define IOCTL_QUSB_SET_FEATURE                  QUSB_IOCTL(33, METHOD_BUFFERED)
*/
#define IOCTL_QUSB_SET_TIMEOUT                  QUSB_IOCTL(34, METHOD_BUFFERED)

// Deprecated WDM function.
#define IOCTL_QUSB_ASYNC_BULK_READ              QUSB_IOCTL(35, METHOD_OUT_DIRECT)
    
// Deprecated WDM function.
#define IOCTL_QUSB_ASYNC_BULK_WRITE             QUSB_IOCTL(36, METHOD_IN_DIRECT)

// Deprecated WDM function.
#define IOCTL_QUSB_ASYNC_WAIT                   QUSB_IOCTL(37, METHOD_BUFFERED)

// New to KMDF driver v2.15.0
#define IOCTL_QUSB_GET_INTERFACE_DESCRIPTOR     QUSB_IOCTL(38, METHOD_BUFFERED)

// New to KMDF driver v2.15.0
#define IOCTL_QUSB_GET_TIMEOUT                  QUSB_IOCTL(39, METHOD_BUFFERED)

#endif // __QUICKUSB_DRIVER_IOCTL_HEADER__
