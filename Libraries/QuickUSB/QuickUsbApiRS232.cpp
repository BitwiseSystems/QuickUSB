/*=============================================================================
 Title        : QuickUsbApiRS232.cpp
 Description  : QuickUSB QUSB2 Module RS232 API
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
 Purpose :  Set the baud rate of both serial ports.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            baudRate - Baud rate in bits per second
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbSetRS232BaudRate(QHANDLE hDevice, QULONG baudRate) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (baudRate == 0) { // TODO: Perform more thorough validation of baud rate
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_RS232, RS232_SETGET_BAUD, 0, &baudRate, sizeof(QULONG), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Read the number of characters waiting in the received buffer.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - Serial port number
            length - Length to read from serial port
 Output  :  length - Length read from serial port
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetNumRS232(QHANDLE hDevice, QBYTE portNum, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QWORD numChars;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 1) || (length == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_RS232, RS232_SETGET_NUM, portNum, &numChars, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }
    
    // Return the number of bytes
    *length = (QULONG)numChars;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Flush the RS232 port transmit and receive buffers.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - Serial port number
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbFlushRS232(QHANDLE hDevice, QBYTE portNum) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QWORD numChars = 0;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (portNum > 1) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_RS232, RS232_SETGET_NUM, portNum, &numChars, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Read a block of characters from the interrupt receive buffer of the
                specified QuickUSB serial port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - Serial port number
            data - Buffer to store data read from the serial port
            length - Length to read from the serial port
 Output  :  length - Length read from the serial port
            Returns non-zero on success, zero on failure.
 Notes   :  The interrupt buffer is 128 bytes deep.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadRS232(QHANDLE hDevice, QBYTE portNum, PQBYTE data, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QULONG bytesReceived = 0;
    QLONG blockSize;
    PQBYTE block = data;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 1) || (data == 0) || (length == 0) || (*length < 1)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Read the data one block at a time until sent
    while (TRUE) {
        // Calculate the current block size
        blockSize = (QLONG)min(*length - (block - data), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);
        if (blockSize <= 0) {
            break;
        }

        // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
        if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_RS232, RS232_READWRITE, portNum, block, (QULONG)blockSize, &bytesReturned)) {
            return(FALSE);
        }

        // Keep a running count of the bytes received and increment the block pointer
        bytesReceived += bytesReturned;
        block += bytesReturned;
    }

    // Use the length parameter to indicate the actual number of bytes read
    *length = (QWORD) bytesReceived;

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  Write a block of characters to the specified QuickUSB serial port.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            portNum - Serial port number
            data - Buffer containing data to write to the serial port
            length - Length of buffer to write to the serial port
 Output  :  Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteRS232(QHANDLE hDevice, QBYTE portNum, PQBYTE data, QWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;
    QULONG bytesWritten = 0;
    QLONG blockSize;
    PQBYTE block = data;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((portNum > 1) || (data == 0) || (length < 1)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Read the data one block at a time until sent
    while (TRUE) {
        // Calculate the current block size
        blockSize = (QLONG)min(length - (block - data), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);
        if (blockSize <= 0) {
            break;
        }

        // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
        if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_RS232, RS232_READWRITE, portNum, block, (QULONG)blockSize, &bytesReturned)) {
            return(FALSE);
        }

        // Keep a running count of the bytes received and increment the block pointer
        bytesWritten += bytesReturned;
        block += blockSize;
    }

    // Return successfully
    return(TRUE);
}
