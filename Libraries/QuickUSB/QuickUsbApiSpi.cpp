/*=============================================================================
 Title        : QuickUsbApiSpi.cpp
 Description  : QuickUSB QUSB2 Module SPI API
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



QRETURN QuickUsbReadSpi(QHANDLE hDevice, QBYTE portNum, PQBYTE data, PQWORD length) {
/*---------------------------------------------------------------------------
 Purpose :  Read a block of bytes from the specified slave SPI port
 Input   :  hDevice - Offset into array of QUSB_DEVICE_INFO structures
            portNum - SPI device address
            length - Bytes to read from SPI
 Output  :  data - Buffer containing data read from SPI
            length - Length read from SPI
 Returns :  Non-zero on success, zero on failure
 Notes   :  
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 9) || (data == 0) || (length == 0) || (*length < 1) || (*length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_SPI, SPI_READWRITE, portNum, data, *length, &bytesReturned)) {
        return(FALSE);
    }

    // Use the length parameter to indicate the actual number of bytes read
    *length = (QWORD)bytesReturned;

    // Return successfully
    return(TRUE);
}



QRETURN QuickUsbWriteSpi(QHANDLE hDevice, QBYTE portNum, PQBYTE data, QWORD length) {
/*---------------------------------------------------------------------------
 Purpose :  Write a block of bytes to the specified SPI slave port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - SPI device address
            data - Buffer containing data to write to SPI
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 9) || (data == 0) || (length < 1) || (length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_SPI, SPI_READWRITE, portNum, data, length, &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



QRETURN QuickUsbWriteReadSpi(QHANDLE hDevice, QBYTE portNum, PQBYTE data, QWORD length) {
/*---------------------------------------------------------------------------
 Purpose :  Simultaneously write and read a block of data to and from the
            specified SPI slave port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - SPI device address
            data - Buffer containing data to write to SPI
            length - Length to write to SPI
 Output  :  data - Buffer containing data read from SPI
 Returns :  Non-zero on success, zero on failure.
 Notes   :  This function uses the data buffer for both writing data to the
            SPI and to store read data from the SPI. Therefore, the data 
            buffer will always be overwritten on each call to this function.
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 9) || (data == 0) || (length < 1) || (length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_SPI, SPI_WRITEANDREAD, portNum, data, length, &bytesReturned)) {
        return(FALSE);
    }

    // Retrieve the block of data (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_SPI, SPI_WRITEANDREAD, portNum, data, length, &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}
