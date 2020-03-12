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
#ifndef __QUICKUSB_DRIVER_PROTOTYPES_HEADER__
#define __QUICKUSB_DRIVER_PROTOTYPES_HEADER__

// Driver events
DRIVER_INITIALIZE                  DriverEntry;
EVT_WDF_DRIVER_DEVICE_ADD          EvtDeviceAdd;

// Device events
EVT_WDF_DEVICE_CONTEXT_CLEANUP     EvtDriverContextCleanup;
EVT_WDF_DEVICE_PREPARE_HARDWARE    EvtDevicePrepareHardware;
EVT_WDF_DEVICE_D0_ENTRY            EvtDeviceD0Entry;
EVT_WDF_DEVICE_D0_EXIT             EvtDeviceD0Exit;

// Queue events
EVT_WDF_IO_QUEUE_IO_READ           EvtIoRead;
EVT_WDF_IO_QUEUE_IO_WRITE          EvtIoWrite;
EVT_WDF_IO_QUEUE_IO_DEVICE_CONTROL EvtIoDeviceControl;
EVT_WDF_IO_QUEUE_IO_INTERNAL_DEVICE_CONTROL EvtIoInternalDeviceControl;
EVT_WDF_IO_QUEUE_IO_STOP           EvtIoStop;
EVT_WDF_IO_QUEUE_IO_RESUME         EvtIoResume;
EVT_WDF_REQUEST_CANCEL             EvtRequestCancel;

// Completion routines
EVT_WDF_REQUEST_COMPLETION_ROUTINE EvtControlRequestCompletion;
EVT_WDF_REQUEST_COMPLETION_ROUTINE ReadWriteCompletion;
EVT_WDF_REQUEST_COMPLETION_ROUTINE EvtInternalVendorRequestCompletion;

// Work item events
EVT_WDF_WORKITEM AbortPipeWorkItem;
EVT_WDF_WORKITEM ResetPipeWorkItem;

EVT_WDF_REQUEST_COMPLETION_ROUTINE EvtRequestReadWriteCompletion;

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS IssueParentRequest(
    __in WDFREQUEST Request);

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS PerformBulkRequest(
    __in WDFDEVICE                   device,
    __in PDEVICE_CONTEXT             pDeviceContext,
    __in WDFQUEUE                    Queue,
    __in WDFREQUEST                  Request,
    __in size_t                      bytes,
    __in WDF_USB_BMREQUEST_DIRECTION direction,
    __out BOOLEAN                    *requestPending);

NTSTATUS QueuePassiveLevelCallback(
    __in WDFDEVICE       Device,
    __in WDFUSBPIPE      Pipe,
    __in enum WORKITEMID workItemId
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS ResetPipe(
    __in WDFDEVICE   Device,
    __in WDFUSBPIPE  Pipe
    );

VOID StopAllPipes(
    __in PDEVICE_CONTEXT DeviceContext
    );

VOID StartAllPipes(
    __in PDEVICE_CONTEXT DeviceContext
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS ResetDevice(
    __in WDFDEVICE Device
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS AbortPipe(
    __in WDFDEVICE Device,
    __in WDFUSBPIPE  Pipe
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS AbortPipes(
    __in WDFDEVICE Device
    );


USHORT GetDriverErrorFromNtStatus(
    NTSTATUS ntStatus
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
LPCSTR DbgGetVendorStr(
    UCHAR request
    );

NTSTATUS IssueVendorRequest(
    __in BOOLEAN                     internalRequest,
    __in WDFQUEUE                    Queue,
    __in WDFREQUEST                  Request,
    __in WDFMEMORY                   inputMemory,
    __in WDFMEMORY                   outputMemory,
    __in WDFCONTEXT                  context,
    __in PBOOLEAN                    requestPending
    ) ;

NTSTATUS IssueInternalVendorRequest(
    __in WDFDEVICE                   device,
    __in WDFIOTARGET                 ioTarget,
    __in WDFREQUEST                  parentRequest,
    __in BYTE                        direction,
    __in BYTE                        vendorRequest,
    __in USHORT                      value,
    __in USHORT                      index,
    __in WDFMEMORY                   ctlMem
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS SelectInterfaces(
    __in WDFDEVICE Device
    );

NTSTATUS ReenumerateDevice(
    __in PDEVICE_CONTEXT DevContext
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS SetPowerPolicy(
    __in WDFDEVICE Device
    );

__drv_requiresIRQL(PASSIVE_LEVEL)
PCHAR DbgDevicePowerString(
    __in WDF_POWER_DEVICE_STATE Type
    );



#endif // __QUICKUSB_DRIVER_PROTOTYPES_HEADER__
