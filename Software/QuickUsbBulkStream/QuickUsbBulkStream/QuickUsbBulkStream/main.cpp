/* ========================================================================
	Title        : main.cpp
	Description  : QuickUSB Bulk Streaming Example
	Notes        : 
	History      :

	Copyright � 2012 Bitwise Systems.  All rights reserved.
	This software contains confidential information and trade secrets of
	Bitwise Systems and is protected by United States and international
	copyright laws.  Use, disclosure, or reproduction is prohibited
	without the prior express written permission of Bitwise Systems,
	except as agreed in the QuickUSB license agreement.

	Use, duplication or disclosure by the U.S. Government is subject to
	restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
	(1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle
	Real, Suite E, Goleta, CA  93117.

	Bitwise Systems
	6489 Calle Real, Suite E
	Santa Barbara, CA  93117
	Voice: (805) 683-6469
	Fax  : (805) 683-4833
	Web  : www.bitwisesys.com
	email: support@bitwisesys.com
   ======================================================================== */

// QuickUSB Bulk Streaming Example
#include <iostream>
#include <QuickUSB.h>
#include "PlatformIndependentTimer.h"

using namespace std;

// Global user tag data for
struct MyTagStruct {
    QULONG numRequests, numFailedRequests;
    QULONG bytes;
};


///////////////////////////////////////
// The BulkStream completion routine //
///////////////////////////////////////
QVOIDRETURN CompletionRoutine(PQBULKSTREAM BulkStream)
{
    // Check for general errors
    if (!BulkStream)
    {
        return;
    }

    // Retrieve our user data
    MyTagStruct *tag = (MyTagStruct *)BulkStream->Tag;

    // Check for errors
    if (BulkStream->Error) {
        cout << "--> CR: ERROR! Failed with error " << BulkStream->Error << " (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl;
        ++tag->numFailedRequests;
    } else {
        //cout << "--> CR: Completed request for " << BulkStream->BytesTransferred << " bytes" << endl;
    }

    //
    // Now that this request has completed we may process the data
    //

    // Update tag info
    tag->bytes += BulkStream->BytesTransferred;
    ++tag->numRequests;
}


/////////////////////////////////////
// QuickUSB Bulk Streaming Example //
/////////////////////////////////////
int main(int argc, char *argv[]) {
    // The number of data buffers to use.  An asynchronous data request is issued
    // for each data buffer, then the main thread waits for each request to complete
    // then issues a new asynchrnonous request using the same data buffer as the
    // completed request.  To maximize throughout, the number of buffers should be
    // at least equal to the number of threads (see the SETTING_THREADS setting).
    const int NumBuffers = 4;

    // The byte size of the buffers.  To maximize performance the buffer size should
    // be as large as possible and a power of 2.
    const int BufferByteSize = 512 * 1024; // 512 KB

    // Variables
    QCHAR   nameList[1024];
    QRESULT qResult;
    QULONG  qusbError;
    QHANDLE hDevice;
    QBYTE streamID;
    MyTagStruct tag;
    PlatformIndependentTimer timer;
    double tElapsed;

    // Query connected modules
    qResult = QuickUsbFindModules(nameList, 1024);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    // Open the first module
    qResult = QuickUsbOpen(&hDevice, nameList);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    cout << "Streaming data for 5 seconds..." << endl;

    tag.bytes = 0;
    tag.numRequests = 0;
    tag.numFailedRequests = 0;

    // Open the first module
    qResult = QuickUsbOpen(&hDevice, nameList);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    // Start timing
    timer.Start();

    // Begin streaming data
    qResult = QuickUsbReadBulkDataStartStream(hDevice, NULL, NumBuffers, BufferByteSize, &CompletionRoutine, &tag, &streamID, 0, 0);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    // Acquire data in the background for 5 seconds
    do {
        //---------------------------------------------------------
        // Performing some background tasks
        //---------------------------------------------------------

        // Since we are not multithreading the stream in this example, we must
        // allow time for processing requests.  Processing requests simply
        // means checking if a request has completed, execute its completion
        // routine, and re-issue the requests.  QuickUsbProcessStream will
        // return when either any request has completed or the specified time
        // has elapsed.
        qResult = QuickUsbProcessStream(hDevice, streamID, 50);

        // Check if our time has elapsed
        tElapsed = timer.GetElapsedTimeInSeconds();
    } while (tElapsed < 5.0);

    // Stop the data stream, but do not block
    cout << "Shutting down..." << endl;
    qResult = QuickUsbStopStream(hDevice, streamID, TRUE);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    // Now we could perform some other shutting down tasks here while the
    // stream is shutting down.  For now we will just simulate background 
    // shutdown tasks.  Note that this time will incorrectly alter the
    // true data rate.
    //SleepMS(50);

    // Stop the data stream, but block this time
    qResult = QuickUsbStopStream(hDevice, streamID, FALSE);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);

        // Check the error
        if (qusbError == QUICKUSB_ERROR_NOT_STREAMING) {
            // The stream has already finished shutting down.  This is not an error
            // because we want the stream to be stopped, so continue on normally.
        } else {
            cout << "QuickUSB Error: " << qusbError << endl;
        }
    }

    // Close the device
    qResult = QuickUsbClose(hDevice);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
        return qusbError;
    }

    // Display statistics
    timer.Stop();
    tElapsed = timer.GetElapsedTimeInSeconds();
    cout << endl;
    cout << "NumBuffers = " << NumBuffers << ", Length = " << BufferByteSize << ", Data Rate: " << ((tag.bytes / (1024.0f * 1024.0f)) / tElapsed) << " MB/s" << endl;
    cout << "# Requests: " << tag.numRequests << " (" << tag.numFailedRequests << " failed)" << endl;

    // Exit without error
    return 0;
}
