/*=============================================================================
 Title        : QuickUsbStreamFile.cpp
 Description  : QuickUSB File Streaming Sample (Windows, Linux, and Mac)
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
#include "QuickUSB.h"

#include <iostream>
using namespace std;

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
// before stopping the stream.  If set to zero, the write stream will 
// continuosly write data until the stream is explicitly stopped with a call to
// QuickUsbStopStream or until the end of the file has been reached.  If 
// non-zero, the stream will run until the specified number of transactions have 
// completed successfully or the end of the file has been reached.  The total 
// number of bytes written is (MAX_WRITE_TRANSFERS * BUFFER_SIZE).
#define MAX_WRITE_TRANSFERS 0



// Main Program
int main(int argc, char **argv) {
	const QULONG nameListLen = 1024;
	QCHAR nameList[nameListLen];
    QFLOAT stat;
	
	QHANDLE hDevice;
	QRESULT ok;
	QULONG lastError, status;
    QBYTE streamID;

    // Locate connected QuickUSB devices
	ok = QuickUsbFindModules(nameList, nameListLen);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbFindModules failed with error: " << lastError << endl;
		return 1;
	}

    // Open the first QuickUSB device
    cout << "Opening " << nameList << endl;
	ok = QuickUsbOpen(&hDevice, nameList);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbOpen failed with error: " << lastError << endl;
		return 1;
	}

    // Reset all statistics
    ok = QuickUsbResetStatistic(hDevice, QUICKUSB_STAT_ALL);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbResetStatistic failed with error: " << lastError << endl;
		return 1;
	}

    // Begin reading data 
    cout << endl;
    cout << "Streaming to file (Data read)" << endl;
    ok = QuickUsbReadBulkDataStartStreamToFile(hDevice, "data.bin", NUM_BUFFERS, BUFFER_SIZE, MAX_READ_TRANSFERS, &streamID, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbReadBulkDataStartStreamToFile failed with error: " << lastError << endl;
		return 1;
	}

    // Poll until the stream is paused
    do
    {
        if (!QuickUsbGetStreamStatus(hDevice, streamID, &status, &lastError)) {
		    ok = QuickUsbGetLastError(&lastError);
		    cout << "QuickUsbGetStreamStatus failed with error: " << lastError << endl;
		    return 1;
        }

        // Check for errors
        if (lastError != QUICKUSB_ERROR_NO_ERROR) {
		    cout << "Stream reported error: " << lastError << endl;
            break;
        }

        // Check if the stream has stopped running because we reached the number of 
        // read transfers to complete
        if ((MAX_READ_TRANSFERS != 0) && (status != QUICKUSB_STREAM_STATUS_RUNNING)) {
            break;
        }

        // Don't hog the processor
        SleepMS(50);
    } while (1);

    // Stop steaming
    ok = QuickUsbStopStream(hDevice, streamID, FALSE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbStopStream failed with error: " << lastError << endl;
		return 1;
	}

    // Get the total number of bytes read
    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_TOTAL_DATA_READ, QUICKUSB_STAT_UNIT_MB, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbGetStatistic failed with error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Data Read: " << stat << " MB" << endl;

    // Get the read data rate
    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_READ_THROUGHPUT, QUICKUSB_STAT_UNIT_MB | QUICKUSB_STAT_UNIT_PER_SEC, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbGetStatistic failed with error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Read Throughput: " << stat << " MB/s" << endl;
    cout << endl;

    // Reset all statistics
    ok = QuickUsbResetStatistic(hDevice, QUICKUSB_STAT_ALL);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbResetStatistic failed with error: " << lastError << endl;
		return 1;
	}

    // Begin writing data
    cout << "Streaming from file (Data write)" << endl;
    ok = QuickUsbWriteBulkDataStartStreamFromFile(hDevice, "data.bin", NUM_BUFFERS, BUFFER_SIZE, MAX_WRITE_TRANSFERS, &streamID, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbWriteBulkDataStartStreamFromFile failed with error: " << lastError << endl;
		return 1;
	}

    // Poll until the stream is paused
    do
    {
        if (!QuickUsbGetStreamStatus(hDevice, streamID, &status, &lastError)) {
		    ok = QuickUsbGetLastError(&lastError);
		    cout << "QuickUsbGetStreamStatus failed with error: " << lastError << endl;
		    return 1;
        }

        // Check for errors
        if ((status != QUICKUSB_STREAM_STATUS_RUNNING) || (lastError != QUICKUSB_ERROR_NO_ERROR)) {
            // If we have reached the end of the file and the write stream was directed to
            // run until the end of the file, then we don't need to log an error
            if (!((MAX_WRITE_TRANSFERS == 0) && (lastError == QUICKUSB_ERROR_FILE_EOF))) {
		        cout << "Stream reported error: " << lastError << endl;
            }
            break;
        }

        // Don't hog the processor
        SleepMS(50);
    } while (1);

    // Stop steaming
    ok = QuickUsbStopStream(hDevice, streamID, FALSE);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbStopStream failed with error: " << lastError << endl;
		return 1;
	}

    // Get the number of bytes written
    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_TOTAL_DATA_WRITTEN, QUICKUSB_STAT_UNIT_MB, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbGetStatistic failed with error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Data Write: " << stat << " MB" << endl;

    // Get the write throughput
    ok = QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_WRITE_THROUGHPUT, QUICKUSB_STAT_UNIT_MB | QUICKUSB_STAT_UNIT_PER_SEC, &stat, 0);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbGetStatistic failed with error: " << lastError << endl;
		return 1;
	}
    cout << "Stat Write Throughput: " << stat << " MB/s" << endl;
    cout << endl;

    // Close the QuickUSB device
    cout << "Closing " << nameList << endl;
	ok = QuickUsbClose(hDevice);
	if (!ok) {
		ok = QuickUsbGetLastError(&lastError);
		cout << "QuickUsbClose failed with error: " << lastError << endl;
		return 1;
	}

    // Exit without error
	return 0;
}
