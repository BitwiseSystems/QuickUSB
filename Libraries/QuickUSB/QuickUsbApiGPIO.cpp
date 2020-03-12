/*=============================================================================
 Title        : QuickUsbApiGPIO.cpp
 Description  : QuickUSB QUSB2 Module GPIO API
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
 Purpose :  Read the data direction of each data port bit for the specified
                port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Port address
 Output  :  data - Buffer to store data direction bit values
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadPortDir(QHANDLE hDevice, QWORD address, PQBYTE data) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (address > 4)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_PORT, PORT_GETSET_DIR, address, data, sizeof(QBYTE), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Set the data direction of each data port bit for the specified port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Port address
            data - Buffer containing data direction bit values
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWritePortDir(QHANDLE hDevice, QWORD address, QBYTE data) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (address > 4) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_PORT, PORT_GETSET_DIR, address, &data, sizeof(QBYTE), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Read a series of bytes from the specified port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Port address
            data - Buffer to store port values
            length - Length to read from port
 Output  :  length - Length read from port
            Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadPort(QHANDLE hDevice, QWORD address, PQBYTE data, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize) || (address > 4)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_PORT, PORT_READWRITE, address, data, *length, &bytesReturned)) {
        return(FALSE);
    }

    // Use the length parameter to indicate the actual number of bytes read
    *length = (QWORD)bytesReturned;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Write a series of bytes to the specified port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Port address
            data - Buffer containing values to write to port
            length - Length to write to port
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWritePort(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length < 1) || (length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize) || (address > 4)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_PORT, PORT_READWRITE, address, data, length, &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}
