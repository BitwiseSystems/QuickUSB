/*=============================================================================
 Title        : QuickUsbApiEpcs.cpp
 Description  : QuickUSB QUSB2 Module EPCS API
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
#include "QuickUsbApiEpcs.h"



void BitFlip(PQBYTE value)
{
   QBYTE val, bit;

   // Bitflip the payload
   val = 0;
   for (bit = 0; bit < 8; bit++) {
      val <<= 1;
      val |= (*value >> bit) & 0x01;
   }
   *value = val;
}



QRETURN QuickUsbIdentifyEpcs(QHANDLE hDevice, QBYTE nSS, PQWORD epcsId, PQULONG epcsSize, QULONG flags) {
/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
    QRESULT qResult = TRUE;
    QBYTE dir, value;
    QBYTE spiData[5];
    QWORD len;
    QWORD spi, nCEport, nCEpin;

    // Validate parameters
    if ((nSS > 9) || ((epcsId == 0) && (epcsSize == 0))) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // In order for communication to the EPCS device to work, SPI must be configured
    // to be big-endian, CPOL=0, and CPHA=0
    qResult &= QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, &spi);
    if (!qResult) {
        return(FALSE);
    }
    spi |= 0x01;
    spi &= ~(0x06);
    qResult &= QuickUsbWriteSetting(hDevice, SETTING_SPICONFIG, spi);
    if (!qResult) {
        return(FALSE);
    }

    // Deassert nCE to disable the FPGA.
    // This allows QuickUSB to manipulate the EPCS device directly.
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Determine the port and pin location of the nCE line (it is configurable)
        nCEport = (spi & 0x08) ? PORT_A : PORT_E; // Bit 3 selects SPI port (0=Port E, 1=Port A)
        nCEpin = (spi & 0x10) ? 0x80 : 0x04; // Bit 4 selectes the nCE pin location (0=Pin 2, 1=Pin 7)

        // Check that the MISO pin is not on the same pin as nCE
        if ((spi & 0x20) && !(spi & 0x10)) {
            lastError = QUICKUSB_ERROR_INVALID_OPERATION;
            return(FALSE);
        }

        // Make the nCE pin an output
        qResult &= QuickUsbReadPortDir(hDevice, nCEport, &dir);
        if (!qResult) {
            return(FALSE);
        }
        dir |= nCEpin;
        qResult &= QuickUsbWritePortDir(hDevice, nCEport, dir);
        if (!qResult) {
            return(FALSE);
        }

        // Deassert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value |= nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Read the EPCS ID
    spiData[0] = EPCS_READ_ID;
    spiData[1] = 0;
    spiData[2] = 0;
    spiData[3] = 0;
    spiData[4] = 0;
    qResult &= QuickUsbWriteReadSpi(hDevice, nSS, spiData, 5);
    if (qResult){
        // Output the EPCS ID and capacity
        switch (spiData[4]) {
        case QUICKUSB_EPCS1_ID:
            if (epcsId) {
                *epcsId = QUICKUSB_EPCS1_ID;
            }
            if (epcsSize) {
                *epcsSize = QUICKUSB_EPCS1_SIZE;
            }
            break;

        case QUICKUSB_EPCS4_ID:
            if (epcsId) {
                *epcsId = QUICKUSB_EPCS4_ID;
            }
            if (epcsSize) {
                *epcsSize = QUICKUSB_EPCS4_SIZE;
            }
            break;

        case QUICKUSB_EPCS16_ID:
            if (epcsId) {
                *epcsId = QUICKUSB_EPCS16_ID;
            }
            if (epcsSize) {
                *epcsSize = QUICKUSB_EPCS16_SIZE;
            }
            break;

        case QUICKUSB_EPCS64_ID:
            if (epcsId) {
                *epcsId = QUICKUSB_EPCS64_ID;
            }
            if (epcsSize) {
                *epcsSize = QUICKUSB_EPCS64_SIZE;
            }
            break;

        default:
            if (epcsId) {
                *epcsId = QUICKUSB_EPCS_ID_UNKNOWN;
            }
            if (epcsSize) {
                *epcsSize = 0;
            }
            break;
        }
    }

    // Enable the FPGA
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Assert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value &= ~nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Return
    return(qResult);
}



QRETURN QuickUsbConfigureEpcs(QHANDLE hDevice, QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
    QRESULT qResult;
    QBYTE dir, value;
    QBYTE spiStatus[2];
    QBYTE spiData[64];
    QWORD len = 0, count;
    QULONG blockLength = 32;
    QULONG index = 0;
    QULONG fileSize, deviceSize;
    QULONG length;
    FILE *input;
    QWORD id;
    QWORD percent, newPercent;
    QWORD spi, nCEport, nCEpin;
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
#endif

    // Validate parameters
    if ((nSS > 9) || (filePath == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Ensure an EPCS is connected
    qResult = QuickUsbIdentifyEpcs(hDevice, nSS, &id, &deviceSize, 0);
    if (!qResult) {
        return(FALSE);
    }
    if (id == QUICKUSB_EPCS_ID_UNKNOWN) {
        lastError = QUICKUSB_ERROR_EPCS_NOT_FOUND;
        return(FALSE);
    }

    // Open the input file for reading
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&input, filePath, "rb");
    if (status != 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
    input = fopen(filePath, "rb");
    if (input == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif
#elif DRIVER==LinuxDriver
    input = fopen(filePath, "rb");
    if (input == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#elif DRIVER==MacOSXDriver
    input = fopen((const char *)filePath, "rb");
    if (input == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif

    // Calculate the file length
    if (fseek(input, 0, SEEK_END)) {
        fclose(input);
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }
    fileSize = ftell(input);
    if (fseek(input, 0, SEEK_SET)) {
        fclose(input);
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }

    // Verify that the file will fit in the EPCS device
    if (fileSize > deviceSize) {
        fclose(input);
        lastError = QUICKUSB_ERROR_EPCS_TOO_SMALL;
        return(FALSE);
    }

    // Start
    if (callback) {
        percent = 0;
        newPercent = 0;
        callback(hDevice, 0, tag);
    }
	
    // Deassert nCE to disable the FPGA.
    // This allows QuickUSB to manipulate the EPCS device directly.
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Determine the port and pin location of the nCE line (it is configurable)
        qResult &= QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, &spi);
        if (!qResult) {
            return(FALSE);
        }
        nCEport = (spi & 0x08) ? PORT_A : PORT_E; // Bit 3 selects SPI port (0=Port E, 1=Port A)
        nCEpin = (spi & 0x10) ? 0x80 : 0x04; // Bit 4 selectes the nCE pin location (0=Pin 2, 1=Pin 7)

        // Check that the MISO pin is not on the same pin as nCE
        if ((spi & 0x20) && !(spi & 0x10)) {
            lastError = QUICKUSB_ERROR_INVALID_OPERATION;
            return(FALSE);
        }

        // Make the nCE pin an output
        qResult &= QuickUsbReadPortDir(hDevice, nCEport, &dir);
        if (!qResult) {
            return(FALSE);
        }
        dir |= nCEpin;
        qResult &= QuickUsbWritePortDir(hDevice, nCEport, dir);
        if (!qResult) {
            return(FALSE);
        }

        // Deassert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value |= nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Make sure the EPCS device is not busy
    do {
        // Read the EPCS status
        spiStatus[0] = EPCS_READ_STATUS;
        spiStatus[1] = 0;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiStatus, 2);
        if (!qResult) {
            fclose(input);
            return(FALSE);
        }   
    } while (spiStatus[1] & EPCS_STATUS_WIP);

    // Perform a write enable
    if (!(flags & QUICKUSB_EPCS_SKIP_ERASE)) {
        spiData[0] = EPCS_WRITE_ENABLE;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 1);
        if (!qResult) {
            fclose(input);
            return(FALSE);
        }

        // Perform an erase bulk
        spiData[0] = EPCS_ERASE_BULK;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 1);
        if (!qResult) {
            fclose(input);
            return(FALSE);
        }

        // Wait the typical amount of bulk erase time (Do this because it
        // seems that polling the status register is unreliable)
        switch (id) {
            case QUICKUSB_EPCS1_ID:
                SleepMS(3000);
                break;
            case QUICKUSB_EPCS4_ID:
                SleepMS(5000);
                break;
            case QUICKUSB_EPCS16_ID:
                SleepMS(17000);
                break;
            case QUICKUSB_EPCS64_ID:
                SleepMS(68000);
                break;
        }

        // Wait for the erase bulk to complete
        do {
            // Read the EPCS status
            spiStatus[0] = EPCS_READ_STATUS;
            spiStatus[1] = 0;
            qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiStatus, 2);
            if (!qResult) {
                fclose(input);
                return(FALSE);
            }
        } while (spiStatus[1] & EPCS_STATUS_WIP);
    }

    // Send the FPGA configuration data one block at a time
    do {
        // Calculate the next block size
        length = min(blockLength, fileSize - index);

        // Perform a write enable
        spiData[0] = EPCS_WRITE_ENABLE;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 1);
        if (!qResult) {
            fclose(input);
            return(FALSE);
        }

        // Set the opcode and the address
        spiData[0] = EPCS_WRITE_BYTES;
        spiData[1] = (index / 0x10000) & 0xFF;
        spiData[2] = (index / 0x100) & 0xFF;
        spiData[3] = index & 0xFF;

        // Read the data from the file and put it into fpgaData array
        fread(spiData+4, 1, length, input);

        // Bitflip the payload
        for (count = 0; count < length; count++) {
            BitFlip(spiData+count+4);
        }

        // Write the data to the EEPROM
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, (unsigned short) length + 4);
        if (!qResult) {
            fclose(input);
            return(FALSE);
        }

        // Make sure the EPCS device is not busy
        do {
            // Read the EPCS status
            spiStatus[0] = EPCS_READ_STATUS;
            spiStatus[1] = 0;
            qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiStatus, 2);
            if (!qResult) {
                fclose(input);
                return(FALSE);
            }   
        } while (spiStatus[1]/* & EPCS_STATUS_WIP*/);

        // Update the counters
        index = index + length;

        // Call progress callback
        if (callback) {
            newPercent = (QWORD)((index * 100) / fileSize);
            if ((newPercent != percent) && (newPercent != 100)) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }
    } while(index < fileSize);

    // Close the file
    fclose(input);

    // Enable the FPGA
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Assert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value &= ~nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Finish our progress updates
    if (callback) {
        callback(hDevice, 100, tag);
    }

    // Success
    return(TRUE);
}



QRETURN QuickUsbVerifyEpcs(QHANDLE hDevice, QBYTE nSS, PCQCHAR filePath, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
    QRESULT qResult;
    QWORD id;
    QULONG deviceSize, fileSize;
    FILE *input;
    QBYTE dir, value;
    QWORD len, count;
    QULONG errorCount, length, blockLength = 32, index = 0;
    QBYTE spiData[64], spiDataRead[64];
    QWORD percent, newPercent;
    QWORD spi, nCEport, nCEpin;
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
#endif

    // Validate parameters
    if ((nSS > 9) || (filePath == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Ensure an EPCS is connected
    qResult = QuickUsbIdentifyEpcs(hDevice, nSS, &id, &deviceSize, 0);
    if (!qResult) {
        return(FALSE);
    }
    if (id == QUICKUSB_EPCS_ID_UNKNOWN) {
        lastError = QUICKUSB_ERROR_EPCS_NOT_FOUND;
        return(FALSE);
    }

    // Open the input file for reading
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&input, filePath, "rb");
    if (status != 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
    input = fopen(filePath, "rb");
    if (input == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif
#elif DRIVER==LinuxDriver
    input = fopen(filePath, "rb");
    if (input == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#elif DRIVER==MacOSXDriver
    input = fopen((const char *)filePath, "rb");
    if (input == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif

    // Calculate the file length
    if (fseek(input, 0, SEEK_END)) {
        fclose(input);
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }
    fileSize = ftell(input);
    if (fseek(input, 0, SEEK_SET)) {
        fclose(input);
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }

    // Verify that the file will fit in the EPCS device
    if (fileSize > deviceSize) {
        fclose(input);
        lastError = QUICKUSB_ERROR_EPCS_TOO_SMALL;
        return(FALSE);
    }

    // Start
    if (callback) {
        percent = 0;
        newPercent = 0;
        callback(hDevice, 0, tag);
    }
	
    // Deassert nCE to disable the FPGA.
    // This allows QuickUSB to manipulate the EPCS device directly.
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Determine the port and pin location of the nCE line (it is configurable)
        qResult &= QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, &spi);
        if (!qResult) {
            return(FALSE);
        }
        nCEport = (spi & 0x08) ? PORT_A : PORT_E; // Bit 3 selects SPI port (0=Port E, 1=Port A)
        nCEpin = (spi & 0x10) ? 0x80 : 0x04; // Bit 4 selectes the nCE pin location (0=Pin 2, 1=Pin 7)

        // Check that the MISO pin is not on the same pin as nCE
        if ((spi & 0x20) && !(spi & 0x10)) {
            lastError = QUICKUSB_ERROR_INVALID_OPERATION;
            return(FALSE);
        }

        // Make the nCE pin an output
        qResult &= QuickUsbReadPortDir(hDevice, nCEport, &dir);
        if (!qResult) {
            return(FALSE);
        }
        dir |= nCEpin;
        qResult &= QuickUsbWritePortDir(hDevice, nCEport, dir);
        if (!qResult) {
            return(FALSE);
        }

        // Deassert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value |= nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Read the FPGA configuration data one block at a time
    errorCount = 0;
    do {      
        // Calculate the next block size
        length = min(blockLength, fileSize - index);

        // Set the opcode and the address
        spiDataRead[0] = EPCS_READ_BYTES;
        spiDataRead[1] = (index / 0x10000) & 0xFF;
        spiDataRead[2] = (index / 0x100) & 0xFF;
        spiDataRead[3] = index & 0xFF;
        memset(spiDataRead + 4, 0, length);

        // Read data from the EEPROM
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiDataRead, (unsigned short) length+4);
        if (qResult == 0){
            fclose(input);
            return(FALSE);
        }

        // Read the data from the file and put it into fpgaData array
        fread(spiData, 1, length, input);

        // Bitflip the payload
        for (count = 0; count < length; count++) {
            BitFlip(spiData+count);
        }

        // Verify the payload
        for (count = 0; count < length; count++) {
            if (spiDataRead[count+4] != spiData[count]) {
                errorCount++;
                break;
            }
        }
        if (errorCount) {
            break;
        }

        // Update the counters
        index = index + length;

        // Call progress callback
        if (callback) {
            newPercent = (QWORD)((index * 100) / fileSize);
            if ((newPercent != percent) && (newPercent != 100)) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }
    } while(index < fileSize);

    // Close the file
    fclose(input);

    // Enable the FPGA
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Assert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value &= ~nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // If we had any errors return unsuccessfully
    if (errorCount) {
        lastError = QUICKUSB_ERROR_VERIFY_FAILED;
        return(FALSE);
    }

    // Finish our progress updates
    if (callback) {
        callback(hDevice, 100, tag);
    }

    // Success
    return(TRUE);
}



QRETURN QuickUsbEraseEpcs(QHANDLE hDevice, QBYTE nSS, QULONG flags, PQPROGRESS_CALLBACK callback, PQVOID tag) {
/*---------------------------------------------------------------------------
 Purpose :  
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
 Output  :  None
 Returns :  Non-zero on success, zero on failure.
 Notes   :  
 ---------------------------------------------------------------------------*/
    QRESULT qResult;
    QWORD id;
    QBYTE dir, value;
    QBYTE spiData[64];
    QWORD len;
    QWORD spi, nCEport, nCEpin;
    clock_t stime;
    QULONG waitTime, elapsedTime;

    // Validate parameters
    if (nSS > 9) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Ensure an EPCS is connected
    qResult = QuickUsbIdentifyEpcs(hDevice, nSS, &id, NULL, 0);
    if (!qResult) {
        return(FALSE);
    }
    if (id == QUICKUSB_EPCS_ID_UNKNOWN) {
        lastError = QUICKUSB_ERROR_EPCS_NOT_FOUND;
        return(FALSE);
    }

    // Start
    if (callback) {
        callback(hDevice, 0, tag);
    }
	
    // Deassert nCE to disable the FPGA.
    // This allows QuickUSB to manipulate the EPCS device directly.
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Determine the port and pin location of the nCE line (it is configurable)
        qResult &= QuickUsbReadSetting(hDevice, SETTING_SPICONFIG, &spi);
        if (!qResult) {
            return(FALSE);
        }
        nCEport = (spi & 0x08) ? PORT_A : PORT_E; // Bit 3 selects SPI port (0=Port E, 1=Port A)
        nCEpin = (spi & 0x10) ? 0x80 : 0x04; // Bit 4 selectes the nCE pin location (0=Pin 2, 1=Pin 7)

        // Check that the MISO pin is not on the same pin as nCE
        if ((spi & 0x20) && !(spi & 0x10)) {
            lastError = QUICKUSB_ERROR_INVALID_OPERATION;
            return(FALSE);
        }

        // Make the nCE pin an output
        qResult &= QuickUsbReadPortDir(hDevice, nCEport, &dir);
        if (!qResult) {
            return(FALSE);
        }
        dir |= nCEpin;
        qResult &= QuickUsbWritePortDir(hDevice, nCEport, dir);
        if (!qResult) {
            return(FALSE);
        }

        // Deassert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value |= nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Make sure the EPCS device is not busy
    do {
        // Read the EPCS status
        spiData[0] = EPCS_READ_STATUS;
        spiData[1] = 0;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 2);
        if (!qResult) {
            return(FALSE);
        }   
    } while (spiData[1] & EPCS_STATUS_WIP);

    // Perform a write enable
    spiData[0] = EPCS_WRITE_ENABLE;
    qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 1);
    if (!qResult) {
        return(FALSE);
    }

    // Perform an erase bulk
    spiData[0] = EPCS_ERASE_BULK;
    qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 1);
    if (!qResult) {
        return(FALSE);
    }

    // Wait the typical amount of bulk erase time (Do this because it
    // seems that polling the status register is unreliable)
    waitTime = 0;
    switch (id) {
        case QUICKUSB_EPCS1_ID:
            waitTime = 3 * 1000;
            break;
        case QUICKUSB_EPCS4_ID:
            waitTime = 5 * 1000;
            break;
        case QUICKUSB_EPCS16_ID:
            waitTime = 17 * 1000;
            break;
        case QUICKUSB_EPCS64_ID:
            waitTime = 68 * 1000;
            break;
        default:
            waitTime = 0;
            break;
    }

    stime = clock();
    elapsedTime = 0;
    do {
        // Don't hog processor time
        SleepMS(50);
        elapsedTime += 50;

        // Call progress callback.  We can't return a progress, so just continually
        // return 0 until we finish.
        if (callback) {
            callback(hDevice, 0, tag);
        }
    } while (elapsedTime < waitTime);

    // Wait for the erase bulk to complete
    do {
        // Read the EPCS status
        spiData[0] = EPCS_READ_STATUS;
        spiData[1] = 0;
        qResult = QuickUsbWriteReadSpi(hDevice, nSS, spiData, 2);
        if (!qResult) {
            return(FALSE);
        }

        // Call progress callback.  We can't return a progress, so just continually
        // return 0 until we finish.
        if (callback) {
            callback(hDevice, 0, tag);
        }
    } while (spiData[1] & EPCS_STATUS_WIP);   

    // Enable the FPGA
    if (!(flags & QUICKUSB_EPCS_IGNORE_NCE)) {
        // Assert the nCE pin
        len = 1;
        qResult &= QuickUsbReadPort(hDevice, nCEport, &value, &len);
        if (!qResult) {
            return(FALSE);
        }
        value &= ~nCEpin;
        qResult &= QuickUsbWritePort(hDevice, nCEport, &value, 1);
        if (!qResult) {
            return(FALSE);
        }
    }

    // Finish our progress updates
    if (callback) {
        callback(hDevice, 100, tag);
    }

    // Success
    return(TRUE);
}
