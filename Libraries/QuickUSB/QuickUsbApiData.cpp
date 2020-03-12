/*=============================================================================
 Title        : QuickUsbApiData.cpp
 Description  : QuickUSB QUSB2 Module Data API
 Notes        : None
 History      :

 Copyright (c) 2012 Bitwise Systems.  All rights reserved.
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
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/
#include "QuickUsbUtil.h"



/*---------------------------------------------------------------------------
 Purpose :  Write a block of command values to the high-speed parallel port
                using the QuickUSB module.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Starting value of the HSPP address bus
            data - Data to write to the high-speed parallel port
            length - Length to write to the high-speed parallel port
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  The data buffer can receive data values of any type.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteCommand(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length < 1) || (length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    
#if DRIVER==MacOSXDriver
#ifdef IMPLEMENT_ASYNC
    QULONG k;
    
    // The R/W Command API shares use of the GPIF with all of the R/W data commands.  If
    // there are any outstanding asynchronous data requests we must wait for them to
    // complete before trying to issue command R/W requests.  On Windows and Linux this
    // is handled in the driver.
    for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
        // Check if the stream is active
        if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
            // If the stream is running
            if (!(QusbDeviceInfo[(QULONG) hDevice].stream[k]->info & (QUSB_INFO_STREAMING_PAUSED | QUSB_INFO_STREAMING_STOP))) {
                // Pause the stream
                if (!QuickUsbPauseStream(hDevice, (QBYTE)k, FALSE)) { // Sets lastError on failure
                    return(FALSE);
                }
                
                // Keep track of the fact that we internally paused this stream
                QusbDeviceInfo[(QULONG) hDevice].stream[k]->info |= QUSB_INFO_STREAMING_IPAUSED;
            }
        }
    }
    
    // Now all async requests are on hold so we may issue the command request
#endif
#endif
    
    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_COMMAND, address, length, data, length, &bytesReturned)) {
        return(FALSE);
    }
    
#if DRIVER==MacOSXDriver
#ifdef IMPLEMENT_ASYNC
    for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
        // Check if the stream is active
        if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
            // If the stream was internally paused by us we must now resume it
            if (QusbDeviceInfo[(QULONG) hDevice].stream[k]->info & QUSB_INFO_STREAMING_IPAUSED) {
                // Clear our flag
                QusbDeviceInfo[(QULONG) hDevice].stream[k]->info &= ~(QUSB_INFO_STREAMING_IPAUSED);
                
                // Resume the stream
                if (!QuickUsbResumeStream(hDevice, (QBYTE)k)) { // Sets lastError on failure
                    return(FALSE);
                }
            }
        }
    }
    
    // Now all async requests may continue to be serviced
#endif
#endif
    
    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Read a block of command values from the high-speed parallel port
            using the QuickUSB module.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Starting value of the HSPP address bus
            data - Buffer for data read from the high-speed parallel port
            length - Length to read from the high-speed parallel port
 Output  :  length - Length read from the high-speed parallel port
            Returns non-zero on success, zero on failure.
 Notes   :  The data buffer can contain data values of any type.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadCommand(QHANDLE hDevice, QWORD address, PQBYTE data, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
#if DRIVER==MacOSXDriver
    QULONG k;
#endif
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    
#if DRIVER==MacOSXDriver
#ifdef IMPLEMENT_ASYNC
    // The R/W Command API shares use of the GPIF with all of the R/W data commands.  If
    // there are any outstanding asynchronous data requests we must wait for them to
    // complete before trying to issue command R/W requests.  On Windows and Linux this
    // is handled in the driver.
    for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
        // Check if the stream is active
        if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
            // If the stream is running
            if (!(QusbDeviceInfo[(QULONG) hDevice].stream[k]->info & (QUSB_INFO_STREAMING_PAUSED | QUSB_INFO_STREAMING_STOP))) {
                // Pause the stream
                if (!QuickUsbPauseStream(hDevice, (QBYTE)k, FALSE)) { // Sets lastError on failure
                    return(FALSE);
                }
                
                // Keep track of the fact that we internally paused this stream
                QusbDeviceInfo[(QULONG) hDevice].stream[k]->info |= QUSB_INFO_STREAMING_IPAUSED;
            }
        }
    }
    
    // Now all async requests are on hold so we may issue the command request
#endif
#endif

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_COMMAND, address, *length, data, *length, &bytesReturned)) {
        return(FALSE);
    }

    // Use the length parameter to indicate the actual number of bytes read
    *length = (QWORD)bytesReturned;
    
#if DRIVER==MacOSXDriver
#ifdef IMPLEMENT_ASYNC
    for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
        // Check if the stream is active
        if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
            // If the stream was internally paused by us we must now resume it
            if (QusbDeviceInfo[(QULONG) hDevice].stream[k]->info & QUSB_INFO_STREAMING_IPAUSED) {
                // Clear our flag
                QusbDeviceInfo[(QULONG) hDevice].stream[k]->info &= ~(QUSB_INFO_STREAMING_IPAUSED);
                
                // Resume the stream
                if (!QuickUsbResumeStream(hDevice, (QBYTE)k)) { // Sets lastError on failure
                    return(FALSE);
                }
            }
        }
    }
    
    // Now all async requests may continue to be serviced
#endif
#endif
    
    
    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Write a block of data values to the high-speed parallel port using
                the QuickuUSB module.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            data - Buffer of data values to write to the high-speed parallel
                port
            length - Length to write to the high-speed parallel port
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  The maximum length is 16 MB (16777216 bytes).
            The data buffer can contain data values of any type.
            Transfers of more than 262144 (256K) Bytes handled in driver if
                later than version 1.0.0.
            Certain transaction lengths are invalid and will timeout.  To
                determine if a given length is valid, use the following
                pseudo code:
                if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                    QULONG packetSize = (highspeed) ? 512 : 64;
                    QULONG preRead = (wordwide) ? 4 : 2;
                    QULONG mod = (length % packetSize);
                    QULONG even = (length % 2);
                    QBOOL validLength = (mod == 0) || ((mod >= preRead)
                        && !(wordwide && !even));
                }
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteData(QHANDLE hDevice, PQBYTE data, QULONG length) {
    QDEVHANDLE hDevHandle;
    QULONG totalBytesWritten;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length < 1) || (length > QUSB_MAX_BULK_TRANSFER_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    DWORD bytesTransferred;

    // Initialize the overlapped structure
    OVERLAPPED ovl;
    memset(&ovl, 0, sizeof(OVERLAPPED));
    ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);

    // Issue the write request
    result = WriteFile(hDevHandle,
        data, 
        length,
        NULL,
        &ovl);

    // Check if the write completed synchronously, asynchronously, or failed
    if (!result) {
        lastDriverError = GetLastError();
        if (lastDriverError == ERROR_IO_PENDING) {
            lastDriverError = 0;

            // Request was asynchronously issued
            if (!GetOverlappedResult(hDevHandle, &ovl, &bytesTransferred, TRUE)) {
                lastDriverError = GetLastError();
                lastError = TranslateSystemError(lastDriverError);
                CloseHandle(ovl.hEvent);
                return(FALSE);
            }
        } else {
            // Request was not issued (failure)
            lastError = TranslateSystemError(lastDriverError);
            CloseHandle(ovl.hEvent);
            return(FALSE);
        }
    } else {
        // Request was synchronously completed
    }

    // Close the event handle
    CloseHandle(ovl.hEvent);

    totalBytesWritten = (QULONG) bytesTransferred;
    }
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    {
    ssize_t result;

    result = write(hDevHandle, data, (size_t) length);

    if (result < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }

    totalBytesWritten = (QULONG) result;
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    QLONG bytesWritten;
    QBOOL result;
    kern_return_t err;
    unsigned short regVal;
    PQBYTE block;
    QULONG blockSize;
    
    UInt8 outPipe;
    QWORD timeout;
    
    result = TRUE;
    regVal = 0;
    bytesWritten = 0;
    outPipe = 0;
    block = data;
    
    // Get out pipe and timeout
    outPipe = QusbDeviceInfo[hDevice].outPipe;
    timeout = QusbDeviceInfo[hDevice].timeout;
    
    // Send the data one buffer at a time until sent
    while (result != FALSE) {
        // Calculate the current block size
        blockSize = length - (block - data);
        if (blockSize > QUSB_MAX_BULK_BLOCK_SIZE) {
            blockSize = QUSB_MAX_BULK_BLOCK_SIZE;
        }
        if (blockSize == 0) {
            break;
        }
        
        // Send block
        err = (*hDevHandle)->WritePipeTO(hDevHandle,    // Interface
                outPipe,                                // Pipe
                block,                                  // Data
                blockSize,                              // Length
                timeout,                                // No data timeout
                timeout);                               // Transfer timeout
        if (err == kIOReturnSuccess) {
            block += blockSize;
        }
        
        // If there was an error set the last error code
        else {
            if (err == kIOUSBTransactionTimeout) {
                lastError = QUICKUSB_ERROR_TIMEOUT;
                QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorTimeout;
                
                // Clear pipe
                // Note: The pipe MUST be cleared if a timeout occurs or
                //       else no further transfers can be processed.
                err = (*hDevHandle)->ClearPipeStallBothEnds(hDevHandle, outPipe);
                if (err) {
                    lastError = QUICKUSB_ERROR_IOCTL_FAILED;
                    QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorPipe;
                }
            }
            else {
                lastError = QUICKUSB_ERROR_IOCTL_FAILED;
                QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorBulk;
            }
            result = FALSE;
        }
    }
    
    totalBytesWritten = (QULONG) length;
    }
#endif // DRIVER==MacOSXDriver

    // Keep track of statistics
    QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten += totalBytesWritten;
    QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten += totalBytesWritten;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteDataEx(QHANDLE hDevice, PQBYTE data, PQULONG length, QULONG flags) {
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QUSB_MAX_BULK_TRANSFER_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    // Check if the request should be issued out-of-order
    if (flags & QUICKUSB_OUT_OF_ORDER) {
        BOOL winResult;
        DWORD winError;
        QULONG bytesTransferred;

        // Issuing the request by calling DeviceIoControl instead of WriteFile will place the request in the
        // default driver queue instead of the GPIF queue.  This will then cause the request to be dispatched
        // without regard for any other requests in the GPIF queue, thus causing the request to be fulfilled
        // out-of-order.  This may be useful to do in some applications where, for example, you are performing
        // streaming reads and need to periodically perform synchronous writes that are processed without 
        // having to first wait for all pending reads to complete.
        winResult = DeviceIoControl(hDevHandle, IOCTL_QUSB_BULK_WRITE, data, *length, data, *length, &bytesTransferred, NULL);
        *length = bytesTransferred;

        // Check for errors
        if (!winResult) {
            winError = GetLastError();
            lastError = TranslateSystemError(winError);
            return(FALSE);
        }

        // Keep track of statistics
        QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten += *length;
        QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten += *length;

        // Return successfully
        return(TRUE);
    }

    // Perform the normal bulk write
    return QuickUsbWriteData(hDevice, data, *length);
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    // Perform the normal bulk write
    return QuickUsbWriteData(hDevice, data, *length);
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    // Perform the normal bulk write
    return QuickUsbWriteData(hDevice, data, *length);
#endif // DRIVER==MacOSXDriver
}



QRETURN QuickUsbReadData(QHANDLE hDevice, PQBYTE data, PQULONG length) {
/*---------------------------------------------------------------------------
 Purpose :  Read a block of data values from the high-speed parallel port
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            data - Buffer to store data values read
            length - Amount of data to read (in bytes)
 Output  :  length - Amount of data actually read (in bytes)
 Notes   :  The maximum length is 16 MB (16777216 bytes).
            The data buffer can receive data values of any type.
            The data buffer length must be a multiple of the packet size (in 
                some cases).
            The module needs to send a request of how much to read before the
                bulk read (Starting with v2.15.0 this is automatically 
                handled in the driver).
            Large transfers, for example those greater than 256KB in Windows,
                must be broken into many smaller transfers.  These
                requirements are handled in the driver.
            Certain transaction lengths are invalid and will timeout.  To
                determine if a given length is valid, use the following
                pseudo code:
                if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                    QULONG packetSize = (highspeed) ? 512 : 64;
                    QULONG preRead = (wordwide) ? 4 : 2;
                    QULONG mod = (length % packetSize);
                    QULONG even = (length % 2);
                    QBOOL validLength = (mod == 0) || ((mod >= preRead) 
                        && !(wordwide && !even));
                }
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;

    //QuickUsbWriteLog(0, "QuickUsbReadData()");

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QUSB_MAX_BULK_TRANSFER_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    DWORD bytesTransferred;
    OVERLAPPED ovl;

    // Initialize the overlapped structure
    ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    ovl.Offset = 0;
    ovl.OffsetHigh = 0;

    // Issue the read request
    result = ReadFile(hDevHandle,
        data, 
        *length,
        &bytesTransferred,
        &ovl);

    // Check if the read completed synchronously, asynchronously, or failed
    if (!result) {
        lastDriverError = GetLastError();
        if (lastDriverError == ERROR_IO_PENDING) {
            lastDriverError = 0;

            // Request was asynchronously issued
            if (!GetOverlappedResult(hDevHandle, &ovl, &bytesTransferred, TRUE)) {
                lastDriverError = GetLastError();
                lastError = TranslateSystemError(lastDriverError);
                CloseHandle(ovl.hEvent);
                return(FALSE);
            }
        } else {
            // Request was not issued (failure)
            lastError = TranslateSystemError(lastDriverError);
            CloseHandle(ovl.hEvent);
            return(FALSE);
        }
    } else {
        // Request was synchronously completed
    }

    // Close the event handle
    CloseHandle(ovl.hEvent);

    // Return the number of bytes received
    *length = bytesTransferred;
    }
#endif

#if DRIVER==LinuxDriver
    {
    ssize_t result;

    // Check for a valid length
    if ((*length < 1) || (*length > QUSB_MAX_BULK_TRANSFER_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    result = (QULONG) read(hDevHandle, data, (size_t) *length);

    if (result < 0) {
        lastError = TranslateSystemError(errno);
        *length = 0;
        return(FALSE);
    }

    *length = (QULONG) result;
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    IOUSBDevRequest request;
    kern_return_t err;
    
    QULONG bytesReceived;
    UInt32 blockSize, blockMultiplier;
    UCHAR inPipe;
    QWORD maxPacketSize;
    QWORD timeout;
    QBOOL result;
    QWORD regVal;
    PQBYTE block;
    
    result = TRUE;

    bytesReceived = 0;
    blockSize = 0;
    blockMultiplier = 0;
    inPipe = 0;
    maxPacketSize = 0;
    block = data;
    
    regVal = 0;
    
    // Get in pipe and timeout
    inPipe = QusbDeviceInfo[hDevice].inPipe;
    timeout = QusbDeviceInfo[hDevice].timeout;
    
    // Build the vendor request
    request.bmRequestType = USBmakebmRequestType(kUSBOut, kUSBVendor, kUSBDevice);
    request.bRequest = QUICKUSB_READDATALEN;
    request.wIndex = 0;
    request.wValue = 0;
    request.wLength = sizeof(UInt32);
    request.pData = &blockSize;
    
    // Send the data one buffer at a time until sent
    while (result != FALSE) {
        // Calculate the current block size
        blockSize = *length - (block - data);
        if (blockSize > QUSB_MAX_BULK_BLOCK_SIZE) {
            blockSize = QUSB_MAX_BULK_BLOCK_SIZE;
        }
        if (blockSize <= 0) {
            break;
        }
        
        // Swap bytes
        // Note: The block size MUST be swapped to little endian before the
        //       control request. If block size is not swapped, the number
        //       of bytes requested to be read will be unexpected. As a
        //       result, the module could be stalled on a read and no writes
        //       are possible until the requested amount is actually read.
        blockSize = CFSwapInt32HostToLittle(blockSize);
        
        // Send control request
        err = (*hDevHandle)->ControlRequest(hDevHandle,
                0,
                &request);
        if (err != kIOReturnSuccess) {
            lastError = QUICKUSB_ERROR_IOCTL_FAILED;
            QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorURB;
            result = FALSE;
            break;
        }
        
        // Swap bytes
        // Note: Since the block size is currently in little endian, the
        //       value MUST be swapped to the host's endianness.
        blockSize = CFSwapInt32LittleToHost(blockSize);
        
        // Send block
        err = (*hDevHandle)->ReadPipeTO(hDevHandle, // Interface
                inPipe,                             // Pipe
                block,                              // Data
                &blockSize,                         // Length
                timeout,                            // No data timeout
                timeout);                           // Transfer timeout
        if (err == kIOReturnSuccess) {
            // Increment
            block += blockSize;
            bytesReceived += blockSize;
        }
        
        // If there was an error set the last error code
        else {
            if (err == kIOUSBTransactionTimeout) {
                lastError = QUICKUSB_ERROR_TIMEOUT;
                QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorTimeout;
                
                // Clear pipe
                // Note: The pipe MUST be cleared if a timeout occurs or
                //       else no further transfers can be processed.
                err = (*hDevHandle)->ClearPipeStallBothEnds(hDevHandle, inPipe);
                if (err) {
                    lastError = QUICKUSB_ERROR_IOCTL_FAILED;
                    QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorPipe;
                }
            }
            else {
                lastError = QUICKUSB_ERROR_IOCTL_FAILED;
                QusbDeviceInfo[hDevice].devError.DriverError = DriverErrorBulk;
            }
            result = FALSE;
        }
    }
    
    // Set length to the actual number of bytes received
    *length = bytesReceived;
    }
#endif // DRIVER==MacOSXDriver
    
    // Keep track of statistics
    QusbDeviceInfo[(QULONG) hDevice].totalBytesRead += *length;
    QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead += *length;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadDataEx(QHANDLE hDevice, PQBYTE data, PQULONG length, QULONG flags) {
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QUSB_MAX_BULK_TRANSFER_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    // Check if the request should be issued out-of-order
    if (flags & QUICKUSB_OUT_OF_ORDER) {
        BOOL winResult;
        DWORD winError;
        QULONG bytesTransferred;

        // Issuing the request by calling DeviceIoControl instead of WriteFile will place the request in the
        // default driver queue instead of the GPIF queue.  This will then cause the request to be dispatched
        // without regard for any other requests in the GPIF queue, thus causing the request to be fulfilled
        // out-of-order.  This may be useful to do in some applications where, for example, you are performing
        // streaming reads and need to periodically perform synchronous writes that are processed without 
        // having to first wait for all pending reads to complete.
        winResult = DeviceIoControl(hDevHandle, IOCTL_QUSB_BULK_READ, data, *length, data, *length, &bytesTransferred, NULL);
        *length = bytesTransferred;

        // Check for errors
        if (!winResult) {
            winError = GetLastError();
            lastError = TranslateSystemError(winError);
            return(FALSE);
        }

        // Keep track of statistics
        QusbDeviceInfo[(QULONG) hDevice].totalBytesRead += *length;
        QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead += *length;

        // Return successfully
        return(TRUE);
    }

    // Perform the normal bulk read
    return QuickUsbReadData(hDevice, data, length);
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    // Perform the normal bulk read
    return QuickUsbReadData(hDevice, data, length);
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    // Perform the normal bulk read
    return QuickUsbReadData(hDevice, data, length);
#endif // DRIVER==MacOSXDriver
}



/*---------------------------------------------------------------------------
 Purpose :  Read a block of data values from the high-speed parallel port using
                an asynchronous function call. To send a group of partial IRPs
                describing 256K transfers to the USBD stack and return control
                to the application software.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            length - Length to read from high-speed parallel port
 Output  :  data - Buffer containing data read from high-speed parallel port
            length - Length read from high-speed parallel port
            transaction - Transaction identifier
            Returns non-zero on success, zero on failure.
 Notes   :  This function is currently not supported for MacOSX.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadDataAsync(QHANDLE hDevice, PQBYTE data, PQULONG length, PQBYTE transaction) {

#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if DRIVER==WindowsDriver
    QDEVHANDLE hDevHandle;
    QULONG k;
    PQIOSTREAM stream;
    QRESULT qResult;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QUSB_MAX_BULK_TRANSFER_SIZE) || (transaction == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // If the stream has not been created yet we must create it.  It will be freed in the QuickUsbClose function.
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] == NULL) {
        // Allocate the stream
        QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] = (PQIOSTREAM) malloc(sizeof(QIOSTREAM));
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM];

        // Initialize the stream.  Note that for legacy async support we need to pre-allocate 256
        // QBULKSTREAM sturctures and be single-threaded.
        qResult = QuickUsbInitializeStream(QUICKUSB_LEGACY_ASYNC_STREAM, stream, hDevice, 256, 0, 0);
        if (!qResult) {
            free(stream);
            stream = NULL;
            QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] = NULL;
            return(FALSE);
        }

        // Fill out additional stream info
        stream->StreamType = QUSB_STREAMTYPE_BULK; // IN or OUT
        stream->info = QUSB_INFO_ASYNC;
    } else {
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM];

        // We must make sure not to issue this request if the stream is stopped.  The
        // stream is stopped when QuickUsbClose is called but the users app could have 
        // another thread that is trying to issue a request and doing so can prevent
        // QuickUsbClose from closing successfully.
        if (stream->info & QUSB_INFO_STREAMING_STOP) {
            lastError = QUICKUSB_ERROR_DEVICE_IS_CLOSING;
            return(FALSE);
        }
    }

    // Search for a free ioRequest
    for (k = 0; k < 256; ++k) {
        if (((PQBULKSTREAM)(stream->ioRequest))[k].Buffer == NULL) {
            // Fill out BulkStream structure
            ((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = data;
            ((PQBULKSTREAM)(stream->ioRequest))[k].BytesRequested = *length;
            ((PQBULKSTREAM)(stream->ioRequest))[k].StreamID = QUICKUSB_LEGACY_ASYNC_STREAM;
            ((PQBULKSTREAM)(stream->ioRequest))[k].StreamType = QUSB_STREAMTYPE_BULK_IN_EP6;

            // Issue the read request
            if (!QuickUsbIssueStreamBulkRequest(QUICKUSB_LEGACY_ASYNC_STREAM, stream, &((PQBULKSTREAM)(stream->ioRequest))[k], TRUE)) { // Sets lastError on failure
                // NOTE: When QuickUsbIssueStreamBulkRequest fails to issue a request, the BulkStream 
                // entry is cleared out so that the request may be reused.

                // Return with error
                return(FALSE);
            }

            // Return the transaction ID
            *transaction = (QBYTE)k;

            // Success
            return(TRUE);
        }
    }
    
    // There are too many outstanding transactions
    lastError = QUICKUSB_ERROR_TOO_MANY_REQUESTS;
    return(FALSE);
#endif
}



/*---------------------------------------------------------------------------
 Purpose :  Write a block of data values to the high-speed parallel port using
                an asynchronous function call. To send a group of partial IRPs
                describing 256K transfers to the USBD stack and return control
                to the application software.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            data - Buffer containing data to write to high-speed parallel port
            length - Length to read from high-speed parallel port
 Output  :  transaction - Transaction identifier
            Returns non-zero on success, zero on failure.
 Notes   :  This function is currently not supported for MacOSX.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteDataAsync(QHANDLE hDevice, PQBYTE data, QULONG length, PQBYTE transaction) {

#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if DRIVER==WindowsDriver
    QDEVHANDLE hDevHandle;
    QULONG k;
    PQIOSTREAM stream;
    QRESULT qResult;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (length < 1) || (length > QUSB_MAX_BULK_TRANSFER_SIZE) || (transaction == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // If the stream has not been created yet we must create it.  It will be freed in the QuickUsbClose function.
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] == NULL) {
        // Allocate the stream
        QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] = (PQIOSTREAM) malloc(sizeof(QIOSTREAM));
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM];

        // Initialize the stream.  Note that for legacy async support we need to pre-allocate 256
        // QBULKSTREAM sturctures and be single-threaded.
        qResult = QuickUsbInitializeStream(QUICKUSB_LEGACY_ASYNC_STREAM, stream, hDevice, 256, 0, 0);
        if (!qResult) {
            free(stream);
            stream = NULL;
            QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM] = NULL;
            return(FALSE);
        }

        // Fill out additional stream info
        stream->StreamType = QUSB_STREAMTYPE_BULK; // IN or OUT
        stream->info = QUSB_INFO_ASYNC;
    } else {
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM];

        // We must make sure not to issue this request if the stream is stopped.  The
        // stream is stopped when QuickUsbClose is called but the users app could have 
        // another thread that is trying to issue a request and doing so can prevent
        // QuickUsbClose from closing successfully.
        if (stream->info & QUSB_INFO_STREAMING_STOP) {
            lastError = QUICKUSB_ERROR_DEVICE_IS_CLOSING;
            return(FALSE);
        }
    }

    // Search for a free ioRequest
    for (k = 0; k < 256; ++k) {
        if (((PQBULKSTREAM)(stream->ioRequest))[k].Buffer == NULL) {
            // Fill out BulkStream structure
            ((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = data;
            ((PQBULKSTREAM)(stream->ioRequest))[k].BytesRequested = length;
            ((PQBULKSTREAM)(stream->ioRequest))[k].StreamID = QUICKUSB_LEGACY_ASYNC_STREAM;
            ((PQBULKSTREAM)(stream->ioRequest))[k].StreamType = QUSB_STREAMTYPE_BULK_OUT_EP2;

            // Issue the read request
            if (!QuickUsbIssueStreamBulkRequest(QUICKUSB_LEGACY_ASYNC_STREAM, stream, &((PQBULKSTREAM)(stream->ioRequest))[k], TRUE)) { // Sets lastError on failure
                // NOTE: When QuickUsbIssueStreamBulkRequest fails to issue a request, the BulkStream 
                // entry is cleared out so that the request may be reused.

                // Return with error
                return(FALSE);
            }

            // Return the transaction ID
            *transaction = (QBYTE)k;

            // Success
            return(TRUE);
        }
    }
    
    // There are too many outstanding transactions
    lastError = QUICKUSB_ERROR_TOO_MANY_REQUESTS;
    return(FALSE);
#endif
}



/*---------------------------------------------------------------------------
 Purpose :  Wait for an asynchronous transfer to complete.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            transaction - Transaction identifier
            immediate - Immediate value
 Output  :  bytecount - Length transferred asynchronously
            Returns non-zero on success, zero on failure.
 Notes   :  If a timeout was sensed, the timeout error code is set.
            If the function succeeds, the number of bytes transferred is set.
            This function is currently not supported for MacOSX.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbAsyncWait(QHANDLE hDevice, PQULONG bytecount, QBYTE transaction, QBYTE immediate) {

#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#endif

#if DRIVER==WindowsDriver
    QDEVHANDLE hDevHandle;
    QULONG transError;
    PQIOSTREAM stream;
    PQBULKSTREAM bulkStream;
    DWORD BytesTransferred, winError;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Verify parameters
    stream = QusbDeviceInfo[(QULONG)hDevice].stream[QUICKUSB_LEGACY_ASYNC_STREAM];
    if (stream == NULL) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    bulkStream = &(((PQBULKSTREAM)(stream->ioRequest))[transaction]);
    if (bulkStream->Buffer == NULL) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Indicate that the request is not yet complete
    *bytecount = 0;

    // Check/Wait for the transaction
    if (!GetOverlappedResult(hDevHandle, &bulkStream->InternalData, &BytesTransferred, (immediate == FALSE))) {
        winError = GetLastError();

        // Check for transactions that are still pending
        if ((winError == ERROR_IO_INCOMPLETE) || (winError == ERROR_IO_PENDING)) {
            // The proper way for this function to indicate that the request is still 
            // pending is to return successfully, but have bytecount = 0
            return(TRUE);
        }

        // Some unknown error occurred.  Free resources and exit with error.
        bulkStream->Buffer = NULL;
        bulkStream->InternalStatus = 0;
        CloseHandle(bulkStream->InternalData.hEvent);
        bulkStream->InternalData.hEvent = NULL;

        // Return with error
        lastError = TranslateSystemError(winError);
        return(FALSE);
    }

    // Note: if InternalData.Internal == STATUS_PENDING (0x103), then the request has not
    // completed and we cannot free the OVERLAPPED data stored in InternalData or the
    // program will crash.  This occurs if the InternalData.hEvent member is set to NULL
    // causing the GetOverlappedResult() call above to signal off any event on the file.
    // Even though we are using CreateEvent() in the IssueBulkDataAsync() call for legacy
    // operations, we should check it here to be absolutely sure we never crash the program.
    if (bulkStream->InternalData.Internal == STATUS_PENDING) {
        lastError = QUICKUSB_ERROR_NOT_COMPLETED;
        return(FALSE);
    }

    // Indicate that the transaction has completed
    bulkStream->InternalStatus |= QUSB_BULKSTREAM_IO_COMPLETE;

    // Store the transction error
    transError = TranslateSystemError((DWORD)(bulkStream->InternalData.Internal));

    // Store the number of bytes transferred
    *bytecount = BytesTransferred;

    // Free resources
    bulkStream->Buffer = NULL;
    CloseHandle(bulkStream->InternalData.hEvent);
    bulkStream->InternalData.hEvent = NULL;

    // We must protect access to shared information across threads
    QUSB_LOCK(stream); {
        // Move on to the next request to process
        ++stream->NextRequestIDToProcess;

        // Keep track of outstanding requests
        --(stream->pendingRequests);

        // Keep track of statistics
        if (stream->StreamType & QUSB_STREAMTYPE_IN) {
            QusbDeviceInfo[(QULONG) hDevice].totalBytesRead += *bytecount;
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead += *bytecount;
        } else {
            QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten += *bytecount;
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten += *bytecount;
        }

        // Exit the code protected across threads in the process
    } QUSB_UNLOCK(stream);

    // Check for transaction errors
    if (transError != QUICKUSB_ERROR_NO_ERROR) {
        lastError = transError;
        return(FALSE);
    }

    // Success!
    return(TRUE);
#endif
}



QRETURN QuickUsbReadBulkDataAsync(QHANDLE hDevice, PQBYTE buffer, QULONG bytes, PQBULKSTREAM bulkStream, 
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag) {
/*---------------------------------------------------------------------------
 Purpose : Issue an asynchronous data read.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures.
           buffer - The data buffer.  This buffer must remain valid until
             the request completes.
           bytes - The number of bytes to read.
           bulkStream - A pointer to a user allocated QBULKSTREAM used to
             store all the information about the request.  This data must
             remain valid until the request completes.
           cRoutine - (Optional) The completion routine to call when the
             request completes.  This routine executes from another thread
             and multiple threads may execute this routine at the same time.
           tag - (Optional) A user pointer passed on to the completion
             routine used to associate user information with the request.
 Output  : buffer - Asynchronously written to as data is received.
           bulkStream - Updated with information about the request.
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : Certain transaction lengths are invalid and will timeout.  To
             determine if a given length is valid, use the following
             pseudo code:
             if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                 QULONG packetSize = (highspeed) ? 512 : 64;
                 QULONG preRead = (wordwide) ? 4 : 2;
                 QULONG mod = (length % packetSize);
                 QULONG even = (length % 2);
                 QBOOL validLength = (mod == 0) || ((mod >= preRead) 
                     && !(wordwide && !even));
             }
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else

    QDEVHANDLE hDevHandle;
    PQIOSTREAM stream;
    QRESULT qResult;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((bytes < 1) || (bytes > QUSB_MAX_BULK_TRANSFER_SIZE) || (buffer == 0) || (bulkStream == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==LinuxDriver
    // On Linux, data buffers for asynchronous requests must be 512-byte aligned
    if (((long unsigned int)buffer & (512 - 1)) != 0) {
        lastError = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED;
        return(FALSE);
    }
#endif

    // If the stream has not been created yet we must create it.  It will be freed in the QuickUsbClose function.
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] == NULL) {
        // Allocate the stream
        QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = (PQIOSTREAM) malloc(sizeof(QIOSTREAM));
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM];

        // Initialize the stream.  Note that for async support we do not pre-allocate request objects.
        qResult = QuickUsbInitializeStream(QUICKUSB_ASYNC_STREAM, stream, hDevice, 0, 0, 0);
        if (!qResult) {
            free(stream);
            stream = NULL;
            QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = NULL;
            return(FALSE);
        }

        // Fill out additional stream info
        stream->StreamType = QUSB_STREAMTYPE_BULK; // IN or OUT
        stream->info = QUSB_INFO_ASYNC;
        stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
        stream->cRoutine = (PQVOID)cRoutine;
        stream->tag = tag;
    } else {
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM];

        // We must make sure not to issue this request if the stream is stopped.  The
        // stream is stopped when QuickUsbClose is called but the users app could have 
        // another thread that is trying to issue a request and doing so can prevent
        // QuickUsbClose from closing successfully.
        if (stream->info & QUSB_INFO_STREAMING_STOP) {
            lastError = QUICKUSB_ERROR_DEVICE_IS_CLOSING;
            return(FALSE);
        }

        // Always update the completion info contained in the stream so that it
        // is copied to the BulkStream objects in QuickUsbIssueStreamBulkRequest
        stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
        stream->cRoutine = (PQVOID)cRoutine;
        stream->tag = tag;
    }

    // Fill out BulkStream structure
    bulkStream->Buffer = buffer;
    bulkStream->BytesRequested = bytes;
    bulkStream->StreamID = QUICKUSB_ASYNC_STREAM;
    bulkStream->RequestID = stream->NextRequestID;
    bulkStream->StreamType = QUSB_STREAMTYPE_BULK_IN_EP6;

    // Issue the read request
    if (!QuickUsbIssueStreamBulkRequest(QUICKUSB_ASYNC_STREAM, stream, bulkStream, TRUE)) { // Sets lastError on failure
        // NOTE: When QuickUsbIssueStreamBulkRequest fails to issue a request, the BulkStream 
        // entry is cleared out so that the request may be reused.

        // Return with error
        return(FALSE);
    }
    
#if DRIVER==MacOSXDriver
    // Dispatch queued requests 
    DispatchRequests(&QusbDeviceInfo[(QULONG) hDevice]);
#endif
    
    // Success
    return(TRUE);
#endif
}



QRETURN QuickUsbWriteBulkDataAsync(QHANDLE hDevice, PQBYTE buffer, QULONG bytes, 
    PQBULKSTREAM bulkStream, PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag) {
/*---------------------------------------------------------------------------
 Purpose : Issue an asynchronous data write.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures.
           buffer - The data buffer.  This buffer must remain valid until
             the request completes.
           bytes - The number of bytes to write.
           bulkStream - A pointer to a user allocated QBULKSTREAM used to
             store all the information about the request.  This data must
             remain valid until the request completes.
           cRoutine - (Optional) The completion routine to call when the
             request completes.
           tag - (Optional) A user pointer passed on to the completion
             rountine used to associate user information with the request.
 Output  : buffer - Asynchronously read from as data is written.
           bulkStream - Updated with information about the request.
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : Certain transaction lengths are invalid and will timeout.  To
             determine if a given length is valid, use the following
             pseudo code:
             if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                 QULONG packetSize = (highspeed) ? 512 : 64;
                 QULONG preRead = (wordwide) ? 4 : 2;
                 QULONG mod = (length % packetSize);
                 QULONG even = (length % 2);
                 QBOOL validLength = (mod == 0) || ((mod >= preRead) 
                     && !(wordwide && !even));
             }
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    PQIOSTREAM stream;
    QRESULT qResult;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((bytes < 1) || (bytes > QUSB_MAX_BULK_TRANSFER_SIZE) || (buffer == 0) || (bulkStream == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==LinuxDriver
    // On Linux, data buffers for asynchronous requests must be 512-byte aligned
    if (((long unsigned int)buffer & (512 - 1)) != 0) {
        lastError = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED;
        return(FALSE);
    }
#endif

    // If the stream has not been created yet we must create it.  It will be freed in the QuickUsbClose function.
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] == NULL) {
        // Allocate the stream
        QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = (PQIOSTREAM) malloc(sizeof(QIOSTREAM));
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM];

        // Initialize the stream.  Note that for async support we do not pre-allocate request objects.
        qResult = QuickUsbInitializeStream(QUICKUSB_ASYNC_STREAM, stream, hDevice, 0, 0, 0);
        if (!qResult) {
            free(stream);
            stream = NULL;
            QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = NULL;
            return(FALSE);
        }

        // Fill out additional stream info
        stream->StreamType = QUSB_STREAMTYPE_BULK; // IN or OUT
        stream->info = QUSB_INFO_ASYNC;
        stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
        stream->cRoutine = (PQVOID)cRoutine;
        stream->tag = tag;
    } else {
        stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM];

        // We must make sure not to issue this request if the stream is stopped.  The
        // stream is stopped when QuickUsbClose is called but the users app could have 
        // another thread that is trying to issue a request and doing so can prevent
        // QuickUsbClose from closing successfully.
        if (stream->info & QUSB_INFO_STREAMING_STOP) {
            lastError = QUICKUSB_ERROR_DEVICE_IS_CLOSING;
            return(FALSE);
        }

        // Always update the completion info contained in the stream so that it
        // is copied to the BulkStream objects in QuickUsbIssueStreamBulkRequest
        stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
        stream->cRoutine = (PQVOID)cRoutine;
        stream->tag = tag;
    }

    // Fill out BulkStream structure
    bulkStream->Buffer = buffer;
    bulkStream->BytesRequested = bytes;
    bulkStream->StreamID = QUICKUSB_ASYNC_STREAM;
    bulkStream->RequestID = stream->NextRequestID;
    bulkStream->StreamType = QUSB_STREAMTYPE_BULK_OUT_EP2;

    // Issue the read request
    if (!QuickUsbIssueStreamBulkRequest(QUICKUSB_ASYNC_STREAM, stream, bulkStream, TRUE)) { // Sets lastError on failure
        // NOTE: When QuickUsbIssueStreamBulkRequest fails to issue a request, the BulkStream 
        // entry is cleared out so that the request may be reused.

        // Return with error
        return(FALSE);
    }
    
#if DRIVER==MacOSXDriver
    // Dispatch queued requests 
    DispatchRequests(&QusbDeviceInfo[(QULONG) hDevice]);
#endif
    
    // Success
    return(TRUE);
#endif
}



QRETURN QuickUsbBulkWait(QHANDLE hDevice, PQBULKSTREAM bulkStream, QBYTE immediate) {
/*---------------------------------------------------------------------------
 Purpose : Have the current thread block until the asynchronous request has
             completed and executed its completion routine.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
           bulkStream - (Optional) A pointer to the user allocated
             QBULKSTREAM containing all the information about the request.  
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : If no bulkStream is specified, then this function will wait until 
             ALL pending requests complete and execute their completion 
             routines.
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    clock_t tStart;
    float tElapsed, maxWaitTime;
    QULONG pendingRequests;
    PQIOSTREAM stream;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (bulkStream && (((QULONG) bulkStream->Handle != (QULONG) hDevice) || (bulkStream->InternalStatus & QUSB_BULKSTREAM_MAGIC_MASK) != QUSB_BULKSTREAM_MAGIC)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Get the stream
    stream = QusbDeviceInfo[(QULONG)hDevice].stream[QUICKUSB_ASYNC_STREAM];

    // We must take extra care to not loop forever here as the API should 
    // never lock up.  If the stream fails to shutdown, we need to quit.
    tStart = clock();
    maxWaitTime = (float)max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) hDevice].timeout) / 1000.0f; // In seconds

    // See if we're checking if all legacy requests have completed
    if (bulkStream == 0) {
        // Are we to return immmediately?
        if (immediate) {
            // For single-threaded apps, we need to process the stream ourself
            if (stream->numThreads == 0) {
                if (!QuickUsbProcessStream(hDevice, QUICKUSB_ASYNC_STREAM, 0)) { // Sets lastError on failure
                    return(FALSE);
                }
            }

            // Read the shared 'pendingRequests' member
            QUSB_LOCK(stream); {
                pendingRequests = stream->pendingRequests;
            } QUSB_UNLOCK(stream);

            // Quickly check if all transactions are completed
            if (pendingRequests == 0) {
                // Indicate that all transaction have completed (ie there are no outstanding requests)
                return(TRUE);
            } else {
                // Indicate that there is at least one outstanding request
                lastError = QUICKUSB_ERROR_NOT_COMPLETED;
                return(FALSE);
            }
        } else {
            // Read the shared 'pendingRequests' member
            QUSB_LOCK(stream); {
                pendingRequests = stream->pendingRequests;
            } QUSB_UNLOCK(stream);

            // We need to wait the default timeout for all requests to complete
            while (pendingRequests != 0) {
                if (stream->numThreads == 0) {
                    // For single-threaded apps, we need to process the stream ourself
                    qResult = QuickUsbProcessStream(hDevice, QUICKUSB_ASYNC_STREAM, (int)(maxWaitTime * 1000.0f));
                } else {
                    // For multi-threaded apps, yield our thread so that the worker threads may do their work
                    SleepMS(0);
                }

                // Read the shared 'pendingRequests' member
                QUSB_LOCK(stream); {
                    pendingRequests = stream->pendingRequests;
                } QUSB_UNLOCK(stream);

                // Check if too much time has elapsed
                tElapsed = ((float)(clock() - tStart) / (float)CLOCKS_PER_SEC);
                if (tElapsed >= maxWaitTime) {
                    break;
                }
            }

            // If not all requests are complete, we have timed out
            if (pendingRequests != 0) {
                lastError = QUICKUSB_ERROR_TIMEOUT;
                return(FALSE);
            }

            // Indicate that all requests have completed (ie there are no outstanding requests)
            return(TRUE);
        }
    } else {
        // Are we to return immmediately?
        if (immediate) {
            // For single-threaded apps, we need to process the stream ourself
            if (stream->numThreads == 0) {
                qResult = QuickUsbProcessStream(hDevice, QUICKUSB_ASYNC_STREAM, 0);
            }

            // Quickly check if the transaction has completed
            if (bulkStream->InternalStatus & QUSB_BULKSTREAM_CR_COMPLETE) {
                // Indicate that the transaction has completed
                return(TRUE);
            } else {
                // Indicate that the request is still outstanding
                lastError = QUICKUSB_ERROR_NOT_COMPLETED;
                return(FALSE);
            }
        } else {
            // Wait the default timeout for the specified request to complete
            // NOTE: We don't want to sleep the thread here because it could negatively affect
            // performance.  Instead we should just repeatedly scan the time.
            while (((bulkStream->InternalStatus & QUSB_BULKSTREAM_CR_COMPLETE) == 0)) {
                if (stream->numThreads == 0) {
                    // For single-threaded apps, we need to process the stream ourself
                    qResult = QuickUsbProcessStream(hDevice, QUICKUSB_ASYNC_STREAM, QusbDeviceInfo[(QULONG) hDevice].timeout); // Sets lastError on failure
                    if (!qResult) {
                        return(FALSE);
                    }
                } else {
                    // For multi-threaded apps, yield our thread so that the worker threads may do their work
                    SleepMS(0);
                }

                // Check if too much time has elapsed
                tElapsed = ((float)(clock() - tStart) / (float)CLOCKS_PER_SEC);
                if (tElapsed >= maxWaitTime) {
                    break;
                }
            }

            // Check that the request was processed
            if ((bulkStream->InternalStatus & QUSB_BULKSTREAM_CR_COMPLETE) == 0) {
                // If we were unable to process the request then it indicates that the request should
                // have automatically timed out and been processed but didn't, which is an indicator
                // that there is an internal error with request timeouts, so we'll assert for easier
                // debugging
#if defined(_DEBUG)
                assert(0);
#endif
                lastError = QUICKUSB_ERROR_TIMEOUT;
                return(FALSE);
            }
        }
    }

    // Success
    return(TRUE);
#endif
}



QRETURN QuickUsbSetNumAsyncThreads(QHANDLE hDevice, QWORD numThreads, QWORD concurrency) {
/*---------------------------------------------------------------------------
 Purpose : 
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   :
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    PQIOSTREAM stream;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((numThreads != 0) && (concurrency == 0)) { // Concurrency must be at least 1 when multithreading
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // If the stream has already been created we must destroy it
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] != NULL) {
        // Stop the stream
        if (!QuickUsbStopStream(hDevice, QUICKUSB_ASYNC_STREAM, FALSE)) { // Sets lastError on failure
            return(FALSE);
        }
    }

    // Allocate the stream
    QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = (PQIOSTREAM) malloc(sizeof(QIOSTREAM));
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM];

    // Initialize the stream.  Note that for async support we do not pre-allocate request objects.
    qResult = QuickUsbInitializeStream(QUICKUSB_ASYNC_STREAM, stream, hDevice, 0, numThreads, concurrency); // Sets lastError on failure
    if (!qResult) {
        // The stream could potentially already be freed, so be careful
        if (stream) {
            free(stream);
            stream = NULL;
        }
        QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] = NULL;
        return(FALSE);
    }

    // Fill out additional stream info
    stream->StreamType = QUSB_STREAMTYPE_BULK; // IN or OUT
    stream->info = QUSB_INFO_ASYNC;

    // Success
    return(TRUE);
#endif
};



QRETURN QuickUsbGetNumAsyncThreads(QHANDLE hDevice, PQWORD numThreads, PQWORD concurrency) {
/*---------------------------------------------------------------------------
 Purpose : 
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   :
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((numThreads == 0) || (concurrency == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Retrieve the number of threads and thread concurrency
    if (QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM] != NULL) {
        *numThreads = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM]->numThreads;
        *concurrency = QusbDeviceInfo[(QULONG) hDevice].stream[QUICKUSB_ASYNC_STREAM]->threadConcurrency;
    } else {
        *numThreads = 0;
        *concurrency = 0;
    }

    // Success
    return(TRUE);
#endif
};



#if DRIVER==WindowsDriver
    // DLL entry points
    typedef BOOL (WINAPI* CANCELIOEX)(HANDLE, LPOVERLAPPED);
#endif



QRETURN QuickUsbBulkAbort(QHANDLE hDevice, PQBULKSTREAM bulkStream) {
/*---------------------------------------------------------------------------
 Purpose : 
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   :
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
#if DRIVER==WindowsDriver
	HMODULE hKernel32;
    CANCELIOEX CancelIoExFuncPtr;
    PQIOSTREAM pStream;
    QULONG k;
    DWORD winError;
#endif
#if DRIVER==LinuxDriver
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters - None to verify

#if DRIVER==WindowsDriver
    hKernel32 = GetModuleHandle(TEXT("kernel32.dll"));      // There is no corresponding free function to call
    if (hKernel32 == NULL) {
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }
    CancelIoExFuncPtr = (CANCELIOEX) GetProcAddress(hKernel32, "CancelIoEx");

    // If no bulkstream object is specified, we do our best to abort everything.  On XP,
    // we will only be able to abort IO issued from the active thread.  On Vista and later, 
    // all IO will be cancelled
    if (bulkStream == NULL) {
        // If we can use CancelIoEx (Vista and later), then do so
        if (CancelIoExFuncPtr) {
            if (!CancelIoExFuncPtr(hDevHandle, NULL)) {
                winError = GetLastError();
                if (winError != ERROR_NOT_FOUND) {
                    lastError = TranslateSystemError(GetLastError());
                    return(FALSE);
                }
            }
        } else if (!CancelIo(hDevHandle)) {
            lastError = TranslateSystemError(GetLastError());
            return(FALSE);
        }

        // Streams have their own handle that we must also stop IO on
        for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
            // Check if the stream is active
            if (QusbDeviceInfo[(QULONG) hDevice].stream[k] != NULL) {
                if (CancelIoExFuncPtr) {
                    if (!CancelIoExFuncPtr(QusbDeviceInfo[(QULONG) hDevice].stream[k]->handle, NULL)) {
                        winError = GetLastError();
                        if (winError != ERROR_NOT_FOUND) {
                            lastError = TranslateSystemError(GetLastError());
                            return(FALSE);
                        }
                    }
                } else if (!CancelIo(QusbDeviceInfo[(QULONG) hDevice].stream[k]->handle)) {
                    lastError = TranslateSystemError(GetLastError());
                    return(FALSE);
                }
            }
        }
    } else {
        // The user wants to abort a specific IO request.  This operation is not support in XP, so we
        // must first check if we can call CancelIoEx on the running platform.
        if (CancelIoExFuncPtr == NULL) {
            lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
            return(FALSE);
        }

        // Get the pointer to the stream
        pStream = QusbDeviceInfo[(QULONG) hDevice].stream[bulkStream->StreamID];
        if (pStream == NULL) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }

        // Cancel the IO request
        if (!CancelIoExFuncPtr(pStream->handle, (LPOVERLAPPED) bulkStream)) {
            if (GetLastError() != ERROR_NOT_FOUND) {
                lastError = TranslateSystemError(GetLastError());
                return(FALSE);
            }
        }
    }
#endif

#if DRIVER==LinuxDriver
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
    
    // TODO: Implement abort on LinuxDriver
    /*
    // If no bulkstream object is specified, we do our best to abort everything.
    if (bulkStream == NULL) {
        // Streams have their own handle that we must also stop IO on
        for (k = 0; k < QUICKUSB_MAX_STREAMS; ++k) {
            // Get the pointer to the stream
            pStream = QusbDeviceInfo[(QULONG) hDevice].stream[k];
            
            // Check if the stream is active
            if (pStream != NULL) {
                // Destroying the asynchronous context will about all pending requests and block for completion
                result = io_destroy(pStream->hIOCP);
                if(result != 0) {
                    lastError = TranslateSystemError(errno);
                    return(FALSE);
                }

                pStream->hIOCP = 0;
            }

            // We need to recreate the asynchronous context so the stream may continue to be used
            result = io_setup(1, &pStream->hIOCP);
            if (result != 0) {
                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }
    } else {
        // The user wants to abort a specific IO request.

        // Clear event structure
        memset(&event, 0, sizeof(struct io_event));

        // Get the pointer to the stream
        pStream = QusbDeviceInfo[(QULONG) hDevice].stream[bulkStream->StreamID];
        if (pStream == NULL) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }

        // Attemp to cancel the asynchronous request
        result = io_cancel(pStream->hIOCP, (struct iocb *) bulkStream, &event);
        if (result != 0) {
            // If we failed to cancel the IO request, we still return will success
            if (result == -EAGAIN) {
                return(TRUE);
            }

            lastError = TranslateSystemError(errno);
            return(FALSE);
        }

        // Extract the error and number of bytes
        requestResult = event.res;
        bytes = event.res2;

        // io_cancel will cancel the event and remove it from the completion queue if the
        // request was successfully cancelled, so we must still complete the request.
        CompleteBulkRequest(pStream, bulkStream, bytes, requestResult);
        //CompleteBulkRequest(pStream, bulkStream, 0, QUICKUSB_ERROR_ABORTED);
    }*/
#endif

#if DRIVER==MacOSXDriver
    // If no bulkstream object is specified, we do our best to abort everything.
    if (bulkStream == NULL) {
        (*hDevHandle)->AbortPipe(hDevHandle, QusbDeviceInfo[(QULONG) hDevice].inPipe);
        (*hDevHandle)->AbortPipe(hDevHandle, QusbDeviceInfo[(QULONG) hDevice].outPipe);
    } else {
        // The user wants to abort a specific IO request -- Not supported on Mac due to IOKIT limitations
        lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
        return(FALSE);
    }
#endif

    // Success
    return(TRUE);
#endif
};
