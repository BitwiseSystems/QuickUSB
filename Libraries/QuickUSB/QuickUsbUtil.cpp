/*=============================================================================
 Title        : QuickUsbApi.cpp
 Description  : QuickUSB QUSB2 Module Base API
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

// NOTE: On Windows, every process that loads a DLL gets its own copy of global
// and static variables.  In order to share global data between processes, such
// as the QusbDeviceInfo table, we must provice a DllMain function that creates
// a file mapping of global data for each process.



// Global varables (All platforms)
int Timeout = 1000;          // Default timeout (in milliseconds)
QULONG lastError = QUICKUSB_ERROR_NO_ERROR; // Initially there is no error
QULONG lastDriverError = 0;
QBOOL initialized = FALSE; // Indicates QuickUsbFindModules() has not been called yet
QUSB_DEVICE_INFO QusbDeviceInfo[MAX_DEVICENUM];
QFILEHANDLE hFileTemp;
PQULONG QusbDeviceRefCount = NULL;

// Global variables (All platforms)
QUSB_DRIVER_VERSION DriverVersion;

// Windows
#if DRIVER==WindowsDriver

HANDLE hMapObject = NULL;
QULONG processCount = 0;

// The DLL entry-point function
 BOOL WINAPI DllMain(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved) {
    BOOL fInit, fIgnore; 
 
    switch (fdwReason)  { 
        // DLL load due to process initialization or LoadLibrary
        case DLL_PROCESS_ATTACH: 
            // Create a named file mapping object 
            hMapObject = CreateFileMapping( 
                INVALID_HANDLE_VALUE,    // use paging file
                NULL,                    // default security attributes
                PAGE_READWRITE,          // read/write access
                0,                       // size: high 32-bits
                sizeof(QULONG) * MAX_DEVICENUM, // size: low 32-bits
                TEXT("qusbdllfilemap")); // name of map object
            if (hMapObject == NULL) {
                return FALSE; 
            }

            // The first process to attach initializes memory
            fInit = (GetLastError() != ERROR_ALREADY_EXISTS); 

            // Get a pointer to the file-mapped shared memory
            QusbDeviceRefCount = MapViewOfFile( 
                hMapObject,     // object to map view of
                FILE_MAP_WRITE, // read/write access
                0,              // high offset:  map from
                0,              // low offset:   beginning
                0);             // default: map entire file
            if (QusbDeviceRefCount == NULL) {
                return FALSE; 
            }

            // Initialize memory if this is the first process
            if (fInit) {
                memset(QusbDeviceRefCount, 0, sizeof(QULONG) * MAX_DEVICENUM); 
            }

            // Clear device table
            memset(QusbDeviceInfo, 0, sizeof(QUSB_DEVICE_INFO) * MAX_DEVICENUM); 

            // Keep count of the number of attached processes
            ++processCount;

            break; 

        // The attached process creates a new thread
        case DLL_THREAD_ATTACH: 
            break; 

        // The thread of the attached process terminates
        case DLL_THREAD_DETACH: 
            break; 

        // DLL unload due to process termination or FreeLibrary
        case DLL_PROCESS_DETACH: 
            // Keep count of the number of attached processes
            --processCount;

            // If the last process is detaching, go ahead and clear the shared memory.
            // This way if there is some reference counting error at least the user 
            // will not need to reboot in order for everything to get back to normal.
            if (processCount == 0) {
                memset(QusbDeviceRefCount, 0, sizeof(QULONG) * MAX_DEVICENUM);
            }

            // Unmap shared memory from the process's address space
            fIgnore = UnmapViewOfFile(QusbDeviceRefCount); 

            // Close the process's handle to the file-mapping object
            fIgnore = CloseHandle(hMapObject); 

            break; 

        default: 
            break; 
     } 
 
    return(TRUE);
} 



QBOOL QuickUsbIoctl(QHANDLE hDevice, QDEVHANDLE hDevHandle, QULONG controlCode, PQVOID inBuffer, 
                    QULONG inBufferSize, PQVOID outBuffer, QULONG outBufferSize, PQULONG bytesReturned) {
    QBOOL result;
    OVERLAPPED ovl;
    QULONG bytesTransferred;

    // Initialize the overlapped structure
    ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    ovl.Offset = 0;
    ovl.OffsetHigh = 0;

    // Send the block
    result = DeviceIoControl(hDevHandle,
        controlCode,
        inBuffer,
        inBufferSize,
        outBuffer,
        outBufferSize,
        &bytesTransferred,
        &ovl);

    // Check if the IOCTL completed synchronously, asynchronously, or failed
    if (!result) {
        lastDriverError = GetLastError();
        if (lastDriverError == ERROR_IO_PENDING) {
            // Request was asynchronously issued
            if (!GetOverlappedResult(hDevHandle, &ovl, &bytesTransferred, TRUE)) {
                CloseHandle(ovl.hEvent);
                lastError = TranslateSystemError(GetLastError());
                return(FALSE);
            }
        } else {
            // Request was not issued (failure)
            CloseHandle(ovl.hEvent);

            lastError = TranslateSystemError(lastDriverError);
            return(FALSE);
        }
    } else {
        // Request was synchronously completed
    }

    // Close the event handle
    CloseHandle(ovl.hEvent);

    // Return the number of bytes transferred
    if (bytesReturned) {
        *bytesReturned = bytesTransferred;
    }

    // Return successfully
    return(TRUE);
}
#endif // DRIVER==WindowsDriver



QBOOL QuickUsbVendorRequest(QHANDLE hDevice, QDEVHANDLE hDevHandle, QBYTE direction, QBYTE request, QWORD index, QWORD value, PQVOID data, QULONG length, QULONG *bytesReturned) {
    QUICKUSB_VENDOR_REQUEST vRequest;

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    OVERLAPPED ovl;
    DWORD bytesTransferred;

    // Initialize the overlapped structure
    ovl.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    ovl.Offset = 0;
    ovl.OffsetHigh = 0;

    // Build the vendor request
    vRequest.direction = direction; // 0=out, 1=in
    vRequest.requestType = 2;       // vendor specific request type (2)
    vRequest.recepient = 0;         // recepient is device (0)
    vRequest.request = request;
    vRequest.index = index;
    vRequest.value = value;

    // Send the block
    result = DeviceIoControl(hDevHandle,
        IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST,
        &vRequest,
        sizeof(QUICKUSB_VENDOR_REQUEST),
        data,
        length,
        &bytesTransferred,
        &ovl);

    // Check if the IOCTL completed synchronously, asynchronously, or failed
    if (!result) {
        lastDriverError = GetLastError();
        if (lastDriverError == ERROR_IO_PENDING) {
            lastDriverError = 0;

            // Request was asynchronously issued
            if (!GetOverlappedResult(hDevHandle, &ovl, &bytesTransferred, TRUE)) {
                CloseHandle(ovl.hEvent);
                lastError = TranslateSystemError(GetLastError());
                return(FALSE);
            }
        } else {
            // Request was not issued (failure)
            CloseHandle(ovl.hEvent);
    
            lastError = TranslateSystemError(lastDriverError);
            return(FALSE);
        }
    } else {
        // Request was synchronously completed
    }

    // Close the event handle
    CloseHandle(ovl.hEvent);

    // Return the number of bytes transferred
    *bytesReturned = bytesTransferred;

    // Return successfully
    return(TRUE);
    }
#endif // DRIVER==WindowsDriver
    
#if DRIVER==LinuxDriver
    {
    int result;
    
    // Build the vendor request
    vRequest.direction = direction; // 0=out, 1=in
    vRequest.requestType = 2;       // vendor specific request type (2)
    vRequest.recepient = 0;         // recepient is device (0)
    vRequest.request = request;
    vRequest.index = index;
    vRequest.value = value;
    vRequest.length = length;
    vRequest.data = data;

    // Send the block
    result = ioctl(hDevHandle, IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST, &vRequest);
    if (result < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }

    // Report the number of bytes transferred
    *bytesReturned = vRequest.length;

    // Return successfully
    return(TRUE);
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    QBOOL result;
    // Build the vendor request
    vRequest.bmRequestType = USBmakebmRequestType((direction == QUICKUSB_REQUEST_IN) ? kUSBIn : kUSBOut, kUSBVendor, kUSBDevice);
    vRequest.bRequest = request;
    vRequest.wIndex = index;
    vRequest.wValue = value;
    vRequest.wLength = length;
    vRequest.pData = data;
    
    // Send control request to get string descriptor
    result = ((*hDevHandle)->ControlRequest(hDevHandle, 0, &vRequest) == kIOReturnSuccess);

    // Report the number of bytes transferred
    *bytesReturned = result ? (QULONG)vRequest.wLenDone : 0; // Does wLenDone need to be swapped?
    
    // If there was an error set the last error code
    if (!result) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
    }

    return(result);
    }
#endif // DRIVER==MacOSXDriver

}



#if DRIVER==WindowsDriver
QBOOL QuickUsbVendorRequestAsyncWait(QHANDLE hDevice, /*QDEVHANDLE hDevHandle,*/ OVERLAPPED *ovl, QULONG *bytesTransferred) {
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Check the result of the vendor request
    if (!GetOverlappedResult(hDevHandle, ovl, (DWORD *) bytesTransferred, TRUE)) {
        CloseHandle(ovl->hEvent);
        lastError = TranslateSystemError(GetLastError());
        return(FALSE);
    }

    // Close the event handle
    CloseHandle(ovl->hEvent);

    return(TRUE);
}


QBOOL QuickUsbVendorRequestAsync(QHANDLE hDevice, /*QDEVHANDLE hDevHandle,*/ QBYTE direction, QBYTE request, QWORD index, QWORD value, PQVOID data, QULONG length, OVERLAPPED *ovl) {
    QUICKUSB_VENDOR_REQUEST vRequest;

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    DWORD bytesTransferred;
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Initialize the overlapped structure
    memset(ovl, 0, sizeof(OVERLAPPED));
    ovl->hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
    ovl->Offset = 0;
    ovl->OffsetHigh = 0;

    // Build the vendor request
    vRequest.direction = direction; // 0=out, 1=in
    vRequest.requestType = 2;       // vendor specific request type (2)
    vRequest.recepient = 0;         // recepient is device (0)
    vRequest.request = request;
    vRequest.index = index;
    vRequest.value = value;

    // Send the block
    result = DeviceIoControl(hDevHandle,
        IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST,
        &vRequest,
        sizeof(QUICKUSB_VENDOR_REQUEST),
        data,
        length,
        &bytesTransferred,
        ovl);

    // Check if the IOCTL completed synchronously, asynchronously, or failed
    if (!result) {
        lastDriverError = GetLastError();
        if (lastDriverError == ERROR_IO_PENDING) {
            // Request was asynchronously issued
            return(TRUE);
        } else {
            // Request was not issued (failure)
            CloseHandle(ovl->hEvent);
    
            lastError = TranslateSystemError(lastDriverError);
            return(FALSE);
        }
    } else {
        // Request was synchronously completed.  Return the number of bytes transferred
    }

    // Return successfully
    return(TRUE);
    }
#endif // DRIVER==WindowsDriver
    
#if DRIVER==LinuxDriver
    {
    int result;
    
    // Build the vendor request
    vRequest.direction = direction; // 0=out, 1=in
    vRequest.requestType = 2;       // vendor specific request type (2)
    vRequest.recepient = 0;         // recepient is device (0)
    vRequest.request = request;
    vRequest.index = index;
    vRequest.value = value;
    vRequest.length = length;
    vRequest.data = data;

    // Send the block
    result = ioctl(hDevHandle, IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST, &vRequest);
    if (result < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }

    // Report the number of bytes transferred
    *bytesReturned = vRequest.length;

    // Return successfully
    return(TRUE);
    }
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    {
    QBOOL result;
    // Build the vendor request
    vRequest.bmRequestType = USBmakebmRequestType((direction == QUICKUSB_REQUEST_IN) ? kUSBIn : kUSBOut, kUSBVendor, kUSBDevice);
    vRequest.bRequest = request;
    vRequest.wIndex = index;
    vRequest.wValue = value;
    vRequest.wLength = length;
    vRequest.pData = data;
    
    // Send control request to get string descriptor
    result = ((*hDevHandle)->ControlRequest(hDevHandle, 0, &vRequest) == kIOReturnSuccess);

    // Report the number of bytes transferred
    *bytesReturned = result ? (QULONG)vRequest.wLenDone : 0; // Does wLenDone need to be swapped?
    
    // If there was an error set the last error code
    if (!result) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
    }

    return(result);
    }
#endif // DRIVER==MacOSXDriver

}
#endif



#if DRIVER==MacOSXDriver
void QuickUsbLoad() {
    /*---------------------------------------------------------------------------
     Purpose :  Initialize QuickUsb when the library is loaded.
     Input   :  None.
     Output  :  None.
     Notes   :  None.
     ---------------------------------------------------------------------------*/
    PQUSB_DEVICE_INFO devInfo;
    UCHAR i;
    
    for (i = 0; i < MAX_DEVICENUM; i++) {
        // Store pointer to device info
        devInfo = &QusbDeviceInfo[i];
        
        // Initialize all members of QUSB_DEVICE_INFO
        devInfo->devName[0] = '\0';
        devInfo->hDevHandle = INVALID_QHANDLE_VALUE;
        devInfo->module = NULL;
        devInfo->devQusb = 0;
        devInfo->devInterface = 0;
        devInfo->devType = OTHER_DRIVER;
        devInfo->deviceOpen = 0;
        //devInfo->iRefCount = 0;
        devInfo->defaultOffset = 0;
        devInfo->outPipe = 0;
        devInfo->inPipe = 0;
        devInfo->maxPacketSize = 0;
        devInfo->devError.UsbError = 0;
        devInfo->devError.DriverError = 0;
        devInfo->devError.DriverState = 0;
        devInfo->timeout = 1000;
        devInfo->isHighSpeed = 1;
    }
}
#endif // DRIVER==MacOSXDriver



#if DRIVER==MacOSXDriver
void QuickUsbUnload() {
    /*---------------------------------------------------------------------------
     Purpose :  Clean up QuickUsb when the library is unloaded.
     Input   :  None.
     Output  :  None.
     Notes   :  None.
     ---------------------------------------------------------------------------*/
}
#endif // DRIVER==MacOSXDriver



#if DRIVER==WindowsDriver
QULONG TranslateSystemError(DWORD systemError) {
/*---------------------------------------------------------------------------
 Purpose : Convert the system error codes reported by the Windows API 
           GetLastError() function to the standard QuickUSB library error 
           codes (QUICKUSB_ERROR_XXX).
 Input   : The driver error code
 Output  : The corresponding QuickUSB Library error code
 Notes   : The driver error is stored in the pdx-DriverStatus member in the
           driver, but when it is fetched by the library with the
           QuickUsbGetError() method, it is stored in GetError->DriverError
           member.
 ---------------------------------------------------------------------------*/
    switch (systemError) {
        case ERROR_SUCCESS:
            return QUICKUSB_ERROR_NO_ERROR;

        case WAIT_TIMEOUT:
        case ERROR_SEM_TIMEOUT:
            return QUICKUSB_ERROR_TIMEOUT;

        case ERROR_GEN_FAILURE:         // Occurs on surprise removal
        case ERROR_FILE_NOT_FOUND:      // Occurs on surprise removal
        case ERROR_BAD_COMMAND:         // Occurs on surprise removal
            return QUICKUSB_ERROR_CANNOT_FIND_DEVICE;

        case ERROR_OPERATION_ABORTED:
        case ERROR_PROCESS_ABORTED:
            return QUICKUSB_ERROR_ABORTED;

        case ERROR_ALREADY_EXISTS:
        case ERROR_FILE_EXISTS:
            return QUICKUSB_ERROR_FILE_ALREADY_EXISTS;

        case ERROR_HANDLE_EOF:
            return QUICKUSB_ERROR_FILE_EOF;

		case ERROR_INVALID_NAME:
			return QUICKUSB_ERROR_FILE_NAME;

		case ERROR_ACCESS_DENIED:
			return QUICKUSB_ERROR_ACCESS_DENIED;

		case ERROR_SHARING_VIOLATION:
			return QUICKUSB_ERROR_SHARING_VIOLATION;

        default:
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
    };
}
#endif



#if DRIVER==LinuxDriver
QULONG TranslateSystemError(int systemError) {
/*---------------------------------------------------------------------------
 Purpose : Convert the system error codes reported by Linux
 Input   : The error code
 Output  : The corresponding QuickUSB Library error code
 Notes   : 
 ---------------------------------------------------------------------------*/
    // Sometimes errors are stored as -errno, so we'll make the error positive
    if (systemError < 0) {
        systemError = -systemError;
    }
    
    switch (systemError) {
        case 0:
            // No error
            return QUICKUSB_ERROR_NO_ERROR;

        case ENOMEM:
            // No memory
            return QUICKUSB_ERROR_OUT_OF_MEMORY;

        case ECONNRESET:
            // The USB was canceled by usb_unlink_urb
            return QUICKUSB_ERROR_ABORTED;
            
        case ENOENT: // No such file or directory
            return QUICKUSB_ERROR_FILE_NOT_FOUND;

        case EINPROGRESS:
            // The USB is still pending
            return QUICKUSB_ERROR_NOT_COMPLETED;

        case EPROTO:
            // a) Bitstuff, b) Unknown USB error
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case EILSEQ:
            // CRC mismatch
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case EOVERFLOW:
            // The amount of data returned by the endpoint was greater than either the
            // max packet size of the endpoint or the remaining buffer size. "Babble".
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case EPIPE:
            // a) Babble detect, b) Endpoint stalled
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case ETIMEDOUT:
            // Transfer timed out, NAK
            return QUICKUSB_ERROR_TIMEOUT;

        case ENODEV:
            // Device was removed
            return QUICKUSB_ERROR_CANNOT_FIND_DEVICE;

        case EREMOTEIO:
            // The data read from the endpoint did not fill the specified buffer, and
            // URB_SHORT_NOT_OK was set in urb>transfer_flags.
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case EXDEV:
            // ISO transfer only partially completed.  Look at individual frame status for details.
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        
        case EFAULT :
            // Invalid data
            return QUICKUSB_ERROR_INVALID_PARAMETER;

        case EINVAL:
            // ISO madness, if this happens: Log off and go home
            return QUICKUSB_ERROR_INVALID_PARAMETER;

        case EAGAIN:
            // Too many queued ISO transfers
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case EFBIG:
            // Too many requested ISO frames
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

        case ECOMM:
            // During an IN transfer, the host controller received data from an endpoint faster
            // than it could be written to system memory.
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case ESHUTDOWN:
            // The device or host controller has been disabled due to some
            // problem that could not be worked around.
            return QUICKUSB_ERROR_CANNOT_FIND_DEVICE;

        case EACCES:
            // Invalid permissions
            return QUICKUSB_ERROR_ACCESS_DENIED;

        case ENOSYS:
            // Not supported on this system
            return QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;

        default:
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
    }
}
#endif



#if DRIVER==MacOSXDriver
QULONG TranslateSystemError(int systemError) {
    /*---------------------------------------------------------------------------
     Purpose : Convert the system error codes reported by Linux
     Input   : The error code
     Output  : The corresponding QuickUSB Library error code
     Notes   : 
     ---------------------------------------------------------------------------*/
    switch (systemError) {
        // 
        // Standard error.h Codes
        //
        case ENOENT: // No such file or directory
            return QUICKUSB_ERROR_FILE_NOT_FOUND;
                        
        case EACCES:
            // Invalid permissions
            return QUICKUSB_ERROR_ACCESS_DENIED;

        //
        // IOKit Return Codes
        //
        case kIOReturnSuccess:
            return QUICKUSB_ERROR_NO_ERROR;
            
        case kIOReturnNotOpen:
            return QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
            
        case kIOReturnNoDevice:
            return QUICKUSB_ERROR_CANNOT_FIND_DEVICE;
            
        case kIOReturnExclusiveAccess:
            return QUICKUSB_ERROR_ALREADY_OPENED;
            
        case kIOReturnBadArgument:
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOReturnAborted:
            return QUICKUSB_ERROR_ABORTED;
            
        case kIOReturnNotResponding:
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
          
        //
        // USB Error Codes
        //
        case kIOUSBUnknownPipeErr:  // 0xe0004061  Pipe ref not recognized
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBTooManyPipesErr: // 0xe0004060  Too many pipes
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBNoAsyncPortErr:  // 0xe000405f  no async port
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBNotEnoughPipesErr:   // 0xe000405e  not enough pipes in interface
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBNotEnoughPowerErr:   // 0xe000405d  not enough power for selected configuration
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBEndpointNotFound:    // 0xe0004057  Endpoint Not found
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBConfigNotFound:    // 0xe0004056  Configuration Not found
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBTransactionTimeout:  // 0xe0004051  Transaction timed out
            return QUICKUSB_ERROR_TIMEOUT;
            
        case kIOUSBTransactionReturned: // 0xe0004050  The transaction has been returned to the caller
                                        // kIOUSBDevicePortWasNotSuspended - 0xe0004050  Port was not suspended
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBPipeStalled: // 0xe000404f  Pipe has stalled, error needs to be cleared
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBInterfaceNotFound:   // 0xe000404e  Interface ref not recognized
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBLowLatencyBufferNotPreviouslyAllocated:  // 0xe000404d  Attempted to use user land low latency isoc calls w/out calling PrepareBuffer (on the data buffer) first 
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBLowLatencyFrameListNotPreviouslyAllocated:   // 0xe000404c  Attempted to use user land low latency isoc calls w/out calling PrepareBuffer (on the frame list) first
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBHighSpeedSplitError: // 0xe000404b Error to hub on high speed bus trying to do split transaction
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBSyncRequestOnWLThread:   // 0xe000404a  A synchronous USB request was made on the workloop thread (from a callback?).  Only async requests are permitted in that case
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        case kIOUSBDeviceNotHighSpeed:  // 0xe0004049  The device is not a high speed device, so the EHCI driver returns an error
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
        default:
            return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            
    }
}
#endif



/*---------------------------------------------------------------------------
 Purpose :  To get driver specific error codes.
 Input   :  hDevice - offset into array of _QUSB_DEVICE_INFO structures
 Output  :  pGetError - Driver error
            Returns non-zero on success, zero on failure.
 Notes   :  This function is currently not supported.
 ---------------------------------------------------------------------------*/
int QuickUsbGetError(QHANDLE hDevice, PQUSB_GET_DRIVER_ERROR pGetError) {
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    {
    QBOOL result;
    QULONG bytesReceived;

    // Send IOCTL (Note: QuickUsbIoctl is Windows only and sets lastError on failure)
    result = QuickUsbIoctl(hDevice, hDevHandle,
        IOCTL_QUSB_GET_LAST_ERROR,
        NULL,
        (QWORD)0,
        (PVOID) pGetError,
        (QWORD)sizeof(QUSB_GET_DRIVER_ERROR),
        &bytesReceived);

    if (!result) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }
    
    }
#endif

#if DRIVER==MacOSXDriver
    pGetError->UsbError = QusbDeviceInfo[hDevice].devError.UsbError;
    pGetError->DriverError = QusbDeviceInfo[hDevice].devError.DriverError;
    pGetError->DriverState = QusbDeviceInfo[hDevice].devError.DriverState;
#endif

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Check the QuickUsb.sys driver is recent enough for the requested
                function.
 Input   :  major - Minimum required QuickUsb.sys major driver version
            minor - Minimum required QuickUsb.sys minor driver version
            build - Minimum required QuickUsb.sys build driver version
 Output  :  Return non-zero on success, zero on failure.
 Notes   :  Driver version is obtained in QuickUsbFindModules.
 ---------------------------------------------------------------------------*/
BOOLEAN CheckDriverVersion(unsigned short major, unsigned short minor, unsigned short build) {
    if (DriverVersion.MajorVersion < major) {
        return(FALSE);
    }
    if ((DriverVersion.MajorVersion == major) &&
        (DriverVersion.MinorVersion < minor)) {
            return(FALSE);
    }
    if ((DriverVersion.MajorVersion == major) &&
        (DriverVersion.MinorVersion == minor) &&
        (DriverVersion.BuildVersion < build)) {
            return(FALSE);
    }
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  major - Minimum required QuickUsb.sys major driver version
            minor - Minimum required QuickUsb.sys minor driver version
            build - Minimum required QuickUsb.sys build driver version
 Output  :  Return non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
BOOLEAN IsAtLeastVersion(unsigned short minMajor, unsigned short minMinor, unsigned short minBuild,
                       unsigned short major, unsigned short minor, unsigned short build) {
    if (major < minMajor) {
        return(FALSE);
    }
    if ((major == minMajor) &&
        (minor < minMinor)) {
            return(FALSE);
    }
    if ((major == minMajor) &&
        (minor == minMinor) &&
        (build < minBuild)) {
            return(FALSE);
    }
    return(TRUE);
}


/*---------------------------------------------------------------------------
 Purpose :  Set QuickUSB to reset state.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            resetBit - Reset state
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  This function is currently not supported.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbSetResetState(QHANDLE hDevice, QBYTE resetBit) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters - None to verify

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, ANCHOR_LOAD_INTERNAL, 0, CPUCS_REG_FX2, &resetBit, sizeof(QBYTE), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  N/A.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            outputs - N/A
 Output  :  inputs - N/A
            Returns non-zero on success, zero on failure.
 Notes   :  This function is currently not supported.
 ---------------------------------------------------------------------------*/
QRESULT QuickUsbJtagDirect(QHANDLE hDevice, unsigned short outputs, unsigned short *inputs) {
    QBOOL result;

#if DRIVER==WindowsDriver
    unsigned long bytesReceived;
    QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL request;
    QDEVHANDLE hDevHandle;
    QusbDriverType type;

    if (!QuickUsbGetDeviceType(hDevice, &type))
        return(FALSE);

    // Convert the generic handle to the device specific handle
    if (!QuickUsbGetDeviceHandle(hDevice, &hDevHandle)) {
        lastError = QUICKUSB_ERROR_CANNOT_FIND_DEVICE;         
        return(FALSE);
    }

    // Handle using the QuickUSB driver
    if (type == QUSB_DRIVER)  {
        // Build the vendor request
        request.direction = 0;        // 0=out, 1=in
        request.requestType = 2;      // vendor specific request type (2)
        request.recepient = 0;        // recepient is device (0)
        request.request = QUICKUSB_JTAG; // Vendor request configure FPGAs
        request.index = outputs;      // Setting address
        request.value = 0;

        // Send the command to the driver
        result = DeviceIoControl(hDevHandle,
            IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST,
            &request,
            sizeof(QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL),
            &inputs,
            sizeof(unsigned short),
            &bytesReceived,
            NULL
            );

        // If there was an error set the last error code
        if (!result) {
            lastDriverError = GetLastError();
            lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        }
    }

    // Handle an undefined driver type
    else {
        lastError = QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE;
        return(FALSE);
    }
#endif // DRIVER==WindowsDriver

#if DRIVER==LinuxDriver
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    result = FALSE;
#endif // DRIVER==LinuxDriver

#if DRIVER==MacOSXDriver
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    result = FALSE;
#endif // DRIVER==MacOSXDriver

    return(result);
}
