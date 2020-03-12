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
EvtControlRequestCompletion(
    __in WDFREQUEST                     Request,
    __in WDFIOTARGET                    Target,
    __in PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    __in WDFCONTEXT                     Context
    )
/*++

Routine Description:

    This is the completion routine for control transfers.

Arguments:

    Context - Driver supplied context
    Device - Device handle
    Request - Request handle
    Params - request completion params

Return Value:
    None

--*/
{
    PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;
    NTSTATUS        status;
    size_t          bytes;
    ULONG           bytesTransferred;
    WDFDEVICE       device;
    PDEVICE_CONTEXT pDeviceContext;
    ULONG           ControlRequestCode;
    WDFMEMORY       inputMemory;
    WDFMEMORY       outputMemory;
    PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL inputBuffer;
    PVOID           outputBuffer;
    ULONG           inputBufferLength;
    ULONG           outputBufferLength;

    //
    // initialize variables
    //
    ControlRequestCode = (ULONG) Context;
    device = WdfIoTargetGetDevice(Target);
    pDeviceContext = GetDeviceContext(device);
    usbCompletionParams = CompletionParams->Parameters.Usb.Completion;
    bytes = usbCompletionParams->Parameters.DeviceControlTransfer.Length;
    //inputMemory = CompletionParams->Parameters.Ioctl.Input.Buffer;
    //outputMemory = CompletionParams->Parameters.Ioctl.Output.Buffer;

    // Get the vendor request input and output buffers
    //inputBuffer = (PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL) WdfMemoryGetBuffer(inputMemory, &inputBufferLength);
    //outputBuffer = (PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL) WdfMemoryGetBuffer(outputMemory, &outputBufferLength);

    // Request the input buffer sent to the IOCTL function call
    status = WdfRequestRetrieveInputBuffer(
        Request,                                        // Request
        sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL),   // Minimum required size
        &inputBuffer,                                   // Input buffer
        NULL);                                          // Input buffer size (optional)

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
    }

    // Debug messages
    if (NT_SUCCESS(CompletionParams->IoStatus.Status)) {
        QusbDbgPrint(DEBUG_INFO, ("--> Control request succeeded (%ld bytes)\n", (ULONG)bytes));
    } else {
        QusbDbgPrint(DEBUG_ERROR, ("--> Control request failed - request status 0x%x UsbdStatus 0x%x\n",
            CompletionParams->IoStatus.Status, usbCompletionParams->UsbdStatus));
    }

    if (inputBuffer->direction == BmRequestHostToDevice) {
        bytes = 0;
    }

    // Complete this request
    //if (inputBuffer->request != QUICKUSB_QUERY) {
        QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, CompletionParams->IoStatus.Status, bytes);
    //}
}



VOID EvtIoInternalDeviceControl(
  __in  WDFQUEUE Queue,
  __in  WDFREQUEST Request,
  __in  size_t OutputBufferLength,
  __in  size_t InputBufferLength,
  __in  ULONG IoControlCode
)
{
    // Pass the internal IOCTL straight on for normal processing
    EvtIoDeviceControl(Queue, Request, OutputBufferLength, InputBufferLength, IoControlCode);
}



VOID
EvtIoDeviceControl(
    __in WDFQUEUE   Queue,
    __in WDFREQUEST Request,
    __in size_t     OutputBufferLength,
    __in size_t     InputBufferLength,
    __in ULONG      IoControlCode    
    )
/*++

Routine Description:

    This event is called when the framework receives IRP_MJ_DEVICE_CONTROL
    requests from the system.

Arguments:

    Queue - Handle to the framework queue object that is associated
            with the I/O request.
    Request - Handle to a framework request object.

    OutputBufferLength - length of the request's output buffer,
                        if an output buffer is available.
    InputBufferLength - length of the request's input buffer,
                        if an input buffer is available.

    IoControlCode - the driver-defined or system-defined I/O control code
                    (IOCTL) that is associated with the request.
Return Value:

    VOID

--*/
{
    WDFDEVICE           device;
    PDEVICE_CONTEXT     pDeviceContext;
    size_t              bytesReturned = 0;
    BOOLEAN             requestPending = FALSE;
    NTSTATUS            status = STATUS_INVALID_DEVICE_REQUEST;

    UNREFERENCED_PARAMETER(InputBufferLength);
    UNREFERENCED_PARAMETER(OutputBufferLength);

    //
    // initialize variables
    //
    device = WdfIoQueueGetDevice(Queue);
    pDeviceContext = GetDeviceContext(device);

    switch(IoControlCode) {

    case IOCTL_QUSB_GET_DEVICE_DESCRIPTOR: { // Synchronous
        // Variables
        PUSB_DEVICE_DESCRIPTOR pDeviceDescriptor;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_DEVICE_DESCRIPTOR\n"));

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                        // Request
            sizeof(USB_DEVICE_DESCRIPTOR),  // Minimum required size
            &pDeviceDescriptor,             // Output buffer
            NULL);                          // Output buffer size (optional)

        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Retrieve the USB Configuration Descriptor
        WdfUsbTargetDeviceGetDeviceDescriptor(        
            pDeviceContext->UsbDevice,   // USB Device
            pDeviceDescriptor);       // Device descriptor

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(USB_DEVICE_DESCRIPTOR);

        }
        break;



    case IOCTL_QUSB_GET_CONFIGURATION_DESCRIPTOR: { // Synchronous
        // Variables
        PUSB_CONFIGURATION_DESCRIPTOR   configurationDescriptor = NULL;
        USHORT                          requiredSize = 0;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_CONFIGURATION_DESCRIPTOR\n"));

        //
        // Retrieving the configuration descriptor returns a USB_CONFIGURATION_DESCRIPTOR structure, 
        // followed by one or more USB_INTERFACE_DESCRIPTOR and USB_ENDPOINT_DESCRIPTOR structures.
        //

        // Get the size of the config descriptor
        status = WdfUsbTargetDeviceRetrieveConfigDescriptor(
            pDeviceContext->UsbDevice,    // USB Device
            NULL,                      // Configuration descriptor
            &requiredSize);            // Length of configuration descriptor

        if (status != STATUS_BUFFER_TOO_SMALL) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceRetrieveConfigDescriptor failed 0x%x\n", status));
            break;
        }

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                    // Request
            (size_t)requiredSize,       // Minimum required size
            &configurationDescriptor,   // Output buffer
            NULL);                      // Output buffer size (optional)

        if (!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Retrieve the USB Configuration Descriptor
        status = WdfUsbTargetDeviceRetrieveConfigDescriptor(        
            pDeviceContext->UsbDevice,     // USB Device
            configurationDescriptor,    // Configuration descriptor
            &requiredSize);             // Length of configuration descriptor

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceRetrieveConfigDescriptor failed 0x%x\n", status));
            break;
        }

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = requiredSize;

        }
        break;



    case IOCTL_QUSB_GET_INTERFACE_DESCRIPTOR: { // Synchronous
        BYTE                       settingIndex;
        PUSB_INTERFACE_DESCRIPTOR  pInterfaceDescriptor;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_INTERFACE_DESCRIPTOR\n"));

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                           // Request
            sizeof(USB_INTERFACE_DESCRIPTOR),  // Minimum required size
            &pInterfaceDescriptor,             // Output buffer
            NULL);                             // Output buffer size (optional)

        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Get the setting index used for the current USB interface
        settingIndex = WdfUsbInterfaceGetConfiguredSettingIndex(
            pDeviceContext->UsbInterface
            );

        // Retrieve the current interface descriptor
        WdfUsbInterfaceGetDescriptor(
            pDeviceContext->UsbInterface,
            settingIndex,
            pInterfaceDescriptor
            );

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(USB_INTERFACE_DESCRIPTOR);
        }
        break;



    case IOCTL_QUSB_GET_PIPE_INFO: { // Synchronous
        PGET_PIPE_INFO_IN          pInputParams;
        PWDF_USB_PIPE_INFORMATION  pPipeInfo;
        WDFUSBPIPE                 Pipe;
        BYTE                       count;
        BYTE                       k;
        UCHAR                      pipeIndex;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_PIPE_INFO\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,                     // Request
            sizeof(GET_PIPE_INFO_IN),    // Minimum required size
            &pInputParams,               // Input buffer
            NULL);                       // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        // Store the pipe index
        pipeIndex = pInputParams->PipeIndex;

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                           // Request
            sizeof(WDF_USB_PIPE_INFORMATION),  // Minimum required size
            &pPipeInfo,                        // Output buffer
            NULL);                             // Output buffer size (optional)

        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Get the number of configured pipes
        count = WdfUsbInterfaceGetNumConfiguredPipes(pDeviceContext->UsbInterface);

        // Ensure that the requested pipe index is valid
        if (pInputParams->PipeIndex < count) {
            WDF_USB_PIPE_INFORMATION_INIT(pPipeInfo);

            // Retrieve the pipe information
            Pipe = WdfUsbInterfaceGetConfiguredPipe(
                pDeviceContext->UsbInterface,
                pipeIndex,
                pPipeInfo
                );

            // Ensure that the function call succeeded
            if (Pipe != NULL) {
                // Set the number bytes written to the output buffer of the IOCTL function call
                bytesReturned = sizeof(WDF_USB_PIPE_INFORMATION);
            } else {
                QusbDbgPrint(DEBUG_WARN, ("--> WdfUsbInterfaceGetConfiguredPipe on pipe %i returned NULL", (int)pipeIndex));
                status = STATUS_INVALID_PARAMETER;
            }
        } else {
            QusbDbgPrint(DEBUG_WARN, ("--> Invalid pipe index %i (%i configured)", (int)pipeIndex, (int)count));
            status = STATUS_INVALID_PARAMETER;
        }

        }
        break;



    case IOCTL_QUSB_GET_STRING_DESCRIPTOR: { // Synchronous
        // Variables
        WDFMEMORY StringMemory;
        PQUSB_GET_STRING_DESCRIPTOR_IN  inDesc;
        PQUSB_GET_STRING_DESCRIPTOR_OUT outDesc;
        USHORT NumCharacters;
        UCHAR index;
        //LPWSTR str;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_STRING_DESCRIPTOR\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,                                // Request
            sizeof(QUSB_GET_STRING_DESCRIPTOR_IN),  // Minimum required size
            &inDesc,                                // Input buffer
            NULL);                                  // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        // Get the string descriptor
        status = WdfUsbTargetDeviceAllocAndQueryString(
            pDeviceContext->UsbDevice,      // USB device
            WDF_NO_OBJECT_ATTRIBUTES,    // String memory attributes (optional)
            &StringMemory,               // String memory
            &NumCharacters,              // Number of characters returned (optional)
            inDesc->Index,               // String index
            inDesc->LanguageId);         // Language ID (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("WdfUsbTargetDeviceAllocAndQueryString failed 0x%x\n", status));
            break;
        }

        if (StringMemory == NULL) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceAllocAndQueryString returned no string memory\n"));
            break;
        }

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                                // Request
            sizeof(QUSB_GET_STRING_DESCRIPTOR_OUT), // Minimum required size
            &outDesc,                               // Output buffer
            NULL);                                  // Output buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Fill out our output buffer
        outDesc->bLength = (UCHAR)((NumCharacters + 1) * sizeof(WCHAR));          // The QuickUSB Library expects number of bytes, not characters
        outDesc->bDescriptorType = USB_STRING_DESCRIPTOR_TYPE;              // Legacy field from original WDM driver
        status = WdfMemoryCopyToBuffer(StringMemory, 0, outDesc->bString, NumCharacters * sizeof(WCHAR));
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfMemoryCopyToBuffer failed 0x%x\n", status));
            break;
        }

        // Always ensure that the string is NULL-terminated
        outDesc->bString[MAX_STRING_DESCRIPTOR_OUT_WCHARS - 1] = 0;

        // Free the memory buffer previously allocated by WdfUsbTargetDeviceAllocAndQueryString()
        WdfObjectDelete(StringMemory);

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(QUSB_GET_STRING_DESCRIPTOR_OUT);

        }
        break;



    case IOCTL_QUSB_GET_DRIVER_VERSION: { // Synchronous
        // Variables
        PQUSB_DRIVER_VERSION version;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_DRIVER_VERSION\n"));

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                            // Request
            sizeof(QUSB_DRIVER_VERSION),        // Minimum required size
            &version,                           // Output buffer
            NULL);                              // Output buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        QusbDbgPrint(DEBUG_INFO, ("--> v%i.%i.%i\n", QUSB_MAJOR_VERSION, QUSB_MINOR_VERSION, QUSB_BUILD_VERSION));

        // Fill out our output buffer
        version->MajorVersion = QUSB_MAJOR_VERSION;
        version->MinorVersion = QUSB_MINOR_VERSION;
        version->BuildVersion = QUSB_BUILD_VERSION;

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(QUSB_DRIVER_VERSION);

        }
        break;



    case IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST: { // Asynchronous
        // Variables
        PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL inputBuffer;
        ULONG                            bytesTransferred;
        WDFMEMORY                        inputMemory;
        WDFMEMORY                        outputMemory;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,        // Request
            sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL),   // Minimum required size
            &inputBuffer,   // Input buffer
            NULL);          // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        // Request the input memory
        status = WdfRequestRetrieveInputMemory(
            Request,        // Request
            &inputMemory);  // Memory

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputMemory failed 0x%x\n", status));
            break;
        }

        // Request the output memory
        status = WdfRequestRetrieveOutputMemory(
            Request,        // Request
            &outputMemory); // Memory

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputMemory failed 0x%x\n", status));
            break;
        }

        // Send the vendor request
        status = IssueVendorRequest(FALSE, Queue, Request, inputMemory, outputMemory, NULL, &requestPending);
        if (!NT_SUCCESS(status)) {
            status = WdfRequestGetStatus(Request);
        }

        }
        break;



    case IOCTL_QUSB_SET_TIMEOUT: { // Synchronous
        // Variables
        PULONG pTimeout;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_SET_TIMEOUT\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,          // Request
            sizeof(ULONG),    // Minimum required size
            &pTimeout,        // Input buffer
            NULL);            // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        pDeviceContext->Timeout = WDF_REL_TIMEOUT_IN_MS(*pTimeout);

        QusbDbgPrint(DEBUG_INFO, ("--> Timeout: %ld ms (%ld)\n", (ULONG)*pTimeout, (ULONG)pDeviceContext->Timeout));
        }
        break;



    case IOCTL_QUSB_GET_TIMEOUT: { // Synchronous
        // Variables
        PULONG pTimeout;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_TIMEOUT\n"));

        // Request the output buffer sent to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,            // Request
            sizeof(ULONG),      // Minimum required size
            (PVOID) &pTimeout,  // Output buffer
            NULL);              // Output buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        *pTimeout = (ULONG)pDeviceContext->Timeout;

        QusbDbgPrint(DEBUG_INFO, ("--> Timeout: %ld ms\n", (ULONG)pDeviceContext->Timeout));

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(ULONG);

        }
        break;



    case IOCTL_QUSB_BULK_READ: // Asynchronous (Windows IO manager will make this synchrounous if it's not overlapped)
        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_BULK_READ\n"));
     
        //
        // Start the request
        //
        status = PerformBulkRequest(device, pDeviceContext, Queue, Request, OutputBufferLength, BmRequestDeviceToHost, &requestPending);
        break;



    case IOCTL_QUSB_BULK_WRITE: // Asynchronous (Windows IO manager will make this synchrounous if it's not overlapped)
        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_BULK_WRITE\n"));

        //
        // Start the request
        //
        status = PerformBulkRequest(device, pDeviceContext, Queue, Request, OutputBufferLength, BmRequestHostToDevice, &requestPending);
        break;



    case IOCTL_QUSB_GET_LAST_ERROR: { // Synchronous
        // Variables
        PQUSB_GET_DRIVER_ERROR pDriverError;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_GET_LAST_ERROR\n"));

        // Request the output buffer send to the IOCTL function call
        status = WdfRequestRetrieveOutputBuffer(
            Request,                      // Request
            sizeof(QUSB_GET_DRIVER_ERROR),     // Minimum required size
            &pDriverError,                // Output buffer
            NULL);                        // Output buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputBuffer failed 0x%x\n", status));
            break;
        }

        // Copy over the error codes
        pDriverError->UsbdStatus = pDeviceContext->UsbdStatus;             // Legacy name: LastFailedUrbStatus
        pDriverError->QusbDriverError = pDeviceContext->QusbDriverError;   // Legacy name: DriverStatus
        pDriverError->DriverState = 0;                                  // Unused

        QusbDbgPrint(DEBUG_INFO, ("--> UsbdStatus: 0x%lx, QusbDriverError: 0x%x, DriverState = 0x%lx\n",
            pDriverError->UsbdStatus, 
            pDriverError->QusbDriverError, 
            pDriverError->DriverState));

        // Set the number bytes written to the output buffer of the IOCTL function call
        bytesReturned = sizeof(QUSB_GET_DRIVER_ERROR);
        }
        break;



    // Asynchronous bulk transfer are no longer directly supported through the
    // IOCTL interface.  Instead use the OVERLAPPED parameter when issuing a
    // Read, Write, or DeviceControlCode function call in user land.
    case IOCTL_QUSB_ASYNC_BULK_READ:
    case IOCTL_QUSB_ASYNC_BULK_WRITE:
    case IOCTL_QUSB_ASYNC_WAIT:
        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_ASYNC_*\n"));

        pDeviceContext->QusbDriverError = DriverErrorDeprecated;
        status = STATUS_NOT_IMPLEMENTED;
        break;



    case IOCTL_QUSB_CYCLE:
        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_CYCLE\n"));

        // We first must stop all IO before performing a cycle port request
        StopAllPipes(pDeviceContext);

        // Perform a cycle port request.  This power-cycles the port, causing 
        // surprise removal and re-enumeration.
        status = WdfUsbTargetDeviceCyclePortSynchronously(pDeviceContext->UsbDevice); // PASSIVE_LEVEL execution

        // Restart all IO
        StartAllPipes(pDeviceContext);

        if (!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> Cycle Port failed 0x%x\n", status));
        }
        break;



    case IOCTL_QUSB_RESET:
        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_RESET\n"));

        status = ResetDevice(device);

        if (!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> Reset Port failed 0x%x\n", status));
        }
        break;



    case IOCTL_QUSB_RESETPIPE: {
        PUCHAR                    pPipeNum;
        WDF_REQUEST_SEND_OPTIONS  sendOptions;
        WDFUSBPIPE                Pipe;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_RESETPIPE\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,          // Request
            sizeof(UCHAR),    // Minimum required size
            &pPipeNum,        // Input buffer
            NULL);            // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        WDF_REQUEST_SEND_OPTIONS_INIT(
            &sendOptions,
            WDF_REQUEST_SEND_OPTION_TIMEOUT
            );

        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
            &sendOptions,
            pDeviceContext->Timeout
            );

        // Get a handle to the pipe
        Pipe = WdfUsbInterfaceGetConfiguredPipe(
            pDeviceContext->UsbInterface,
            *pPipeNum,
            NULL
            );

        // Ensure we have a valid pipe handle
        if (Pipe != NULL) {
            // We first must stop all IO before performing a pipe reset
            StopAllPipes(pDeviceContext);

            // Reset the pipe
            status = WdfUsbTargetPipeResetSynchronously(
                Pipe,
                Request,
                &sendOptions
                );

            // Restart all IO
            StartAllPipes(pDeviceContext);

            if (!NT_SUCCESS(status)) {
                QusbDbgPrint(DEBUG_ERROR, ("--> ResetPipe failed 0x%x\n", status));

                status = ResetDevice(device);
                if (!NT_SUCCESS(status)) {
                    QusbDbgPrint(DEBUG_ERROR, ("--> ResetDevice failed 0x%x\n", status));
                }
            }
        } else {
            QusbDbgPrint(DEBUG_ERROR, ("--> Invalid pipe index %i\n", (int)*pPipeNum));
            status = STATUS_INVALID_PARAMETER;
        }

        }
        break;



    case IOCTL_QUSB_ABORTPIPE: {
        PUCHAR                    pPipeNum;
        WDF_REQUEST_SEND_OPTIONS  sendOptions;
        WDFUSBPIPE                Pipe;

        QusbDbgPrint(DEBUG_INFO, ("IOCTL_QUSB_ABORTPIPE\n"));

        // Request the input buffer sent to the IOCTL function call
        status = WdfRequestRetrieveInputBuffer(
            Request,          // Request
            sizeof(UCHAR),    // Minimum required size
            &pPipeNum,        // Input buffer
            NULL);            // Input buffer size (optional)

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputBuffer failed 0x%x\n", status));
            break;
        }

        WDF_REQUEST_SEND_OPTIONS_INIT(
            &sendOptions,
            WDF_REQUEST_SEND_OPTION_TIMEOUT
            );

        WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
            &sendOptions,
            pDeviceContext->Timeout
            );

        // Get a handle to the pipe
        Pipe = WdfUsbInterfaceGetConfiguredPipe(
            pDeviceContext->UsbInterface,
            *pPipeNum,
            NULL
            );

        // Ensure we have a valid pipe handle
        if (Pipe != NULL) {
            // Abort the pipe
            status = WdfUsbTargetPipeResetSynchronously(
                Pipe,
                Request,
                &sendOptions
                );

            if (!NT_SUCCESS(status)) {
                QusbDbgPrint(DEBUG_ERROR, ("--> AbortPipe failed 0x%x\n", status));

                status = ResetDevice(device);
                if (!NT_SUCCESS(status)) {
                    QusbDbgPrint(DEBUG_ERROR, ("--> ResetDevice failed 0x%x\n", status));
                }
            }
        } else {
            QusbDbgPrint(DEBUG_ERROR, ("--> Invalid pipe index %i\n", (int)*pPipeNum));
            status = STATUS_INVALID_PARAMETER;
        }

        }
        break;



    default: // Synchronous
        QusbDbgPrint(DEBUG_ERROR, ("Invalid IOCTL Code: 0x%x\n", IoControlCode));

        status = STATUS_INVALID_DEVICE_REQUEST;
        break;
    }

    //
    // If the request is not pending (either it is sync or failed), then we 
    // must complete it
    //
    if (requestPending == FALSE) {
        // Complete the request
        QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, bytesReturned);
    }

    return;
}
