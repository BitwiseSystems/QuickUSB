/*=============================================================================
 Title        : QuickUsbApiSettings.cpp
 Description  : QuickUSB QUSB2 Module Settings and Defaults API
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



QRESULT FindDefaultsInEeprom(QHANDLE hDevice) {
/*---------------------------------------------------------------------------
 Purpose :  To find default(DFTL) tag in the EEPROM
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None
 Returns :  Non-zero on success, zero on failure
 Notes   :  None
 ---------------------------------------------------------------------------*/
    QRESULT result;
    PQBYTE eepromData;
    QWORD eepromAddress = 0;
    QWORD fileLength;
    QWORD length;
    QWORD four = 4;
    QWORD defaultAddress = 0;
    QWORD i;

    eepromData = (PQBYTE)malloc(16384);

    if (eepromData == NULL) {
        lastError = QUICKUSB_ERROR_OUT_OF_MEMORY;         
        return(FALSE);
    }

    // Check if this is an 8K or 16K EEPROM
    result = QuickUsbReadEepromData(hDevice, 0, eepromData + 0, &four);
    if (result == 0) {
        free (eepromData);
        return(FALSE);
    }

    result = QuickUsbReadEepromData(hDevice, 8192, eepromData + 4, &four);
    if (result == 0) {
        free (eepromData);
        return(FALSE);
    }

    // An 8K EEPROM will wrap after 8K
    if ((*(eepromData + 0) == *(eepromData + 4)) &&
        (*(eepromData + 1) == *(eepromData + 5)) &&
        (*(eepromData + 2) == *(eepromData + 6)) &&
        (*(eepromData + 3) == *(eepromData + 7))) {
            fileLength = 8192;
    }
    else {
        fileLength = 16384;
    }

    while (eepromAddress < fileLength) {
        // Calculate the block size
        if ((fileLength - eepromAddress) > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize) {
            length = QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize;
        }
        else {
            length = fileLength - eepromAddress;
        }

        // Read the data from the EEPROM
        result = QuickUsbReadEepromData(hDevice, eepromAddress,
            eepromData + eepromAddress, &length);

        if (result == 0) {               // bad read status???
            free(eepromData);
            return(FALSE);
        }

        // Search this chuck of memory for the DFLT section (plus previous 4 bytes in
        // case the section is in between chunks)
        result = FALSE;
        for (i = max(0, eepromAddress - 4); i < min(eepromAddress + length, fileLength - 4); ++i) {
            if (memcmp("DFLT", eepromData + i, 4) == 0) {
                defaultAddress = i + 4;
                result = TRUE;
                break;
            }
        }
        if (result) {
            break;
        }

        // Update the EEPROM address
        eepromAddress += length;
    }

    if (defaultAddress == 0) {          /* DFLT string not found??? */
        defaultAddress = 0xFFFF;         /* yes...use special value */
        result = FALSE;
    }

    QusbDeviceInfo[(QBYTE)hDevice].defaultOffset = defaultAddress;
    free(eepromData);
    return result;
}



QRETURN QuickUsbReadSetting(QHANDLE hDevice, QWORD address, PQWORD value) {
/*---------------------------------------------------------------------------
 Purpose :  To read QuickUSB module settings
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Address for request
 Output  :  value - Setting value
 Returns :  Non-zero on success, zero on failure
 Notes   :  None
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((address > 19) || (value == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_SETTING, address, 0, value, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



QRETURN QuickUsbWriteSetting(QHANDLE hDevice, QWORD address, QWORD value) {
/*---------------------------------------------------------------------------
 Purpose :  To write QuickUSB module settings
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Setting address
            value - Setting value
 Output  :  None
 Returns :  Non-zero on success, zero on failure
 Notes   :  None
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
	if (address > 19) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_SETTING, address, 0, &value, sizeof(QWORD), &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



QRETURN QuickUsbReadDefault(QHANDLE hDevice, QWORD address, PQWORD data) {
/*---------------------------------------------------------------------------
 Purpose :  To read QuickUSB modules defaults
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Default address
 Output  :  data - Default value
 Returns :  Non-zero on success, zero on failure.
 Notes   :  The defaults are non-volatile and are read into the settings table
            on power up.
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QBYTE index = (QBYTE)hDevice;
    QBYTE  EepromData[2];
    QWORD length = sizeof(QWORD);

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((address > 19) || (data == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Locate the defaults in EEPROM if they have not already been found
    if (QusbDeviceInfo[index].defaultOffset == 0) {
        // Check if this is old firmware without DFLT section
        if (!FindDefaultsInEeprom(hDevice) || QusbDeviceInfo[index].defaultOffset == 0xFFFF) {
            lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
            return(FALSE);
        }
    }

    // Read from the DFLT section of the EEPROM
    if (QuickUsbReadEepromData(hDevice, QusbDeviceInfo[index].defaultOffset + (address * sizeof(QWORD)), EepromData, &length)) {
        SWAPBYTES((PQBYTE)EepromData, (PQBYTE)data, length);

        // Do not allow the user to read our reserved patch status byte.
        if (address == SETTING_PATCHES) {
            *data &= 0x00FF;
        }
    }

    // Success
    return(TRUE);
}



QRETURN QuickUsbWriteDefault(QHANDLE hDevice, QWORD address, QWORD data) {
/*---------------------------------------------------------------------------
 Purpose :  To write QuickUSB modules defaults. Write 16 bits in the 32 byte
            DFLT section in EEPROM
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Default address
            data - Default value
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  The defaults are non-volatile and are read into the settings table
            on power up.
 ---------------------------------------------------------------------------*/
    QDEVHANDLE hDevHandle;
    QBYTE index = (QBYTE)hDevice;
    QBYTE  EepromData[2];
    QWORD length = sizeof(QWORD);
    QWORD patch_status;
    QWORD major, minor, build;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (address > 19) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Protect the user from rendering a device unusable by setting the QuickUSB device to
    // Full-Speed on firmware earlier than 2.15.0
    if ((address == SETTING_CPUCONFIG) && !(data & 0x8000)) {
        if (!QuickUsbGetFirmwareVersion(hDevice, &major, &minor, &build)) { // Sets lastError on failure
            return(FALSE);
        }
        if (!IsAtLeastVersion(2, 15, 0, major, minor, build)) {
            lastError = QUICKUSB_ERROR_PROTECTION;
            return(FALSE);
        }
    }

    // Locate the defaults in EEPROM if they have not already been found
    if (QusbDeviceInfo[index].defaultOffset == 0) {
        // Check if this is old firmware without DFLT section
        if (!FindDefaultsInEeprom(hDevice) || QusbDeviceInfo[index].defaultOffset == 0xFFFF) {
            lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
        return(FALSE);
        }
    }

    // Get the current firmware patch status setting.
    if (!QuickUsbReadEepromData(hDevice, QusbDeviceInfo[index].defaultOffset + (SETTING_PATCHES * sizeof(QWORD)), EepromData, &length)) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }

    SWAPBYTES((PQBYTE)EepromData, (PQBYTE)&patch_status, length);
    patch_status |= (PATCHED_DEFAULTS << 8);

    // Do not allow user writes to the patch status byte.
    if (address == SETTING_PATCHES) {
        data &= 0x00FF;
        data |= (patch_status & 0xFF00);
    }

    SWAPBYTES((PQBYTE)&data, (PQBYTE)EepromData, length);

    // Write to the DFLT section of the EEPROM
    if (!QuickUsbWriteEepromData(hDevice, QusbDeviceInfo[index].defaultOffset + (address * sizeof(QWORD)), EepromData, length)) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }

    if (address == SETTING_PATCHES) {
        return(TRUE);
    }

    // Set that we patched a default.
    SWAPBYTES((PQBYTE)&patch_status, (PQBYTE)EepromData, length);

    if (!QuickUsbWriteEepromData(hDevice, QusbDeviceInfo[index].defaultOffset + (SETTING_PATCHES * sizeof(QWORD)), EepromData, length)) {
        lastError = QUICKUSB_ERROR_IOCTL_FAILED;
        return(FALSE);
    }

    // Success
    return(TRUE);
}
