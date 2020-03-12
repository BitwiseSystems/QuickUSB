/*=============================================================================
 Title        : QuickUsbApiProgram.cpp
 Description  : QuickUSB QUSB2 Module EEPROM Programming API
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
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, Suite E,
 Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/
#include "QuickUsbApiProgram.h"



/*---------------------------------------------------------------------------
 Purpose :  To write data to the EEPROM.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Address for request
            data - Data to EEPROM
            length - Size of data buffer
 Output  :  length - Size of written EEPROM data
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteEepromData(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length) {
    QDEVHANDLE hDevHandle;
    QRESULT result;
    QBYTE payload[STORAGE_PAGE_SIZE + 2];
    QWORD bytesToWrite, numBytesWritten = 0, eepromAddress;
    QULONG bytesReturned;
    QWORD major, minor, build;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (data == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Get the current firmware version
    result = QuickUsbGetFirmwareVersion(hDevice, &major, &minor, &build);
    if (!result) {
        // This function could fail because the device is not running from QuickUSB firmware
        // loaded from EEPROM (for example, after a bix file is sent to RAM before programming
        // the EEPROM).  In this case we'll simply fake that the firmware version is really
        // old so that norrmal I2C commands will be used to write the EEPROM.
        major = 0;
        minor = 0;
        build = 0;  // TODO: This hack won't work for QUSB3!
    }

    // Break large requests into smaller requests
    while (numBytesWritten < length) {
        // Don't allow writes to go beyond a page or to cross page boundaries
        eepromAddress = (address + numBytesWritten);

        if (IsAtLeastVersion(3, 0, 0, major, minor, build)) {
            // With v3.0.0 of firmware or later, the firmware handles all the weirdness with writing to flash,
            // so all we must do is sent the data as quickly as possibly using the max control transfer size

            // We can write at most (MAX_CNTL_XFER_SIZE) bytes
            bytesToWrite = min((QWORD)(length - numBytesWritten), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);

            // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
            if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_EEPROM, 0, eepromAddress, &data[numBytesWritten], bytesToWrite, &bytesReturned)) {
                return(FALSE);
            }
        } else if (IsAtLeastVersion(2, 15, 0, major, minor, build)) {
            // IMPROVEMENT: Versions of firmware older than v2.15.0 had such a poor firmware
            // implementation of the QUICKUSB_EEPROM command that normal I2C commands to R/W
            // the firmware EEPROM were faster.  v2.15.0 and later versions of firmware have
            // a better implementation, so depending on our firmware we should choose the
            // faster method.

            // We can write at most (MAX_CNTL_XFER_SIZE) bytes
            bytesToWrite = min(min((QWORD)(length - numBytesWritten), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize), STORAGE_PAGE_SIZE - (eepromAddress % STORAGE_PAGE_SIZE));

            // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
            if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_EEPROM, 0, eepromAddress, &data[numBytesWritten], bytesToWrite, &bytesReturned)) {
                return(FALSE);
            }

            // NOTE: In v2.15.0 firmware and later the QUICKUSB_EEPROM command automatically
            // waits for the EEPROM write cycle to complete so we don't need to wait here.
        } else {
            // We can write at most (MAX_CNTL_XFER_SIZE - 2) bytes since we must fit in the address
            bytesToWrite = min(min((QWORD)(length - numBytesWritten), QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize - 2), STORAGE_PAGE_SIZE - (eepromAddress % STORAGE_PAGE_SIZE));

            // Copy data to payload
            payload[0] = (eepromAddress >> 8) & 0xff;
            payload[1] = (eepromAddress & 0xff);
            memcpy(&payload[2], &data[numBytesWritten], bytesToWrite);

            // Write the data to EEPROM
            if (!QuickUsbWriteI2C(hDevice, STORAGE_I2C_ADDR, payload, bytesToWrite + 2)) {
                return(FALSE);
            }

            // The maximum write cycle time for the EEPROM is 5ms, so we will wait for 
            // 25 ms just to be absolutely certain that the write cycle has completed.
            SleepMS(25);
        }

        // Keep track of the number of bytes written
        numBytesWritten += bytesToWrite;
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To read data from the EEPROM.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Address for request
            length - Size of data buffer
 Output  :  data - Data from EEPROM
            length - Size of EEPROM data
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadEepromData(QHANDLE hDevice, QWORD address, PQBYTE data, PQWORD length) {
    QDEVHANDLE hDevHandle;
    QWORD totalBytes, bytesToRead;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    totalBytes = 0;
    do {
        // Don't read more than can fit in a USB control packet
        bytesToRead = min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, (*length - totalBytes));

        // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
        if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_IN, QUICKUSB_EEPROM, 0, address + totalBytes, &data[totalBytes], bytesToRead, &bytesReturned)) {
            return(FALSE);
        }

        // Move past the read data
        totalBytes += (QWORD) bytesReturned;
    } while (totalBytes < *length);

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose :  To write data to RAM.
 Input   :  hDevice - Offset into array of _QUSB_DEVICE_INFO structures
            address - Address for request
            data - Data to RAM
            length - Size of data buffer
 Output  :  length - Size of written RAM data
            Returns non-zero on success, zero on failure.
 Notes   :  None.
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteRamData(QHANDLE hDevice, QWORD address, PQBYTE data, QWORD length) {
    QDEVHANDLE hDevHandle;
    QULONG bytesReturned;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((data == 0) || (length > QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
    if (!QuickUsbVendorRequest(hDevice, hDevHandle, QUICKUSB_REQUEST_OUT, FIRMWARE_LOAD, 0, address, data, length, &bytesReturned)) {
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



// Note: The firmware that is read out of a module is encrypted and is not exactly the same as that contained
// in a QUSB file.  Some of the differences include the descriptor strings, customized srings, customized PID,
// the patch setting, and patched GPIF data.
QUSBLIB QRETURN QuickUsbReadFirmware(QHANDLE hDevice, PCQCHAR fileName, PQPROGRESS_CALLBACK callback, PQVOID tag) {
	QWORD Index;
	QLONG result;
	QWORD length;
	QWORD fileLength;
	FILE *outputFile;
	QBYTE *eepromData;
    QWORD percent, newPercent;
    QBYTE currentByte;
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
	
    // Start
    if (callback) {
        percent = 0;
        newPercent = 0;
        callback(hDevice, 0, tag);
    }
	
	// Calculate the file length
	fileLength = 8192;
	eepromData = (PQBYTE) malloc(fileLength);
	
	// Fill the EEPROM data with 0xFF
	for (Index = 0; Index < fileLength; ++Index) {
		eepromData[Index] = 0xFF;
	}

	// Open the input file for writing
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&outputFile, fileName, "wb");
    if (status != 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
	outputFile = fopen(fileName, "wb");
	if (outputFile == 0)
	{
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
		return(FALSE);
	}
#endif
	
	// Read the EEPROM data
	Index = 0;
	do {
		// Calculate the block size
		length = min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, fileLength - Index);
		result = QuickUsbReadEepromData(hDevice, Index, &eepromData[Index], &length);
		
		if (result == 0) {
            // QuickUsbReadEepromData sets lastError
			return(FALSE);
		}

        if (callback) {
            newPercent = 100 * Index / fileLength;
            if (newPercent != percent) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }
		Index = Index + length;
	} while (Index < fileLength);

	// Ecrypt file
	for (Index = 0; Index < fileLength; ++Index) {
		currentByte = eepromData[Index];
		currentByte = currentByte ^ 187;
		currentByte = currentByte + 15;
		eepromData[Index] = currentByte & 0xff;
	}

	// Write the disk file
	fwrite(eepromData, sizeof(QBYTE), fileLength, outputFile);
	
	// Close the file
	fclose(outputFile);

    // End
    if (callback) {
        callback(hDevice, 100, tag);
    }
	
	return(TRUE);
}


// TODO: Ensure that the I2C IgnoreACK setting is not set (could cause corruption of EEPROM)
// TODO: Verify patch settings are being read and written properly
QUSBLIB QRETURN QuickUsbWriteFirmware(QHANDLE hDevice, PCQCHAR fileName, QULONG options, PQPROGRESS_CALLBACK callback, PQVOID tag) {
	QWORD Index;
	QULONG OutIndex;
	QLONG result;
	QWORD length;
	QWORD address;
	QLONG realFileLength;
	FILE *inputFile = 0;
	QWORD currentByte;
	QBOOL encrypt;
	QBYTE *SavedSerial = 0;
	QWORD SerialNumLength;
	QBOOL OldSerialPresent;
	PQCHAR NewSerial = 0;
	QWORD filePatchAddress;
	QWORD eepromPatchAddress;
	QWORD fileStringAddress;
	QWORD eepromStringAddress;
	QWORD fileDefaultAddress;
	QWORD eepromDefaultAddress;
	QWORD fileAddress;
	QWORD eepromAddress;
	QULONG fileCurString;
	QULONG eepromCurString;
	QWORD fileStringLen;
	QWORD eepromStringLen;
	QWORD StringIndex;
	QBOOL Fx2lpPart;
	QBOOL Fx2lpCode;
	QWORD revid;
	QCHAR eepromPatches;
	QCHAR filePatches;
	QBYTE *eepromData = 0;
	QBYTE *fileData = 0;
	QBYTE *eepromBinaryData = 0;
	QBYTE *fileBinaryData = 0;
	QULONG eepromPid;
	QULONG filePid;
	QULONG PidLo;
	QULONG PidHi;
	QCHAR eepromIoModel[100];
	QCHAR fileIoModel[100];
    QWORD percent, newPercent;
    QBOOL oldFirmware;
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif

    // Validate options
    if (options == 0) {
        options = (QUICKUSB_PRESERVE_CUSTOMIZATION | QUICKUSB_PRESERVE_DEFAULTS | QUICKUSB_PRESERVE_SERIAL);
    } else {
        if ((options & QUICKUSB_PROGRAM_SERIAL)) {
            if (options & QUICKUSB_PRESERVE_SERIAL) {
                lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
                return(FALSE);
            }
            if ((options & QUICKUSB_PROGRAM_SERIAL_MASK) == 0) {
                lastError = QUICKUSB_ERROR_INVALID_SERIAL;
                return(FALSE);
            }
        }
    }

    // Start
    if (callback) {
        percent = 0;
        newPercent = 0;
        callback(hDevice, 0, tag);
    }

	// Initialize variables
	filePatches = 0;

	// Open the input file for reading
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&inputFile, fileName, "rb");
    if (status != 0) {
	    //QuickUsbClose(hDevice);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
	inputFile = fopen(fileName, "rb");
	if (inputFile == 0)
	{
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
		return(FALSE);
	}
#endif

	// Get the file length
	fseek(inputFile, 0L, SEEK_END); 
	realFileLength = ftell(inputFile); 
	fseek(inputFile, 0L, SEEK_SET); 

	// Allocate memory
	eepromData = (PQBYTE) malloc(MAX_16K_ADDRESS);
	fileData = (PQBYTE) malloc(realFileLength);
	eepromBinaryData = (PQBYTE) malloc(UNCOMPRESSED_FILE_SIZE);
	fileBinaryData = (PQBYTE) malloc(UNCOMPRESSED_FILE_SIZE);
	
	// Read and close input firmware file
	fread(fileData, sizeof(QCHAR), realFileLength, inputFile);
	fclose(inputFile);
	inputFile = 0;
	
	// Read old firmware from QuickUSB device
	Index = 0;
	do {
		// Calculate the block size
		length = (QWORD)min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, MAX_16K_ADDRESS - Index);
		result = QuickUsbReadEepromData(hDevice, Index, &eepromData[Index], &length);
		if (result == 0) {
			free(eepromData);
			free(fileData);
			free(eepromBinaryData);
			free(fileBinaryData);
            // QuickUsbReadEepromData sets lastError
			return(FALSE);
		}

        if (callback) {
            newPercent = 50 * Index / MAX_16K_ADDRESS; // 50 % of time spent here
            if (percent != newPercent) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }

		Index = Index + length;
	} while (Index < MAX_16K_ADDRESS);

	// Expand the old firmware
	result = IicExpand(eepromData, MAX_16K_ADDRESS, eepromBinaryData, UNCOMPRESSED_FILE_SIZE);
	
	// Find and check the patch status from the old firmware
	eepromDefaultAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"DFLT");
	if (eepromDefaultAddress != 0) {
		eepromPatchAddress = eepromDefaultAddress + (SETTING_PATCHES * 2);
		eepromPatches = eepromBinaryData[eepromPatchAddress];
	}
	else {
		eepromPatches = 0;
	}

	// Find and save the I/O model and serial number from the old firmware
	eepromStringAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"STRS");
	address = eepromStringAddress;
    OldSerialPresent = FALSE;
	if (address != 0) {
		// The I/O model is in the 4th string, step over strings 0-3
		// The serial number is in the 5th string, step over strings 0-4
		for (Index = 0; Index <= 3; ++Index) {
			address = address + eepromBinaryData[address];
		}
		
		// Save the old I/O model
		if (eepromBinaryData[address + 1] == 3) {
			OutIndex = 1;
			// Find start of I/O model string (it is inside parentheses)
			for (Index = 0; Index <= (min(eepromBinaryData[address] - 2, 100) - 1); ++Index) {
				if (eepromBinaryData[address + Index + 2] == '(') {
					OutIndex = 0;
					break;
				}
			}
			
			if (OutIndex == 0) {
				// Save the I/O model string
				for (Index = Index; Index <= (min(eepromBinaryData[address] - 2, 100) - 1); ++Index) {
					eepromIoModel[OutIndex] = eepromBinaryData[address + Index + 2];
					if (eepromBinaryData[address + Index + 2] == ')') {
						break;
					}
					OutIndex = OutIndex + 1;
				}
			}
		}
		
		// The serial number is in the 5th string, step over string 4
		address = address + eepromBinaryData[address];
		if (eepromBinaryData[address + 1] == 3) {
			SerialNumLength = eepromBinaryData[address] - 2;
			if (SerialNumLength > 4) {
				SavedSerial = (PQBYTE) malloc(SerialNumLength);
				for (Index = 0; Index < SerialNumLength; ++Index) {
					SavedSerial[Index] = eepromBinaryData[address + Index + 2];
					if ((SavedSerial[Index] != 0) && (SavedSerial[Index] != '0') && ((options & QUICKUSB_PRESERVE_SERIAL) || (!(options & QUICKUSB_PRESERVE_MASK) && !(options & QUICKUSB_PROGRAM_SERIAL)))) {
						OldSerialPresent = TRUE;
					}
				}
			}
		}
	}
	
	// If serial number is not set, use the first two bytes of the options parameter
    // with the serial option flag to set the serial
	if (OldSerialPresent == FALSE) {
		if (SavedSerial != 0) {
			free(SavedSerial);
            SavedSerial = 0;
		}

		// Validate serial number
		if (!(options & QUICKUSB_PROGRAM_SERIAL) || !(options & QUICKUSB_PROGRAM_SERIAL_MASK)) {
			free(eepromData);
			free(fileData);
			free(eepromBinaryData);
			free(fileBinaryData);

			lastError = QUICKUSB_ERROR_INVALID_SERIAL;
			return(FALSE);
		}

		// Convert serial number to string
		NewSerial = (PQCHAR) malloc(16);
#ifdef USE_SAFE_FUNCTIONS
        status = _itoa_s(options & QUICKUSB_PROGRAM_SERIAL_MASK, NewSerial, 16, 10);
        if (status != 0) {
			free(eepromData);
			free(fileData);
			free(eepromBinaryData);
			free(fileBinaryData);
            free(NewSerial);

			lastError = QUICKUSB_ERROR_INVALID_SERIAL;
			return(FALSE);
        }
#else
#if DRIVER==WindowsDriver
		_itoa((options & QUICKUSB_PROGRAM_SERIAL_MASK), NewSerial, 10);
#else
        sprintf(NewSerial, "%u", (unsigned int)(options & QUICKUSB_PROGRAM_SERIAL_MASK));
#endif
#endif
	}
	
	// The 2nd and 3rd bytes of the file contain the constant BB0F hex, which
	// is 0FC3 hex for encrypted files.  If this value is something else, the
	// file is invalid.
	if ((fileData[1] == 0xBB) && (fileData[2] == 0xF)) {
		encrypt = FALSE;
	}
	else if ((fileData[1] == 0xF) && (fileData[2] == 0xC3)) {
		encrypt = TRUE;
	}
	else {
		free(eepromData);
		free(fileData);
		free(eepromBinaryData);
		free(fileBinaryData);
		if (NewSerial != 0) {
			free(NewSerial);
		}
		if (SavedSerial != 0) {
			free(SavedSerial);
		}
        lastError = QUICKUSB_ERROR_FIRMWARE_ERROR;
		return(FALSE);
	}
	
	// Decrypt new file if needed
	if (encrypt == TRUE) {
		for (Index = 0; Index < realFileLength; ++Index) {
			currentByte = fileData[Index];
			currentByte = currentByte - 15;
			currentByte = currentByte ^ 187;
			fileData[Index] = currentByte & 0xff;
		}
	}
	
	// Expand the new file
	Fx2lpCode = IicExpand(fileData, realFileLength, fileBinaryData, UNCOMPRESSED_FILE_SIZE);
	
	// The EXUSB-FX2 has 8K of RAM.  The EZUSB-FX2LP has 16K of RAM.
	// If this is EZUSB-FX2LP code, and the QuickUSB module is not an EZUSB-FX2LP part,
	// do NOT allow the code to be downloaded.
	revid = 0xFFFF;
	result = QuickUsbReadSetting(hDevice, SETTING_VERSIONSPEED, &revid);
    if (!result) {
        oldFirmware = TRUE;
    } else {
        oldFirmware = FALSE;
    }
	
	// The chip revision is the high order byte of setting 17.  The low order byte
	// is the USB bus speed.  The FX2 part has a revid of 0, 2, or 4.  The FX2LP
	// part has a revid of 1.
	revid = (revid & 0xFF00) / 256;
	if (revid == 0x1) {
		Fx2lpPart = TRUE;
	}
	else {
		Fx2lpPart = FALSE;
	}
	
	if ((!oldFirmware) && (Fx2lpCode == TRUE) && (Fx2lpPart == FALSE)) {
		free(eepromData);
		free(fileData);
		free(eepromBinaryData);
		free(fileBinaryData);
		if (NewSerial != 0) {
			free(NewSerial);
		}
		if (SavedSerial != 0) {
			free(SavedSerial);
		}
        lastError = QUICKUSB_ERROR_FIRMWARE_ERROR;
		return(FALSE);
	}
	
	// Patch serial number into new firmware
	fileStringAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"STRS");
	address = fileStringAddress;
	if (address != 0) {
		// The serial number is in the 3rd string, step over strings 0-2
		for (Index = 0; Index <= 2; ++Index) {
			address = address + fileBinaryData[address];
		}
		
		// Make sure this is a string descriptor
		if (fileBinaryData[address + 1] == 3) {
			SerialNumLength = fileBinaryData[address] - 2;
			
			// If the old serial number was not present, format the new serial number
			if (OldSerialPresent == FALSE) {
				if (SavedSerial != 0) {
					free(SavedSerial);
				}
				SavedSerial = (PQBYTE) malloc(SerialNumLength);

				// Be sure to blank out the new serial number
				for (Index = 0; Index < SerialNumLength; ++Index) {
					SavedSerial[Index] = 0;
				}
	
				// If the entered serial number is too long, quit
                if ((QWORD)strlen(NewSerial) > (SerialNumLength / 2)) {
		            free(eepromData);
		            free(fileData);
		            free(eepromBinaryData);
		            free(fileBinaryData);
		            if (NewSerial != 0) {
			            free(NewSerial);
		            }
		            if (SavedSerial != 0) {
			            free(SavedSerial);
		            }
                    lastError = QUICKUSB_ERROR_INVALID_SERIAL;
		            return(FALSE);
	            }
				
				// Format the entered serial number
				for (Index = 0; Index < min(SerialNumLength / 2, (QWORD)(strlen(NewSerial))) ; ++Index) {
					SavedSerial[Index * 2] = NewSerial[Index];
				}
			}
			
			// Patch the serial number into string 3
			for (Index = 0; Index < SerialNumLength; ++Index) {
				fileBinaryData[address + Index + 2] = SavedSerial[Index];
			}
		}
		
		// The serial number is also in the 5th string, step over strings 3 & 4
		for (Index = 3; Index <= 4; ++Index) {
			address = address + fileBinaryData[address];
		}
		
		// Make sure this is a string descriptor
		if (fileBinaryData[address + 1] == 3) {
			SerialNumLength = fileBinaryData[address] - 2;
			
			// Patch the serial number into string 5 also
			for (Index = 0; Index < SerialNumLength; ++Index) {
				fileBinaryData[address + Index + 2] = SavedSerial[Index];
			}
		}
	}
	
	address = fileStringAddress;
	if (address != 0) {
		// The I/O model is in the 4th string, step over strings 0-3
		for (Index = 0; Index <= 3; ++Index) {
			address = address + fileBinaryData[address];
		}
		
		// Save the new I/O model
		if (fileBinaryData[address + 1] == 3) {
			OutIndex = 1;
			// Find start of I/O model string (it is inside parentheses)
			for (Index = 0; Index <= (min(fileBinaryData[address] - 2, 100) - 1); ++Index) {
				if (fileBinaryData[address + Index + 2] == '(') {
					OutIndex = 0;
					break;
				}
			}
			
			if (OutIndex == 0) {
				// Save the I/O model string
				for (Index = 0; Index <= (min(fileBinaryData[address] - 2, 100) - 1); ++Index) {
					fileIoModel[OutIndex] = fileBinaryData[address + Index + 2];
					if (fileBinaryData[address + Index + 2] == ')') {
						break;
					}
					OutIndex = OutIndex + 1;
				}
			}
		}
	}
		
	// Find the patch setting in the new firmware
	fileDefaultAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"DFLT");
	if (fileDefaultAddress != 0) {
		filePatchAddress = fileDefaultAddress + (SETTING_PATCHES * 2);
	}

	// If the user wants defaults preserved, and we are changing I/O models, warn user.
	if ((options & QUICKUSB_PRESERVE_DEFAULTS) && ((eepromPatches & PATCHED_DEFAULTS) != 0)) {
		for (Index = 0; Index <= 99; ++Index) {
			if (fileIoModel[Index] != eepromIoModel[Index]) {
				eepromPatches = eepromPatches & (PATCHED_CUSTOMIZER | PATCHED_GPIF);
				//MsgBox "I/O model mismatch on new firmware." & vbCrLf & "Defaults will not be saved!", vbExclamation, "Defaults Mismatch"
				break;
			}
		}
	
	    // Patch defaults into new firmware
		if (fileDefaultAddress != 0) {
			filePatchAddress = fileDefaultAddress + (SETTING_PATCHES * 2);
			filePatches = filePatches | PATCHED_DEFAULTS;
			for (Index = 0; Index <= 31 ; ++Index) {
				fileBinaryData[fileDefaultAddress + Index] = eepromBinaryData[eepromDefaultAddress + Index];
			}
		}
	}
		
	// Check that the PID matches.
	eepromAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"DSCR");
	fileAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"DSCR");
	if (eepromAddress != 0) {
		PidLo = eepromBinaryData[eepromAddress + 10];
		PidHi = eepromBinaryData[eepromAddress + 11];
		eepromPid = ((PidHi * 256) + PidLo) & 0xFFFF;
	}
	else {
		eepromPid = 0;
	}
	if (fileAddress != 0) {
		PidLo = fileBinaryData[fileAddress + 10];
		PidHi = fileBinaryData[fileAddress + 11];
		filePid = ((PidHi * 256) + PidLo) & 0xFFFF;
	}
	else {
		filePid = 0;
	}
	if ((eepromPid != 0) && (filePid != 0) && (eepromPid != filePid)) {
		free(eepromData);
		free(fileData);
		free(eepromBinaryData);
		free(fileBinaryData);
		if (NewSerial != 0) {
			free(NewSerial);
		}
		if (SavedSerial != 0) {
			free(SavedSerial);
		}
        lastError = QUICKUSB_ERROR_FIRMWARE_ERROR;
		return(FALSE);
	}
	
	// Copy any customized PID and strings to the new firmware
	if (((options & QUICKUSB_PRESERVE_CUSTOMIZATION) || !(options & QUICKUSB_PRESERVE_MASK)) && ((eepromPatches & PATCHED_CUSTOMIZER) != 0)) {
		// Copy any customized PID
		if ((eepromAddress != 0) && (fileAddress != 0)) {
			fileBinaryData[fileAddress + 10] = eepromBinaryData[eepromAddress + 10];
			fileBinaryData[fileAddress + 11] = eepromBinaryData[eepromAddress + 11];
		}
	
		// Copy any customized strings to the new firmware
		if ((eepromStringAddress != 0) && (fileStringAddress != 0)) {
			eepromCurString = eepromStringAddress;
			fileCurString = fileStringAddress;
			// Process strings 1 to 3
			for (StringIndex = 1; StringIndex <= 3 ; ++StringIndex) {
				eepromCurString = eepromCurString + eepromBinaryData[eepromCurString];
				fileCurString = fileCurString + fileBinaryData[fileCurString];
				if ((eepromBinaryData[eepromCurString + 1] = 3) && (fileBinaryData[fileCurString + 1] == 3)) {
					filePatches = filePatches | PATCHED_CUSTOMIZER;
					eepromStringLen = eepromBinaryData[eepromCurString];
					fileStringLen = fileBinaryData[fileCurString];
					for (Index = 2; Index < min(eepromStringLen, fileStringLen); ++Index) {
						fileBinaryData[fileCurString + Index] = eepromBinaryData[eepromCurString + Index];
					}
				}
			}
		}
	}
			
	// Copy any patched GPIF settings
	if ((options & QUICKUSB_PRESERVE_GPIF) && ((eepromPatches & PATCHED_GPIF) != 0)) {
		// Copy WAVE data, if present
		eepromAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"WAVE");
		fileAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"WAVE");
		if ((eepromAddress != 0) && (fileAddress != 0)) {
			filePatches = filePatches | PATCHED_GPIF;
			eepromAddress = eepromAddress + 5;
			fileAddress = fileAddress + 5;
			for (Index = 0; Index <= 127 ; ++Index) {
				fileBinaryData[fileAddress + Index] = eepromBinaryData[eepromAddress + Index];
			}
		}
		
		// Copy FLOW data, if present
		eepromAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"FLOW");
		fileAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"FLOW");
		if ((eepromAddress != 0) && (fileAddress != 0)) {
			filePatches = filePatches | PATCHED_GPIF;
			eepromAddress = eepromAddress + 5;
			fileAddress = fileAddress + 5;
			for (Index = 0; Index <= 35 ; ++Index) {
				fileBinaryData[fileAddress + Index] = eepromBinaryData[eepromAddress + Index];
			}
		}
		
		// Copy INIT data, if present
		eepromAddress = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"INIT");
		fileAddress = FindSection(fileBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"INIT");
		if ((eepromAddress != 0) && (fileAddress != 0)) {
			filePatches = filePatches | PATCHED_GPIF;
			eepromAddress = eepromAddress + 4;
			fileAddress = fileAddress + 4;
			for (Index = 0; Index <= 6 ; ++Index) {
				fileBinaryData[fileAddress + Index] = eepromBinaryData[eepromAddress + Index];
			}
		}
	}
	
	// Set the patched status setting
	if (filePatchAddress != 0) {
		fileBinaryData[filePatchAddress] = filePatches;
	}
	
	// Compress the updated file code
	IicCompress(fileData, (QWORD)realFileLength, fileBinaryData, UNCOMPRESSED_FILE_SIZE);
	
	// Write the new firmware to EEPROM
	address = 0;
	do {
        if (oldFirmware) {
		    length = (QWORD)min((QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize / 2), realFileLength - address);
        } else {
		    length = (QWORD)min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, realFileLength - address);
        }

		// Write the data to the EEPROM
		result = QuickUsbWriteEepromData(hDevice, address, &fileData[address], length);
        if (!result) {
            break;
        }

        // Old versions of firmware rely don't wait for the EEPROM write cycle to complete
        // so we must sleep in software
        if (oldFirmware) {
            SleepMS(25);
        }
		
        if (callback) {
            newPercent = (QWORD)(50 + (50 * address / realFileLength)); // 50 % of time spent here
            if (percent != newPercent) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }

		// Update the counters
		address = address + length;
	} while (address < realFileLength);
		
	// Free the memory
	free(eepromData);
	free(fileData);
	free(eepromBinaryData);
	free(fileBinaryData);
	if (NewSerial != 0) {
		free(NewSerial);
	}
	if (SavedSerial != 0) {
		free(SavedSerial);
	}

    // End
    if (callback) {
        callback(hDevice, 100, tag);
    }

	// Check if everthing went ok
	if (!result) {
        // QuickUsbWriteEepromData in above do...while loop sets lastError
        return(FALSE);
    }

    // Success
    return(TRUE);
}



QUSBLIB QRETURN QuickUsbVerifyFirmware(QHANDLE hDevice, PCQCHAR fileName, PQPROGRESS_CALLBACK callback, PQVOID tag) {
	QWORD Index;
	QWORD StringIndex;
	QWORD numErrorsFound;
	QULONG result;
	QWORD length;
	QWORD address;
	QWORD stringAddress;
	QWORD fileLength;
	FILE *inputFile;
	QULONG currentByte;
	QBOOL encrypt;
	QCHAR patchByte;
	QBYTE *eepromData;
	QBYTE *fileData;
	QBYTE *eepromBinaryData;
	QBYTE *fileBinaryData;
    QWORD percent, newPercent;
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
	
    // Start
    if (callback) {
        percent = 0;
        newPercent = 0;
        callback(hDevice, 0, tag);
    }

	// Open the input file for reading
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&inputFile, fileName, "rb");
    if (status != 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
	inputFile = fopen(fileName, "rb");
	if (inputFile == 0)
	{
	    //QuickUsbClose(hDevice);
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
		return(FALSE);
	}
#endif
	fseek(inputFile, 0L, SEEK_END); 
	fileLength = (QWORD)ftell(inputFile); 
	fseek(inputFile, 0L, SEEK_SET);

	eepromData = (PQBYTE) malloc(MAX_16K_ADDRESS);
	fileData = (PQBYTE) malloc(fileLength);
	eepromBinaryData = (PQBYTE) malloc(UNCOMPRESSED_FILE_SIZE);
	fileBinaryData = (PQBYTE) malloc(UNCOMPRESSED_FILE_SIZE);
	
	// Read the disk file
	fread(fileData, sizeof(QBYTE), fileLength, inputFile);
	
	// Validate and decrypt the disk file if needed
	// The 2nd and 3rd bytes of the file contain the constant BB0F hex, which
	// is 0FC3 hex for encrypted files.  If this value is something else, the
	// file is invalid.
	if ((fileData[1] == 0xBB) && (fileData[2] == 0xF)) {
		encrypt = FALSE;
	}
	else if ((fileData[1] == 0xF) && (fileData[2] == 0xC3)) {
		encrypt = TRUE;
	}
	else {
		fclose(inputFile);
		free(eepromData);
		free(fileData);
		free(eepromBinaryData);
		free(fileBinaryData);
        lastError = QUICKUSB_ERROR_FIRMWARE_ERROR;
		return(FALSE);
	}
	
	// Decrypt new file if needed
	if (encrypt == TRUE) {
		for (Index = 0; Index < fileLength ; ++Index) {
			currentByte = fileData[Index];
			currentByte = currentByte - 15;
			currentByte = currentByte ^ 187;
			fileData[Index] = (QBYTE)(currentByte & 255);
		}
	}
	
	// Read the EEPROM data
	Index = 0;
	do {
		// Calculate the block size
		length = (QWORD)min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, fileLength - Index);
		result = QuickUsbReadEepromData(hDevice, Index, &eepromData[Index], &length);
		
		if (result == 0) {
			fclose(inputFile);
			free(eepromData);
			free(fileData);
			free(eepromBinaryData);
			free(fileBinaryData);
            // QuickUsbReadEepromData sets lastError
			return(FALSE);
		}

        if (callback) {
            newPercent = 90 * Index / fileLength; // 90% of time spent here
            if (percent != newPercent) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }

		Index = Index + length;
	} while (Index < fileLength);
	
	// Close the file
	fclose(inputFile);
	
	// Uncompress the EEPROM and input file IIC code
	IicExpand(fileData, fileLength, fileBinaryData, UNCOMPRESSED_FILE_SIZE);
	IicExpand(eepromData, fileLength, eepromBinaryData, UNCOMPRESSED_FILE_SIZE);
	
	// Find and check the patch status of the QuickUSB module
	address = FindSection(eepromBinaryData, MAX_16K_ADDRESS, (const PQCHAR)"DFLT");
	if (address != 0) {
		address = address + (SETTING_PATCHES * 2);
		patchByte = eepromBinaryData[address];
	}
	else {
		patchByte = 0;
	}
	
	// Locate and clear the serial number in the input file
	stringAddress = FindSection(fileBinaryData, fileLength, (const PQCHAR)"STRS");
	address = stringAddress;
	
	if (address != 0) {
		// Skip over strings 0 through 2
		for (Index = 0; Index <= 2 ; ++Index) {
			address = address + fileBinaryData[address];
		}
		// Clear serial number string 3
		if (fileBinaryData[address + 1] == 3) {
			// Clear the string
			for (Index = 2; Index < fileBinaryData[address]; ++Index) {
				fileBinaryData[address + Index] = 0;
			}
		}
		// Skip over strings 3 through 4
		for (Index = 3; Index <= 4 ; ++Index) {
			address = address + fileBinaryData[address];
		}
		// Clear serial number string 5
		if (fileBinaryData[address + 1] == 3) {
			// Clear the string
			for (Index = 2; Index < fileBinaryData[address]; ++Index) {
				fileBinaryData[address + Index] = 0;
			}
		}
	}
	
	// Locate and clear the customizable strings in the input file
	if ((patchByte & PATCHED_CUSTOMIZER) != 0) {
		address = stringAddress;
	
		if (address != 0) {
			// Clear out strings 1 through 2
			for (StringIndex = 1; StringIndex <= 2 ; ++StringIndex) {
				address = address + fileBinaryData[address];
				if (fileBinaryData[address + 1] == 3) {
					// Clear the string
					for (Index = 2; Index < fileBinaryData[address]; ++Index) {
						fileBinaryData[address + Index] = 0;
					}
				}
			}
		}
	}
	
	// Locate and clear the serial number in the EEPROM data
	stringAddress = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"STRS");
	address = stringAddress;
	
	if (address != 0) {
		// Skip over strings 0 through 2
		for (Index = 0; Index <= 2 ; ++Index) {
			address = address + eepromBinaryData[address];
		}
		// Clear serial number string 3
		if (eepromBinaryData[address + 1] == 3) {
			// Clear the string
			for (Index = 2; Index < eepromBinaryData[address]; ++Index) {
				eepromBinaryData[address + Index] = 0;
			}
		}
		// Skip over strings 3 through 4
		for (Index = 3; Index <= 4 ; ++Index) {
			address = address + eepromBinaryData[address];
		}
		// Clear serial number string 5
		if (eepromBinaryData[address + 1] == 3) {
			// Clear the string
			for (Index = 2; Index < eepromBinaryData[address]; ++Index) {
				eepromBinaryData[address + Index] = 0;
			}
		}
	}
	
	// Locate and clear the customizable strings in the EEPROM data
	if ((patchByte & PATCHED_CUSTOMIZER) != 0) {
		address = stringAddress;
	
		if (address != 0) {
			// Clear out strings 1 through 2
			for (StringIndex = 1; StringIndex <= 2 ; ++StringIndex) {
				address = address + eepromBinaryData[address];
				if (eepromBinaryData[address + 1] == 3) {
					// Clear the string
					for (Index = 2; Index < eepromBinaryData[address]; ++Index) {
						eepromBinaryData[address + Index] = 0;
					}
				}
			}
		}
	}
	
	// Locate and clear the defaults in the input file
	if ((patchByte & PATCHED_DEFAULTS) != 0) {
		address = FindSection(fileBinaryData, fileLength, (const PQCHAR)"DFLT");
	
		if (address != 0) {
			for (Index = 0; Index <= 31 ; ++Index) {
				fileBinaryData[address + Index] = 0;
			}
		}
	}
	
	// Locate and clear the defaults in the EEPROM data
	// Always clear the PATCHES status byte.
	address = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"DFLT");
	if (address != 0) {
		if ((patchByte & PATCHED_DEFAULTS) != 0) {
			for (Index = 0; Index <= 31 ; ++Index) {
				eepromBinaryData[address + Index] = 0;
			}
		}
		else {
			eepromBinaryData[address + (SETTING_PATCHES * 2)] = 0;
		}
	}
	
	// Locate and clear the PID in the input file
	if ((patchByte & PATCHED_CUSTOMIZER) != 0) {
		address = FindSection(fileBinaryData, fileLength, (const PQCHAR)"DSCR");
	
		if (address != 0) {
			for (Index = 10; Index <= 11 ; ++Index) {
				fileBinaryData[address + Index] = 0;
			}
		}
	
	    // Locate and clear the PID in the EEPROM data
	    if ((patchByte & PATCHED_CUSTOMIZER) != 0) {
		    address = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"DSCR");
    	
		    if (address != 0) {
			    for (Index = 10; Index <= 11 ; ++Index) {
				    eepromBinaryData[address + Index] = 0;
			    }
		    }
	    }
    }
	
	// Locate and clear the WAVE settings in the input file
	if ((patchByte & PATCHED_GPIF) != 0) {
		address = FindSection(fileBinaryData, fileLength, (const PQCHAR)"WAVE");
	
		if (address != 0) {
			address = address + 5;
			for (Index = 0; Index <= 127 ; ++Index) {
				fileBinaryData[address + Index] = 0;
			}
		}
	
	    // Locate and clear the WAVE in the EEPROM data
	    if ((patchByte & PATCHED_GPIF) != 0) {
		    address = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"WAVE");
    	
		    if (address != 0) {
			    address = address + 5;
			    for (Index = 0; Index <= 127 ; ++Index) {
				    eepromBinaryData[address + Index] = 0;
			    }
		    }
	    }
    	
	    // Locate and clear the FLOW settings in the input file
	    if ((patchByte & PATCHED_GPIF) != 0) {
		    address = FindSection(fileBinaryData, fileLength, (const PQCHAR)"FLOW");
    	
		    if (address != 0) {
			    address = address + 5;
			    for (Index = 0; Index <= 35 ; ++Index) {
				    fileBinaryData[address + Index] = 0;
			    }
		    }
	    }
    	
	    // Locate and clear the FLOW in the EEPROM data
	    if ((patchByte & PATCHED_GPIF) != 0) {
		    address = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"FLOW");
    	
		    if (address != 0) {
			    address = address + 5;
			    for (Index = 0; Index <= 35 ; ++Index) {
				    eepromBinaryData[address + Index] = 0;
			    }
		    }
	    }
    	
	    // Locate and clear the INIT settings in the input file
	    if ((patchByte & PATCHED_GPIF) != 0) {
		    address = FindSection(fileBinaryData, fileLength, (const PQCHAR)"INIT");
    	
		    if (address != 0) {
			    address = address + 4;
			    for (Index = 0; Index <= 6 ; ++Index) {
				    fileBinaryData[address + Index] = 0;
			    }
		    }
	    }
    	
	    // Locate and clear the INIT in the EEPROM data
	    if ((patchByte & PATCHED_GPIF) != 0) {
		    address = FindSection(eepromBinaryData, fileLength, (const PQCHAR)"INIT");
    	
		    if (address != 0) {
			    address = address + 4;
			    for (Index = 0; Index <= 6 ; ++Index) {
				    eepromBinaryData[address + Index] = 0;
			    }
		    }
	    }
    }
	
	// Compress the EEPROM and input file IIC code
	IicCompress(fileData, fileLength, fileBinaryData, UNCOMPRESSED_FILE_SIZE);
	IicCompress(eepromData, UNCOMPRESSED_FILE_SIZE, eepromBinaryData, UNCOMPRESSED_FILE_SIZE);
	
	// Compare the two files, byte by byte
	numErrorsFound = 0;
	for (Index = 0; Index < fileLength; ++Index) {
		if (fileData[Index] != eepromData[Index]) {
			++numErrorsFound;

            // As soon an a single error is found, quit
            break;
		}

        if (callback) {
            newPercent = 90 + (10 * Index / fileLength); // 10% of time spent here
            if (percent != newPercent) {
                percent = newPercent;
                callback(hDevice, percent, tag);
            }
        }
	}

	// Free the memory
	free(eepromData);
	free(fileData);
	free(eepromBinaryData);
	free(fileBinaryData);
	
    // End
    if (callback) {
        callback(hDevice, 100, tag);
    }

	if (numErrorsFound != 0) {
        lastError = QUICKUSB_ERROR_VERIFY_FAILED;
	    return(FALSE);
	}

	return(TRUE);
}



// This subroutine expands the compressed IIC file to a user buffer.
// If the expanded code is over 8K, the function returns TRUE.
// If the expanded code is less than 8K, the function returns FALSE.
QBOOL IicExpand(PQBYTE InIic, QULONG InIicLen, PQBYTE UserBin, QULONG UserBinLen) {
   QULONG IicIndex;
   QULONG BinIndex;
   QULONG segIndex;
   QULONG segLength;
   QBOOL result;
   
   // Assume the code is less than 8K
   result = FALSE;
   
   // Zero out the expanded buffer
   for (BinIndex = 0; BinIndex < UserBinLen; ++BinIndex) {
      UserBin[BinIndex] = 0;
   }
   
   // Check if the input IIC file is valid
   if (InIic[0] != 0xC2) {
      return(FALSE);
   }
   
   // The IIC file has an 8 byte header.  Skip over same.
   // Each 4 byte segment descriptor has a 2 byte length, and a 2 byte address.
   IicIndex = 8;
   do {
       // Is this the reset vector at the end of the file?
       if (InIic[IicIndex + 0] == 0x80 && InIic[IicIndex + 1] == 0x01) {
         break;
       }
      
      segLength = (InIic[IicIndex + 0] * 256) + InIic[IicIndex + 1];
      BinIndex = (InIic[IicIndex + 2] * 256) + InIic[IicIndex + 3];
      IicIndex = IicIndex + 4;
      
      // Check for a bad segment descriptor
      if ((segLength < 1) || (segLength >= MAX_IIC_SEGMENT)) {
         return(FALSE);
      }
      
      for (segIndex = 0; segIndex < segLength; ++segIndex) {
         UserBin[BinIndex] = InIic[IicIndex + segIndex];
         
         // Are we out of the range of the EZUSB-FX2 part?
         if ((BinIndex >= MAX_8K_ADDRESS) && (BinIndex < MAX_16K_ADDRESS)) {
            result = TRUE;
         }
         
         BinIndex = BinIndex + 1;
      }
   
      IicIndex = IicIndex + segLength;
   } while (IicIndex < InIicLen - 3);

   return result;
}



void WriteRamFunction(PQCHAR devName, PQCHAR fileName) {
	QWORD Index;
	QLONG result;
	QWORD length;
	QULONG fileLength;
	QHANDLE hDevice;
	QBYTE *eepromData;
	FILE *inputFile;
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
	
	// Open the device
	result = QuickUsbOpen(&hDevice, devName);
	if (result == 0) {
		//Display.ShowStatus "Cannot open Device:" & devName
		return;
	}
	
	// Open the input file for reading
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&inputFile, fileName, "rb");
    if (status != 0) {
	    QuickUsbClose(hDevice);
        return;
    }
#else
	inputFile = fopen(fileName, "rb");
	if (inputFile == 0)
	{
	    QuickUsbClose(hDevice);
		return;
	}
#endif
	fseek(inputFile, 0L, SEEK_END); 
	fileLength = ftell(inputFile); 
	fseek(inputFile, 0L, SEEK_SET);
	
	// Put the micro in reset
	result = QuickUsbSetResetState(hDevice, 1);
	if (result == 0) {
		//Display.ShowStatus "Cannot set reset state"
		QuickUsbClose(hDevice);
		return;
	}
	
	// Allocate storage for data elements
	eepromData = (PQBYTE) malloc(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);
	
	Index = 0;
	//Display.Progress.Value = 0;
	//Display.ShowStatus "Write QuickUSB RAM..."
	do {
		// Read the data
		length = (QWORD)min(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize, fileLength - Index);
		
		//Read the data from the file;
		fread(eepromData, sizeof(QBYTE), length, inputFile);
				
		// Write the data to the EEPROM
		result = QuickUsbWriteRamData(hDevice, Index, eepromData, length);
		if (result == 0) {
			break;
		}
		
		// Update the counters
		Index = Index + length;
		
		// Update the progress bar
		//Display.Progress.Value = (Index / fileLength) * 100
		
		// Redraw the screen
		//DoEvents;
	} while (Index < fileLength);
		
	// Take the micro out of reset
	result = QuickUsbSetResetState(hDevice, 0);
	if (result == 0) {
		//Display.ShowStatus "Cannot set reset state"
		QuickUsbClose(hDevice);
		free(eepromData);
		return;
	}
	
	// Close the device when you're done
	QuickUsbClose(hDevice);
	
	// Close the file
	fclose(inputFile);
	
	// Free the memory
	free(eepromData);
	
	//Display.Progress.Value = 0;
}



// This subroutine updates a compressed IIC file from the expanded user buffer
void IicCompress(PQBYTE InIic, QULONG InIicLen, PQBYTE UserBin, QULONG UserBinLen) {
   QULONG IicIndex;
   QULONG BinIndex;
   QULONG segIndex;
   QULONG segLength;
   
   // The IIC file has an 8 byte header.  Skip over same.
   // Each 4 byte segment descriptor has a 2 byte length, and a 2 byte address.
   IicIndex = 8;
   do {
      // Is this the reset vector at the end of the file?
       if (InIic[IicIndex + 0] == 0x80 && InIic[IicIndex + 1] == 0x01) {
         break;
       }
      
      segLength = (InIic[IicIndex + 0] * 256) + InIic[IicIndex + 1];
      BinIndex = (InIic[IicIndex + 2] * 256) + InIic[IicIndex + 3];
      IicIndex = IicIndex + 4;
      
      // Check for a bad segment descriptor
      if ((segLength < 1) || (segLength >= MAX_IIC_SEGMENT)) {
         return;
      }
      
      for (segIndex = 0; segIndex < segLength; ++segIndex) {
         InIic[IicIndex + segIndex] = UserBin[BinIndex];
         BinIndex = BinIndex + 1;
      }
   
      IicIndex = IicIndex + segLength;
   } while (IicIndex < (InIicLen - 3));
}


// This function locates a 4 byte ASCII string in the byte array
QWORD FindSection(PQBYTE eepromData, QWORD length, const PQCHAR whatFor) {
   QWORD i;
   
   for (i = 0; i < (length - 4); ++i) {
       if ((eepromData[i + 0] == whatFor[0]) && (eepromData[i + 1] == whatFor[1]) && (eepromData[i + 2] == whatFor[2]) && (eepromData[i + 3] == whatFor[3])) {
         return i + 4;
       }
   }
   
   // String not found in input array.  Return 0
   return 0;
}



void EraseEepromFunction(PQCHAR devName) {
	QULONG result;
	QHANDLE hDevice;
	QBYTE *eepromData;
	
	// Open the device
	result = QuickUsbOpen(&hDevice, devName);
	if (result == 0) {
		//Display.ShowStatus "Cannot open Device:" & devName
		return;
	}
	
	// Allocate storage for data elements
	eepromData = (PQBYTE) malloc(QusbDeviceInfo[(QULONG) hDevice].maxControlPacketSize);
	
	// Warn about disconnecing
	//result = MsgBox("This operation will permanently erase the QuickUSB EEPROM." & vbCrLf & _;
	//		 "This option should only be undertaken for testing purposes.", vbOKCancel || vbCritical, _;
	//		 "Firmware Update Warning");
	//if (result != vbOK) {
	//	return;
	//}
			
	// Set the EEPROM to be like factory blank
	eepromData[0] = 0xFF;
	eepromData[1] = 0xFF;
	eepromData[2] = 0xFF;
	eepromData[3] = 0xFF;
	
	// Erase the first 2 bytes of the EEPROM
	//Display.ShowStatus "Updating QuickUSB firmware..."
	result = QuickUsbWriteEepromData(hDevice, 0, &eepromData[0], 2);
	// Erase the first segment descriptor
	result = QuickUsbWriteEepromData(hDevice, 8, &eepromData[0], 4);
	
	// Free the memory
	free(eepromData);
	
	// Reset the micro
	result = QuickUsbSetResetState(hDevice, 1);
	if (result == 0) {
		//Display.ShowStatus "Cannot set reset state"
		QuickUsbClose(hDevice);
		return;
	}
	
	// Wait for a second
	SleepMS(1000);
	
	// Take the micro out of reset
	result = QuickUsbSetResetState(hDevice, 0);
	if (result == 0) {
		//Display.ShowStatus "Cannot set reset state"
		QuickUsbClose(hDevice);
		return;
	}
		
	// Close the device when you're done
	QuickUsbClose(hDevice);
	
	// Notify the user
	if (result != 0) {
		//Display.ShowStatus "QuickUSB firmware was successfully updated"
		//MsgBox "Please disconnect and reconnect the USB connector " & vbCr & _;
		//		 "from the QuickUSB module to load the new firmware", , "Firmware Updated";
	}
	//Display.Progress.Value = 0;
}
