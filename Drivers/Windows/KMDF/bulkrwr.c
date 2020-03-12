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
EvtIoRead(
    __in WDFQUEUE         Queue,
    __in WDFREQUEST       Request,
    __in size_t           Length
    )
/*++

Routine Description:

    Called by the framework when it receives Read or Write requests.

Arguments:

    Queue - Default queue handle
    Request - Handle to the read/write request
    length - Length of the data buffer associated with the request.
                 The default property of the queue is to not dispatch
                 zero length read & write requests to the driver and
                 complete is with status success. So we will never get
                 a zero length request.

Return Value:


--*/
{
    NTSTATUS          status;
    WDFDEVICE         device;
    PDEVICE_CONTEXT   pDeviceContext;
    BOOLEAN           requestPending;

    QusbDbgPrint(DEBUG_INFO, ("EvtIoRead\n"));

    //
    // initialize variables
    //
    device = WdfIoQueueGetDevice(Queue);
    pDeviceContext = GetDeviceContext(device);

    //
    // Start the request
    //
    status = PerformBulkRequest(device, pDeviceContext, Queue, Request, Length, BmRequestDeviceToHost, &requestPending);

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> PerformBulkRequest failed 0x%x\n", status));

        // Check if the request was already completed.  If requestPending==TRUE, then PerformBulkRequest
        // took control of the request and handled the completion.  Otherwise, we must handle completion.
        if (!requestPending) {
            QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, 0);
        }

        return;
    }
}



VOID 
EvtIoWrite(
    __in WDFQUEUE         Queue,
    __in WDFREQUEST       Request,
    __in size_t           Length
    ) 
/*++

Routine Description:

    Called by the framework when it receives Read or Write requests.

Arguments:

    Queue - Default queue handle
    Request - Handle to the read/write request
    length - Length of the data buffer associated with the request.
                 The default property of the queue is to not dispatch
                 zero length read & write requests to the driver and
                 complete is with status success. So we will never get
                 a zero length request.

Return Value:


--*/
{
    NTSTATUS          status;
    WDFDEVICE         device;
    PDEVICE_CONTEXT   pDeviceContext;
    BOOLEAN           requestPending;

    QusbDbgPrint(DEBUG_INFO, ("EvtIoWrite\n"));

    //
    // initialize variables
    //
    device = WdfIoQueueGetDevice(Queue);
    pDeviceContext = GetDeviceContext(device);

    //
    // Start the request
    //
    status = PerformBulkRequest(device, pDeviceContext, Queue, Request, Length, BmRequestHostToDevice, &requestPending);

    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> PerformBulkRequest failed 0x%x\n", status));

        // Check if the request was already completed.  If requestPending==TRUE, then PerformBulkRequest
        // took control of the request and handled the completion.  Otherwise, we must handle completion.
        if (!requestPending) {
            QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, 0);
        }

        return;
    }
}



VOID
EvtIoStop(
    __in WDFQUEUE         Queue,
    __in WDFREQUEST       Request,
    __in ULONG            ActionFlags
    )
/*++

Routine Description:

    This callback is invoked on every inflight request when the device
    is suspended or removed. Since our inflight read and write requests
    are actually pending in the target device, we will just acknowledge
    its presence. Until we acknowledge, complete, or requeue the requests
    framework will wait before allowing the device suspend or remove to
    proceeed. When the underlying USB stack gets the request to suspend or
    remove, it will fail all the pending requests.

Arguments:

Return Value:
    None

--*/
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(ActionFlags);

    QusbDbgPrint(DEBUG_INFO, ("EvtIoStop\n"));

    if (ActionFlags & WdfRequestStopActionSuspend) {
        WdfRequestStopAcknowledge(Request, FALSE); // Don't requeue
        QusbDbgPrint(DEBUG_ERROR, ("--> Request Stopped: WdfRequestStopActionSuspend\n"));
    } else if (ActionFlags & WdfRequestStopActionPurge) {
        WdfRequestCancelSentRequest(Request);
        QusbDbgPrint(DEBUG_ERROR, ("--> Request Stopped: WdfRequestCancelSentRequest\n"));
    }

    return;
}



// SDV: If a driver registers an IoEvtStop and calls WdfRequestStopAcknowledge
// with $2==FALSE, then the driver should register a EvtIoResume callback.
VOID 
EvtIoResume(
    __in WDFQUEUE Queue, 
    __in WDFREQUEST Request
    ) 
/*++
Routine Description:

    The EvtIoResume callback function resumes processing a previously 
    stopped I/O request. The framework calls an I/O queue's EvtIoResume 
    callback function when it resumes delivering I/O requests to the driver 
    from the queue, after the device has returned to its working state.uest 
    to suspend or remove, it will fail all the pending requests.

Arguments:

Return Value:
    None
--*/
{
    UNREFERENCED_PARAMETER(Queue);
    UNREFERENCED_PARAMETER(Request);

    QusbDbgPrint(DEBUG_INFO, ("EvtIoStop\n"));

    return;
}



NTSTATUS IssueParentRequest(__in WDFREQUEST Request) {
    // Variables
    NTSTATUS                 status;
    size_t                   splitK, k;
    PREQUEST_CONTEXT         pParentRequestContext;
    PREQUEST_CONTEXT         pSplitRequestContext;
    ULONG                    count;
    WDF_OBJECT_ATTRIBUTES    attributes;
    WDFREQUEST               tempRequest;
    BOOLEAN                  bSucceeded;
    WDFDEVICE                Device;
    PDEVICE_CONTEXT          pDeviceContext;
    WDFQUEUE                 Queue;
    WDF_REQUEST_SEND_OPTIONS reqOptions;
    WDFCOLLECTION             issueCollection;

    // NOTE: We own the request, so if anything fails we must be sure to complete the request

    // Get the parent request context
    pParentRequestContext = GetRequestContext(Request);

    // Get the queue
    Queue = WdfRequestGetIoQueue(Request);

    // Get the device
    Device = WdfIoQueueGetDevice(Queue);

    // Get the device context
    pDeviceContext = GetDeviceContext(Device);

    // Set the request options: Have a timeout
    WDF_REQUEST_SEND_OPTIONS_INIT(
        &reqOptions, 
        WDF_REQUEST_SEND_OPTION_TIMEOUT);
    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
        &reqOptions,
        pDeviceContext->Timeout);

    // Mark the parent request as cancellable
    status = WdfRequestMarkCancelableEx(Request, EvtRequestCancel);
    if (!NT_SUCCESS(status)) {
        if (status == STATUS_CANCELLED) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestMarkCancelableEx indicated request was cancelled before being issued\n"));
            // From MSDN: If the I/O manager or another driver cancels the request before 
            // WdfRequestMarkCancelableEx returns, WdfRequestMarkCancelableEx returns 
            // STATUS_CANCELLED. In this case, the driver must perform the same cancellation 
            // activities that the EvtRequestCancel callback function performs and then 
            // call WdfRequestComplete, specifying a status value of STATUS_CANCELLED.

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            // Complete the request
            QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, pParentRequestContext->bytes);

            return status;
        } else {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestMarkCancelableEx failed 0x%x\n", status));
        }
    }

    //
    // Create the SpinLock to synchronize the cancellation of requests
    //
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = Request;  // The framework will delete the object when it deletes the parent object
    status = WdfSpinLockCreate(&attributes, &pParentRequestContext->collectionLock);
    if (!NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfSpinLockCreate failed 0x%x\n", status));

        // Delete all requests that have been created
        count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
        for (k = 0; k < count; ++k) {
            tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
            if (tempRequest != NULL) {
                WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                WdfObjectDelete(tempRequest);
            } else {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
            }
        }

        // Complete the request
        QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, pParentRequestContext->bytes);

        return status;
    }

    // Create a copy of the request collection so that as we issue requests, the completion handler may remove the requests
    // from the request collection without messing up the request issue loop.  This allows us to not have to hold the
    // request collection lock while we issue requests, allowing for better parallel operation.
    
    // Create a collection for the split requests we will issue
    status = WdfCollectionCreate(WDF_NO_OBJECT_ATTRIBUTES, &issueCollection);
    if (!NT_SUCCESS(status)){
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionCreate failed 0x%x\n", status));

        // Delete all requests that have been created
        count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
        for (k = 0; k < count; ++k) {
            tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
            if (tempRequest != NULL) {
                WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                WdfObjectDelete(tempRequest);
            } else {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
            }
        }

        // Complete the request
        QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, pParentRequestContext->bytes);

        return status;
    }

    // Loop over each split request and add it to the issue collection
    count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
    for (splitK = 0; splitK < count; ++splitK) {
        // Get the request
        tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, splitK);

        // Add the request to the cancel collection
        status = WdfCollectionAdd(issueCollection, tempRequest);
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionAdd failed 0x%x\n", status));

            // Delete the cancel collection object
            WdfObjectDelete(issueCollection);

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
            tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
            if (tempRequest != NULL) {
                WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                WdfObjectDelete(tempRequest);
            } else {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
            }
            }

            // Complete the request
            QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, 0);

            return status;
        }
    }

    //
    // Issue the requests.  Since we have a copy of the request collection, we do not need to hold the lock.
    //
    bSucceeded = FALSE;
    count = WdfCollectionGetCount(issueCollection);
    
    QusbDbgPrint(DEBUG_INFO, ("--> Issuing %ld split requests\n", (ULONG)count));

    for (splitK = 0; splitK < count; ++splitK) {
        // Get the request context
        tempRequest = WdfCollectionGetItem(issueCollection, splitK);
        ASSERT(tempRequest != NULL);

        // SDV Check: Cannot call WdfRequestSend with request==NULL
        if (tempRequest == NULL) {
            bSucceeded = FALSE;
            status = STATUS_INVALID_PARAMETER;
            QusbDbgPrint(DEBUG_ERROR, ("--> Cannot call WdfRequestSend with request==NULL\n"));
            break;
        }
        
        if (WdfRequestSend(tempRequest, WdfUsbTargetPipeGetIoTarget(pParentRequestContext->Pipe), &reqOptions) == FALSE) { // Send away!
            // Framework couldn't send the request for some reason
            bSucceeded = FALSE;
            status = WdfRequestGetStatus(tempRequest);
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestSend failed 0x%x\n", status));
            break;
        }

        // Everything is good so far
        bSucceeded = TRUE;
    }

    if (bSucceeded) {
        // Delete the issue collection object
        WdfObjectDelete(issueCollection);
    } else {
        // SDV: If for any reason no requests were issued, we must complete the request with failure
        BOOLEAN oneCancelled = FALSE;

        // HACK! We're not going to issue any more of the split requests, but we
        // still need the last successfully issued request to complete the
        // parent request, so we need to fool it into doing so.
        pParentRequestContext->splitRequestTotal = splitK;  // TODO: This could fail if all issued requests have already completed!!!
        pParentRequestContext->status = status;

        // Cancel issued requests that are pending
        for (k = 0; k < splitK; ++k) {
            tempRequest = WdfCollectionGetItem(issueCollection, k);

            if (WdfRequestCancelSentRequest(tempRequest)) {
                oneCancelled = TRUE;
            }
        }

        // Delete the issue collection object
        WdfObjectDelete(issueCollection);

        // If not even a single request was cancelled, then no completion
        // routine will be called, so we must complete the request here
        if (!oneCancelled) {
            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, Request, status, pParentRequestContext->bytes);
        }

        // NOTE: Any requests that have already been issued will be
        // serviced and may or may not fail.  Either way the whole IO
        // request will never be fully serviced and must return an
        // error.

        return status;
    }

    //
    // For writes, we must commit writes so that data is transferred from the TX DMA buffer to the GPIF, even when
    // transfers are not multiples of the DMA buffer size.
    //
    if (pParentRequestContext->Pipe == pDeviceContext->BulkWritePipe) {
        // We only need to send the ZLP for QUSB3 FX3 devices
        if ((pDeviceContext->DeviceInfo.ProductClass == 0x02) && (pDeviceContext->DeviceInfo.ProductSubClass == 0x03) && (pDeviceContext->DeviceInfo.ProductArchitecture == 0x03)) {
            WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
            attributes.ParentObject = Request;  // The framework will delete the object when it deletes the parent object

            // Create the request
            status = WdfRequestCreate(&attributes, WdfUsbTargetPipeGetIoTarget(pParentRequestContext->Pipe), &tempRequest);
            if (!NT_SUCCESS(status)) {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestCreate failed 0x%x\n", status));
                return status;
            }

            // Configure the request
            status = WdfUsbTargetPipeFormatRequestForWrite(pParentRequestContext->Pipe, tempRequest, NULL, NULL);
            if (!NT_SUCCESS(status)) {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetPipeFormatRequestForWrite failed 0x%x\n", status));
                return status;
            }

            // Issue the request
            if (WdfRequestSend(tempRequest, WdfUsbTargetPipeGetIoTarget(pParentRequestContext->Pipe), NULL) == FALSE) { // Send away!
                // Framework couldn't send the request for some reason
                status = WdfRequestGetStatus(tempRequest);
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestSend failed 0x%x\n", status));
                return status;
            }
        }
    }

    ASSERT(NT_SUCCESS(status));
    return status;
}



NTSTATUS PerformBulkRequest(
    __in WDFDEVICE                   device,
    __in PDEVICE_CONTEXT             pDeviceContext,
    __in WDFQUEUE                    Queue,
    __in WDFREQUEST                  Request,
    __in size_t                      bytes,
    __in WDF_USB_BMREQUEST_DIRECTION direction,
    __out BOOLEAN                    *requestPending) {

    // Variables
    NTSTATUS                 status;
    PVOID                    buffer;
    WDFMEMORY                reqMemory;
    WDF_REQUEST_SEND_OPTIONS reqOptions;
    WDFMEMORY_OFFSET         memoryOffset;
    WDF_REQUEST_REUSE_PARAMS reuseParams;
    PREQUEST_CONTEXT         pParentRequestContext;
    PREQUEST_CONTEXT         pSplitRequestContext;
    ULONG                    splitRequestTotal;
    ULONG                    splitRequestCount;
    ULONG                    count;
    WDFUSBPIPE               pipe;
    size_t                   splitK, k;
    WDF_OBJECT_ATTRIBUTES    attributes;
    WDFREQUEST               splitRequest[QUSB_MAX_BULK_SPLIT_REQUESTS];
    WDFREQUEST               tempRequest;
    BOOLEAN                  bSucceeded;
    WDFMEMORY                ctlMem;
    ULONG                   *ctlMemBuf;

    // The request isn't pending until we have actually issued it.  We must make
    // sure that this is always set properly so that WdfRequestComplete is called
    // on every request
    *requestPending = FALSE;

    // Get the IO pipe we are working with
    pipe = ((direction == BmRequestHostToDevice) ? pDeviceContext->BulkWritePipe : pDeviceContext->BulkReadPipe);

    // Determine the number of split requests needed to fulfill this request.
    // The last split request is always the main/parent request
    splitRequestTotal = bytes / QUSB_MAX_BULK_REQUEST_TRANSFER_SIZE;
    if ((bytes % QUSB_MAX_BULK_REQUEST_TRANSFER_SIZE) != 0) {
        ++splitRequestTotal;
    }
    QusbDbgPrint(DEBUG_INFO, ("--> Issuing bulk request (Bytes = %ld, SplitRequestTotal = %ld, Timeout = %ld)\n",  (ULONG)bytes, (ULONG)splitRequestTotal, (ULONG)pDeviceContext->Timeout));

    // Get the parent request context
    pParentRequestContext = GetRequestContext(Request);

    // Set the IO Pipe associate with the request
    pParentRequestContext->Pipe = pipe;

    // NOTE: With the DeviceIoControl calls, the input buffer contains the bulk
    // pipe number.  We do not use that value in this driver because the bulk 
    // pipe handle is stored in the device context once it is located 
    // (in the SelectInterfaces() method).  With the ReadFile call their is
    // only and output buffer and with the WriteFile call their is only an
    // input buffer.

    // Request the output buffer sent to the IOCTL function call
    // SDV: Check the request direction to get the appropriate memory buffer
    if (direction == BmRequestHostToDevice) { // Write
        status = WdfRequestRetrieveInputMemory(Request, &reqMemory);
        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveInputMemory failed 0x%x\n", status));
            return status;
        }

        // Writes to QUSB3 that are not a multiple of the DMA buffer size must send a ZLP
        // to commit the partial DMA buffer to the GPIF
    } else { // Read
        status = WdfRequestRetrieveOutputMemory(Request, &reqMemory);
        if(!NT_SUCCESS(status)){
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestRetrieveOutputMemory failed 0x%x\n", status));
            return status;
        }
    }

    // Create a collection to store the handles to all of the split
    // requests associated with the parent request
    WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
    attributes.ParentObject = Request;  // The framework will delete the object when it deletes the parent object
    status = WdfCollectionCreate(&attributes, &pParentRequestContext->requestCollection);
    if(!NT_SUCCESS(status)){
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionCreate failed 0x%x\n", status));
        return status;
    }

    // Set the request options: Have a timeout
    WDF_REQUEST_SEND_OPTIONS_INIT(
        &reqOptions, 
        WDF_REQUEST_SEND_OPTION_TIMEOUT);
    WDF_REQUEST_SEND_OPTIONS_SET_TIMEOUT(
        &reqOptions,
        pDeviceContext->Timeout);

    //
    // Create, initialize and issue the requests.  To ensure that we do this in 
    // the most stable way possible, we will first create all the requests, then
    // initialize the requests, and finally issue the requests.  Isolating each
    // step ensures that if a single failure arise it will occur as early as
    // possible (ideally before a single request is actually sent).
    //

    //
    // Create requests
    //
    for (splitK = 0; splitK < splitRequestTotal; ++splitK) {
        // Set the context for split requests
        WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, REQUEST_CONTEXT);
        attributes.ParentObject = Request;

        // Create the split requests
        status = WdfRequestCreate(
            &attributes,
            WdfUsbTargetPipeGetIoTarget(pipe),
            &splitRequest[splitK]);

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfCreateRequest failed 0x%x\n", status));

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }
        
        // Add the request to the parent's collection
        status = WdfCollectionAdd(pParentRequestContext->requestCollection, splitRequest[splitK]);
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionAdd failed 0x%x\n", status));

            // Delete the request that could not be added
            WdfObjectDelete(splitRequest[splitK]);

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }
    }

    //
    // Initialize requests
    //
    pParentRequestContext->splitRequestTotal = splitRequestTotal;
    for (splitK = 0, memoryOffset.BufferOffset = 0; (splitK < splitRequestTotal); ++splitK, memoryOffset.BufferOffset += memoryOffset.BufferLength) {
        // Get the request context
        pSplitRequestContext = GetRequestContext(splitRequest[splitK]);

        // Calculate the amount of data to read/write for this request
        memoryOffset.BufferLength = min(QUSB_MAX_BULK_REQUEST_TRANSFER_SIZE, bytes - memoryOffset.BufferOffset);

        // Fill out the request context
        pSplitRequestContext->parentRequest = Request;
        pSplitRequestContext->splitRequestCount = 0;
        pSplitRequestContext->splitRequestTotal = splitRequestTotal;
        pSplitRequestContext->splitRequestBytes = memoryOffset.BufferLength;
        pSplitRequestContext->bytes = 0;
        pSplitRequestContext->status = STATUS_SUCCESS; // Later changed in completion routine
        pSplitRequestContext->Pipe = pipe;

        //
        // The format call validates to make sure that you are reading or
        // writing to the right pipe type, sets the appropriate transfer flags,
        // creates an URB and initializes the request.
        //
        if (direction == BmRequestHostToDevice) {
            status = WdfUsbTargetPipeFormatRequestForWrite(
                pipe,
                splitRequest[splitK],
                reqMemory,
                &memoryOffset
                );
        } else {
            status = WdfUsbTargetPipeFormatRequestForRead(
                pipe,
                splitRequest[splitK],
                reqMemory,
                &memoryOffset
                );
        }

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfUsbTargetPipeFormatRequestFor(Read|Write) failed 0x%x\n", status));

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }

        // Set the completion routine
        WdfRequestSetCompletionRoutine(
            splitRequest[splitK],
            EvtRequestReadWriteCompletion,
            pipe);

        // WdfRequestAllocateTimer should be called before WdfRequestSend when using
        // a timeout to ensure that WdfRequestSend will not fail if there are 
        // insufficient system resources to allocate a timer.
        status = WdfRequestAllocateTimer(splitRequest[splitK]);
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestAllocateTimer failed 0x%x\n", status));

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }
    }

    //
    // For reads, send number of bytes to read
    //
    if (direction == BmRequestDeviceToHost) {
        //
        // Before performing a read, we must send the number of bytes required for
        // the transaction by means of an internal vendor request
        //
        WDF_OBJECT_ATTRIBUTES_INIT(&attributes);
        attributes.ParentObject = Request;

        // We need to allocate memory to store the number of bytes
        status = WdfMemoryCreate(
             &attributes,
             NonPagedPool,
             0,
             sizeof(ULONG),
             &ctlMem,
             &ctlMemBuf
             );

        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfMemoryCreate failed 0x%x\n", status));

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }

        // Record the number of bytes to read
        *ctlMemBuf = bytes;

        // Send the vendor request
        status = IssueInternalVendorRequest(
            device,                 // Device
            pDeviceContext->IoTargetSelf,
            Request,                // Parent request
            0,                      // OUT
            QUICKUSB_READDATALEN,   // Request
            0x01,                   // Value - 0x01 means vendor request was internally issued and should not be disregarded
            0,                      // Index
            ctlMem);

        // If we were unable to send the number of bytes to read, then our reads will
        // surely fail, so return with error now
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> IssueInternalVendorRequest failed 0x%x\n", status));

            // Delete all requests that have been created
            count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
            for (k = 0; k < count; ++k) {
                tempRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);
                if (tempRequest != NULL) {
                    WdfCollectionRemove(pParentRequestContext->requestCollection, tempRequest);
                    WdfObjectDelete(tempRequest);
                } else {
                    QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionGetItem returned NULL object\n"));
                }
            }

            return status;
        }
    }

    // We will now take ownership of the request.  This will require that we 
    // make sure that from this point forward we complete the IO request, fail 
    // or succeed.  We will issue all requests as asynchronous transactions.  
    // WdfRequestComplete should not be called as it is called in the callback 
    // for the requests.
    *requestPending = TRUE;

    // Now that everything has been set up properly for the request, we may now issue
    // all requests
    status = IssueParentRequest(Request);

    // Return!
    return status;
}



VOID
EvtRequestReadWriteCompletion(
    __in WDFREQUEST   Request,
    __in WDFIOTARGET  Target,
    __in PWDF_REQUEST_COMPLETION_PARAMS CompletionParams,
    __in WDFCONTEXT   Context
    )
/*++

Routine Description:

    This is the completion routine for reads/writes
    If the irp completes with success, we check if we
    need to recirculate this irp for another stage of
    transfer.

Arguments:

    Context - Driver supplied context
    Device - Device handle
    Request - Request handle
    Params - request completion params

Return Value:
    None

--*/
{
    WDFDEVICE           device;
    PDEVICE_CONTEXT     pDeviceContext;
    PREQUEST_CONTEXT    pRequestContext;
    PREQUEST_CONTEXT    pParentRequestContext;
    NTSTATUS            status;
    size_t              bytes;
    PWDF_USB_REQUEST_COMPLETION_PARAMS usbCompletionParams;
    WDF_REQUEST_REUSE_PARAMS  params;

    WDFREQUEST          parentRequest;
    WDFREQUEST          splitRequest;
    WDFREQUEST          tempRequest;
    NTSTATUS            parentStatus;
    size_t              parentBytes;
    size_t              splitK;
    ULONG               count;
    ULONG               k;

    UNREFERENCED_PARAMETER(Context);

    QusbDbgPrint(DEBUG_INFO, ("--> EvtRequestReadWriteCompletion\n"));

    //
    // initialize variables
    //
    device = WdfIoTargetGetDevice(Target);
    pDeviceContext = GetDeviceContext(device);
    pRequestContext = GetRequestContext(Request);
    usbCompletionParams = CompletionParams->Parameters.Usb.Completion;
    bytes = usbCompletionParams->Parameters.PipeRead.Length;
    status = CompletionParams->IoStatus.Status;

    // Get the parent request and context
    parentRequest = pRequestContext->parentRequest;
    pParentRequestContext = GetRequestContext(parentRequest);

    // Update number of bytes transferred for the parent IO request
    pParentRequestContext->bytes += bytes;

    // If the split request failed, keep track of it in the parent request context
    if (!NT_SUCCESS(status)) {
        // Only update the status if it does not already indicate an error
        if (NT_SUCCESS(pParentRequestContext->status)) {
            pParentRequestContext->status = status;
        }
    }

    // Debug messages
    if (NT_SUCCESS(status)) {
        QusbDbgPrint(DEBUG_INFO, ("--> IO Request succeeded (%ld of %ld bytes)\n", (ULONG)bytes, (ULONG)pRequestContext->splitRequestBytes));
    } else {
        QusbDbgPrint(DEBUG_ERROR, ("--> IO Request failed - request status 0x%x UsbdStatus 0x%x\n", status, usbCompletionParams->UsbdStatus));
    }

    // Grab the result values of the request
    parentStatus = pParentRequestContext->status;
    parentBytes = pParentRequestContext->bytes;

    // Acquire the collection SpinLock
    WdfSpinLockAcquire(pParentRequestContext->collectionLock);

    // Remove the request handle from the request collection
    count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
    for (splitK = 0; splitK < count; ++splitK) {
        if (WdfCollectionGetItem(pParentRequestContext->requestCollection, splitK) == Request) {
            WdfCollectionRemoveItem(pParentRequestContext->requestCollection, splitK);
            break;
        }
    }

    // Release the collection SpinLock
    WdfSpinLockRelease(pParentRequestContext->collectionLock);

    // Delete the split request object
    WdfObjectDelete(Request);
    
    // Make sure the request is deleted by indicating we will reuse it
    WDF_REQUEST_REUSE_PARAMS_INIT(
        &params,
        WDF_REQUEST_REUSE_NO_FLAGS,
        STATUS_SUCCESS
        );

    status = WdfRequestReuse(Request, &params);
    if(!NT_SUCCESS(status)){
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestReuse failed 0x%x\n", status));
    }

    // If we have finished all split requests, then complete the main request
    if (++pParentRequestContext->splitRequestCount == pParentRequestContext->splitRequestTotal) {
        // We may no longer allow the request to be cancelled
        status = WdfRequestUnmarkCancelable(parentRequest);
        if (!NT_SUCCESS(status)){
            // We won't be able to unmark the parent request from being cancelled if it was cancelled, but
            // that is ok as long as EvtRequestCancel doesn't try to complete the request
            if (status != STATUS_CANCELLED) {
                QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestUnmarkCancelable failed 0x%x\n", status));
            }
        }

        if (!NT_SUCCESS(parentStatus)) {
            // Only perform the abort and reset if the request failed for some reason other than
            // being cancelled, and if the abort and reset haven't been done yet.  This way
            // each split request doesn't issue an abort and reset, only the first
            // one that fails does
            if ((parentStatus != STATUS_CANCELLED) && !pParentRequestContext->PipeReset) {
                //
                // Queue a workitem to reset the pipe because the completion could be
                // running at DISPATCH_LEVEL.
                //
                QusbDbgPrint(DEBUG_INFO, ("--> Issuing a pipe reset (via a passive level callback)\n"));
                if (!NT_SUCCESS(QueuePassiveLevelCallback(
                    WdfIoTargetGetDevice(Target), 
                    pRequestContext->Read ? pDeviceContext->BulkReadPipe : pDeviceContext->BulkWritePipe,
                    WorkItemResetPipe))) {

                    QusbDbgPrint(DEBUG_ERROR, ("--> QueuePassiveLevelCallback failed 0x%x\n", status));
                }

                pParentRequestContext->PipeReset = TRUE;
            }
        }

        // Complete the parent request
        QUSB_REQUEST_COMPLETE_WITH_INFO(pDeviceContext, parentRequest, parentStatus, parentBytes);
    }
}



VOID EvtRequestCancel(
  __in  WDFREQUEST Request
)
{
    PREQUEST_CONTEXT    pParentRequestContext;
    PREQUEST_CONTEXT    pSplitRequestContext;
    WDFREQUEST          parentRequest;
    WDFREQUEST          splitRequest;
    ULONG               splitK, k;
    ULONG               count;
    WDFCOLLECTION       cancelCollection;
    NTSTATUS            status;
    WDF_REQUEST_REUSE_PARAMS  params;

    QusbDbgPrint(DEBUG_INFO, ("EvtRequestCancel\n"));

    // The Request passed in could be a parent request or a split request
    parentRequest = Request;
    pParentRequestContext = GetRequestContext(parentRequest);
    if (pParentRequestContext->parentRequest != NULL) {
        parentRequest = pParentRequestContext->parentRequest;
        pParentRequestContext = GetRequestContext(parentRequest);
    }

    // Create a collection for the split requests we will cancel
    status = WdfCollectionCreate(WDF_NO_OBJECT_ATTRIBUTES, &cancelCollection);
    if(!NT_SUCCESS(status)){
        QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionCreate failed 0x%x\n", status));
        return;
    }

    // Acquire the collection SpinLock
    WdfSpinLockAcquire(pParentRequestContext->collectionLock);

    // Loop over each split request and increase the reference count
    count = WdfCollectionGetCount(pParentRequestContext->requestCollection);
    for (splitK = 0; splitK < count; ++splitK) {
        // Get the request
        splitRequest = WdfCollectionGetItem(pParentRequestContext->requestCollection, 0);

        // Add the request to the cancel collection
        status = WdfCollectionAdd(cancelCollection, splitRequest);
        if (!NT_SUCCESS(status)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfCollectionAdd failed 0x%x\n", status));

            // On failure, just quit.  Any requests that were added to the cancel collection
            // will still be canceled, and if nothing was added everything will clean up
            // nicely.  
        }

        // Remove the request from the request collection
        WdfCollectionRemove(pParentRequestContext->requestCollection, splitRequest);

        // Increase the reference count on the request object so it will not be deleted
        // before we have a chance to cancel it
        WdfObjectReference(splitRequest);
    }

    // Release the collection SpinLock
    WdfSpinLockRelease(pParentRequestContext->collectionLock);

    // Loop over each split request cancel it
    count = WdfCollectionGetCount(cancelCollection);
    for (splitK = 0; splitK < count; ++splitK) {
        // Get the request
        splitRequest = WdfCollectionGetItem(cancelCollection, 0);

        // Remove the request from the cancel collection
        WdfCollectionRemove(cancelCollection, splitRequest);

        // Cancel the request
        if (!WdfRequestCancelSentRequest(splitRequest)) {
            QusbDbgPrint(DEBUG_ERROR, ("--> WdfRequestCancelSentRequest failed to cancel request\n"));
        }

        // Decrement the reference count on the request to allow it to be deleted
        WdfObjectDereference(splitRequest);
    }

    // Delete the cancel collection object
    WdfObjectDelete(cancelCollection);
}
