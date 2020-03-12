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



VOID
EvtInternalVendorRequestCompletion(
    __in WDFREQUEST   Request,
    __in WDFIOTARGET  Target,
    __in PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    __in WDFCONTEXT   Context
    )
{
    WDFDEVICE               device;
    PDEVICE_CONTEXT         pDeviceContext;
    PINTERNAL_VENDOR_REQUEST_CONTEXT        pInternalRequestContext;
    PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL   inputBuffer;
    PQUICKUSB_DEVICE_INFO   outputBuffer;
    size_t                  inputBufferLength;
    size_t                  outputBufferLength;

    // Initialize variables
    device = WdfIoTargetGetDevice(Target);
    pDeviceContext = GetDeviceContext(device);
    pInternalRequestContext = GetInternalVendorRequestContext(Request);

    // Get the vendor request memory buffers
    inputBuffer = (PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL) WdfMemoryGetBuffer(pInternalRequestContext->inputMemory, &inputBufferLength); // TODO: Fix bugcheck - Invalid memory referenced at address zero 
    outputBuffer = (PQUICKUSB_DEVICE_INFO) WdfMemoryGetBuffer(pInternalRequestContext->outputMemory, &outputBufferLength);

    // If this was a device info request, copy the information to the device context
    if (inputBuffer->request == QUICKUSB_QUERY) {
        if (NT_SUCCESS(CompletionParams->IoStatus.Status)) {
            memcpy(&(pDeviceContext->DeviceInfo), outputBuffer, min(sizeof(pDeviceContext->DeviceInfo), outputBufferLength));
        }
    }

    // Delete the internal vendor request object
    WdfObjectDelete(Request);
}



NTSTATUS IssueVendorRequest(
    __in BOOLEAN                     internalRequest,
    __in WDFQUEUE                    Queue,
    __in WDFREQUEST                  Request,
    __in WDFMEMORY                   inputMemory,
    __in WDFMEMORY                   outputMemory,
    __in WDFCONTEXT                  context,
    __in PBOOLEAN                    requestPending) 
{
    // Variables
    NTSTATUS                        status;
    WDFDEVICE                       device;
    PDEVICE_CONTEXT                 pDeviceContext;
    WDF_USB_CONTROL_SETUP_PACKET    controlSetupPacket;
    WDF_REQUEST_SEND_OPTIONS        sendOptions;
    ULONG                           bytesTransferred;
    WDF_OBJECT_ATTRIBUTES           attributes;
    PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL inputBuffer;
    PVOID                           outputBuffer;
    size_t                          inputBufferLength;
    size_t                          outputBufferLength;

    //
    // initialize variables
    //
    device = WdfIoQueueGetDevice(Queue);
    pDeviceContext = GetDeviceContext(device);

    // Retrieve the input buffer that describes the vendor request
    inputBuffer = (PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL) WdfMemoryGetBuffer(inputMemory, &inputBufferLength);

    // Retrieve the output buffer that describes the vendor request
    outputBuffer = (PVOID) WdfMemoryGetBuffer(outputMemory, &outputBufferLength);

    // Starting with the KMDF driver, the vendor request to first send the
    // number of bytes to to before a bulk read is now automatically
    // performed within the driver, so if one is issued from the library we 
    // may simply disregard it.  We must, however, process internally issued
    // requests.
    if ((inputBuffer->request == QUICKUSB_READDATALEN) && !(inputBuffer->value & 0x01)) {
        QusbDbgPrint(DEBUG_INFO, ("--> Disregarding %s request\n", DbgGetVendorStr(QUICKUSB_READDATALEN)));
        return STATUS_SUCCESS;
    }

    // Command reads and writes access the GPIF.  Since we may not overlap GPIF data R/Ws
    // with GPIF command R/Ws we must serialize them.  We may easily do this by forwarding
    // the request to our sequential GPIF queue.
    if (inputBuffer->request == QUICKUSB_COMMAND) {
        // Make sure that this request is being processed on the appropriate queue
        if (Queue != pDeviceContext->QueueGPIF) {
            QusbDbgPrint(DEBUG_INFO, ("--> Forwarding %s request\n", DbgGetVendorStr(QUICKUSB_COMMAND)));
            status = WdfRequestForwardToIoQueue(Request, pDeviceContext->QueueGPIF);
            if (!NT_SUCCESS(status)) {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestForwardToIoQueue failed 0x%x\n", status));
                return status;
            } else {
                // We don't want to complete this request because we just forwarded it onto another queue
                if (requestPending != NULL) {
                    *requestPending = TRUE;
                }

                // We certainly do not want to process this request not that it's been forwarded
                return status;
            }
        } else {
            QusbDbgPrint(DEBUG_INFO, ("--> Processing %s request on GPIF queue\n", DbgGetVendorStr(QUICKUSB_COMMAND)));
        }
    }

    WDF_REQUEST_SEND_OPTIONS_INIT(
        &sendOptions,
        WDF_REQUEST_SEND_OPTION_TIMEOUT
        );

    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
        &sendOptions,
        pDeviceContext->Timeout
        );

    WDF_USB_CONTROL_SETUP_PACKET_INIT_VENDOR(
        &controlSetupPacket,    // Control setup packet
        inputBuffer->direction, // Direction
        BmRequestToDevice,      // Recipient
        inputBuffer->request,   // Request
        inputBuffer->value,     // Value
        inputBuffer->index);    // Index

    QusbDbgPrint(DEBUG_INFO, ("--> %s Request 0x%x (%s) for %ld bytes\n", 
        (inputBuffer->direction == BmRequestDeviceToHost) ? "IN" : "OUT",
        (int)inputBuffer->request, DbgGetVendorStr(inputBuffer->request), (ULONG)outputBufferLength));
    QusbDbgPrint(DEBUG_INFO, ("--> Index 0x%x, Value 0x%x (Timeout = %ld)\n",            
        (int)inputBuffer->index, (int)inputBuffer->value, (ULONG)pDeviceContext->Timeout));

    // Setup the control request
    status = WdfUsbTargetDeviceFormatRequestForControlTransfer(
        pDeviceContext->UsbDevice,  // USB device
        Request,                    // Request
        &controlSetupPacket,        // Setup packet
        outputMemory,               // Memory (optional)
        NULL);                      // Memory offset (optional)

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceFormatRequestForControlTransfer failed 0x%x\n", status));
        return status;
    }

    // Set the completion routine
    WdfRequestSetCompletionRoutine(
        Request,
        internalRequest ? EvtInternalVendorRequestCompletion : EvtControlRequestCompletion,
        context);

    // Send away!
    if (WdfRequestSend(Request, WdfUsbTargetDeviceGetIoTarget(pDeviceContext->UsbDevice), &sendOptions) == FALSE) {
        // Framework couldn't send the request for some reason
        status = WdfRequestGetStatus(Request);
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestSend failed 0x%x\n", status));
    }
    else {
        if (requestPending != NULL) {
            *requestPending = TRUE;
        }
    }

    return status;
}



NTSTATUS IssueInternalVendorRequest(
    __in WDFDEVICE                   device,
    __in WDFIOTARGET                 ioTarget,
    __in WDFREQUEST                  parentRequest,
    __in BYTE                        vendorRequest,
    __in BYTE                        direction,
    __in USHORT                      value,
    __in USHORT                      index,
    __in WDFMEMORY                   memory) {

    NTSTATUS                       status;
    WDF_REQUEST_SEND_OPTIONS       reqOptions;
    WDFREQUEST                     request;
    PDEVICE_CONTEXT                pDeviceContext;
    WDFMEMORY                      vRequestMem;
    PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL vRequestMemBuf;
    WDF_OBJECT_ATTRIBUTES          attributes;

    // Get the device context
    pDeviceContext = GetDeviceContext(device);

    WDF_REQUEST_SEND_OPTIONS_INIT(
        &reqOptions,
        WDF_REQUEST_SEND_OPTION_TIMEOUT
        );

    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
        &reqOptions,
        pDeviceContext->Timeout
        );

    QusbDbgPrint(DEBUG_INFO, ("--> OUT Internal Vendor Request 0x%x (%s)\n",
        vendorRequest, DbgGetVendorStr(vendorRequest)));

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = parentRequest;

    // Create the request object
    status = WdfRequestCreate(
        &attributes,
        ioTarget,
        &request);

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestCreate failed 0x%x\n", status));

        return status;
    }

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = request;

    // We need to allocate memory to store the vendor request structure
    status = WdfMemoryCreate(
         &attributes,
         NonPagedPool,
         0,
         sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL),
         &vRequestMem,
         &vRequestMemBuf
         );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfMemoryCreate failed 0x%x\n", status));

        // Delete the control request
        WdfObjectDelete(request);

        return status;
    }

    // Set the completion routine
    WdfRequestSetCompletionRoutine(
        request,
        EvtInternalVendorRequestCompletion,
        vRequestMem);

    // Fill out vendor request information
    vRequestMemBuf->direction = direction;  // Host to device
    vRequestMemBuf->requestType = 2;        // Vendor
    vRequestMemBuf->recepient = 0;
    vRequestMemBuf->requestTypeReservedBits = 0;
    vRequestMemBuf->request = vendorRequest;
    vRequestMemBuf->value = value;
    vRequestMemBuf->index = index;

    // Setup the control request
    status = WdfIoTargetFormatRequestForInternalIoctl(ioTarget, request, IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST, vRequestMem, 0, memory, 0);

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceFormatRequestForControlTransfer failed 0x%x\n", status));

        // Delete the control request
        WdfObjectDelete(request);    // Deleting the request will delete the memory since the memory's parent is the request

        return status;
    }

    // By setting up object parents properly, we do not have a need to set the completion routine
    // as all object data (the request, memory, etc) will be automatically freed by the framework.

    // Send away!
    if (WdfRequestSend(request, ioTarget, &reqOptions) == FALSE) {
        // Framework couldn't send the request for some reason
        status = WdfRequestGetStatus(request);
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestSend failed 0x%x\n", status));

        // Delete the control request
        WdfObjectDelete(request);    // Deleting the request will delete the memory since the memory's parent is the request

        return status;
    }

    return STATUS_SUCCESS;
}
