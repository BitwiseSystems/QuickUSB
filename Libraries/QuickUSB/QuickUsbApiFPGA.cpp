/*=============================================================================
 Title        : QuickUsbApiFPGA.cpp
 Description  : QuickUSB QUSB2 Module FPGA API
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
 Purpose :  Start the process of FPGA configuration.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  If the FPGA is in the process of being configured, the process will
                restart. If the FPGA is already configured, it will be reset and
                reconfigured.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbStartFpgaConfiguration(QHANDLE hDevice) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QWORD status;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters - None to verify

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_FPGA, FPGA_START, 0, &status, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }

    // BUGFIX: In this case everything including the IOCTL succeeded, but the init 
    // routine in the firmware failed.  This bug existed in v2.14.3 and earlier.
    if (status == 0) {
        lastError = QUICKUSB_ERROR_FPGA_INIT_FAILED;
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Sends FPGA configuration data to the FPGA using the QuickUSB FPGA
                configuration port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            fpgadata - FPGA configuration data
            datalength - Length of the configuration data
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteFpgaData(QHANDLE hDevice, PQBYTE fpgadata, QULONG datalength) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QULONG bytecount;
    QULONG bufferSize;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((fpgadata == 0) || (datalength < 1)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send the config file to the FPGA in blocks of MAX_CNTL_XFER_SIZE
    bytecount = 0;
    while (bytecount < datalength) {
        // Send the block
        bufferSize = min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, datalength - bytecount);

        // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
        if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_FPGA, FPGA_WRITE, 0, fpgadata + bytecount, bufferSize, &bytesReturned)) {
            return(FALSE);
        }

        bytecount += bufferSize;
    }

    // Return successfully
    return(TRUE);
}


/*---------------------------------------------------------------------------
 Purpose :  Check to see if the FPGA is configured.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  isConfigured - Configuration status of the FPGA
            Returns non-zero on success, zero on failure.
 Notes   :  Return value is the actual state of the CONF_DONE line.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbIsFpgaConfigured(QHANDLE hDevice, PQWORD isConfigured) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QWORD status;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (isConfigured == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_FPGA, FPGA_ISCONFIG, 0, &status, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }

    // Return the configuration status
    *isConfigured = (status & 0x01);

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  
            Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbConfigureFpga(QHANDLE hDevice, PCQCHAR filePath) {

/*
    QULONG result;
    QBYTE buffer[64];
    QULONG len;
    FILE *fin;
    QWORD conf;

    result = QuickUsbStartFpgaConfiguration(hDevice);
    if (!result) {
        return(FALSE);
    }

    fin = fopen(filePath, "rb");
    //do {
        len = fread(buffer, 1, 64, fin);
        result = QuickUsbWriteFpgaData(hDevice, buffer, len);
        if (!result) {
            fclose(fin);
            return(FALSE);
        }
    //} while (!feof(fin));

    result = QuickUsbIsFpgaConfigured(hDevice, &conf);
    if (!conf) {
    fclose(fin);
        return(FALSE);
    }

    fclose(fin);
    return(TRUE);*/

    
    QDEVHANDLE hDevHandle;
    QWORD conf;
    FILE * fin;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (filePath == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    
    // Start configuration
    if (!QuickUsbStartFpgaConfiguration(hDevice)) {
        return(FALSE);
    }
        
#if DRIVER==WindowsDriver
    {
    // The Windows implementation can issue multiple FPGA data writes to increase
    // configuration speed by ~2-4% when IMPLEMENT_OVERLAPPED is defined
#ifdef IMPLEMENT_OVERLAPPED
    #define QUSB_FPGA_NUM_BUFFERS 4
    OVERLAPPED ovl[QUSB_FPGA_NUM_BUFFERS];
    DWORD bytesTransferred;
    DWORD winError;
    int k;
#else
    #define QUSB_FPGA_NUM_BUFFERS 1
#endif
    errno_t err;
    size_t count;
    PQBYTE buffer[QUSB_FPGA_NUM_BUFFERS];
    int curBuffer = 0, curIssued = 0;
    QUICKUSB_VENDOR_REQUEST vRequest;
    QBOOL result;
    DWORD bytesReturned;

    // Allocate buffers
    for (k = 0; k < QUSB_FPGA_NUM_BUFFERS; ++k) {
        buffer[k] = malloc(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);
    }

    // Clear out the overlapped structures
    ZeroMemory(ovl, QUSB_FPGA_NUM_BUFFERS * sizeof(OVERLAPPED));

    // Open the file
    err = fopen_s(&fin, filePath, "rb");
    if (err) {
        // Free buffers
        for (k = 0; k < QUSB_FPGA_NUM_BUFFERS; ++k) {
            free(buffer[k]);
        }

        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }

    // Build the vendor request
    vRequest.direction = 0;         // 0=out, 1=in
    vRequest.requestType = 2;       // vendor specific request type (2)
    vRequest.recepient = 0;         // recepient is device (0)
    vRequest.request = QUICKUSB_FPGA;
    vRequest.index = FPGA_WRITE;
    vRequest.value = 0;

    // Configure the FPGA
    result = TRUE;
    while (!feof(fin)) {
#ifdef IMPLEMENT_OVERLAPPED
        // If all our requests have been issued, we can start waiting for them to complete
        if (curIssued == QUSB_FPGA_NUM_BUFFERS) {
            result = GetOverlappedResult(hDevHandle, &ovl[curBuffer], &bytesTransferred, TRUE);
            CloseHandle(ovl[curBuffer].hEvent);
            ovl[curBuffer].hEvent = NULL;
            --curIssued;
            if (!result) {
                break;
            }
        }
#endif
        // Read part of the file
        count = fread(buffer[curBuffer], sizeof(QBYTE), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, fin);
        if (count == 0) {
            break;
        }

        // Issue a new request
#ifdef IMPLEMENT_OVERLAPPED
        ZeroMemory(&ovl[curBuffer], sizeof(OVERLAPPED));
        ovl[curBuffer].hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
#endif
        result = DeviceIoControl(hDevHandle,
            IOCTL_QUSB_VENDOR_OR_CLASS_REQUEST,
            &vRequest,
            sizeof(QUICKUSB_VENDOR_REQUEST),
            buffer[curBuffer],
            (DWORD)count,
            &bytesReturned,
#ifdef IMPLEMENT_OVERLAPPED
            &ovl[curBuffer]);
#else
            NULL);
#endif

#ifdef IMPLEMENT_OVERLAPPED
        winError = GetLastError();
        if (result == 0 && winError != ERROR_IO_PENDING) {
            lastDriverError = winError;
            CloseHandle(ovl[curBuffer].hEvent);
            ovl[curBuffer].hEvent = NULL;
            result = FALSE;
            break;
        }
        result = TRUE;
#endif
        ++curIssued;

        // Move to the next buffer
        if (++curBuffer >= QUSB_FPGA_NUM_BUFFERS) {
            curBuffer = 0;
        }
    }

    // Close the file
    fclose(fin);

#ifdef IMPLEMENT_OVERLAPPED
    // Wait for last few requests to finish up
    for (k = 0; k < QUSB_FPGA_NUM_BUFFERS; ++k) {
        if (ovl[k].hEvent) {
            result &= GetOverlappedResult(hDevHandle, &ovl[k], &bytesTransferred, TRUE);
            CloseHandle(ovl[k].hEvent);
            ovl[k].hEvent = NULL;
        }
    }
#endif

    // Check if something went wrong
    if (!result) {
        // Free buffers
        for (k = 0; k < QUSB_FPGA_NUM_BUFFERS; ++k) {
            free(buffer[k]);
        }

        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }

    // Free buffers
    for (k = 0; k < QUSB_FPGA_NUM_BUFFERS; ++k) {
        free(buffer[k]);
    }

    }
#endif // DRIVER==WindowsDriver
    
#if DRIVER==LinuxDriver
    {
    QULONG count;
    QBYTE buffer[MAX_CNTL_XFER_SIZE];
    QRESULT result;
    
    // Open the file
    fin = fopen(filePath, "rb");
    if (!fin) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
    
    // Configure the FPGA
    result = TRUE;
    while (!feof(fin)) {
        // Read part of the file
        count = fread(&buffer, sizeof(QBYTE), MAX_CNTL_XFER_SIZE, fin);
        if (count == 0) {
            break;
        }
        
        result = QuickUsbWriteFpgaData(hDevice, buffer, count);
        if (!result) {
            break;
        }
    }
    
    // Close the file
    fclose(fin);
    
    // Check if something went wrong
    if (!result) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }
    
    }    
#endif
    
#if DRIVER==MacOSXDriver
    {
    QULONG count;
    QBYTE buffer[MAX_CNTL_XFER_SIZE];
    QRESULT result;
    
    // Open the file
    fin = fopen(filePath, "rb");
    if (!fin) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
    
    // Configure the FPGA
    result = TRUE;
    while (!feof(fin)) {
        // Read part of the file
        count = fread(&buffer, sizeof(QBYTE), MAX_CNTL_XFER_SIZE, fin);
        if (count == 0) {
            break;
        }
        
        result = QuickUsbWriteFpgaData(hDevice, buffer, count);
        if (!result) {
            break;
        }
    }

    // Close the file
    fclose(fin);

    // Check if something went wrong
    if (!result) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }

    }
#endif
    
    // Check if the FPGA was successfully configured
    if (!QuickUsbIsFpgaConfigured(hDevice, &conf)) { // Sets lastError on failure
        return(FALSE);
    }
    if (!(conf & 0x01)) {
        lastError = QUICKUSB_ERROR_FPGA_CONFIG_FAILED;
        return(FALSE);
    }
    
    // Return successfully
    return(TRUE);
}
