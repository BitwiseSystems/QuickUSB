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
#ifndef __QUICKUSB_DRIVER_CONTEXTS_HEADER__
#define __QUICKUSB_DRIVER_CONTEXTS_HEADER__

#include "ioctl.h"

enum QUICKUSB_DEVICE_TYPE {UNKNOWN, QUSB2, QUSB3};

typedef struct _DEVICE_CONTEXT {
    QUICKUSB_DEVICE_INFO            DeviceInfo;
    WDFUSBDEVICE                    UsbDevice;
    WDFUSBINTERFACE                 UsbInterface;
    WDFUSBPIPE                      BulkReadPipe;
    WDFUSBPIPE                      BulkWritePipe;
    WDFIOTARGET                     IoTargetSelf;
    ULONG                           UsbDeviceTraits;
    ULONG                           BulkReadMaxPacketSize;
    ULONG                           BulkWriteMaxPacketSize;
    UCHAR                           NumberConfiguredPipes;

    // The user setting for a timeout to the nearest millisecond
    LONGLONG Timeout;

    // Queues
    WDFQUEUE QueueDefault;
    WDFQUEUE QueueReadDataLen;  // ReadDataLen requests get their own queue as to not clog up the default queue
    WDFQUEUE QueueGPIF;
    UCHAR    GPIFStopCount;
    UCHAR    DefaultStopCount;
    UCHAR    DeviceControlStopCount;

    // Error information
    USBD_STATUS UsbdStatus;        // Legacy name: USBD_STATUS LastFailedUrbStatus
    USHORT      QusbDriverError;   // Legacy name: USHORT DriverStatus
} DEVICE_CONTEXT, *PDEVICE_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(DEVICE_CONTEXT, GetDeviceContext)



typedef struct _REQUEST_CONTEXT {
    NTSTATUS status;
    size_t bytes;

    WDFREQUEST  parentRequest;
    WDFCOLLECTION requestCollection;
    WDFSPINLOCK collectionLock;
    size_t      splitRequestBytes;
    size_t      splitRequestTotal;
    size_t      splitRequestCount;

    ULONG       Length;     // remaining to xfer
    BOOLEAN     Read;       // TRUE if Read
    BOOLEAN     PipeReset;  // True if the pipe has been aborted and reseted
    WDFUSBPIPE  Pipe;       // The pipe associated with the request
} REQUEST_CONTEXT, *PREQUEST_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(REQUEST_CONTEXT, GetRequestContext)



typedef struct _INTERNAL_VENDOR_REQUEST_CONTEXT {
    WDFMEMORY inputMemory;
    WDFMEMORY outputMemory;
} INTERNAL_VENDOR_REQUEST_CONTEXT, *PINTERNAL_VENDOR_REQUEST_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(INTERNAL_VENDOR_REQUEST_CONTEXT, GetInternalVendorRequestContext)



// Work Item IDs
enum WORKITEMID {
    WorkItemAbortPipe = 1,
    WorkItemResetPipe = 2,
};



typedef struct _WORKITEM_CONTEXT {
    WDFDEVICE        Device;
    WDFUSBPIPE       Pipe;
    enum WORKITEMID  WorkItemId;
} WORKITEM_CONTEXT, *PWORKITEM_CONTEXT;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(WORKITEM_CONTEXT, GetWorkItemContext)



#endif // __QUICKUSB_DRIVER_CONTEXTS_HEADER__
