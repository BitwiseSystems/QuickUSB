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

#ifdef ALLOC_PRAGMA
#pragma alloc_text(PAGE, EvtDeviceAdd)
#pragma alloc_text(PAGE, EvtDevicePrepareHardware)
#pragma alloc_text(PAGE, SelectInterfaces)
#pragma alloc_text(PAGE, ResetPipe)
#pragma alloc_text(PAGE, ResetDevice)
#pragma alloc_text(PAGE, AbortPipe)
#pragma alloc_text(PAGE, AbortPipes)
#endif



NTSTATUS EvtDeviceAdd(
    WDFDRIVER Driver,
    PWDFDEVICE_INIT DeviceInit
    )
/*++
Routine Description:

    EvtDeviceAdd is called by the framework in response to AddDevice
    call from the PnP manager. We create and initialize a device object to
    represent a new instance of the device. All the software resources
    should be allocated in this callback.

Arguments:

    Driver - Handle to a framework driver object created in DriverEntry

    DeviceInit - Pointer to a framework-allocated WDFDEVICE_INIT structure.

Return Value:

    NTSTATUS

--*/
{
    WDF_PNPPOWER_EVENT_CALLBACKS        pnpPowerCallbacks;
    WDF_OBJECT_ATTRIBUTES               attributes;
    WDF_OBJECT_ATTRIBUTES               queueAttributes;
    WDF_OBJECT_ATTRIBUTES               attributesForRequests;
    NTSTATUS                            status;
    WDFDEVICE                           device;
    WDF_DEVICE_PNP_CAPABILITIES         pnpCaps;
    WDF_IO_QUEUE_CONFIG                 ioQueueConfig;
    PDEVICE_CONTEXT                     pDeviceContext;
    int                                 instance = 0;
    DECLARE_UNICODE_STRING_SIZE(refStr, 16);
    DECLARE_UNICODE_STRING_SIZE(symLinkStr, 32);

    UNREFERENCED_PARAMETER(Driver);

    PAGED_CODE();

    QusbDbgPrint(DEBUG_INFO, ("EvtDeviceAdd\n"));

    //
    // Initialize the pnpPowerCallbacks structure.  Callback events for PNP
    // and Power are specified here.  If you don't supply any callbacks,
    // the Framework will take appropriate default actions based on whether
    // DeviceInit is initialized to be an FDO, a PDO or a filter device
    // object.
    //
    WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);

    //
    // For usb devices, PrepareHardware callback is the to place select the
    // interface and configure the device.
    //
    pnpPowerCallbacks.EvtDevicePrepareHardware = EvtDevicePrepareHardware;

    // These two callbacks start and stop the wdfusb pipe continuous reader
    // as we go in and out of the D0-working state.
    //
    pnpPowerCallbacks.EvtDeviceD0Entry = EvtDeviceD0Entry;
    pnpPowerCallbacks.EvtDeviceD0Exit  = EvtDeviceD0Exit;

    WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

    WdfDeviceInitSetIoType(DeviceInit, WdfDeviceIoDirect);

    // Set the context for requests
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributesForRequests, REQUEST_CONTEXT);
    WdfDeviceInitSetRequestAttributes(DeviceInit, &attributesForRequests);

    //
    // Now specify the size of device extension where we track per device
    // context.DeviceInit is completely initialized. So call the framework
    // to create the device and attach it to the lower stack.
    //
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, DEVICE_CONTEXT);

    status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceCreate failed with Status code 0x%x\n", status));
        return status;
    }

    //
    // Get the DeviceObject context by using accessor function specified in
    // the WDF_DECLARE_CONTEXT_TYPE_WITH_NAME macro for DEVICE_CONTEXT.
    //
    pDeviceContext = GetDeviceContext(device);

    //
    // Tell the framework to set the SurpriseRemovalOK in the DeviceCaps so
    // that you don't get the popup in usermode (on Win2K) when you surprise
    // remove the device.
    //
    WDF_DEVICE_PNP_CAPABILITIES_INIT(&pnpCaps);
    pnpCaps.SurpriseRemovalOK = WdfTrue;

    WdfDeviceSetPnpCapabilities(device, &pnpCaps);

    //
    // Create a parallel default queue and register an event callback to
    // receive ioctl requests. 
    //
    WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&ioQueueConfig, WdfIoQueueDispatchParallel);
    ioQueueConfig.EvtIoDeviceControl = EvtIoDeviceControl;

    // Use the no synchronization scope for this queue (See note on 
    // synchronization scope below)
    WDF_OBJECT_ATTRIBUTES_INIT(&queueAttributes);
    queueAttributes.SynchronizationScope = WdfSynchronizationScopeInheritFromParent;

    /*  
        NOTE: Completion Routine Callback Synchronization
       ---------------------------------------------------
       Each device may be associated with multiple queues.  As requests are
       dispatched to the queues (calling the relevant callbacks), sent, and
       completed their completion routines are called.  The synchonization
       of completion routines may be controled such that:

         1) The completion routinues from all the queues may overlap in
            execution (No Scope)

            Code:
              WDF_OBJECT_ATTRIBUTES_INIT(&queueAttributes);
              queueAttributes.SynchronizationScope = WdfSynchronizationScopeNone;

         2) The completion routines for each queue are synchronized so
            that their execution does not overlap, but completion 
            routines between queues may overlap (Queue Scope)

            Code:
              WDF_OBJECT_ATTRIBUTES_INIT(&queueAttributes);
              queueAttributes.SynchronizationScope = WdfSynchronizationScopeQueue;

         3) All completions routines across all queues are synchronized
            such that no completion routines execution overlaps with
            another (Device Scope)

            Code:
              WDF_OBJECT_ATTRIBUTES_INIT(&queueAttributes);
              queueAttributes.SynchronizationScope = WdfSynchronizationScopeDevice;

         The ability to overlap completion routine execution directly
         influences driver performance and should only be set when access to 
         context data in completion routinues needs to be protected.  Also,
         KMDF implements callback synchronization using spin locks.
         Therefore, if the driver specifies device or queue synchronization 
         scope, its device and queue callbacks must be able to run at 
         DISPATCH_LEVEL.

         The default synchronization scope for driver objects is 
         WdfSynchronizationScopeNone. The default synchronization scope for device 
         and queue objects is WdfSynchronizationScopeInheritFromParent.
    */

    status = WdfIoQueueCreate(device,
         &ioQueueConfig,
         &queueAttributes,
         &pDeviceContext->QueueDefault
    );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoQueueCreate failed with Status code 0x%x\n", status));
        return status;
    }

    // 
    // We will create a queue dedicated for READDATALEN control requests, which are internally issued
    // by the driver.  They cannot be issued in the GFIF queue because doing so requires that they
    // first complete before any data requests are issued, which kills performance.  When operating at
    // Super Speed, streams can clog the Default queue which makes all other control requests unresponsive.
    // Putting READDATALEN requests in their own queue solves both of these issues.
    // 
    WDF_IO_QUEUE_CONFIG_INIT(&ioQueueConfig, WdfIoQueueDispatchParallel);
    ioQueueConfig.EvtIoInternalDeviceControl = EvtIoInternalDeviceControl;

    status = WdfIoQueueCreate(device,
         &ioQueueConfig,
         &queueAttributes,
         &pDeviceContext->QueueReadDataLen
         //&pDeviceContext->QueueDefault
    );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoQueueCreate failed with Status code 0x%x\n", status));
        return status;
    }

    //
    // We will create a separate sequential queue and configure it
    // to receive read and write requests.  We also need to register a 
    // EvtIoStop handler so that we can acknowledge requests that are 
    // pending at the target driver.
    //
    WDF_IO_QUEUE_CONFIG_INIT(&ioQueueConfig, WdfIoQueueDispatchSequential);
    ioQueueConfig.EvtIoRead = EvtIoRead;
    ioQueueConfig.EvtIoWrite = EvtIoWrite;
    ioQueueConfig.EvtIoStop = EvtIoStop;
    ioQueueConfig.EvtIoResume = EvtIoResume;

    // We don't want the framework to automatically forward IO Control Requests to
    // our GPIF Queue, but we need to process the ones that have been forwarded to
    // the GPIF Queue.  We do this by providing the appropriate callback but NOT
    // calling WdfDeviceConfigureRequestDispatching for WdfRequestTypeDeviceControl
    // requests.
    ioQueueConfig.EvtIoDeviceControl = EvtIoDeviceControl;

    status = WdfIoQueueCreate(
         device,
         &ioQueueConfig,
         &queueAttributes,
         &pDeviceContext->QueueGPIF // queue handle
     );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoQueueCreate failed with Status code 0x%x\n", status));
        return status;
    }

    status = WdfDeviceConfigureRequestDispatching(
        device,
        pDeviceContext->QueueGPIF,
        WdfRequestTypeRead);

    if (!NT_SUCCESS(status)){
        ASSERT(NT_SUCCESS(status));
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceConfigureRequestDispatching failed with Status code 0x%x\n", status));
        return status;
    }

    status = WdfDeviceConfigureRequestDispatching(
        device,
        pDeviceContext->QueueGPIF,
        WdfRequestTypeWrite);

    if (!NT_SUCCESS(status)){
        ASSERT(NT_SUCCESS(status));
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceConfigureRequestDispatching failed with Status code 0x%x\n", status));
        return status;
    }

    status = WdfDeviceConfigureRequestDispatching(
        device,
        pDeviceContext->QueueReadDataLen,
        WdfRequestTypeDeviceControlInternal);

    if (!NT_SUCCESS(status)){
        ASSERT(NT_SUCCESS(status));
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceConfigureRequestDispatching failed with Status code 0x%x\n", status));
        return status;
    }

    //
    // Set intial error values
    //
    pDeviceContext->UsbdStatus = 0;                     // Legacy name: LastFailedUrbStatus
    pDeviceContext->QusbDriverError = DriverErrorNone;  // Legacy name: DriverStatus

    //
    // Register a device interface so that app can find our device and talk to it.
    // 
    status = WdfDeviceCreateDeviceInterface(device,
                        (LPGUID) &GUID_DEVINTERFACE_QUSB,
                        NULL);

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceCreateDeviceInterface failed 0x%x\n", status));
        return status;
    }

    //
    // Create symbolic link (for legacy support)
    //
    instance = 0;
    do {
        status = RtlUnicodeStringPrintf(&symLinkStr, L"\\DosDevices\\QUSB-%d", instance++);

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> RtlUnicodeStringInit failed 0x%x\n", status));
            return status;
        }

        // Try to create the symbolic link.  This will fail if a symbolic link with
        // the same name already exists.
        status = WdfDeviceCreateSymbolicLink(
            device,          // USB Device
            &symLinkStr);    // Symbolic link name

    } while ((status == STATUS_OBJECT_NAME_COLLISION) && (instance <= QUSB_MAX_DEVICES));

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfDeviceCreateSymbolicLink failed 0x%x\n", status));
        return status;
    }

    // This case should really never occur, but we should handle it nonetheless
    if (instance > QUSB_MAX_DEVICES) {
        QusbDbgPrint(DEBUG_ERROR, ("--> Unable to create symbolic link for QuickUSB device"));
        status = STATUS_UNSUCCESSFUL;
        return status;
    }

    QusbDbgPrint(DEBUG_INFO, ("--> Added Device: %S\n", symLinkStr.Buffer));

    return status;
}



NTSTATUS EvtDevicePrepareHardware(
    WDFDEVICE Device,
    WDFCMRESLIST ResourceList,
    WDFCMRESLIST ResourceListTranslated
    )
/*++

Routine Description:

    In this callback, the driver does whatever is necessary to make the
    hardware ready to use.  In the case of a USB device, this involves
    reading and selecting descriptors.

Arguments:

    Device - handle to a device

Return Value:

    NT status value

--*/
{
    NTSTATUS                            status;
    PDEVICE_CONTEXT                     pDeviceContext;
    WDF_USB_DEVICE_INFORMATION          deviceInfo;
    WDF_IO_TARGET_OPEN_PARAMS           ioParams;
    WDF_OBJECT_ATTRIBUTES               attributes;

    UNREFERENCED_PARAMETER(ResourceList);
    UNREFERENCED_PARAMETER(ResourceListTranslated);

    PAGED_CODE();

    QusbDbgPrint(DEBUG_INFO, ("EvtDevicePrepareHardware\n"));

    pDeviceContext = GetDeviceContext(Device);

    //
    // Create a USB device handle so that we can communicate with the
    // underlying USB stack. The WDFUSBDEVICE handle is used to query,
    // configure, and manage all aspects of the USB device.
    // These aspects include device properties, bus properties,
    // and I/O creation and synchronization. We only create device the first
    // the PrepareHardware is called. If the device is restarted by pnp manager
    // for resource rebalance, we will use the same device handle but then select
    // the interfaces again because the USB stack could reconfigure the device on
    // restart.
    //
    if (pDeviceContext->UsbDevice == NULL) {
        status = WdfUsbTargetDeviceCreate(Device,
                                    WDF_NO_OBJECT_ATTRIBUTES,
                                    &pDeviceContext->UsbDevice);
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceCreate failed with Status code 0x%x\n", status));
            return status;
        }
    }    

    //
    // Retrieve USBD version information, port driver capabilites and device
    // capabilites such as speed, power, etc.
    //
    WDF_USB_DEVICE_INFORMATION_INIT(&deviceInfo);

    status = WdfUsbTargetDeviceRetrieveInformation(pDeviceContext->UsbDevice, &deviceInfo);
    if (NT_SUCCESS(status)) {
        //
        // Save these for use later. 
        //
        pDeviceContext->UsbDeviceTraits = deviceInfo.Traits;
    }

    // Set the initial timeout
    pDeviceContext->Timeout = QUSB_DEFAULT_TIMEOUT;

    // Setting the parent object to the device will cause the driver to delete the IO target
    // when it destroys the device
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = Device;

    // Create an IO target to this driver
    status = WdfIoTargetCreate(Device, &attributes, &pDeviceContext->IoTargetSelf);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoTargetCreate failed 0x%x\n", status));

        return status;
    }

    WDF_IO_TARGET_OPEN_PARAMS_INIT_EXISTING_DEVICE(&ioParams,  WdfDeviceWdmGetDeviceObject(Device));

    // Open an IO target to this driver
    status = WdfIoTargetOpen(pDeviceContext->IoTargetSelf, &ioParams);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoTargetOpen failed 0x%x\n", status));

        WdfObjectDelete(pDeviceContext->IoTargetSelf);

        return status;
    }

    // Select interfaces
    status = SelectInterfaces(Device);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> SelectInterfaces failed 0x%x\n", status));
        return status;
    }

    return status;
}



__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS SelectInterfaces(
    __in WDFDEVICE Device
    )
/*++

Routine Description:

    This helper routine selects the configuration, interface and
    creates a context for every pipe (end point) in that interface.

Arguments:

    Device - Handle to a framework device

Return Value:

    NT status value

--*/
{
    WDF_USB_DEVICE_SELECT_CONFIG_PARAMS     configParams;
    NTSTATUS                                status;
    PDEVICE_CONTEXT                         pDeviceContext;
    WDFUSBPIPE                              pipe;
    WDF_USB_PIPE_INFORMATION                pipeInfo;
    UCHAR                                   index;
    UCHAR                                   numberConfiguredPipes;
    WDFREQUEST                              tempRequest;
    WDFMEMORY                               outputMemory;
    WDFMEMORY                               inputMemory;
    PQUSB_VENDOR_OR_CLASS_REQUEST_CONTROL   vRequest;
    WDF_OBJECT_ATTRIBUTES                   attributes;
    PINTERNAL_VENDOR_REQUEST_CONTEXT        pInternalRequestContext;

    PAGED_CODE();

    QusbDbgPrint(DEBUG_INFO, ("SelectInterfaces\n"));

    pDeviceContext = GetDeviceContext(Device);

    WDF_USB_DEVICE_SELECT_CONFIG_PARAMS_INIT_SINGLE_INTERFACE(&configParams);

    status = WdfUsbTargetDeviceSelectConfig(pDeviceContext->UsbDevice, WDF_NO_OBJECT_ATTRIBUTES, &configParams);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetDeviceSelectConfig failed 0x%x \n", status));
        return status;
    }

    pDeviceContext->UsbInterface = configParams.Types.SingleInterface.ConfiguredUsbInterface;
    numberConfiguredPipes = configParams.Types.SingleInterface.NumberConfiguredPipes;
    pDeviceContext->NumberConfiguredPipes = numberConfiguredPipes;

    //
    // Get pipe handles
    //
    for (index=0; index < numberConfiguredPipes; index++) {
        WDF_USB_PIPE_INFORMATION_INIT(&pipeInfo);

        pipe = WdfUsbInterfaceGetConfiguredPipe(
            pDeviceContext->UsbInterface,
            index,
            &pipeInfo
            );

        //
        // Tell the framework that it's okay to read less than
        // MaximumPacketSize
        //
        WdfUsbTargetPipeSetNoMaximumPacketSizeCheck(pipe);

        if (WdfUsbPipeTypeBulk == pipeInfo.PipeType && WdfUsbTargetPipeIsInEndpoint(pipe)) {
            pDeviceContext->BulkReadPipe = pipe;
            pDeviceContext->BulkReadMaxPacketSize = pipeInfo.MaximumPacketSize;
        }

        if (WdfUsbPipeTypeBulk == pipeInfo.PipeType && WdfUsbTargetPipeIsOutEndpoint(pipe)) {
            pDeviceContext->BulkWritePipe = pipe;
            pDeviceContext->BulkWriteMaxPacketSize = pipeInfo.MaximumPacketSize;
        }
    }

    // Create a new internal vendor request to read the device information.  This only needs to
    // happen once per device and the device data is used to determine what kind of device is
    // connected.  For USB2, the request will fill and the data structure will remain zeros.  For
    // QUSB3, the request will succeed and contain lots of useful information.
    WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, INTERNAL_VENDOR_REQUEST_CONTEXT);

    status = WdfRequestCreate(&attributes, WdfUsbTargetDeviceGetIoTarget(pDeviceContext->UsbDevice), &tempRequest);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestCreate failed 0x%x\n", status));
        return status;
    }

    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = tempRequest;  // The framework will delete the object when it deletes the parent object

    // Create the input buffer describing the vendor request
    status = WdfMemoryCreate(
         &attributes,
         NonPagedPool,
         0,
         sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL),
         &inputMemory,
         &vRequest
         );

    if (!NT_SUCCESS(status)) {
        WdfObjectDelete(tempRequest);

        QusbDbgPrint(DEBUG_ERROR, ("--> WdfMemoryCreate failed 0x%x\n", status));
        return status;
    }

    // Fill out information about the vendor request
    vRequest->requestType = 2;   // Vendor
    vRequest->recepient = 0;     // Device
    vRequest->direction = BmRequestDeviceToHost;
    vRequest->requestTypeReservedBits = 0;
    vRequest->request = QUICKUSB_QUERY;
    vRequest->index = 0;        // DEVICE_INFO
    vRequest->value = 0;

    // Create the output buffer to store the received data
    status = WdfMemoryCreate(
         &attributes,
         NonPagedPool,
         0,
         sizeof(QUICKUSB_DEVICE_INFO),
         &outputMemory,
         NULL
         );

    if (!NT_SUCCESS(status)) {
        WdfObjectDelete(tempRequest);

        QusbDbgPrint(DEBUG_ERROR, ("--> WdfMemoryCreate failed 0x%x\n", status));
        return status;
    }

    // Fill out the request context
    pInternalRequestContext = GetInternalVendorRequestContext(tempRequest);
    pInternalRequestContext->inputMemory = inputMemory;
    pInternalRequestContext->outputMemory = outputMemory;

    //
    // Read the device information
    //
    status = IssueVendorRequest(TRUE, pDeviceContext->QueueDefault, tempRequest, inputMemory, outputMemory, NULL, NULL);
    if (!NT_SUCCESS(status)) {
        status = WdfRequestGetStatus(tempRequest);
        WdfObjectDelete(tempRequest);
    }

    return status;
}



__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS ResetPipe(
    __in WDFDEVICE  Device,
    __in WDFUSBPIPE Pipe
    )
/*++

Routine Description:

    This routine resets the pipe.

Arguments:

    Pipe - framework pipe handle

Return Value:

    NT status value

--*/
{
    //
    //  This routine synchronously submits a URB_FUNCTION_RESET_PIPE
    // request down the stack.
    //
    PDEVICE_CONTEXT  pDeviceContext;
    NTSTATUS         status;

    PAGED_CODE();

    pDeviceContext = GetDeviceContext(Device);

    WdfIoTargetStop(
        WdfUsbTargetPipeGetIoTarget(Pipe),
        WdfIoTargetCancelSentIo
        );

    status = WdfUsbTargetPipeResetSynchronously(Pipe,
        WDF_NO_HANDLE, // WDFREQUEST
        NULL           // PWDF_REQUEST_SEND_OPTIONS
        );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetPipeResetSynchronously failed 0x%x\n", status));
        return status;
    }
    else {
        QusbDbgPrint(DEBUG_INFO, ("--> ResetPipe succeeded\n"));
    }

    status = WdfIoTargetStart(
        WdfUsbTargetPipeGetIoTarget(Pipe)
        );

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfIoTargetStart failed 0x%x\n", status));
        return status;
    }

    return status;
}



VOID StopAllPipes(
    __in PDEVICE_CONTEXT DeviceContext
    )
{
    UCHAR count,i;

    count = DeviceContext->NumberConfiguredPipes;
    for (i = 0; i < count; i++) {
        WDFUSBPIPE pipe;
        pipe = WdfUsbInterfaceGetConfiguredPipe(DeviceContext->UsbInterface,
                                                i, //PipeIndex,
                                                NULL
                                                );
        WdfIoTargetStop(WdfUsbTargetPipeGetIoTarget(pipe),
                        WdfIoTargetCancelSentIo);
    }
}



VOID StartAllPipes(
    __in PDEVICE_CONTEXT DeviceContext
    )
{
    NTSTATUS status;
    UCHAR count,i;

    count = DeviceContext->NumberConfiguredPipes;
    for (i = 0; i < count; i++) {
        WDFUSBPIPE pipe;
        pipe = WdfUsbInterfaceGetConfiguredPipe(DeviceContext->UsbInterface,
                                                i, //PipeIndex,
                                                NULL
                                                );
        status = WdfIoTargetStart(WdfUsbTargetPipeGetIoTarget(pipe));
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("StartAllPipes - failed pipe #%d\n", i));
        }
    }
}



__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS ResetDevice(
    __in WDFDEVICE Device
    )
/*++

Routine Description:

    This routine calls WdfUsbTargetDeviceResetPortSynchronously to reset the device if it's still
    connected.

Arguments:

    Device - Handle to a framework device

Return Value:

    NT status value

--*/
{
    PDEVICE_CONTEXT pDeviceContext;
    NTSTATUS status;

    QusbDbgPrint(DEBUG_INFO, ("--> ResetDevice\n"));

    PAGED_CODE();

    pDeviceContext = GetDeviceContext(Device);
    
    StopAllPipes(pDeviceContext);
    
    //
    // It may not be necessary to check whether device is connected before
    // resetting the port.
    //
    status = WdfUsbTargetDeviceIsConnectedSynchronous(pDeviceContext->UsbDevice);

    if (NT_SUCCESS(status)) {
        status = WdfUsbTargetDeviceResetPortSynchronously(pDeviceContext->UsbDevice);
    } else {
        QusbDbgPrint(DEBUG_ERROR, ("--> Cannot reset...device no longer connected\n"));
    }

    StartAllPipes(pDeviceContext);

    return status;
}



__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS AbortPipe(
    __in WDFDEVICE   Device,
    __in WDFUSBPIPE  Pipe
    )
/*++

Routine Description

    Sends an abort pipe request on the specified pipe.

Arguments:

    Device - Handle to a framework device
    Pipe - Handle to a framework USB pipe

Return Value:

    NT status value

--*/
{
    UCHAR              i;
    ULONG              count;
    NTSTATUS           status;
    PDEVICE_CONTEXT    pDeviceContext;

    PAGED_CODE();

    //
    // initialize variables
    //
    pDeviceContext = GetDeviceContext(Device);

    status = WdfUsbTargetPipeAbortSynchronously(
        Pipe,          // WDFUSBPIPE
        WDF_NO_HANDLE, // WDFREQUEST
        NULL);         // PWDF_REQUEST_SEND_OPTIONS

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetPipeAbortSynchronously failed %x\n", status));
        return status;
    }
    else {
        QusbDbgPrint(DEBUG_INFO, ("--> AbortPipe succeeded\n"));
    }

    return STATUS_SUCCESS;
}



__drv_requiresIRQL(PASSIVE_LEVEL)
NTSTATUS AbortPipes(
    __in WDFDEVICE Device
    )
/*++

Routine Description

    Sends an abort pipe request on all open pipes.

Arguments:

    Device - Handle to a framework device

Return Value:

    NT status value

--*/
{
    UCHAR              i;
    ULONG              count;
    NTSTATUS           status;
    PDEVICE_CONTEXT    pDeviceContext;
    WDFUSBPIPE         pipe;

    PAGED_CODE();

    //
    // initialize variables
    //
    pDeviceContext = GetDeviceContext(Device);

    QusbDbgPrint(DEBUG_INFO, ("AbortPipes\n"));

    count = pDeviceContext->NumberConfiguredPipes;

    for (i = 0; i < count; i++) {
        pipe = WdfUsbInterfaceGetConfiguredPipe(
            pDeviceContext->UsbInterface,
            i,
            NULL
            );

        status = WdfUsbTargetPipeAbortSynchronously(
            pipe,          // WDFUSBPIPE
            WDF_NO_HANDLE, // WDFREQUEST
            NULL);         // PWDF_REQUEST_SEND_OPTIONS

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetPipeAbortSynchronously failed %x\n", status));
            break;
        }
    }

    return STATUS_SUCCESS;
}
