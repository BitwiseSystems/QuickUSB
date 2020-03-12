/*=============================================================================
 Title        : QuickUsbApiIIC.cpp
 Description  : QuickUSB QUSB2 Module I2C API
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
 Purpose :  To get I2C error.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None.
 Notes   :  This function is called when an error is detected from an I2C read
                or write. It gets the I2C error status from the firmware, and
                translates it to a QuickUSB error code. this feature requires
                firmware 2.11 RC 7 or later to work correctly.
 ---------------------------------------------------------------------------*/
void GetI2CError(QHANDLE hDevice) {
    unsigned short I2CStatus;
    unsigned long status;

    // The I2C error status is in the high order byte of setting 8, starting
    // with firmware 2.11 RC 7.
    status = QuickUsbReadSetting(hDevice, SETTING_I2CTL, &I2CStatus);

    if (status == TRUE) {
        // These values must match the values in FX2.H from the firmware.
        I2CStatus = I2CStatus >> 8;
        switch (I2CStatus) {
        case 6:                          // I2C_BERROR
            lastError = QUICKUSB_ERROR_I2C_BUS_ERROR;
            return;
        case 7:                          // I2C_NACK
            lastError = QUICKUSB_ERROR_I2C_NO_ACK;
            return;
        case 10:                         // I2C_SLAVE_WAIT
            lastError = QUICKUSB_ERROR_I2C_SLAVE_WAIT;
            return;
        case 11:                         // I2C_TIMEOUT
            lastError = QUICKUSB_ERROR_I2C_TIMEOUT;
            return;
        }
    }

    lastError = QUICKUSB_ERROR_IOCTL_FAILED;
}



/*---------------------------------------------------------------------------
 Purpose :  Read a block of bytes from the specified I2C port
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - I2C device address
            length - Length to read from I2C
 Output  :  data - Buffer containing data read from I2C
            length - Length read from I2C
            Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadI2C(QHANDLE hDevice, QWORD address, PQBYTE data, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0) || (*length < 1) || (*length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_I2C, I2C_READWRITE, address, data, *length, &bytesReturned)) {
        // Set last error based on I2C status
        GetI2CError(hDevice);
        return(FALSE);
    }

    // Use the length parameter to indicate the actual number of bytes read
    *length = (QWORD)bytesReturned;

    // Success
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Write a block of bytes to the specified I2C port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - I2C device address
            data - Buffer containing data to write to I2C
            length - Length to write to I2C
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteI2C(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length) {
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

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_I2C, I2C_READWRITE, address, data, length, &bytesReturned)) {
        // Set last error based on I2C status
        GetI2CError(hDevice);
        return(FALSE);
    }

    // Success
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Write a block of bytes to the specified I2C port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - I2C device address
            data - Buffer containing data to write to I2C
            length - Length to write to I2C
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbCachedWriteI2C(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD length) {
    return QuickUsbWriteI2C(hDevice, address | (length << 8), data, length);
}
