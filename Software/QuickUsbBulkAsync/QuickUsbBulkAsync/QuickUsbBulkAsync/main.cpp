/* ========================================================================
	Title        : main.cs
	Description  : QuickUSB Asynchronous Example
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

// QuickUSB Asynchronous Example
#include <iostream>
#include <QuickUSB.h>
#include "PlatformIndependentTimer.h"

using namespace std;

// Helper function to allocate data buffers
void AllocateBuffers(PQBYTE *Buffers, int Count, int Size) {
    for (int k = 0; k < Count; ++k) {
        QuickUsbAllocateDataBuffer(&Buffers[k], Size);
    }
}

// Helper function to free data buffers
void FreeBuffers(PQBYTE *Buffers, int Count) {
    for (int k = 0; k < Count; ++k) {
        if (Buffers[k]) {
            QuickUsbFreeDataBuffer(&Buffers[k]);
        }
    }
}

// Tag information global to all transactions
typedef struct BulkStreamGlobalTag {
    QULONG Bytes, RefCount, TransCount;
    QWORD ErrorCount;
    QBOOL Shutdown;
} BulkStreamGlobalTag, *PBulkStreamGlobalTag;

// Tag information unique to each transaction
typedef struct BulkStreamTag {
    QULONG Id;
    PBulkStreamGlobalTag globalTag;
} BulkStreamTag, *PBulkStreamTag;



///////////////////////////////////////
// The BulkStream completion routine //
///////////////////////////////////////
QVOIDRETURN CompletionRoutine(PQBULKSTREAM BulkStream)
{
    // Variables
    PBulkStreamTag tag;

    // Check for general errors
    if (!BulkStream) {
        cout << "BulkStream Error: " << endl;
    }

    // Retrieve our user data (the 'tag' in the async function call)
    tag = (PBulkStreamTag)BulkStream->Tag;

    // Check for errors
    if (BulkStream->Error) {
        cout << "--> CR: ERROR! Request #" << tag->Id << " failed with error " << BulkStream->Error << " (" << BulkStream->BytesTransferred << " of " << BulkStream->BytesRequested << " bytes)" << endl;
        ++tag->globalTag->ErrorCount;
        tag->globalTag->Shutdown = TRUE;
    } else {
        //cout << "--> CR: Completed Request #" << tag->Id << " for " << BulkStream->BytesTransferred << " bytes" << endl;

        //
        // Now that this request has completed we may process the data
        //
    }

    // Keep track of the number of outstanding requests
    --tag->globalTag->RefCount;

    // Keep a running count of bytes transferred
    tag->globalTag->Bytes += BulkStream->BytesTransferred;
}


///////////////////////////////////
// QuickUSB Asynchronous Example //
///////////////////////////////////
int main(int argc, char *argv[]) {
    // Buffer info
    const int NumBuffers = 4;
    const int BufferByteSize = 512 * 1024;
    const int LOOPS = 50;

    // Variables
    int k, j, Id = 0;
    QCHAR   nameList[1024];
    QRESULT qResult;
    QULONG  qusbError;
    QHANDLE hDevice;
    PQBYTE  BufferArray[NumBuffers];
    QBULKSTREAM   BulkStream[NumBuffers];
    BulkStreamGlobalTag globalTag;
    BulkStreamTag tag[NumBuffers];
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

    // Allocate buffers
    AllocateBuffers(BufferArray, NumBuffers, BufferByteSize);

    cout << "Acquiring data...please wait\n" << endl;

    // Start throughput timer
    timer.Start();

    // Aquire
    globalTag.Bytes = 0;
    globalTag.RefCount = 0;
    globalTag.ErrorCount = 0;
    globalTag.TransCount = 0;
    for (j = 0; j < (LOOPS + 1); ++j) {
        for (k = 0; k < NumBuffers; ++k) {
            // If this is not the first loop, wait for the last transaction to complete
            if (j != 0) {
                // Wait for the transaction to complete (The completion routine will be called)
                qResult = QuickUsbBulkWait(hDevice, &(BulkStream[k]), false);
                if (!qResult) {
                    QuickUsbGetLastError(&qusbError);
                    cout << "QuickUSB Error: " << qusbError << endl;
                    qResult = QuickUsbClose(hDevice);
                    FreeBuffers(BufferArray, NumBuffers);
                    return qusbError;
                }

                // Now that this request has completed we may process the data here or in the
                // completion routine, though it is better to perform all processing in the
                // completion routine as they are multi-threaded, allowing the main thread
                // to simply issue and re-issue data requests.
            }

            // If this is not the last loop, issue a new transaction
            if (j != LOOPS) {
                //cout << "Issuing Request #" << (Id + 1) << endl;

                tag[k].Id = ++Id;
                tag[k].globalTag = &globalTag;

                // Issue a new transaction
                qResult = QuickUsbReadBulkDataAsync(
                    hDevice, 
                    BufferArray[k], 
                    (QULONG)BufferByteSize, 
                    &(BulkStream[k]), 
                    &CompletionRoutine, 
                    &tag[k]);

                if (!qResult) {
                    QuickUsbGetLastError(&qusbError);
                    cout << "QuickUSB Error: " << qusbError << endl;
                    QuickUsbClose(hDevice);
                    FreeBuffers(BufferArray, NumBuffers);
                    return qusbError;
                }

                ++globalTag.RefCount;
                ++globalTag.TransCount;
            }
        }
    }

    // Stop the throughput timer
    timer.Stop();
    tElapsed = timer.GetElapsedTimeInSeconds();
    cout << endl;
    cout << "Time elapsed: " << tElapsed << "s" << endl;
    cout << "Total bytes transferred: " << ((float)globalTag.Bytes / (1024.0 * 1024.0)) << " MB" << endl;
    cout << "Data rate: " << ((globalTag.Bytes / (1024.0 * 1024.0)) / tElapsed ) << " MB/s" << endl;
    cout << endl;

    // Close the module
    qResult = QuickUsbClose(hDevice);
    if (!qResult) {
        QuickUsbGetLastError(&qusbError);
        cout << "QuickUSB Error: " << qusbError << endl;
    }

    // Free the buffers
    FreeBuffers(BufferArray, NumBuffers);

    // Report any leaks or errors
    cout << globalTag.TransCount << " transaction(s) issued in total" << endl;
    cout << globalTag.ErrorCount << " transaction(s) failed" << endl;
    cout << globalTag.RefCount << " transaction(s) are still outstanding" << endl;

    // Exit without error
    return 0;
}
