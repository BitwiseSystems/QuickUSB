/*=============================================================================
 Title        : QuickUSBVersion.h
 Description  : QuickUSB Version / Driver Version Information
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
 Goleta, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com
=============================================================================*/
#ifndef _QUICKUSB_VERSION_H_
#define _QUICKUSB_VERSION_H_

#include "QuickUsbApi.h"
//
// Make sure to keep these in sync with the version
// information in ezusbsys.rc
//
#define QUSB_DLL_MAJOR_VERSION     2
#define QUSB_DLL_MINOR_VERSION     15
#define QUSB_DLL_BUILD_VERSION     3

// Driver version
#if defined __APPLE__
#define QUSB_DRIVER_MAJOR_VERSION   QUSB_DLL_MAJOR_VERSION
#define QUSB_DRIVER_MINOR_VERSION   QUSB_DLL_MINOR_VERSION
#define QUSB_DRIVER_BUILD_VERSION   QUSB_DLL_BUILD_VERSION
#endif

//
//QusbDriverType designations
//
typedef enum QusbDriverType {
   OTHER_DRIVER = 0,
   QUSB_DRIVER = 1
} QusbDriverType;


#ifdef IMPLEMENT_ASYNC
#define QUICKUSB_MAX_STREAMS 8

typedef struct _QIOSTREAM {
    // We need to be able to get a reference back to the QUSB_DEVICE_INFO structure for the
    // device that this stream is associated with.  Since the QHANDLE for the device is an
    // index into that structure, we can just store that.
    QHANDLE hDevice;
    
    // For each stream, we need a unique handle to the device/file so that an IO Completion Port
    // may be uniquely associated with it
    QDEVHANDLE handle;
    
    // To support streaming to/from files, we need to keep a reference to the file handle
    QFILEHANDLE hFile;
    
#if DRIVER==WindowsDriver
    // Each stream gets its own IO Completion Port
    QDEVHANDLE hIOCP;
#endif
#if DRIVER==LinuxDriver
    // Each stream gets its own Asynchronous IO Port
    aio_context_t hIOCP;
#endif
    
    // Each stream gets its own thread pool to process IO Completion Packets
#if DRIVER==WindowsDriver
    PQDEVHANDLE hThread;        // Pointer to array of thread handles
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    pthread_t *hThread;
#endif
#if DRIVER==MacOSXDriver
    CFRunLoopSourceRef runLoopSource;
    CFRunLoopRef *runLoop;
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    sem_t *concurrencySem;      // Semaphore used to manage thread concurrency
#endif
    QWORD numThreads;           // Number of threads to create
    QWORD numThreadHandles;     // The number of threads created and valid in 'hWorkerThread'
    QWORD threadConcurrency;    // The number of threads allowed to execute concurrently
    
    // Keep track of the number of requests that have not completed because we cannot free
    // certain memories and release threads if all requests have not completed
    QULONG pendingRequests;

    // IO Streams are really comprised of a number of asynchronous requests that are 
    // automatically created, issued, re-issued, and freed.  Therefore, we must keep a list of
    // the internally created objects that store information about each asynchronous request, 
    // as well as the IO data buffers themselves.
    QULONG numRequests;     // The number of requests
    PQBULKSTREAM ioRequest; // The the request objects

	// Keep track of memory views for memory-mapped files
	PQBYTE memFileView;
	QULONG memFileSize;
	QULONG memFileOffset;

    // Information about the stream
    QULONG StreamID;
    QSTREAMTYPE StreamType;

    // Information about the current stream state
    QULONG info;

    // The completion routine to call and tag data
    QBYTE completionType;
    PQVOID cRoutine;
    PQVOID tag;

    // We must keep track of the RequestIDs of all the requests issued on this stream so that 
    // as requests are issued and completed we may make abolsutely sure that the request 
    // completion routines are executed and re-issued in order, despite the thread concurrency.
    QULONG NextRequestID;
    QULONG NextRequestIDToProcess;

    // We need to keep track of errors that occur on the stream so that they may be reported.
    // This value should not be overwritten if it already contains an error value and it should
    // be cleared when read so that future errors may be reported.
    QULONG lastError;

    // Streams may be setup to automatically pause after a specific number of transfers have 
    // completed (useful for streaming to/from a file)
    QULONG maxTransfers;
    QULONG totalNumberOfTransfers;
    
#if DRIVER==WindowsDriver
    // The critical section structure is essentially a process-wide mutex (not system-wide)
    // and is slightly faster than a system-wide Mutex.  We must use it to synchronize
    // thread access to shared data.
    PCRITICAL_SECTION lock;
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    pthread_mutex_t *lock;
#endif
} QIOSTREAM, *PQIOSTREAM;

#endif // IMPLEMENT_ASYNC


typedef struct _QUSB_DEVICE_INFO {
    QCHAR devName[9];                 // The device name in the form QUSB-XXX
    QDEVHANDLE hDevHandle;            // The actual file handle returned by OpenFile
    QusbDriverType devType;            
    QBYTE deviceOpen;                 // Keep a count of the number of times a module is opened so we know when to close it
    QWORD defaultOffset;

    // Library Timeout
    QULONG timeout; // Milliseconds

#ifdef IMPLEMENT_ASYNC
    // There may be up to QUICKUSB_MAX_STREAMS open streams on each device.  The Async functions
    // are treated as there own stream and are always Stream #0.
    PQIOSTREAM stream[QUICKUSB_MAX_STREAMS];
#endif
    
#if DRIVER==MacOSXDriver
    // Because in MacOS, the DyLib serves as both the driver and the library,
    // we have more information to keep track of per object
    QWORD isHighSpeed;
    IOUSBDeviceInterface245 ** module;
    io_service_t devQusb;
    io_service_t devInterface;
    UCHAR outPipe, inPipe;
    QWORD maxPacketSize;
    QUSB_GET_DRIVER_ERROR devError;
    USBDeviceAddress devAddress;
    
    std::queue<PQBULKSTREAM> *requestQ;
    QSTREAMTYPE LastStreamType;
    QULONG pendingInRequests, pendingOutRequests;
    pthread_mutex_t *lock;
#endif

    // Statistics
    unsigned long long totalBytesRead, totalBytesWritten;
    unsigned long long throughputBytesRead, throughputBytesWritten;
#if defined(__linux__)
    struct timespec tReadStart, tWriteStart;
#endif
#if defined(__APPLE__)
    timeval tReadStart, tWriteStart;
#endif
#if defined(_WIN32)
    LARGE_INTEGER freq, tReadStart, tWriteStart;
#endif

    // QUSB3 Support
    QWORD maxControlPacketSize;
} QUSB_DEVICE_INFO, *PQUSB_DEVICE_INFO;



#endif    //    _QUICKUSB_VERSION_H_

