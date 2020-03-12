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
#include "driver.h"



// Report the error so that QuickUsbGetLastError() will return the correct 
// value
USHORT GetDriverErrorFromNtStatus(NTSTATUS ntStatus) 
{
    switch (ntStatus) {
    case STATUS_SUCCESS:
        return DriverErrorNone;

    case STATUS_INVALID_PARAMETER:
        return DriverErrorInvalidParameter;

    case STATUS_NO_MEMORY:
    case STATUS_BUFFER_TOO_SMALL:
        return DriverErrorMemory;

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorURB;        // Deprecated

    case STATUS_IO_TIMEOUT:
        return DriverErrorTimeout;

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorBulk;       // Deprecated

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorAsync;      // Deprecated

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorPipe;       // Deprecated

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorAsync;      // Deprecated

    //case :
    //    pDeviceContext->QusbDriverError = DriverErrorBusy;       // Deprecated

    case STATUS_NOT_IMPLEMENTED:
        return DriverErrorDeprecated;

    default:
        return DriverErrorFailure;
    }
}



__drv_requiresIRQL(PASSIVE_LEVEL)
LPCSTR DbgGetVendorStr(UCHAR request)
{
    switch (request) {
        case FIRMWARE_LOAD:
            return "FIRMWARE_LOAD";
        case QUICKUSB_EEPROM:
            return "QUICKUSB_EEPROM";
        case QUICKUSB_SETTING:
            return "QUICKUSB_SETTING";
        case QUICKUSB_FPGA:
            return "QUICKUSB_FPGA";
        case QUICKUSB_PORT:
            return "QUICKUSB_PORT";
        case QUICKUSB_RS232:
            return "QUICKUSB_RS232";
        case QUICKUSB_I2C:
            return "QUICKUSB_I2C";
        case QUICKUSB_SPI:
            return "QUICKUSB_SPI";
        case QUICKUSB_READDATALEN:
            return "QUICKUSB_READDATALEN";
        case QUICKUSB_TIMER:
            return "QUICKUSB_TIMER";
        case QUICKUSB_JTAG:
            return "QUICKUSB_JTAG";
        case QUICKUSB_SCRIPT:
            return "QUICKUSB_SCRIPT";
        case QUICKUSB_QUERY:
            return "QUICKUSB_QUERY";
        case QUICKUSB_RESET:
            return "QUICKUSB_RESET";
        case QUICKUSB_TIMEOUT:
            return "QUICKUSB_TIMEOUT";
        case QUICKUSB_COMMAND:
            return "QUICKUSB_COMMAND";
        default:
            return "UNKNOWN";
    }
}



__drv_requiresIRQL(PASSIVE_LEVEL)
PCHAR
DbgDevicePowerString(
    __in WDF_POWER_DEVICE_STATE Type
    )
/*++

Updated Routine Description:
    DbgDevicePowerString does not change in this stage of the function driver.

--*/
{
    switch (Type)
    {
    case WdfPowerDeviceInvalid:
        return "WdfPowerDeviceInvalid";
    case WdfPowerDeviceD0:
        return "WdfPowerDeviceD0";
    case PowerDeviceD1:
        return "WdfPowerDeviceD1";
    case WdfPowerDeviceD2:
        return "WdfPowerDeviceD2";
    case WdfPowerDeviceD3:
        return "WdfPowerDeviceD3";
    case WdfPowerDeviceD3Final:
        return "WdfPowerDeviceD3Final";
    case WdfPowerDevicePrepareForHibernation:
        return "WdfPowerDevicePrepareForHibernation";
    case WdfPowerDeviceMaximum:
        return "PowerDeviceMaximum";
    default:
        return "UnKnown Device Power State";
    }
}



VOID
AbortPipeWorkItem(
    __in WDFWORKITEM  WorkItem
    )
{
    PWORKITEM_CONTEXT pItemContext;
    NTSTATUS          status;
    PDEVICE_CONTEXT   pDeviceContext;

    pItemContext = GetWorkItemContext(WorkItem);
    pDeviceContext = GetDeviceContext(pItemContext->Device);

    status = AbortPipe(pItemContext->Device, pItemContext->Pipe);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> AbortDevice failed 0x%x\n", status));

        status = ResetDevice(pItemContext->Device);
        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> AbortDevice failed 0x%x\n", status));
        }
    }

    // Resume dispatching on the queue now that the work item has completed
    //if (pItemContext->Pipe == pDeviceContext->BulkReadPipe) {
        if (--pDeviceContext->GPIFStopCount == 0) {
            WdfIoQueueStart(pDeviceContext->QueueGPIF);
        }
    //}
    //else if (pItemContext->Pipe == pDeviceContext->BulkWritePipe) {
    //    if (--pDeviceContext->WriteStopCount == 0) {
    //        //WdfIoQueueStart(pDeviceContext->QueueWrite);
    //    }
    //}
    if (--pDeviceContext->DefaultStopCount == 0) {
        WdfIoQueueStart(pDeviceContext->QueueDefault);
    }

    WdfObjectDelete(WorkItem);

    return;
}



VOID
ResetPipeWorkItem(
    __in WDFWORKITEM  WorkItem
    )
{
    PWORKITEM_CONTEXT pItemContext;
    NTSTATUS          status;
    PDEVICE_CONTEXT   pDeviceContext;

    pItemContext = GetWorkItemContext(WorkItem);
    pDeviceContext = GetDeviceContext(pItemContext->Device);

    status = ResetPipe(pItemContext->Device, pItemContext->Pipe);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> ResetPipe failed 0x%x\n", status));

        status = ResetDevice(pItemContext->Device);
        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> ResetDevice failed 0x%x\n", status));
        }
    }

    // Resume dispatching on the queue now that the work item has completed
    //if (pItemContext->Pipe == pDeviceContext->BulkReadPipe) {
        if (--pDeviceContext->GPIFStopCount == 0) {
            WdfIoQueueStart(pDeviceContext->QueueGPIF);
        }
    //}
    //else if (pItemContext->Pipe == pDeviceContext->BulkWritePipe) {
    //    if (--pDeviceContext->WriteStopCount == 0) {
    //        //WdfIoQueueStart(pDeviceContext->QueueWrite);
    //    }
    //}
    if (--pDeviceContext->DefaultStopCount == 0) {
        WdfIoQueueStart(pDeviceContext->QueueDefault);
    }

    WdfObjectDelete(WorkItem);

    return;
}



NTSTATUS
QueuePassiveLevelCallback(
    __in WDFDEVICE       Device,
    __in WDFUSBPIPE      Pipe,
    __in enum WORKITEMID workItemId
    )
/*++

Routine Description:

    This routine is used to queue workitems so that the callback
    functions can be executed at PASSIVE_LEVEL in the conext of
    a system thread.

Arguments:


Return Value:

--*/
{
    NTSTATUS                 status = STATUS_SUCCESS;
    PWORKITEM_CONTEXT        context;
    WDF_OBJECT_ATTRIBUTES    attributes;
    WDF_WORKITEM_CONFIG      workitemConfig;
    WDFWORKITEM              hWorkItem;
    PDEVICE_CONTEXT          pDeviceContext;

    pDeviceContext = GetDeviceContext(Device);

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    WDF_OBJECT_ATTRIBUTES_SET_CONTEXT_TYPE(&attributes, WORKITEM_CONTEXT);
    attributes.ParentObject = Device;

    switch (workItemId) {
        case WorkItemAbortPipe:
            WDF_WORKITEM_CONFIG_INIT(&workitemConfig, AbortPipeWorkItem);
            break;

        case WorkItemResetPipe:
            WDF_WORKITEM_CONFIG_INIT(&workitemConfig, ResetPipeWorkItem);
            break;

        default:
            return STATUS_NOT_IMPLEMENTED;
            break;
    }

    status = WdfWorkItemCreate(&workitemConfig,
                               &attributes,
                               &hWorkItem);

    if (!NT_SUCCESS(status)) {
        return status;
    }

    // Stop dispatching on the queue until the work item completes
    //if (Pipe == pDeviceContext->BulkReadPipe) {
        if (++pDeviceContext->GPIFStopCount == 1) {
            WdfIoQueueStop(pDeviceContext->QueueGPIF, NULL, NULL);
        }
    //}
    //else if (Pipe == pDeviceContext->BulkWritePipe) {
    //    if (++pDeviceContext->WriteStopCount == 1) {
    //        //WdfIoQueueStop(pDeviceContext->QueueWrite, NULL, NULL);
    //    }
    //}
    if (++pDeviceContext->DefaultStopCount == 1) {
        WdfIoQueueStop(pDeviceContext->QueueDefault, NULL, NULL);
    }

    context = GetWorkItemContext(hWorkItem);

    context->Device = Device;
    context->Pipe = Pipe;
    context->WorkItemId = workItemId;

    //
    // Execute this work item.
    //
    WdfWorkItemEnqueue(hWorkItem);

    return STATUS_SUCCESS;
}

