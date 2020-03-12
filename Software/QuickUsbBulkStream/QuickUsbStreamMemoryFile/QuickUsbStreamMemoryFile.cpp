/*=============================================================================
 Title        : QuickUsbStreamMemoryFile.cpp
 Description  : QuickUSB Memory File Streaming Sample (Windows, Linux, and Mac)
 Notes        : Compile as C++ Code
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
#include <iostream>
using namespace std;

#include "QuickUSB.h"

// Define a platform independent sleep macro
#include <ctime>
#if defined(__APPLE__)
    #include <sys/time.h>
#endif
#if defined(__linux__)
    #define SleepMS(x) usleep((x) * 1000)
#endif
#if defined(__APPLE__)
    #define SleepMS(x) usleep((x) * 1000)
#endif
#if defined(_WIN32)
    #define SleepMS(x) Sleep(x)
#endif

//  BUFFER_SIZE is the size of the data buffers to issue, in bytes.  Each
// read/write stream issues read/write transactions in multiples of the buffer
// size.
#define BUFFER_SIZE (256 * 1024)

// NUM_BUFFERS is the number of read/write transactions to simultaneously issue
// on the stream.  Each buffer is BUFFER_SIZE bytes is size.
#define NUM_BUFFERS 4

// MAX_READ_TRANSFERS is the number of read transfers to issue on the stream
// before stopping the stream.  If set to zero, the read stream will 
// continuosly read data until the stream is explicitly stopped with a call to
// QuickUsbStopStream.  If non-zero, the stream will run until the specified
// number of transactions have completed successfully.  The total number of
// bytes read is (MAX_READ_TRANSFERS * BUFFER_SIZE).
#define MAX_READ_TRANSFERS 100

// MAX_WRITE_TRANSFERS is the number of write transfers to issue on the stream
// before stopping the stream.  The stream will run until the specified number 
// of transactions have completed successfully or the end of the file has been 
// reached.  The total number of bytes written is (MAX_WRITE_TRANSFERS * 
// BUFFER_SIZE).
#define MAX_WRITE_TRANSFERS MAX_READ_TRANSFERS

#define TRUE    1
#define FALSE   0

// Main Program
int main(int argc, char **argv) {
	const QULONG nameListLen = 1024;
	QCHAR nameList[nameListLen];
    QFLOAT stat;
	
	QHANDLE hDevice;
	//QHANDLE hFile;
	QRESULT ok;
	QULONG lastError;
	QBYTE readStreamID, writeStreamID;
	QULONG streamStatus, streamError;
#if defined(_WIN32)
	DWORD bytesRead;
	BOOL winResult;
	DWORD winError;
#endif

	ok = QuickUsbFindModules(nameList, nameListLen);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    // ********************************************************************************
    cout << "Opening " << nameList << endl;
	ok = QuickUsbOpen(&hDevice, nameList);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    ok = QuickUsbResetStatistic(hDevice, QUICKUSB_STAT_ALL);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    // ********************************************************************************
	cout << "Starting read stream..." << endl;
	ok = QuickUsbReadBulkDataStartStreamToFile(hDevice, "temp.bin", NUM_BUFFERS, BUFFER_SIZE, MAX_READ_TRANSFERS, &readStreamID, QUICKUSB_STREAM_MEMORY_FILE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

	do {
		ok = QuickUsbGetStreamStatus(hDevice, readStreamID, &streamStatus, &streamError);
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
			cout << "Error: " << lastError << endl;
			break;
		}

        // Check for errors
        if (streamError != QUICKUSB_ERROR_NO_ERROR) {
		    cout << "Stream reported error: " << streamError << endl;
            break;
        }

        // Check if the stream has stopped running because we reached the number of 
        // read transfers to complete
        if (streamStatus != QUICKUSB_STREAM_STATUS_RUNNING) {
            break;
        }

		SleepMS(50);
	} while (TRUE);

    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_TOTAL_DATA_READ, QUICKUSB_STAT_UNIT_MB, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Data Read: " << stat << " MB" << endl;

    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_READ_THROUGHPUT, QUICKUSB_STAT_UNIT_MB | QUICKUSB_STAT_UNIT_PER_SEC, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Read Throughput: " << stat << " MB/s" << endl;
	cout << endl;

	ok = QuickUsbStopStream(hDevice, readStreamID, FALSE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    ok = QuickUsbResetStatistic(hDevice, QUICKUSB_STAT_ALL);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    // ********************************************************************************
	/*cout << "Accessing memory file contents" << endl;

	hFile = OpenFileMapping(FILE_MAP_READ, FALSE, "temp.bin");
	if (!hFile) {
		winError = GetLastError();
		cout << "System Error: " << winError << endl;
		return 1;
	}

	buffer = (PQBYTE) MapViewOfFile(hFile, FILE_MAP_READ, 0, 0, 0);

	// Do stuff with read data

	CloseHandle(hFile);
	UnmapViewOfFile(buffer);*/


    // ********************************************************************************
	cout << "Starting write stream..." << endl;
	ok = QuickUsbWriteBulkDataStartStreamFromFile(hDevice, "temp.bin", NUM_BUFFERS, BUFFER_SIZE, MAX_WRITE_TRANSFERS, &writeStreamID, QUICKUSB_STREAM_MEMORY_FILE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

	do {
		ok = QuickUsbGetStreamStatus(hDevice, writeStreamID, &streamStatus, &streamError);
		if (!ok) {
			ok = QuickUsbGetLastError(&lastError);
			cout << "Error: " << lastError << endl;
			break;
		}

        // Check for errors (Ignore end-of-file errors)
        if ((streamError != QUICKUSB_ERROR_NO_ERROR) && (streamError != QUICKUSB_ERROR_FILE_EOF)) {
		    cout << "Stream reported error: " << streamError << endl;
            break;
        }

        // Check if the stream has stopped running because we reached the number of 
        // read transfers to complete
        if (streamStatus != QUICKUSB_STREAM_STATUS_RUNNING) {
            break;
        }

		SleepMS(50);
	} while (TRUE);

    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_TOTAL_DATA_WRITTEN, QUICKUSB_STAT_UNIT_MB, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Data Written: " << stat << " MB" << endl;

    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_WRITE_THROUGHPUT, QUICKUSB_STAT_UNIT_MB | QUICKUSB_STAT_UNIT_PER_SEC, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Write Throughput: " << stat << " MB/s" << endl;

    // ********************************************************************************
	cout << "Closing streams" << endl;
	/*ok = QuickUsbStopStream(hDevice, readStreamID, FALSE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}*/

	ok = QuickUsbStopStream(hDevice, writeStreamID, FALSE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

    // ********************************************************************************
    cout << "Closing " << nameList << endl;
	ok = QuickUsbClose(hDevice);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "Error: " << lastError << endl;
		return 1;
	}

	return 0;
}
