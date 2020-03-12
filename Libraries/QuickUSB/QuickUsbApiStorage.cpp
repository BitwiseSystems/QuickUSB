/*=============================================================================
 Title        : QuickUsbApiStorage.cpp
 Description  : QuickUSB QUSB2 Module Storage API
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



QRETURN QuickUsbWriteStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes) {
/*---------------------------------------------------------------------------
 Purpose :  Write a block of bytes to memory.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Byte offset into memory
            data - Buffer containing data to write to memory
            bytes - Length to write to memory
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  The address plus the number of bytes to write must not exceed 2048
            or the function will fail.
 ---------------------------------------------------------------------------*/
    QRESULT result;
    QBYTE payload[STORAGE_PAGE_SIZE + 2];
    QWORD bytesToWrite, numBytesWritten = 0, eepromAddress, i2ctl;

    // Validate parameters
    if ((data == 0) || (bytes < 1) || ((address + bytes) > STORAGE_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // We need to make sure that the I2C ignore ACK option is off
    result = QuickUsbReadSetting(hDevice, SETTING_I2CTL, &i2ctl);
    if (!result) {
        return(FALSE);
    }
    if (i2ctl & 0x80) {
        result = QuickUsbWriteSetting(hDevice, SETTING_I2CTL, i2ctl ^ 0x80);
        if (!result) {
            return(FALSE);
        }
    }

    while (numBytesWritten < bytes) {
        // Don't allow writes to go beyond a page or to cross page boundaries
        eepromAddress = STORAGE_BASE_ADDR + (address + numBytesWritten);
        bytesToWrite = min(min(bytes - numBytesWritten, QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize - 2), STORAGE_PAGE_SIZE - (eepromAddress % STORAGE_PAGE_SIZE));

        // Copy data to payload
        payload[0] = (eepromAddress >> 8) & 0xff;
        payload[1] = (eepromAddress & 0xff);
        memcpy(&payload[2], &data[numBytesWritten], bytesToWrite);

        // Write the data to EEPROM
        result = QuickUsbWriteI2C(hDevice, STORAGE_I2C_ADDR, payload, bytesToWrite + 2);
        if (!result) {
            break;
        }
        // The EEPROM will not ACK after a write, so we must wait for the 
        // EEPROM write cycle to complete
        do {
            result = QuickUsbWriteI2C(hDevice, STORAGE_I2C_ADDR, payload, 2);
        } while (!result && (lastError == QUICKUSB_ERROR_I2C_NO_ACK));

        if (!result) {
            break;
        }

        numBytesWritten += bytesToWrite;
    }

    // If the I2C ignore ACK option was on, we should be nice and turn it back on
    if (i2ctl & 0x80) {
        result &= QuickUsbWriteSetting(hDevice, SETTING_I2CTL, i2ctl);
    }

    // Return
    return(result);
}



QRETURN QuickUsbReadStorage(QHANDLE hDevice, QWORD address, QBYTE *data, QWORD bytes) {
/*---------------------------------------------------------------------------
 Purpose :  Read a block of bytes from memory.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Byte offset into memory
            bytes - Length to read from memory
 Output  :  data - Buffer containing data read from memory
 Returns :  Non-zero on success, zero on failure.
 Notes   :  The address plus the number of bytes to read must not exceed 2048
                or the function will fail.
 ---------------------------------------------------------------------------*/
    QRESULT result;
    QBYTE payload[STORAGE_PAGE_SIZE];
    QWORD bytesToRead, numBytesRead = 0, eepromAddress, i2ctl;

    // Validate parameters
    if ((data == 0) || (bytes < 1) || ((address + bytes) > STORAGE_SIZE)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // We need to make sure that the I2C ignore ACK option is off
    result = QuickUsbReadSetting(hDevice, SETTING_I2CTL, &i2ctl);
    if (!result) {
        return(FALSE);
    }
    if (i2ctl & 0x80) {
        result = QuickUsbWriteSetting(hDevice, SETTING_I2CTL, i2ctl ^ 0x80);
        if (!result) {
            return(FALSE);
        }
    }

    // Set starting address of the read
    eepromAddress = STORAGE_BASE_ADDR + address;
    payload[0] = (eepromAddress >> 8) & 0xff;
    payload[1] = (eepromAddress & 0xff);

    // The EEPROM will not ACK after a write, so we must wait for the 
    // EEPROM write cycle to complete
    do {
        result = QuickUsbWriteI2C(hDevice, STORAGE_I2C_ADDR, payload, 2);
    } while (!result && (lastError == QUICKUSB_ERROR_I2C_NO_ACK));

    if (result) {
        // Perform the reads
        while (numBytesRead < bytes) {
            bytesToRead = min(bytes - numBytesRead, QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);

            result = QuickUsbReadI2C(hDevice, STORAGE_I2C_ADDR, &data[numBytesRead], &bytesToRead);
            if (!result) {
                break;
            }

            numBytesRead += bytesToRead;
        }
    }

    // If the I2C ignore ACK option was on, we should be nice and turn it back on
    if (i2ctl & 0x80) {
        result &= QuickUsbWriteSetting(hDevice, SETTING_I2CTL, i2ctl);
    }

    // Return
    return(result);
}
