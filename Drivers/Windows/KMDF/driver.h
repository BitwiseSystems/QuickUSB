/*=============================================================================
 Title        : 
 Description  : 
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
#ifndef __QUICKUSB_DRIVER_HEADER__
#define __QUICKUSB_DRIVER_HEADER__

#pragma warning(disable:4200)  // nameless struct/union
#pragma warning(disable:4201)  // nameless struct/union
#pragma warning(disable:4214)  // bit field types other than int
#include <initguid.h>
#include <ntddk.h>
#include <usbdi.h>
#include <usbdlib.h>
#include <driverspecs.h>

#pragma warning(default:4200)
#pragma warning(default:4201)
#pragma warning(default:4214)

#include <wdf.h>
#include <wdfusb.h>
#define NTSTRSAFE_LIB
#include <ntstrsafe.h>
#include <Ntstatus.h>

#include "version.h"
#include "contexts.h"
#include "prototypes.h"
#include "ioctl.h"

#define QUSB_MAX_DEVICES 127

// Each bulk R/W transaction is limited to a maximum of 16 MB/request.  Each
// request is then broken into 256 KB split requests and performed 
// asynchronously.  The 16 MB limit is imposed by QuickUSB and may be changed
// here, and the 256 KB limit is imposed by Windows and should not be changed.
#define QUSB_MAX_BULK_TRANSFER_SIZE         (16 * 1024 * 1024)
#define QUSB_MAX_BULK_REQUEST_TRANSFER_SIZE (256 * 1024)
#define QUSB_MAX_BULK_SPLIT_REQUESTS        (QUSB_MAX_BULK_TRANSFER_SIZE / QUSB_MAX_BULK_REQUEST_TRANSFER_SIZE)

// Completion macros to ensure that the QuickUSB Driver Error is properly
// translated and reported
#define QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, Status, Bytes) \
    pDeviceContext->QusbDriverError = GetDriverErrorFromNtStatus(Status); \
    WdfRequestCompleteWithInformation(Request, Status, Bytes);

#define POOL_TAG (ULONG) 'FRSO'

#define QUSB_DEFAULT_TIMEOUT WDF_REL_TIMEOUT_IN_SEC(1)

// Debug messages
#define DEBUG_INFO   0
#define DEBUG_WARN   1
#define DEBUG_ERROR  2
#define DEBUG_ALWAYS 1000
#define DEBUG_LEVEL DEBUG_INFO
#ifdef DBG
#define QusbDbgPrint(level, x) if (level >= DEBUG_LEVEL) { KdPrint(("QuickUSB: ")); KdPrint(x); }
#else
#define QusbDbgPrint(level, x)
#endif


//
// Define the vendor commands supported by our device
//
// QuickUSB vendor specific requests
#define FIRMWARE_LOAD          0xA0
#define QUICKUSB_EEPROM        0xA9

#define QUICKUSB_SETTING       0xB0
#define QUICKUSB_FPGA          0xB1
#define QUICKUSB_COMMAND       0xB2
#define QUICKUSB_PORT          0xB3
#define QUICKUSB_RS232         0xB4
#define QUICKUSB_I2C           0xB5
#define QUICKUSB_SPI           0xB6
#define QUICKUSB_READDATALEN   0xB7
#define QUICKUSB_TIMER         0xB8
#define QUICKUSB_JTAG          0xB9
#define QUICKUSB_QUERY         0xBE
#define QUICKUSB_SCRIPT        0xBF
#define QUICKUSB_RESET         0xC0
#define QUICKUSB_TIMEOUT       0xC1



////
// Driver error status, returned from IOCTL_QUSB_GET_LAST_ERROR.  This
// replaces NTSTATUS codes which are undefined in the DLL.  These values
// are stored in the DEVICE CONTEXT DriverStatus member.
//
enum
{
    DriverErrorNone               = 0x0000,
    DriverErrorInvalidParameter   = 0x0001,
    DriverErrorMemory             = 0x0002,
    DriverErrorURB                = 0x0003,    // Deprecated
    DriverErrorTimeout            = 0x0004,
    DriverErrorBulk               = 0x0005,    // Deprecated
    DriverErrorAsync              = 0x0006,    // Deprecated
    DriverErrorPipe               = 0x0007,    // Deprecated
    DriverErrorBusy               = 0x0008,    // Deprecated
    DriverErrorFailure            = 0x0009,

    // NOTE: All new error codes should be defined such that the driver error
    // code matches the library error code
    DriverErrorDeprecated         = 20,
};



#endif //__QUICKUSB_DRIVER_HEADER__
