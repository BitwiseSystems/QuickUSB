/*=============================================================================
 Title        : QuickUsbStream.cpp
 Description  : 
 Notes        : 
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
#include "QuickUsbApiStream.h"


#ifdef IMPLEMENT_ASYNC

QULONG GlobalStreamFlags = 0;

#if DRIVER==LinuxDriver
long io_setup(unsigned nr_reqs, aio_context_t *ctx) {
	return syscall(__NR_io_setup, nr_reqs, ctx);
}

long io_destroy(aio_context_t ctx) {
	return syscall(__NR_io_destroy, ctx);
}

long io_submit(aio_context_t ctx, long n, struct iocb **paiocb) {
	return syscall(__NR_io_submit, ctx, n, paiocb);
}

long io_cancel(aio_context_t ctx, struct iocb *aiocb, struct io_event *res) {
	return syscall(__NR_io_cancel, ctx, aiocb, res);
}

long io_getevents(aio_context_t ctx, long min_nr, long nr, struct io_event *events, struct timespec *tmo) {
	return syscall(__NR_io_getevents, ctx, min_nr, nr, events, tmo);
}

void init_iocb(struct iocb *iocb, int fd, void const *buf, size_t nbytes, off_t offset, int opcode) {
	memset(iocb, 0, sizeof(*iocb));
	iocb->aio_fildes = fd;
	iocb->aio_lio_opcode = opcode;
	iocb->aio_reqprio = 0;
#ifdef __amd64
    iocb->aio_buf = (u_int64_t) buf;
#else
    iocb->aio_buf = (u_int32_t) buf;
#endif
	iocb->aio_nbytes = nbytes;
	iocb->aio_offset = offset;
	iocb->aio_flags = 0;
}
#endif



// Create threads (if any), allocate data structures, and fill out information
// about the stream
QRESULT QuickUsbInitializeStream(QBYTE streamID, PQIOSTREAM stream, QHANDLE hDevice, QULONG numRequests, QWORD numThreads, QWORD threadConcurrency) {
    QULONG k;
    QULONG waitTime, maxWaitTime;
    QBOOL ok;
#if (DRIVER==WindowsDriver) || (DRIVER==LinuxDriver)
    QCHAR devicePath[MAX_DEVICEPATH];
#endif
#if DRIVER==LinuxDriver
    int result;
#endif
#if DRIVER==MacOSXDriver
    kern_return_t err;
#endif
    
    // Clear stream structure
    memset(stream, 0, sizeof(QIOSTREAM));

    // Fill out basic stream information
    stream->hDevice = hDevice;
    stream->StreamID = streamID;

    // Allocate IO objects (not used by new async API)
    if (numRequests > 0) {
        stream->ioRequest = (PQBULKSTREAM) malloc(sizeof(QBULKSTREAM) * numRequests);
        memset(stream->ioRequest, 0, sizeof(QBULKSTREAM) * numRequests);
        stream->numRequests = numRequests;
    }

    // Store the thread concurrency
    stream->threadConcurrency = threadConcurrency;

#if DRIVER==WindowsDriver
    if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
        // On Windows, the legacy async API does not open a new handle on the device,
        // does not create an IOCP, and does not create any threads.
        
        // The handle to the device is the same as the handle used for async IO
        stream->handle = QusbDeviceInfo[(QULONG) hDevice].hDevHandle;
        return(TRUE);
    }

    // Build the complete device name
#ifdef USE_SAFE_FUNCTIONS
    sprintf_s(devicePath, MAX_DEVICEPATH, "\\\\.\\%s", QusbDeviceInfo[(QULONG) hDevice].devName);
#else
    sprintf(devicePath, "\\\\.\\%s", QusbDeviceInfo[(QULONG) hDevice].devName);
#endif

    // Get a new handle to the device
    stream->handle = CreateFile((LPCSTR) devicePath,
        GENERIC_READ | GENERIC_WRITE,
        FILE_SHARE_READ | FILE_SHARE_WRITE,
        NULL,
        OPEN_EXISTING,
#ifdef IMPLEMENT_OVERLAPPED
        FILE_FLAG_OVERLAPPED,
#else
        0,
#endif
        NULL
        );
        
    // Check the returned handle
    if (stream->handle == INVALID_HANDLE_VALUE) {
        if (stream->ioRequest != NULL) {
            free(stream->ioRequest);
            stream->ioRequest = NULL;
        }

        lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
        return (FALSE);
    }
    
    // Create completion port (the number of concurrent threads is best set to the number of processors)
    // Windows thread concurrency is automatically handled by the IO Complation Port
    stream->hIOCP = CreateIoCompletionPort(stream->handle, NULL, QUSB_THREAD_CMD_RUN, threadConcurrency);
    if (stream->hIOCP == NULL) {
        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }

    // If we're multithreading, we need to create threads
    if (numThreads > 0) {
        // Init critical section structure
        stream->lock = (PCRITICAL_SECTION) malloc(sizeof(CRITICAL_SECTION));
        InitializeCriticalSection(stream->lock);

        // Create threads (twice as many threads as processors is recommended)
        stream->hThread = malloc(sizeof(HANDLE) * numThreads);
        for (k = 0; k < numThreads; ++k) {
            // Create the thread
            stream->hThread[k] = CreateThread(NULL, 0, &QuickUsbStreamThread, stream, 0, NULL);

            // Check if the thread was successfully created
            if (stream->hThread[k] != NULL) {
                ++(stream->numThreadHandles);
            } else {
                // Stop the stream
                QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
                stream = NULL;

                // Return with error
                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }

        // Check if threads have initialize and wait longer if not
        waitTime = 0;
        maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
        ok = FALSE;
        do {
            QUSB_LOCK(stream); {
                if (stream->numThreads == numThreads) {
                    ok = TRUE;
                }
            } QUSB_UNLOCK(stream);

            if (!ok) {
                SleepMS(QUSB_TIGHT_LOOP_WAIT);
                waitTime += QUSB_TIGHT_LOOP_WAIT;
            }
        } while (!ok && (waitTime < maxWaitTime));

        // If the threads failed to initialize, we need to clean up and quit
        if (!ok) {
            // Stop the stream
            QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
            stream = NULL;

            // Return with error
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
    }
#endif

#if DRIVER==LinuxDriver
    if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
        // On Linux, the legacy async API does not open a new handle on the device,
        // and does not create any threads, but does create an IOCP
        stream->handle = QusbDeviceInfo[(QULONG) hDevice].hDevHandle;
    } else {
        // Build the complete device name
        sprintf(devicePath, "/dev/%s", QusbDeviceInfo[(QULONG) hDevice].devName);
        stream->handle = open(devicePath, O_RDWR);
        
        // Check the returned handle
        if (stream->handle == INVALID_HANDLE_VALUE) {
            if (stream->ioRequest != NULL) {
                free(stream->ioRequest);
                stream->ioRequest = NULL;
            }

            lastError = QUICKUSB_ERROR_CANNOT_OPEN_MODULE;
            return (FALSE);
        }
        
        // Linux thread concurrency must be manually managed through use of a counting semaphore
        if (stream->threadConcurrency > 0) {
            stream->concurrencySem = (sem_t*) malloc(sizeof(sem_t));
            result = sem_init(stream->concurrencySem, 0, stream->threadConcurrency);
            if (result != 0) {
                if (stream->ioRequest != NULL) {
                    free(stream->ioRequest);
                    stream->ioRequest = NULL;
                }

                free(stream->concurrencySem);
                stream->concurrencySem = NULL;

                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }
    }
    
    // Setup the Asynchronous IO context
    result = io_setup(1, &stream->hIOCP);
    if (result != 0) {
        if (stream->ioRequest != NULL) {
            free(stream->ioRequest);
            stream->ioRequest = NULL;
        }

        if (stream->concurrencySem != NULL) {
            sem_destroy(stream->concurrencySem);
            free(stream->concurrencySem);
            stream->concurrencySem = NULL;
        }

        lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        return(FALSE);
    }

    // If we're multithreading, we need to create threads
    if ((numThreads > 0) && (streamID != QUICKUSB_LEGACY_ASYNC_STREAM)) {
        // Init mutex
        stream->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(stream->lock, NULL);

        // Create threads (twice as many threads as processors is recommended)
        stream->hThread = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
        for (k = 0; k < numThreads; ++k) {
            // Create the thread
            if (pthread_create(&stream->hThread[k], NULL, &QuickUsbStreamThread, (PQVOID)stream) == 0) {
                ++(stream->numThreadHandles);
            } else {
                // Stop the stream
                QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
                stream = NULL;

                // Return with error
                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }

        // Check if threads have initialize and wait longer if not
        waitTime = 0;
        maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
        ok = FALSE;
        do {
            QUSB_LOCK(stream); {
                if (stream->numThreads == numThreads) {
                    ok = TRUE;
                }
            } QUSB_UNLOCK(stream);

            if (!ok) {
                SleepMS(QUSB_TIGHT_LOOP_WAIT);
                waitTime += QUSB_TIGHT_LOOP_WAIT;
            }
        } while (!ok && (waitTime < maxWaitTime));
        
        // If the threads failed to initialize, we need to clean up and quit
        if (!ok) {
            // Stop the stream
            QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
            stream = NULL;

            // Return with error
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
    }
#endif
    
#if DRIVER==MacOSXDriver
    if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
        // On Mac, the legacy async API does not open a new handle on the device,
        // and does not create any threads, but does create an IOCP
        stream->handle = QusbDeviceInfo[(QULONG) hDevice].hDevHandle;
    } else {
        stream->handle = QusbDeviceInfo[(QULONG) hDevice].hDevHandle;
        
        // Create async interface
        err = (*stream->handle)->CreateInterfaceAsyncEventSource(stream->handle, &stream->runLoopSource);
        if (err != kIOReturnSuccess) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
        CFRunLoopAddSource(CFRunLoopGetCurrent(), stream->runLoopSource, kCFRunLoopDefaultMode);
        
        // Mac thread concurrency must be manually managed through use of a counting semaphore
        if (stream->threadConcurrency > 0) {
            QCHAR semName[32];
            
            // Create a unique semaphore name
            sprintf(semName, "com.QUSB%i.Semaphore.Stream%i", stream->hDevice, (int)streamID);
            sem_unlink(semName);
            stream->concurrencySem = sem_open(semName, O_CREAT | O_EXCL, S_IRWXU, stream->threadConcurrency);
            if (stream->concurrencySem == SEM_FAILED) {
                stream->concurrencySem = NULL;
                if (stream->ioRequest != NULL) {
                    free(stream->ioRequest);
                    stream->ioRequest = NULL;
                }
                
                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }
    }
    
    // If we're multithreading, we need to create threads
    if ((numThreads > 0) && (streamID != QUICKUSB_LEGACY_ASYNC_STREAM)) {
        // Init mutex
        stream->lock = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t));
        pthread_mutex_init(stream->lock, NULL);
        
        // Create threads (twice as many threads as processors is recommended)
        stream->hThread = (pthread_t *)malloc(sizeof(pthread_t) * numThreads);
        stream->runLoop = (CFRunLoopRef *) malloc(sizeof(CFRunLoopRef) * numThreads);
        memset(stream->runLoop, 0, sizeof(CFRunLoopRef) * numThreads);
        for (k = 0; k < numThreads; ++k) {
            // Create the thread
            if (pthread_create(&stream->hThread[k], NULL, &QuickUsbStreamThread, (PQVOID)stream) == 0) {
                ++(stream->numThreadHandles);
            } else {
                // Stop the stream
                QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
                stream = NULL;
                
                // Return with error
                lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                return(FALSE);
            }
        }
        
        // Check if threads have initialize and wait longer if not
        waitTime = 0;
        maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
        ok = FALSE;
        do {
            QUSB_LOCK(stream); {
                if (stream->numThreads == numThreads) {
                    ok = TRUE;
                }
            } QUSB_UNLOCK(stream);
            
            if (!ok) {
                SleepMS(QUSB_TIGHT_LOOP_WAIT);
                waitTime += QUSB_TIGHT_LOOP_WAIT;
            }
        } while (!ok && (waitTime < maxWaitTime));
        
        // If the threads failed to initialize, we need to clean up and quit
        if (!ok) {
            // Stop the stream
            QuickUsbStopStream(hDevice, streamID, FALSE); // Frees allocated memories
            stream = NULL;
            
            // Return with error
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
    }
#endif

    // Return successfully
    return(TRUE);
}

#endif // IMPLEMENT_ASYNC


// Streaming API
QRETURN QuickUsbReadBulkDataStartStream(QHANDLE hDevice, PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency) {
/*---------------------------------------------------------------------------
 Purpose : Start streaming data into the computer from the QuickUSB device.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
           buffers - (Optional) An array of pointers to the user allocated
             buffers used to store the read data.  These buffers must 
             remain valid until the read stream has been stopped.  If this
             parameter is null then the API will allocate and managed the 
             memory for the data buffers.  
           numBuffers - The number of buffers.
           bufferSize - The byte size of each buffer.
           cRoutine - The completion routine (callback function) that is
             called each time a buffer is filled with data.
           tag - (Optional) A pointer containing user-specific data that is
             passed into the completion routine.
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : If the 'buffers' parameter is null then the API will allocate and
             managed the memory for the data buffers.
           When done streaming data, you must call QuickUsbBulkDataStopStream
             to stop the stream.
           Certain transaction lengths are invalid and will timeout.  To
             determine if a given length is valid, use the following
             pseudo code:
             if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                 QULONG packetSize = (highspeed) ? 512 : 64;
                 QULONG preRead = (wordwide) ? 4 : 2;
                 QULONG mod = (length % packetSize);
                 QULONG even = (length % 2);
                 QBOOL validLength = (mod == 0) || ((mod >= preRead) 
                     && !(wordwide && !even));
             }
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    QULONG k;
    QBYTE id;
    PQIOSTREAM stream;
	PQBULKSTREAM bulkStream;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((numBuffers < 1) || (bufferSize < 1) || (bufferSize > QUSB_MAX_BULK_TRANSFER_SIZE) || (cRoutine == 0) || ((numThreads != 0) && (concurrency == 0))) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

	// If the user has specified an array of buffer pointers to use, then we need to make sure
	// that each buffer pointer is not null.  Memory files, however, don't specify an array
	// of pointers and instead send a pointer to a virtually contigious view of memory.
    if (buffers != 0) {
        if (cRoutine == CompleteFileStream) {
            // Memory file buffer
#if DRIVER==LinuxDriver
                // On Linux, data buffers for asynchronous requests must be 512-byte aligned
                if (((long unsigned int)buffers & (512 - 1)) != 0) {
                    lastError = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED;
                    return(FALSE);
                }
#endif
        } else {
            // User specified data buffers
            for (k=0; k<numBuffers; ++k) {
                if (buffers[k] == 0) {
                    lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
                    return(FALSE);
                }
            
#if DRIVER==LinuxDriver
                // On Linux, data buffers for asynchronous requests must be 512-byte aligned
                if (((long unsigned int)buffers[k] & (512 - 1)) != 0) {
                    lastError = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED;
                    return(FALSE);
                }
#endif
            }
        }
    }

    // Locate an unused stream
    for (id = QUICKUSB_BULK_STREAM_START; id < QUICKUSB_MAX_STREAMS; ++id) {
        if (QusbDeviceInfo[(QULONG) hDevice].stream[id] == NULL) {
            break;
        }
    }

    // Check that we found a free stream
    if (id >= QUICKUSB_MAX_STREAMS) {
        lastError = QUICKUSB_ERROR_TOO_MANY_REQUESTS;
        return(FALSE);
    }

    // Allocate the stream
    stream = (PQIOSTREAM)malloc(sizeof(QIOSTREAM));
    QusbDeviceInfo[(QULONG) hDevice].stream[id] = stream;

    // Initialize the stream
    qResult = QuickUsbInitializeStream(id, stream, hDevice, numBuffers, numThreads, concurrency);
    if (!qResult) {
        free(stream);
        stream = NULL;
        QusbDeviceInfo[(QULONG) hDevice].stream[id] = NULL;
        return(FALSE);
    }
    
    // Fill out additional stream info
    stream->StreamType = QUSB_STREAMTYPE_BULK_IN_EP6;
    stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
    stream->cRoutine = (PQVOID)cRoutine;
    stream->tag = tag;
    stream->info = QUSB_INFO_STREAMING_READ;
    if (buffers == 0) {
        stream->info |= QUSB_INFO_INTERNAL_ALLOC;
    }
	if (cRoutine == CompleteFileStream) {
        stream->hFile = (QFILEHANDLE) tag;

		// The difference between streaming to a file and a memory file is that the memory file will
		// specify a buffer
		if (buffers != 0) {
			stream->info |= QUSB_INFO_MEMORY_FILE;
		}
    }
    for (k = 0; k < numBuffers; ++k) {
        // Get the BulkStream object
        bulkStream = &(stream->ioRequest[k]);
		
		if (stream->info & QUSB_INFO_MEMORY_FILE) {
			// Memory-mapped files specify a single pointer, not an array of pointers
			((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = (PQBYTE)buffers + (k * bufferSize * sizeof(QBYTE));

			// We must keep track of the offset in the memory file so that we may stream to new regions
			// of memory as requests complete
			stream->memFileOffset += (bufferSize * sizeof(QBYTE));
		} else {
			if (stream->info & QUSB_INFO_INTERNAL_ALLOC) {
				// Internally allocated data buffers
				QuickUsbAllocateDataBuffer(&(bulkStream->Buffer), bufferSize * sizeof(QBYTE));
			} else {
				// User allocated data buffers
				((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = buffers[k];
			}
		}

		// Fill out information about each request
		bulkStream->BytesRequested = bufferSize;
		bulkStream->StreamType = stream->StreamType;
    }

    // Start the stream
    qResult = QuickUsbStartStream(stream); // Sets lastError on failure
    if (!qResult) {
        return(FALSE); 
    } else {
        *streamID = id;
    }

    // Return successfully
    return(TRUE);
#endif
}



QRETURN QuickUsbWriteBulkDataStartStream(QHANDLE hDevice, PQBYTE buffers[], QULONG numBuffers, QULONG bufferSize,
    PQBULKSTREAM_COMPLETION_ROUTINE cRoutine, PQVOID tag, PQBYTE streamID, QWORD numThreads, QWORD concurrency) {
/*---------------------------------------------------------------------------
 Purpose : Start streaming data from the computer to the QuickUSB device.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
           buffers - An array of pointers to the user allocated buffers  
             holding the data to write.  These buffers must remain valid 
             until the read stream has been stopped.
           numBuffers - The number of buffers.
           bufferSize - The byte size of each buffer.
           cRoutine - The completion routine (callback function) that is
             called each time a buffer has been written.  This buffer must
             be refilled with the new data to write from within the 
             completion routine.
           tag - (Optional) A pointer containing user-specific data that is
             passed into the completion routine.
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : When done streaming data, you must call QuickUsbBulkDataStopStream
             to stop the stream.
           Certain transaction lengths are invalid and will timeout.  To
             determine if a given length is valid, use the following
             pseudo code:
             if (Firmware IO Model is Simple, BlockHS, or PIPE) {
                 QULONG packetSize = (highspeed) ? 512 : 64;
                 QULONG preRead = (wordwide) ? 4 : 2;
                 QULONG mod = (length % packetSize);
                 QULONG even = (length % 2);
                 QBOOL validLength = (mod == 0) || ((mod >= preRead) 
                     && !(wordwide && !even));
             }
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    QULONG k;
    QBYTE id;
    PQIOSTREAM stream;
    PQBULKSTREAM bulkStream;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((cRoutine != CompleteFileStream)) { // Only streaming to files can use internally allocated buffers
        if (buffers == 0) {
            lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
            return(FALSE);
        }
        for (k=0; k<numBuffers; ++k) {
            if (buffers[k] == 0) {
                lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
                return(FALSE);
            }
            
#if DRIVER==LinuxDriver
            // On Linux, data buffers for asynchronous requests must be 512-byte aligned
            if (((long unsigned int)buffers[k] & (512 - 1)) != 0) {
                lastError = QUICKUSB_ERROR_BUFFER_NOT_ALIGNED;
                return(FALSE);
            }
#endif
        }
    }
    if ((numBuffers < 1) || (bufferSize < 1) || (bufferSize > QUSB_MAX_BULK_TRANSFER_SIZE) || (cRoutine == 0) || ((numThreads != 0) && (concurrency == 0))) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Locate an unused stream
    for (id = QUICKUSB_BULK_STREAM_START; id < QUICKUSB_MAX_STREAMS; ++id) {
        if (QusbDeviceInfo[(QULONG) hDevice].stream[id] == NULL) {
            break;
        }
    }

    // Check that we found a free stream
    if (id >= QUICKUSB_MAX_STREAMS) {
        lastError = QUICKUSB_ERROR_TOO_MANY_REQUESTS;
        return(FALSE);
    }

    // Allocate the stream
    stream = (PQIOSTREAM)malloc(sizeof(QIOSTREAM));
    QusbDeviceInfo[(QULONG) hDevice].stream[id] = stream;

    // Initialize the stream
    qResult = QuickUsbInitializeStream(id, stream, hDevice, numBuffers, numThreads, concurrency);
    if (!qResult) {
        free(stream);
        stream = NULL;
        QusbDeviceInfo[(QULONG) hDevice].stream[id] = NULL;
        return(FALSE);
    }

    // Fill out additional stream info
    stream->StreamType = QUSB_STREAMTYPE_BULK_OUT_EP2;
    stream->completionType = QUICKUSB_CRTYPE_VOID_QBULKSTREAM;
    stream->cRoutine = (PQVOID)cRoutine;
    stream->tag = tag;
    stream->info = QUSB_INFO_STREAMING_WRITE;
    if (buffers == 0) {
        stream->info |= QUSB_INFO_INTERNAL_ALLOC;
    }
	if (cRoutine == CompleteFileStream) {
        stream->hFile = (QFILEHANDLE) tag;

		// The difference between streaming to a file and a memory file is that the memory file will
		// specify a buffer
		if (buffers != 0) {
			stream->info |= QUSB_INFO_MEMORY_FILE;
		}
    }
    if (GlobalStreamFlags & QUICKUSB_STREAM_LOOP_AT_EOF) {
        stream->info |= QUSB_INFO_LOOP_AT_EOF;
    }
    for (k = 0; k < numBuffers; ++k) {
        // Get the BulkStream object
        bulkStream = &(stream->ioRequest[k]);
		
		if (stream->info & QUSB_INFO_MEMORY_FILE) {
			// Memory-mapped files specify a single pointer, not an array of pointers
			((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = (PQBYTE)buffers + (k * bufferSize * sizeof(QBYTE));

			// We must keep track of the offset in the memory file so that we may stream to new regions
			// of memory as requests complete
			stream->memFileOffset += (bufferSize * sizeof(QBYTE));
		} else {
			if (stream->info & QUSB_INFO_INTERNAL_ALLOC) {
				// We can internally allocate write buffers, but we need to fill them with data before
				// issuing requests.  We will do this by calling the completion routine with an error
				// indicating we need data.
				QuickUsbAllocateDataBuffer(&(bulkStream->Buffer), bufferSize * sizeof(QBYTE));
	            
				// Check that the user supplied a completion routine
				bulkStream->Handle = hDevice;
				bulkStream->StreamID = id;
				bulkStream->Tag = tag;
				bulkStream->BytesRequested = bufferSize;
				bulkStream->Error = QUICKUSB_ERROR_NEED_DATA;

				// First we must figure out what kind of completion routine to call
				switch (stream->completionType) {
					case QUICKUSB_CRTYPE_VOID_QBULKSTREAM:
						((PQBULKSTREAM_COMPLETION_ROUTINE)stream->cRoutine)(bulkStream);
						break;
					default:
						// Internal error
						break;
				}
			} else {
				// User allocated data buffers
				((PQBULKSTREAM)(stream->ioRequest))[k].Buffer = buffers[k];
			}
		}

		// Fill out information about each request
		bulkStream->BytesRequested = bufferSize;
		bulkStream->StreamType = stream->StreamType;
    }

    // Start the stream
    qResult = QuickUsbStartStream(stream); // Sets lastError on failure
    if (!qResult) {
        return(FALSE);
    } else {
        *streamID = id;
    }

    // Return successfully
    return(TRUE);
#endif
}



#ifdef IMPLEMENT_ASYNC

// Issue initial set of asynchronous requests to start the stream
QRESULT QuickUsbStartStream(PQIOSTREAM stream) {
    QULONG k;
    QRESULT qResult;
    QULONG firstRequestIndex;
    QULONG firstRequestID;
    QULONG requestIndex;
    QULONG prevError;
    QULONG pendingRequests;

    // When we start a stream, we must make sure to issue IO Requests in the order dictated by their 
    // RequestID, otherwise streaming pausing/resuming will fail. 
    firstRequestIndex = 0;  // Assume we're starting at index 0
    firstRequestID = (stream->NextRequestIDToProcess - stream->numRequests); // If the stream was paused, then we must check for the first ID

    for (k = 0; k < stream->numRequests; ++k) {
        if (((PQBULKSTREAM)(stream->ioRequest))[k].RequestID == firstRequestID) {
            firstRequestIndex = k;
            break;
        }
    }

    // We must hold the lock on the stream while we issue the initial set of requests so that
    // if one of the initial requests completes before we have issued all of the intial requests
    // it does not try to reissue itself (streaming only) causing this main thread to deadlock
    QUSB_LOCK(stream); {
        // Issue requests
        requestIndex = firstRequestIndex;
        for (k = 0; k < stream->numRequests; ++k) {
            // Set the RequestID to the next one to process so that the request will be issued.  Just before the
            // request is issued the real RequestID will be set (in QuickUsbIssueStreamBulkRequest).
            ((PQBULKSTREAM)(stream->ioRequest))[requestIndex].RequestID = stream->NextRequestIDToProcess;

            // We need to indicate to our internal completion routine that these requests
            // are streams and need to be re-issued upon completion
            ((PQBULKSTREAM)(stream->ioRequest))[requestIndex].InternalStatus = (QUSB_BULKSTREAM_MAGIC | QUSB_BULKSTREAM_AUTO_REISSUE | QUSB_BULKSTREAM_IO_COMPLETE);

            // Issue async request.  We're already holding the lock so don't try to get it again or we'll deadlock (thus acquireLock = FALSE)
            qResult = QuickUsbIssueStreamBulkRequest((QBYTE) stream->StreamID, stream, &((PQBULKSTREAM)(stream->ioRequest))[requestIndex], FALSE); // Sets lastError on failure

            if (!qResult) {
                pendingRequests = stream->pendingRequests;

                // Release the lcok
                QUSB_UNLOCK(stream);

                // If we issued any requests we'll need to clean up
                if (pendingRequests != 0) {
                    // At least one request was successfully issued, so we may simply stop
                    // the stream and let the internal completion routine clean up
                    prevError = lastError;
                    qResult = QuickUsbStopStream(stream->hDevice, (QBYTE) stream->StreamID, FALSE);
                    lastError = prevError;
                }

                // Return with failure (lastError set by call to QuickUsbWriteBulkDataAsync)
                return(FALSE);
            }

            // Move to next request
            if (++requestIndex >= stream->numRequests) {
                requestIndex = 0;
            }
        }
    } QUSB_UNLOCK(stream);

#if DRIVER==MacOSXDriver
    // Dispatch queued requests 
    DispatchRequests(&QusbDeviceInfo[(QULONG) stream->hDevice]);
#endif
    
    // Return successfully
    return(TRUE);
}

#endif // IMPLEMENT_ASYNC


QRETURN QuickUsbPauseStream(QHANDLE hDevice, QBYTE streamID, QBOOL immediate) {
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    QULONG waitTime, maxWaitTime;
    PQIOSTREAM stream;
    QULONG pendingRequests, lastPendingRequests;
    
    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((streamID >= QUICKUSB_MAX_STREAMS) || QusbDeviceInfo[(QULONG) hDevice].stream[streamID] == NULL) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[streamID];

    // Pause streaming
    QUSB_LOCK(stream); {
        stream->info |= QUSB_INFO_STREAMING_PAUSED;
    } QUSB_UNLOCK(stream);

    // Wait for outstanding requests to complete
    if (!immediate) {
        // We must take extra care to not loop forever here as the API should 
        // never lock up.  If the stream fails to shutdown, we need to quit.
        waitTime = 0;
        maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
        lastPendingRequests = 0;
        do {
            if (stream->numThreads == 0) {
                // If we're single-threaded then we need to process the rest of the
                // requests from this thread

                // Check if there's more to do
                if (stream->pendingRequests == 0) {
                    break;
                }
                
                // Process remaining requests
                if (!QuickUsbProcessSTStream(stream, QUSB_TIGHT_LOOP_WAIT)) { // Sets lastError on failure
                    return(FALSE);
                }
            } else {
                // If we're multi-threaded then we'll have our threads process the
                // remaining requests

                // Read the shared 'pendingRequests' variable
                QUSB_LOCK(stream); {
                    pendingRequests = stream->pendingRequests;
                } QUSB_UNLOCK(stream);
                
                if (pendingRequests == 0) {
                    break;
                }
                
                if (pendingRequests != lastPendingRequests) {
                    lastPendingRequests = pendingRequests;
                    waitTime = 0;
                }

                SleepMS(QUSB_TIGHT_LOOP_WAIT);
                waitTime += QUSB_TIGHT_LOOP_WAIT;
            }
        } while (waitTime < maxWaitTime);
        
        if (pendingRequests != 0) {
            // Read the shared 'pendingRequests' variable one final time
            QUSB_LOCK(stream); {
                pendingRequests = stream->pendingRequests;
            } QUSB_UNLOCK(stream);
        }
        
        // If the stream did not shutdown, report an error
        if (pendingRequests != 0) {
            lastError = QUICKUSB_ERROR_TIMEOUT;
            return(FALSE);
        }
    }

    // Return successfully
    return(TRUE);
#endif
}



QRETURN QuickUsbResumeStream(QHANDLE hDevice, QBYTE streamID) {
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;
    PQIOSTREAM stream;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((streamID >= QUICKUSB_MAX_STREAMS) || QusbDeviceInfo[(QULONG) hDevice].stream[streamID] == NULL) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[streamID];

    // Verify the stream is paused
    QUSB_LOCK(stream); {
        if (!(stream->info & QUSB_INFO_STREAMING_PAUSED)) {
            // Release the lock
            QUSB_UNLOCK(stream);

            // Return with error
            lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
            return(FALSE);
        }

        // Reset pause bit
        stream->info &= ~QUSB_INFO_STREAMING_PAUSED;
    } QUSB_UNLOCK(stream);

    // Reissue data requests
    if ((streamID != QUICKUSB_ASYNC_STREAM) && (streamID != QUICKUSB_LEGACY_ASYNC_STREAM)) {
        if (!QuickUsbStartStream(stream)) { // Sets lastError on failure
            return(FALSE);
        }
    }

    // Return successfully
    return(TRUE);
#endif
}



#ifdef IMPLEMENT_ASYNC

void CompleteBulkRequest(PQIOSTREAM stream, PQBULKSTREAM bulkStream, QULONG bytesTransferred, QULONG requestResult) {
    QRESULT reissued;
    QULONG info;
    QULONG requestError;
    
    // Set the number of bytes transferred
    bulkStream->BytesTransferred = bytesTransferred;
    
    // Copy over the error
    if (bulkStream->BytesRequested == bulkStream->BytesTransferred) {
        bulkStream->Error = QUICKUSB_ERROR_NO_ERROR;
    } else {
        lastDriverError = requestResult;
        bulkStream->Error = TranslateSystemError(requestResult);
        
        // Make sure that we report an error as we did not receive the correct amount of data
        if (bulkStream->Error == QUICKUSB_ERROR_NO_ERROR) {
            bulkStream->Error = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
        }
    }
    requestError = bulkStream->Error;

    // Save the error to the stream for later reporting
    if (stream->lastError == QUICKUSB_ERROR_NO_ERROR) {
        stream->lastError = requestError;
    }
    
    // Keep track whether or not the transaction was aborted.  This information is not yet used
    // for anything but could be useful in the future.
#if DRIVER==WindowsDriver
    if (bulkStream->Error == ERROR_OPERATION_ABORTED) {
        bulkStream->InternalStatus |= QUSB_BULKSTREAM_ABORTED;
    }
#endif
    
    // Check if the user supplied a completion routine
    if (bulkStream->CompletionRoutine) {
        // First we must figure out what kind of completion routine to call
        switch (bulkStream->CompletionType) {
            case QUICKUSB_CRTYPE_VOID_QBULKSTREAM:
                bulkStream->InternalStatus |= QUSB_BULKSTREAM_IN_CR;
                ((PQBULKSTREAM_COMPLETION_ROUTINE)bulkStream->CompletionRoutine)(bulkStream);
                bulkStream->InternalStatus &= ~QUSB_BULKSTREAM_IN_CR;
                break;
            default:
                // Internal error
                break;
        }
    }
    
    // NOTE: The user is allowed to 'clear' the error on a failed transaction to indicated that
    // the stream should continue on and not halt.  The user does so by clearing the error
    // from within the completion routine, which is reflected in bulkStream->Error.  If the user
    // cleared the error then we should also clear the stream error.
    if ((bulkStream->Error == QUICKUSB_ERROR_NO_ERROR) && (stream->lastError == requestError)) {
        stream->lastError = QUICKUSB_ERROR_NO_ERROR;
    }
    
    // Signal that this transaction has completed
    bulkStream->InternalStatus |= QUSB_BULKSTREAM_IO_COMPLETE;
    
    // Check if this request was re-issued in the users completion routine
    reissued = (bulkStream->InternalStatus & QUSB_BULKSTREAM_IO_COMPLETE) == 0;

    // Check if we need to internally reissue the request.  We must be careful not to
    // reissue transactions if the last one failed because the device was removed.
    if ((bulkStream->InternalStatus & QUSB_BULKSTREAM_AUTO_REISSUE) && !reissued && (bulkStream->Error == 0)) {
        // Access shared stream data
        QUSB_LOCK(stream); {
            // Only count successful transfers
            if (requestError == QUICKUSB_ERROR_NO_ERROR) {
                ++(stream->totalNumberOfTransfers);
            }

            if (stream->maxTransfers != 0) {
                // If we have reached the total number of transfers we may issue then pause the stream
                if ((stream->totalNumberOfTransfers + stream->pendingRequests - 1) >= stream->maxTransfers) {
                    stream->info |= QUSB_INFO_STREAMING_PAUSED;
                }
            }

            // Get stream info
            info = stream->info;
        } QUSB_UNLOCK(stream);
        

        // Only re-issue the request if the stream is running and we are not going to exceed the 
        // maximum number of transfers allowed
        if (!(info & (QUSB_INFO_STREAMING_PAUSED | QUSB_INFO_STREAMING_STOP))) {
            // Reissue the request
            reissued = QuickUsbIssueStreamBulkRequest((QBYTE) stream->StreamID, stream, bulkStream, TRUE); // Sets lastError on failure

            // If we fail to reissue the request, we need to propagate the error back to the user
            // as to why.  We've already called the completion routine for the request, so now
            // we must call the completion routine as a 'special case' to indicate failure.
            if (!reissued && bulkStream->CompletionRoutine) {
                // Save the error to the stream for later reporting
                if (stream->lastError == QUICKUSB_ERROR_NO_ERROR) {
                    stream->lastError = lastError;
                }

                // First we must figure out what kind of completion routine to call
                // Note: lastError has already been set from previous call to IssueBulkDataAsync
                switch (bulkStream->CompletionType) {
                    case QUICKUSB_CRTYPE_VOID_QBULKSTREAM:
                        bulkStream->InternalStatus |= QUSB_BULKSTREAM_IN_CR;
                        ((PQBULKSTREAM_COMPLETION_ROUTINE)bulkStream->CompletionRoutine)(NULL);
                        bulkStream->InternalStatus &= ~QUSB_BULKSTREAM_IN_CR;
                        break;
                    default:
                        // Internal error
                        break;
                }
            }
        }
    }
    
    // We must protect access to shared information across threads
    QUSB_LOCK(stream); {
        // Finally, indicate that our completion routine is done
        bulkStream->InternalStatus |= QUSB_BULKSTREAM_CR_COMPLETE;
        
        // Move on to the next request to process
        ++stream->NextRequestIDToProcess;
        
#ifdef _DEBUG
        // Keep track of outstanding requests
        if (stream->pendingRequests == 0) {
            printf("Internal error\n");
        }
#endif

        // Keep track of statistics
        if (stream->StreamType & QUSB_STREAMTYPE_IN) {
            QusbDeviceInfo[(QULONG) stream->hDevice].totalBytesRead += bytesTransferred;
            QusbDeviceInfo[(QULONG) stream->hDevice].throughputBytesRead += bytesTransferred;
        } else {
            QusbDeviceInfo[(QULONG) stream->hDevice].totalBytesWritten += bytesTransferred;
            QusbDeviceInfo[(QULONG) stream->hDevice].throughputBytesWritten += bytesTransferred;
        }

        // The absolute last thing to do it decrement the number of pending requests
        --(stream->pendingRequests);
    } QUSB_UNLOCK(stream);
}



QRESULT QuickUsbProcessSTStream(PQIOSTREAM stream, QULONG milliseconds) {
    // Windows implementation
#if DRIVER==WindowsDriver
    BOOL winResult;
    PQBULKSTREAM bulkStream;
    DWORD bytes, requestResult;
    ULONG_PTR completionKey;
    LPOVERLAPPED ovl;

    winResult = GetQueuedCompletionStatus(stream->hIOCP, &bytes, &completionKey, &ovl, milliseconds);
    requestResult = GetLastError();

    // Check GetQueuedCompletionStatus value
    if (!winResult) {

        if (ovl == NULL) {
            // Documentation on GetQueuedCompletionStatus from MSDN:
            // If *lpOverlapped is NULL, the function did not dequeue a completion packet from the 
            // completion port. In this case, the function does not store information in the variables 
            // pointed to by the lpNumberOfBytes and lpCompletionKey parameters, and their values are 
            // indeterminate. 
            // ...
            // If a completion packet does not appear within the specified time, the function times out, 
            // returns FALSE, and sets *lpOverlapped to NULL.

            lastError = QUICKUSB_ERROR_TIMEOUT;
            return(FALSE);
        } else {
            // Documentation on GetQueuedCompletionStatus from MSDN:
            // If *lpOverlapped is not NULL and the function dequeues a completion packet for a failed 
            // I/O operation from the completion port, the function stores information about the failed 
            // operation in the variables pointed to by lpNumberOfBytes, lpCompletionKey, and 
            // lpOverlapped. To get extended error information, call GetLastError. 
        }
    }

    // Was the completed request a command to this thread
    if (completionKey == QUSB_THREAD_CMD_SHUTDOWN) {
        // Returning false to indicate that the thread needs to shutdown
        lastError = QUICKUSB_ERROR_NOT_STREAMING;
        return(FALSE);
    }
    
    // The OVERLAPPED parameter is really a pointer to a QBULKSTREAM structure that contains the
    // OVERLAPPED variable at the start of the structure.
    bulkStream = (PQBULKSTREAM)ovl;
#endif

    // Linux implementation
#if DRIVER==LinuxDriver
    PQBULKSTREAM bulkStream;
    QULONG bytes;
    struct timespec timeout;
    struct io_event *events;
    int result, requestResult;
    
    // Allocate event list
    events = (struct io_event *) malloc(1 * sizeof(struct io_event));
    
    do {
        // Wait for the requests (events) to complete
        memset(events, 0, sizeof(struct io_event));
        if (milliseconds == QUICKUSB_INFINITE_TIME) {
            result = io_getevents(stream->hIOCP, 1, 1, events, NULL);
        } else {
            // Calculate sec and nsec for timeout
            int nsec, sec;
            sec = (milliseconds / 1000); // Always rounds down
            nsec = (milliseconds - (sec * 1000)) * 1e6;

            timeout.tv_sec = sec;
            timeout.tv_nsec = nsec;

            result = io_getevents(stream->hIOCP, 1, 1, events, &timeout);
        }

        if (result == 0) {
            // io_getevents returns 0 when the requested number of events did not complete or the timeout elapsed
            free(events);
            
            lastError = QUICKUSB_ERROR_TIMEOUT;
            return(FALSE);
        } else if (result == -1) {
            // The call failed - error information is in errno
            if (errno == EINTR) {
                // The io_getevents call was interrupted by a signal handler...just continue on processing
                continue;
            }
            
            free(events);
            
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        } else if (result != 1) {
            // We only asked for 1 event, if the call succeeded with more than 1
            // even, something is really wrong
            free(events);
    
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        } else {
            // Everything is good, so we'll break out of the loop
            break;
        }
    } while (TRUE);

    // The .obj field of the event is the IOCB object, which really is a  pointer to a QBULKSTREAM 
    // structure that contains the IOCB variable at the start of the structure.
#ifdef __amd64
    bulkStream = (PQBULKSTREAM) events[0].obj;
#else
    bulkStream = (PQBULKSTREAM) (__u32) events[0].obj;
#endif

    // Check if event was to shutdown
    if ((bulkStream->Buffer == NULL) && (bulkStream->BytesTransferred == QUSB_THREAD_CMD_SHUTDOWN)) {
        // The QBULKSTREAM object that was allocated to issue the special command
        // must now be freed
        free(bulkStream);
        free(events);
        
        // Returning false to indicate that the thread needs to shutdown
        lastError = QUICKUSB_ERROR_NOT_STREAMING;
        return(FALSE);
    }
    
    // Extract the error and number of bytes
    requestResult = events[0].res;
    bytes = events[0].res2;

    // Free memory
    free(events);
#endif
    
#if DRIVER==MacOSXDriver
    PQBULKSTREAM bulkStream;
    QULONG bytes;
    int requestResult;
    
    switch (CFRunLoopRunInMode(kCFRunLoopDefaultMode, milliseconds / 1000.0, TRUE))
    {
        case kCFRunLoopRunFinished:
            // This should not occur
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
#ifdef _DEBUG
            //printf("*** Runloop Finished ***\n");
#endif
            return(FALSE);
            
        case kCFRunLoopRunStopped:
            // Returning false to indicate that the thread needs to shutdown
            lastError = QUICKUSB_ERROR_NOT_STREAMING;
#ifdef _DEBUG
            //printf("*** RunLoop %p stopping on thread %i\n", CFRunLoopGetCurrent(), gettid());
#endif
            return(FALSE);
            
        case kCFRunLoopRunTimedOut:
#ifdef _DEBUG
            //printf("*** Runloop: Transaction timed out ***\n");
#endif
            break;
            
        case kCFRunLoopRunHandledSource:
#ifdef _DEBUG
            //printf("*** Runloop: Async transaction completed ***\n");
#endif
            break;
    }
    
    // When requests are issued, the MacStreamBulkCallback function is specified as the
    // callback function to be executed when picked up by a RunLoop.  The MacStreamBulkCallback
    // function then calls CompleteBulkRequest, so all we must do here is return.
    return(TRUE);
#endif

    // Complete the requests.  This handles filling out the bulkStream object,
    // calling the completion routine, and re-issuing requests.
    CompleteBulkRequest(stream, bulkStream, bytes, (QULONG) requestResult);

    // Return successfully
    return(TRUE);
}



// Stop a stream (does not release threads or free resources)
QRESULT StopStream(PQIOSTREAM stream, QBOOL immediate) {
    QULONG waitTime, maxWaitTime, transaction;
    QULONG bytes;
    QULONG pendingRequests, prevPendingRequests;
    
    // Stop streaming
    QUSB_LOCK(stream); {
        stream->info |= QUSB_INFO_STREAMING_STOP;
    } QUSB_UNLOCK(stream);

    // Stop the legacy async stream
    if (stream->StreamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
        if (stream->pendingRequests > 0) {
            // We can easily clean up all pending legacy requests simply by waiting
            // on all of them
            for (transaction=0; transaction < 255; ++transaction) {
                if ((((PQBULKSTREAM)(stream->ioRequest))[transaction]).Buffer != NULL) {
                    QuickUsbAsyncWait(stream->hDevice, &bytes, (QBYTE)transaction, immediate); // Sets lastError on failure

                    // If QuickUsbAsyncWait returns unsuccessfully then we should ignore the error 
                    // since the user didn't wait on the pending requests before closing the device
                    // (causing the stream to stop) and we have no way to inform the user of errors
                    // for each request this occurs for.
                }
            }
        }
    } else {
        // Wait for outstanding requests to complete
        if (!immediate) {
            if (stream->numThreads == 0) {
                // If we're single-threaded then we need to process the rest of the
                // requests from this thread.  We do not have to worry about locking
                // up since all the requests will automatically timeout if they 
                // cannot be serviced
                while (stream->pendingRequests != 0) {
                    if (!QuickUsbProcessSTStream(stream, QusbDeviceInfo[(QULONG) stream->hDevice].timeout)) { // Sets lastError on failure
                        // If QuickUsbProcessSTStream returns unsuccessfully it is 
                        // because it timed out.  We'll only ever catch that here
                        // if there is an internal error with requests not properly
                        // timing out, so we will assert for easier debugging.
#if defined(_DEBUG)
                        assert(0);
#endif
                        // We want to overwrite the error set by 'QuickUsbProcessSTStream' so that we
                        // catch the fact that this is an internal error.
                        lastError = QUICKUSB_ERROR_INTERNAL_ERROR;
                        return(FALSE);
                    }
                }
            } else {
                // For multithreaded streams, we'll have our threads process pending
                // requests.  We must take extra care to not loop forever here as the 
                // API should never lock up.  If the stream fails to shutdown, we need 
                // to return with error.

                // Read the shared 'pendingRequests' variable
                QUSB_LOCK(stream); {
                    pendingRequests = stream->pendingRequests;
                } QUSB_UNLOCK(stream);
                prevPendingRequests = pendingRequests;

                waitTime = 0;
                maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
                while ((pendingRequests != 0) && (waitTime < maxWaitTime)) {
                    // Read the shared 'pendingRequests' variable
                    QUSB_LOCK(stream); {
                        pendingRequests = stream->pendingRequests;
                    } QUSB_UNLOCK(stream);

                    // If there's more processing to do
                    if (pendingRequests != 0) {
                        // If we have made progress in completing requests, don't sleep and reset our timeout
                        if (prevPendingRequests != pendingRequests) {
                            prevPendingRequests = pendingRequests;
                            maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
                        } else {
                            SleepMS(QUSB_TIGHT_LOOP_WAIT);
                            waitTime += QUSB_TIGHT_LOOP_WAIT;
                        }
                    }
                }

                // If the stream did not shutdown, report an error
                if (pendingRequests != 0) {
                    lastError = QUICKUSB_ERROR_TIMEOUT;
                    return(FALSE);
                }
            }
        }
    }

    // Return successfully
    return(TRUE);
}

#endif // IMPLEMENT_ASYNC



QRETURN QuickUsbStopStream(QHANDLE hDevice, QBYTE streamID, QBOOL immediate) {
/*---------------------------------------------------------------------------
 Purpose : Stop a previously started read or write data stream.
 Input   : hDevice - Offset into array of QUSB_DEVICE_INFO structures
           immediate - 
 Output  : None
 Returns : Non-zero on success, zero on failure.  Extended error information
             is stored in lastError and retrieved with QuickUsbGetLastError.
 Notes   : If immediate is true, you must ensure that the stream has fully
             stopped before freeing the stream buffers (if they were 
             user-allocated).
 ---------------------------------------------------------------------------*/
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
#if DRIVER==WindowsDriver
    BOOL winResult;
#endif

    QDEVHANDLE hDevHandle;
    QULONG count;
    QBOOL ok;
    QULONG waitTime, maxWaitTime;
    PQIOSTREAM stream;
    QULONG k;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((streamID >= QUICKUSB_MAX_STREAMS) || (QusbDeviceInfo[(QULONG) hDevice].stream[streamID] == NULL)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[streamID];

    // Stop the stream
    if (!StopStream(stream, immediate)) { // Sets lastError on failure
        // If we're unable to stop the stream then we must report an error.  This
        // could occur if the completion routine callbacks are blocked causing
        // the worker threads to block and not die.  It could also occur if 
        // StopStream/PauseStream is called from within a completion routine with 
        // the immediate parameter set to 'false'.  If an error is not returned
        // then stream resources will be freed on objects that are still in use,
        // causing a program crash.
        return(FALSE);
    }

    // If we're supposed to return immediately then return without error.  All
    // resources will later be freed when this function is called with immediate
    // set to false.
    if (immediate) {
        return(TRUE);
    }

    // Kill threads
    QUSB_LOCK(stream); {
        count = stream->numThreads;
    } QUSB_UNLOCK(stream);
    
#if DRIVER==WindowsDriver
    for (k = 0; k < count; ++k) {
        winResult = PostQueuedCompletionStatus(stream->hIOCP, sizeof(QULONG), (ULONG_PTR)QUSB_THREAD_CMD_SHUTDOWN, NULL);
        if (!winResult) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
    }
#endif
    
#if DRIVER==LinuxDriver
    if (count > 0) {
        int result;
        struct iocb *pIOCB;
        PQBULKSTREAM bs;

        for (k = 0; k < count; ++k) {
            // Allocate each BulkStream object
            bs = (PQBULKSTREAM) malloc(sizeof(QBULKSTREAM));
            memset(bs, 0, sizeof(struct QBULKSTREAM));
            pIOCB = &(bs->InternalData);
        
            // Initialize shutdown request
            init_iocb(pIOCB, stream->handle, NULL, QUSB_THREAD_CMD_SHUTDOWN, 0, IOCB_CMD_PREAD);
            bs->BytesTransferred = QUSB_THREAD_CMD_SHUTDOWN;

            // Issue shutdown request
            result = io_submit(stream->hIOCP, 1, &pIOCB); // Returns the number of requests successfully issued
            if (result != 1) {
                free(bs);

                // Errors will be reported as timeouts, so just continue on...
            }
        }
    }
#endif
    
#if DRIVER==MacOSXDriver
    // Kill threads on Mac
    if (stream->runLoop) {
        QULONG k;
        QBOOL done;
        
        waitTime = 0;
        maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
        do {
            // Issue stops on all running RunLoops
            done = TRUE;
            QUSB_LOCK(stream); {
                for (k = 0; k < stream->numThreadHandles; ++k) {
                    if (stream->runLoop[k] != NULL) {
                        // Signal the RunLoop to stop
#ifdef _DEBUG
                        //printf("Stopping runloop at %p\n", runLoop);
#endif
                        CFRunLoopStop(stream->runLoop[k]);
                        done = FALSE;
                    }
                }
            } QUSB_UNLOCK(stream);
            
            if (!done) {
                SleepMS(QUSB_TIGHT_LOOP_WAIT);
                waitTime += QUSB_TIGHT_LOOP_WAIT;
            }
        } while (!done && (waitTime < maxWaitTime));
    }
    
    // Verify all threads have died
    QUSB_LOCK(stream); {
        ok = (stream->numThreads == 0);
    } QUSB_UNLOCK(stream);
#else
    // Check that the threads have all died, or wait longer
    waitTime = 0;
    maxWaitTime = max(QUSB_TIGHT_LOOP_MIN_TIMEOUT, QusbDeviceInfo[(QULONG) stream->hDevice].timeout);
    ok = FALSE;
    do {
        QUSB_LOCK(stream); {
            if (stream->numThreads == 0) {
                ok = TRUE;
            }
        } QUSB_UNLOCK(stream);

        if (!ok) {
            SleepMS(QUSB_TIGHT_LOOP_WAIT);
            waitTime += QUSB_TIGHT_LOOP_WAIT;
        }
    } while (!ok && (waitTime < maxWaitTime));
#endif
    
    // Check that the threads have all exited
    if (!ok) {
        lastError = QUICKUSB_ERROR_TIMEOUT;
        return(FALSE);
    }

    // Close thread handles
#if DRIVER==WindowsDriver
    for (k = 0; k < stream->numThreadHandles; ++k) {
        if (stream->hThread[k] != NULL) {
            winResult = CloseHandle(stream->hThread[k]);
            stream->hThread[k] = NULL;
        }
    }
#endif
    
    // Free the list of thread handles
    if (stream->hThread != NULL) {
        free(stream->hThread);
        stream->hThread = NULL;
    }
    stream->numThreadHandles = 0;

#if DRIVER==WindowsDriver
    // Close the IO completion port
    if (stream->hIOCP) {
        winResult = CloseHandle(stream->hIOCP);
        stream->hIOCP = NULL;
    }
#endif
    
#if DRIVER==LinuxDriver
    // Destroy the Aysynchronous IO context
    if (stream->hIOCP) {
        int result = io_destroy(stream->hIOCP);
        if (result != 0) {
            lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
            return(FALSE);
        }
        stream->hIOCP = 0;
    }

    // Free the thread concurrency semaphore
    if (stream->concurrencySem != NULL) {
        sem_destroy(stream->concurrencySem);
        free(stream->concurrencySem);
        stream->concurrencySem = NULL;
    }
#endif
    
#if DRIVER==MacOSXDriver
    if (stream->runLoop != NULL) {
        free(stream->runLoop);
        stream->runLoop = NULL;
    }
    
    // Free the thread concurrency semaphore
    if (stream->concurrencySem != NULL) {
        sem_close(stream->concurrencySem);
    }
#endif
        
    // Free ioRequest objects
    if (stream->ioRequest != NULL) {
        // Free internally allocated data buffers
        if (stream->info & QUSB_INFO_INTERNAL_ALLOC) {
            for (k = 0; k < stream->numRequests; ++k) {
                if (((PQBULKSTREAM)(stream->ioRequest))[k].Buffer != NULL) {
                    QuickUsbFreeDataBuffer(&((PQBULKSTREAM)(stream->ioRequest))[k].Buffer);
                }
            }
        }

        free(stream->ioRequest);
        stream->ioRequest = NULL;
    }
    stream->numRequests = 0;

    // Close handle to file opened by the stream (if is not the main handle)
    if ((stream->handle != 0) && (stream->handle != hDevHandle)) {
#if DRIVER==WindowsDriver
        CloseHandle(stream->handle);
#endif
#if DRIVER==LinuxDriver
        close(stream->handle);
#endif
        stream->handle = 0;
    }

    // For streams to files, close the file handle
    if (stream->hFile != 0) {
#if DRIVER==WindowsDriver
        CloseHandle(stream->hFile);
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        if (stream->info & QUSB_INFO_MEMORY_FILE) {
            // TODO: For memory files, uncomment the following line
            //close((QMEMFILEHANDLE) (QULONG) stream->hFile);
        } else {
            fclose((QFILEHANDLE) stream->hFile);
        }
#endif
		stream->hFile = 0;
    }

	// For memory-mapped files, we need to unmap the memory view
	if (stream->memFileView != 0) {
#if DRIVER==WindowsDriver
		UnmapViewOfFile(stream->memFileView);
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        if (munmap(stream->memFileView, stream->memFileSize) < 0) {
            // Failed to unmap memory file
        }
#endif
		stream->memFileView = 0;
	}

    // Free the critical section mutex
    if (stream->lock != NULL) {
#if DRIVER==WindowsDriver
        DeleteCriticalSection(stream->lock);
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        pthread_mutex_destroy(stream->lock);
#endif
        free(stream->lock);
        stream->lock = NULL;
    }

    // Free the stream entry
    QusbDeviceInfo[(QULONG) hDevice].stream[streamID] = NULL;
    free(stream);
    stream = NULL;

    // Return successfully
    return(TRUE);
#endif
}



QRETURN QuickUsbProcessStream(QHANDLE hDevice, QBYTE streamID, QULONG milliseconds) {
#ifndef IMPLEMENT_ASYNC
    lastError = QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED;
    return(FALSE);
#else
    QDEVHANDLE hDevHandle;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((streamID >= QUICKUSB_MAX_STREAMS) || QusbDeviceInfo[(QULONG) hDevice].stream[streamID] == NULL) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Process IO completion packets
    return QuickUsbProcessSTStream(QusbDeviceInfo[(QULONG) hDevice].stream[streamID], milliseconds); // Sets lastError on failure
#endif
}



#ifdef IMPLEMENT_ASYNC

#if DRIVER==MacOSXDriver

QULONG DispatchRequest(PQUSB_DEVICE_INFO qusb, PQBULKSTREAM bulkStream) {
    IOReturn ioResult;
    IOUSBDevRequestTO request;
    kern_return_t err;
    QULONG swappedLength;
    PQIOSTREAM stream = (PQIOSTREAM) bulkStream->InternalData;
    
    if (bulkStream->StreamType & QUSB_STREAMTYPE_IN) {
        // On Windows and Linux the driver handles sending out the number of bytes to
        // read before bulk reads, but in Mac their is no driver so we must send out
        // the control request containing the number of bytes to read here.
        
        // Swap bytes
        // Note: The block size MUST be swapped to little endian before the
        //       control request. If block size is not swapped, the number
        //       of bytes requested to be read will be unexpected. As a
        //       result, the module could be stalled on a read and no writes
        //       are possible until the requested amount is actually read.
        swappedLength = CFSwapInt32HostToLittle(bulkStream->BytesRequested);
        
        // Build the vendor request
        request.bmRequestType = USBmakebmRequestType(kUSBOut, kUSBVendor, kUSBDevice);
        request.bRequest = QUICKUSB_READDATALEN;
        request.wValue = 0;
        
        request.wIndex = 0;
        request.wLength = sizeof(QULONG);
        request.pData = &swappedLength;
        request.wLenDone = 0;
        request.noDataTimeout = qusb->timeout;
        request.completionTimeout = qusb->timeout;
        
        // Send control request
        err = (*stream->handle)->ControlRequestTO(stream->handle, 0, &request);
        if (err != kIOReturnSuccess) {
            // Clear the bulk stream
            memset(bulkStream, 0, sizeof(QBULKSTREAM));
            
            // Return with error
            return QUICKUSB_ERROR_IOCTL_FAILED;
        }
        
        // Issue the bulk read
        ioResult = (*stream->handle)->ReadPipeAsyncTO(stream->handle,               // Interface
                                                      qusb->inPipe,                 // Pipe ref
                                                      bulkStream->Buffer,           // Buffer
                                                      bulkStream->BytesRequested,   // Size
                                                      qusb->timeout,                // Data timeout
                                                      qusb->timeout,                // Completion timeout
                                                      MacStreamBulkCallback,        // Callback
                                                      bulkStream);                  // RefCon
#ifdef _DEBUG
        //printf("--> Dispatched read\n");
#endif
    } else {
        // Issue the bulk write
        ioResult = (*stream->handle)->WritePipeAsyncTO(stream->handle,              // Interface
                                                       qusb->outPipe,               // Pipe ref
                                                       bulkStream->Buffer,          // Buffer
                                                       bulkStream->BytesRequested,  // Size
                                                       qusb->timeout,               // Data timeout
                                                       qusb->timeout,               // Completion timeout
                                                       MacStreamBulkCallback,       // Callback
                                                       bulkStream);                 // RefCon
#ifdef _DEBUG
        //printf("--> Dispatched write\n");
#endif
    }
    
    // Check the the bulk request was successfully issued
    if (ioResult != kIOReturnSuccess) {
        // Clear the bulk stream
        memset(bulkStream, 0, sizeof(QBULKSTREAM));
        
        // Return with error
        return QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
    }
    
    // Keep track of pending transactions
    if (bulkStream->StreamType & QUSB_STREAMTYPE_IN) {
        ++(qusb->pendingInRequests);
    } else {
        ++(qusb->pendingOutRequests);
    }
    
    // Return sucessfully
    return 0;
}



void DispatchRequests(PQUSB_DEVICE_INFO qusb) {
    PQBULKSTREAM request;
    QULONG result;
    QBOOL issued;
    
#ifdef _DEBUG
    //printf("DispatchRequests\n");
#endif
    QUSB_LOCK(qusb); {
        do {
            issued = false;
            
            // Peek at the next request we must issue
            if (qusb->requestQ->empty()) {
#ifdef _DEBUG
                //printf("--> Queue empty\n");
#endif
                break;
            }
            request = qusb->requestQ->front();
            
            // Is the request a read or a write?
            if (request->StreamType & QUSB_STREAMTYPE_IN) {
                // For reads, we may issue requests as long as there are no pending writes
                if (qusb->pendingOutRequests == 0) {
                    result = DispatchRequest(qusb, request);
                    if (result != 0) {
                        // We were unable to dispatch the request, so we must complete it here
                        QUSB_UNLOCK(qusb);
                        CompleteBulkRequest((PQIOSTREAM) request->InternalData, request, 0, kIOUSBUnknownPipeErr);
#ifdef _DEBUG
                        //printf("--> Dispatch failed\n");
#endif
                        QUSB_LOCK(qusb);
                    }
                    qusb->requestQ->pop();
                    issued = true; // Continue dispatching requests
                } else {
#ifdef _DEBUG
                    //printf("--> Cannot dispatch reads as there are pending writes\n");
#endif
                }
            } else {
                // For writes, we may issue requests as long as there are no pending reads
                if (qusb->pendingInRequests == 0) {
                    result = DispatchRequest(qusb, request);
                    if (result != 0) {
                        // We were unable to dispatch the request, so we must complete it here
                        QUSB_UNLOCK(qusb);
                        CompleteBulkRequest((PQIOSTREAM) request->InternalData, request, 0, kIOUSBUnknownPipeErr);           
#ifdef _DEBUG
                        //printf("--> Dispatch failed\n");
#endif
                        QUSB_LOCK(qusb);
                    }
                    qusb->requestQ->pop();
                    issued = true; // Continue dispatching requests
                } else {
#ifdef _DEBUG
                    //printf("--> Cannot dispatch writes as there are pending reads\n");
#endif
                }
            }
        } while (issued);
    } QUSB_UNLOCK(qusb);
#ifdef _DEBUG
    //printf("Dispatch complete\n");
#endif
}



/*void QueueRequestAndDispatch(PQUSB_DEVICE_INFO qusb, PQBULKSTREAM bulkStream) {
    // Queue the request
    QUSB_LOCK(qusb); {
        qusb->requestQ.push(bulkStream);
    } QUSB_UNLOCK(qusb);
    
    // Dispatch requests
    DispatchRequests(qusb);
}*/



void MacStreamBulkCallback(void *refcon, IOReturn result, void *arg) {
    // 'refcon' is QBULKSTREAM object for the request
    // 'result' is error code for the request
    // 'arg' is the number of bytes transferred
    PQBULKSTREAM bulkStream = (PQBULKSTREAM) refcon;
    PQIOSTREAM   stream = (PQIOSTREAM) bulkStream->InternalData;
    PQUSB_DEVICE_INFO qusb = &QusbDeviceInfo[bulkStream->Handle];
    
    // Complete the requests.  This handles filling out the bulkStream object,
    // calling the completion routine, and re-issuing requests.
    CompleteBulkRequest(stream, bulkStream, (int)(long) arg, (int)(long) result);
    
    // Keep track of pending transactions
    QUSB_LOCK(qusb); {
        if (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_IN_EP6) {
            --(qusb->pendingInRequests);
#ifdef _DEBUG
            //printf("Completed read on thread %i\n", gettid());
#endif
        } else if (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_OUT_EP2) {
            --(qusb->pendingOutRequests);
#ifdef _DEBUG
            //printf("Completed write on thread %i\n", gettid());
#endif
        } else {
#ifdef _DEBUG
            //printf("ERROR! Completed unknown!\n");
#endif
        }
    } QUSB_UNLOCK(qusb);
    
    // Dispatch queued requests 
    DispatchRequests(qusb);
}

#endif



// Issue a bulk data request on a stream
QRESULT QuickUsbIssueStreamBulkRequest(QBYTE streamID, PQIOSTREAM stream, PQBULKSTREAM bulkStream, QBOOL acquireLock) {
    QBOOL inCR = FALSE;
    QBOOL autoReissue = FALSE;
    QBOOL myTurn;

#if DRIVER==WindowsDriver
    BOOL  winResult;
    QULONG bytesReturned;
#endif
#if DRIVER==LinuxDriver
    int result;
    struct iocb *pIOCB;
#endif

    // Check if this bulk stream has internally stored information
    if ((bulkStream->InternalStatus & QUSB_BULKSTREAM_MAGIC_MASK) == QUSB_BULKSTREAM_MAGIC) {
        // Check if this request has already been issued but not yet completed
        if (!(bulkStream->InternalStatus & QUSB_BULKSTREAM_IO_COMPLETE)) {
            lastError = QUICKUSB_ERROR_NOT_COMPLETED;
            return(FALSE);
        }

        // Check if this request is being reissued from within the users completion routine
        inCR = (bulkStream->InternalStatus & QUSB_BULKSTREAM_IN_CR) != 0;

        // Check if this request should be automatially reissued by the internal completion routine
        autoReissue = (bulkStream->InternalStatus & QUSB_BULKSTREAM_AUTO_REISSUE) != 0;
    }

    // Fill out the rest of the QBULKSTREAM structure
    bulkStream->StructSize = (QBYTE)sizeof(QBULKSTREAM);
    bulkStream->Handle = stream->hDevice;
    bulkStream->CompletionRoutine = stream->cRoutine;
    bulkStream->CompletionType = stream->completionType;
    bulkStream->Tag = stream->tag;
    bulkStream->StreamID = streamID;
    if (bulkStream->StreamType == 0) {  // We cannot overwrite this value so only it if it's empty
        bulkStream->StreamType = stream->StreamType;
    }
    bulkStream->InternalReserved = NULL;
    bulkStream->BytesTransferred = 0;
    bulkStream->Error = 0;
    bulkStream->Reserved = 0;
    bulkStream->InternalStatus = QUSB_BULKSTREAM_MAGIC;
    if (inCR) {
        bulkStream->InternalStatus |= QUSB_BULKSTREAM_IN_CR;
    }
    if (autoReissue) {
        bulkStream->InternalStatus |= QUSB_BULKSTREAM_AUTO_REISSUE;
    }

    // Fill out the requests InternalData
#if DRIVER==WindowsDriver
    // Fill out the overlapped structure
    // Note that hEvent is unused by WinAPI when using APC and is free for internal use,
    // so we will use it to hold a reference to the BulkStream so we may access it
    // in the completion routine.
    if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
        bulkStream->InternalData.hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        bulkStream->InternalStatus |= QUSB_BULKSTREAM_EVENT_OK; // Signal that the hEvent contains a valid handle
    } else {
        bulkStream->InternalData.hEvent = 0;
    }
    bulkStream->InternalData.Offset = 0;
    bulkStream->InternalData.OffsetHigh = 0;
    bulkStream->InternalData.Internal = 0;
    bulkStream->InternalData.Pointer = 0;
#endif
#if DRIVER==LinuxDriver
    // Fill out the iocb structure
    memset(&bulkStream->InternalData, 0, sizeof(struct iocb));
    init_iocb(&bulkStream->InternalData, stream->handle, bulkStream->Buffer, bulkStream->BytesRequested, 0, (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_IN_EP6) ? IOCB_CMD_PREAD : IOCB_CMD_PWRITE);
#endif
#if DRIVER==MacOSXDriver
    bulkStream->InternalData = (PQVOID) stream;
#endif

    // Issue the request asynchronously
    //---------------------------------------------------------------------------------------------
    // MSDN Note: After an instance of an open handle is associated with an I/O completion port, 
    // it cannot be used in the ReadFileEx or WriteFileEx function because these functions have
    // their own asynchronous I/O mechanisms. 

    // If we are using an IOCP, and have more than one thread then it's possible to re-issue 
    // requests out of order.  We must prevent this by waiting for our turn to re-issue a request
    // when using IOCPs.  Note that this only applies to requests that are re-issued, i.e. issued
    // from within a completion routine.  
    do {
        acquireLock ? QUSB_LOCK(stream) : 0; {
            // NOTE: It is still possible for the user to accidentally issue requests out of order 
            // using the new Async API by re-issuing requests from within a completion routine.
            myTurn = (stream->NextRequestIDToProcess == bulkStream->RequestID) || (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) || (streamID == QUICKUSB_ASYNC_STREAM);
            if (myTurn) {
                // Assign the request id
                bulkStream->RequestID = stream->NextRequestID;

                // Issue the async IO request
#if DRIVER==WindowsDriver
                if (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_IN_EP6) {
                    // Send vendor request (Note: QuickUsbVendorRequest is platform independent and sets lastError on failure)
                    if (!QuickUsbVendorRequest(stream->hDevice, QusbDeviceInfo[(QULONG) stream->hDevice].hDevHandle, QUICKUSB_REQUEST_OUT, QUICKUSB_READDATALEN, 0, 0, &(bulkStream->BytesRequested), sizeof(QULONG), &bytesReturned)) {
                        return(FALSE);
                    }
                    //OVERLAPPED ovl;
                    //if (!QuickUsbVendorRequestAsync(stream->hDevice, QUICKUSB_REQUEST_OUT, QUICKUSB_READDATALEN, 0, 0, &(bulkStream->BytesRequested), sizeof(QULONG), &ovl)) {
                    //    return(FALSE);
                    //}
                    winResult = ReadFile(
                        stream->handle, 
                        bulkStream->Buffer, 
                        bulkStream->BytesRequested, 
                        NULL,
                        (LPOVERLAPPED)bulkStream);
                    //if (!QuickUsbVendorRequestAsyncWait(stream->hDevice, &ovl, &bytesReturned)) {
                    //    return(FALSE);
                    //}
                } else if (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_OUT_EP2) {
                    winResult = WriteFile(
                        stream->handle, 
                        bulkStream->Buffer, 
                        bulkStream->BytesRequested, 
                        NULL,
                        (LPOVERLAPPED)bulkStream);
                } else {
                    // Some internal error occurred
                    lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;

                    if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
                        // Close the event handle
                        CloseHandle(bulkStream->InternalData.hEvent);
                    }

                    // Clear the bulk stream
                    memset(bulkStream, 0, sizeof(QBULKSTREAM));

                    // Release the lock
                    acquireLock ? QUSB_UNLOCK(stream) : 0;

                    // Return with error
                    return(FALSE);
                }

                if (!winResult) {
                    lastDriverError = GetLastError();

                    if (lastDriverError != ERROR_IO_PENDING) {
                        // Get the last error
		                lastError = TranslateSystemError(lastDriverError);

                        if (streamID == QUICKUSB_LEGACY_ASYNC_STREAM) {
                            // Close the event handle
                            CloseHandle(bulkStream->InternalData.hEvent);
                        }

                        // Clear the bulk stream
                        memset(bulkStream, 0, sizeof(QBULKSTREAM));

                        // Release the lock
                        acquireLock ? QUSB_UNLOCK(stream) : 0;

                        // Return with error
                        return(FALSE);
                    }
                }
#endif
#if DRIVER==LinuxDriver
                pIOCB = &bulkStream->InternalData;
                result = io_submit(stream->hIOCP, 1, &pIOCB); // Returns the number of requests successfully issued
                if (result != 1) {
                    // Clear the bulk stream
                    memset(bulkStream, 0, sizeof(QBULKSTREAM));

                    // Release the lock
                    acquireLock ? QUSB_UNLOCK(stream) : 0;

                    // Return with error
                    lastError = QUICKUSB_ERROR_UNKNOWN_SYSTEM_ERROR;
                    return(FALSE);
                }
#endif
#if DRIVER==MacOSXDriver
#ifdef _DEBUG
                //if (bulkStream->StreamType == QUSB_STREAMTYPE_BULK_IN_EP6) {
                //    printf("Queued read\n");
                //} else {
                //    printf("Queued write\n");
                //}
#endif
                
                // Queue the request
                QUSB_LOCK(&QusbDeviceInfo[stream->hDevice]); {
                    QusbDeviceInfo[stream->hDevice].requestQ->push(bulkStream);
                } QUSB_UNLOCK(&QusbDeviceInfo[stream->hDevice]);
#endif

                // Increment the request id counter
                ++(stream->NextRequestID);

                // Keep track of outstanding requests
                ++(stream->pendingRequests);
            }

            // This stream could be signaled to stop or pause while spinning on trying to issue 
            // the next request.  In that case we just break out of our loop and return without 
            // error.
            if (stream->info & (QUSB_INFO_STREAMING_STOP | QUSB_INFO_STREAMING_PAUSED)) {
                myTurn = TRUE;
            }
        } acquireLock ? QUSB_UNLOCK(stream) : 0;

        // If it's not our requests turn to be issued, then we should yield our processing time 
        // to another thread (since it's another requests turn in another thread)
        if (!myTurn) {
            SleepMS(0);
        }
    } while (!myTurn);
    
    // Return successfully
    return(TRUE);
}



#if DRIVER==WindowsDriver
DWORD WINAPI QuickUsbStreamThread(__in  LPVOID param) {
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    PQVOID QuickUsbStreamThread(PQVOID param) {
#endif
    // Variables
    PQIOSTREAM stream = (PQIOSTREAM)param;
    QRESULT qResult;
    
#if DRIVER==MacOSXDriver
    QULONG runLoopIndex;
        
    // Add the USB Run Loop Source to this threads Run Loop
    CFRunLoopAddSource(CFRunLoopGetCurrent(), stream->runLoopSource, kCFRunLoopDefaultMode);
#endif
    
    // Keep track of the number of threads
    QUSB_LOCK(stream); {
#if DRIVER==MacOSXDriver
        runLoopIndex = stream->numThreads;
        stream->runLoop[runLoopIndex] = CFRunLoopGetCurrent();
#endif
        
        ++(stream->numThreads);
#ifdef _DEBUG
        //printf("Created thread %i with run loop %p\n", gettid(), CFRunLoopGetCurrent());
#endif
    } QUSB_UNLOCK(stream);
    
    // Loop until the thread is messaged to shutdown
    do {
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        // On Mac and Linux we must manage thread concurrency manually to only allow a specific
        // number of threads to simultaneously issue completion routines.  We do this by having
        // a couting semaphore that blocks when the semaphore has been acquired 
        // 'ThreadConcurrency' times.
        if (stream->concurrencySem != NULL) {
            sem_wait(stream->concurrencySem);
        }
#endif

        // Block until a request completes and is able to be processed
        qResult = QuickUsbProcessSTStream(stream, QUICKUSB_INFINITE_TIME);
        
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        // Release the hold on the semaphore
        if (stream->concurrencySem != NULL) {
            sem_post(stream->concurrencySem);
        }
#endif
    } while (qResult);

    // Decrement the total number of threads
    QUSB_LOCK(stream); {
#ifdef _DEBUG
        //printf("Exiting thread %i\n", gettid());
#endif
        --(stream->numThreads);
        
#if DRIVER==MacOSXDriver
        stream->runLoop[runLoopIndex] = NULL;
#endif
    } QUSB_UNLOCK(stream);

    // Exit thread without error
    return 0;
}

#endif // IMPLEMENT_ASYNC



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbAllocateDataBuffer(PQBYTE *buffer, QULONG bytes) {
    // Verify parameters
    if ((buffer == NULL) || (bytes == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if (DRIVER==WindowsDriver) || (DRIVER==MacOSXDriver)
    // Data in Windows does not need to be aligned
    *buffer = (PQBYTE) malloc(bytes);
#endif
#if (DRIVER==LinuxDriver)
    // Data in Linux needs to be 512-byte aligned for asynchronous requests
    posix_memalign((void**)buffer, 512, bytes);
#endif

    // Check that the buffer was allocated
    if (*buffer == NULL) {
        lastError = QUICKUSB_ERROR_OUT_OF_MEMORY;
        return(FALSE);
    }

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbFreeDataBuffer(PQBYTE *buffer) {
    // Verify parameters
    if ((buffer == NULL) || (*buffer == NULL)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Free buffer
    free(*buffer);
    *buffer = NULL;

    // Return successfully
    return(TRUE);
}



#ifdef IMPLEMENT_ASYNC

/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QVOIDRETURN CompleteFileStream(PQBULKSTREAM bulkStream) {
    QULONG qusbError;
    QFILEHANDLE hFile;
    PQIOSTREAM stream;
    QULONG bytes;

    // Perform some sanity checks
    if (((QULONG)bulkStream->Handle >= MAX_DEVICENUM) || (bulkStream->StreamID >= QUICKUSB_MAX_STREAMS)) {
        // Internal error
#if defined(_DEBUG)
        assert(0);
#endif
        return;
    }

    // Get the stream object
    stream = QusbDeviceInfo[(QULONG) bulkStream->Handle].stream[bulkStream->StreamID];
    if (stream == NULL) {
        // Internal error
#if defined(_DEBUG)
        assert(0);
#endif
        return;
    }

    // Check that the request completed successfully
    if ((bulkStream->Error != QUICKUSB_ERROR_NEED_DATA) && (bulkStream->Error != 0)) {
        // Store the error so it may be later retrieved only if an error has not already been reported
        if (stream->lastError == QUICKUSB_ERROR_NO_ERROR) {
            stream->lastError = bulkStream->Error;
        }

        // When an error occurs, we will pause the stream
        QUSB_LOCK(stream); {
            stream->info |= QUSB_INFO_STREAMING_PAUSED;
        } QUSB_UNLOCK(stream);
        return;
    }

	if ((stream->info & QUSB_INFO_MEMORY_FILE)) {
		// When a request completes on a memory file, we must move the data buffer to the
		// current offset into the file
		bulkStream->Buffer = (stream->memFileView + stream->memFileOffset);
		stream->memFileOffset += bulkStream->BytesRequested;
	} else {
		// Retrieve the file handle
		hFile = (QFILEHANDLE) bulkStream->Tag;

		// Is this an IN stream?
		qusbError = QUICKUSB_ERROR_NO_ERROR;
		if (stream->StreamType & QUSB_STREAMTYPE_IN) {
			// Write data to file
#if DRIVER==WindowsDriver
			if (!WriteFile(hFile, bulkStream->Buffer, bulkStream->BytesTransferred, (PQULONG) &bytes, NULL)) {
				// Get the system error
				qusbError = TranslateSystemError(GetLastError());
			}
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
			bytes = fwrite(bulkStream->Buffer, sizeof(QBYTE), bulkStream->BytesTransferred, hFile);
			if (bytes == 0) {
				// Retrieve the error
				qusbError = TranslateSystemError(errno);
			}
#endif
		} else {
#if DRIVER==WindowsDriver
			if (!ReadFile(hFile, bulkStream->Buffer, bulkStream->BytesRequested, (PQULONG) &bytes, NULL) || (bytes == 0)) {
				// Get the system error
				if (bytes == 0) {
                    // Check if we are support to loop through the file
                    if (stream->info & QUSB_INFO_LOOP_AT_EOF) {
                        if (SetFilePointer(hFile, 0, NULL, FILE_BEGIN) ==  INVALID_SET_FILE_POINTER) {
                            qusbError = QUICKUSB_ERROR_FILE_RW;
                        } else {
                            // Now that we have reset the file to the begining, we can try to read it again
                            if (!ReadFile(hFile, bulkStream->Buffer, bulkStream->BytesRequested, (PQULONG) &bytes, NULL) || (bytes == 0)) {
				                // Get the system error
				                if (bytes == 0) {
					                qusbError = QUICKUSB_ERROR_FILE_EOF;
				                } else {
					                qusbError = TranslateSystemError(GetLastError());
				                }
			                }
                        }
                    } else {
					    qusbError = QUICKUSB_ERROR_FILE_EOF;
                    }
				} else {
					qusbError = TranslateSystemError(GetLastError());
				}
			}
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
			bytes = fread(bulkStream->Buffer, sizeof(QBYTE), bulkStream->BytesRequested, hFile);
			if (bytes == 0) {
				// Check for EOF
				if (feof(hFile)) {
					qusbError = QUICKUSB_ERROR_FILE_EOF;
				} else {
					// Retrieve the error
					qusbError = TranslateSystemError(errno);
				}
			}
#endif
		}

		// Check that there were no errors writing/reading the file
		if (qusbError != QUICKUSB_ERROR_NO_ERROR) {
			// Store the error so it may be later retrieved only if an error has not already been reported
			if (stream->lastError == QUICKUSB_ERROR_NO_ERROR) {
				stream->lastError = qusbError;
			}

			// When an error occurs, we will pause the stream
			QUSB_LOCK(stream); {
				stream->info |= QUSB_INFO_STREAMING_PAUSED;
			} QUSB_UNLOCK(stream);
			return;
		}

		// Check that all data was written/read
		if ((bulkStream->Error != QUICKUSB_ERROR_NEED_DATA) && (bytes != bulkStream->BytesTransferred)) {
			// Store the error so it may be later retrieved only if an error has not already been reported
			if (stream->lastError == QUICKUSB_ERROR_NO_ERROR) {
				stream->lastError = QUICKUSB_ERROR_FILE_RW;
			}

			// When an error occurs, we will pause the stream
			QUSB_LOCK(stream); {
				stream->info |= QUSB_INFO_STREAMING_PAUSED;
			} QUSB_UNLOCK(stream);
			return;
		}
	}
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbReadBulkDataStartStreamToFile(QHANDLE hDevice, PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags) {
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    PQIOSTREAM stream;
    QFILEHANDLE hFile;
    QMEMFILEHANDLE hMemFile;
	PQBYTE memFileBuffer = NULL;
	QULONG memFileSize = 0;
#if DRIVER==WindowsDriver
    DWORD winError;
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((path == 0) || (numBuffers == 0) || (bufferSize == 0) || (streamID == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
	if ((flags & QUICKUSB_STREAM_MEMORY_FILE) && (maxTransfers == 0)) {
		// Memory files must have discrete sizes, so maxTransfers must limit the stream
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
	}

    // Use a realistic number of buffers for low values of maxTransfers
    if ((maxTransfers != 0) && (maxTransfers <= numBuffers)) {
        numBuffers = maxTransfers - 1;
    }

    // Open the file
	if (flags & QUICKUSB_STREAM_MEMORY_FILE) {
		memFileSize = (bufferSize * maxTransfers);

        // Create the memory file
        if (!QuickUsbOpenMemoryFile(path, memFileSize, &hMemFile, &memFileBuffer, flags & 0x0001)) { // Sets lastError on failure
            return(FALSE);
        }
	} else {
#if DRIVER==WindowsDriver
		hFile = CreateFile(
			(LPCSTR) path,          // File Name
			GENERIC_WRITE,          // Desired Access
			FILE_SHARE_READ | FILE_SHARE_WRITE,   // Share Mode - Allow other processes to open this file for reading while we are writing
			NULL,                   // Security Sttributes
            (flags & QUICKUSB_STREAM_CREATE_ALWAYS) ? CREATE_ALWAYS : CREATE_NEW,   // Creation Disposition
			FILE_ATTRIBUTE_NORMAL,  // Flags and Attributes
			NULL                    // Template File
			);

		// Check for errors
		if (hFile == INVALID_HANDLE_VALUE) {
			winError = GetLastError();

			// Check for the file not found here so we don't inadvertently return
			// QUICKUSB_ERROR_CANNOT_FIND_DEVICE with TranslateSystemError
			if (winError == ERROR_FILE_NOT_FOUND) {
				lastError = QUICKUSB_ERROR_FILE_NOT_FOUND;
			} else {
				lastError = TranslateSystemError(winError);
			}

			return(FALSE);
		}
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        // Check to make sure we will not overwrite a file unless told to do so
        if (!(flags & QUICKUSB_STREAM_CREATE_ALWAYS)) {
            hFile = fopen(path, "r");
            if (hFile != 0) {
                lastError = QUICKUSB_ERROR_FILE_ALREADY_EXISTS;
                return(FALSE);
            }
            fclose(hFile);
            hFile = NULL;
        }

        hFile = fopen(path, "wb");
        if (hFile == 0) {
            // When opening a file for writing, the error ENOENT means that the file name
            // contains invalid characters instead of meaning that the file was not found
            if (errno == ENOENT) {
                lastError = QUICKUSB_ERROR_FILE_NAME;
            } else {
                lastError = TranslateSystemError(errno);
            }
            return(FALSE);
        }
#endif
	}

    // Start streaming
    //qResult = QuickUsbReadBulkDataStartStream(hDevice, (PQBYTE *) memFileBuffer, numBuffers, bufferSize, &CompleteFileStream, (flags & QUICKUSB_STREAM_MEMORY_FILE) ? (PQVOID) hMemFile : (PQVOID) hFile, streamID, 1, 1); // Sets lastError on failure
    qResult = QuickUsbReadBulkDataStartStream(hDevice, (PQBYTE *) memFileBuffer, numBuffers, bufferSize, &CompleteFileStream, (PQVOID) hFile, streamID, 1, 1); // Sets lastError on failure
    if (!qResult) {
        // We could not start the stream so close the file
	    if (flags & QUICKUSB_STREAM_MEMORY_FILE) {
            QuickUsbCloseMemoryFile(&hMemFile, &memFileBuffer, memFileSize, 0);
        } else {
#if DRIVER==WindowsDriver
            CloseHandle(hFile);
#endif
#if (DRIVER==MacOSXDriver) || (DRIVER==LinuxDriver)
            // Close the file
            fclose(hFile);
#endif
        }
        return(FALSE);
    }

    // Get the stream object
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[*streamID];

    // Immediate lock the stream so we may set global stream information
    QUSB_LOCK(stream); {
        // Store the maximum number of transfers for this stream
        stream->maxTransfers = maxTransfers;

		// Keep track of the memory view so we may release it when we're done
		stream->memFileView = memFileBuffer;
		stream->memFileSize = memFileSize;
    } QUSB_UNLOCK(stream);

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbWriteBulkDataStartStreamFromFile(QHANDLE hDevice, PCQCHAR path, QULONG numBuffers, QULONG bufferSize, QULONG maxTransfers, PQBYTE streamID, QULONG flags) {
    QDEVHANDLE hDevHandle;
    QRESULT qResult;
    PQIOSTREAM stream;
    QFILEHANDLE hFile;
    QMEMFILEHANDLE hMemFile;
	PQBYTE memFileBuffer = NULL;
	QULONG memFileSize = 0;
#if DRIVER==WindowsDriver
    DWORD winError;
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((path == 0) || (numBuffers == 0) || (bufferSize == 0) || (streamID == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
	if ((flags & QUICKUSB_STREAM_MEMORY_FILE) && (maxTransfers == 0)) {
		// Memory files must have discrete sizes, so maxTransfers must limit the stream
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
	}

    // Use a realistic number of buffers for low values of maxTransfers
    if ((maxTransfers != 0) && (maxTransfers <= numBuffers)) {
        numBuffers = maxTransfers - 1;
    }

    // Open the file
	if (flags & QUICKUSB_STREAM_MEMORY_FILE) {
		memFileSize = (bufferSize * maxTransfers);
		qResult = QuickUsbOpenMemoryFile(path, memFileSize, &hMemFile, &memFileBuffer, 0); // Sets lastError on failure
		if (!qResult) {
		    return(FALSE);
		}
	} else {
#if DRIVER==WindowsDriver
		hFile = CreateFile(
			(LPCSTR) path,          // File Name
			GENERIC_READ,           // Desired Access
			FILE_SHARE_READ | FILE_SHARE_WRITE,     // Share Mode
			NULL,                   // Security Sttributes
			OPEN_EXISTING,          // Creation Disposition
			FILE_ATTRIBUTE_NORMAL,  // Flags and Attributes
			NULL                    // Template File
			);

		// Check for errors
		if (hFile == INVALID_HANDLE_VALUE) {
			winError = GetLastError();

			// Check for the file not found here so we don't inadvertently return
			// QUICKUSB_ERROR_CANNOT_FIND_DEVICE with TranslateSystemError
			if (winError == ERROR_FILE_NOT_FOUND) {
				lastError = QUICKUSB_ERROR_FILE_NOT_FOUND;
			} else {
				lastError = TranslateSystemError(winError);
			}

			return(FALSE);
		}
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
        hFile = fopen(path, "rb");
        if (hFile == 0) {
            lastError = TranslateSystemError(errno);
            return(FALSE);
        }
#endif
	}

    // Start streaming
    GlobalStreamFlags = flags;
    //qResult = QuickUsbWriteBulkDataStartStream(hDevice, (PQBYTE *) memFileBuffer, numBuffers, bufferSize, &CompleteFileStream, (flags & QUICKUSB_STREAM_MEMORY_FILE) ? (PQVOID) hMemFile : (PQVOID) hFile, streamID, 1, 1); // Sets lastError on failure
    qResult = QuickUsbWriteBulkDataStartStream(hDevice, (PQBYTE *) memFileBuffer, numBuffers, bufferSize, &CompleteFileStream, (PQVOID) hFile, streamID, 1, 1); // Sets lastError on failure
    GlobalStreamFlags = 0;
    if (!qResult) {
	    if (flags & QUICKUSB_STREAM_MEMORY_FILE) {
	        qResult = QuickUsbCloseMemoryFile(&hMemFile, &memFileBuffer, memFileSize, 0); // Sets lastError on failure
	    } else {
#if DRIVER==WindowsDriver
            CloseHandle(hFile);
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
            fclose(hFile);
#endif
        }
        
        return(FALSE);
    }

    // Get the stream object
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[*streamID];

    // Immediate lock the stream so we may set global stream information
    QUSB_LOCK(stream); {
        // Store the maximum number of transfers for this stream
        stream->maxTransfers = maxTransfers;

		// Keep track of the memory view so we may release it when we're done
		stream->memFileView = memFileBuffer;
        stream->memFileSize = memFileSize;
    } QUSB_UNLOCK(stream);

    // Return successfully
    return(TRUE);
}



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbGetStreamStatus(QHANDLE hDevice, QBYTE streamID, PQULONG status, PQULONG error) {
    QDEVHANDLE hDevHandle;
    PQIOSTREAM stream;
    QULONG info;
	QULONG pendingRequests;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if ((status == NULL) && (error == NULL)) { // User don't have to specify both status and error, just at least one
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    if ((streamID >= QUICKUSB_MAX_STREAMS) || (QusbDeviceInfo[(QULONG) hDevice].stream[streamID] == NULL)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    stream = QusbDeviceInfo[(QULONG) hDevice].stream[streamID];

    // Read global stream information
    QUSB_LOCK(stream); {
        info = stream->info;
        pendingRequests = stream->pendingRequests;
    } QUSB_UNLOCK(stream);

    // Get the stream status
    if (status != NULL) {
        if (info & QUSB_INFO_STREAMING_STOP) {
            *status = QUICKUSB_STREAM_STATUS_STOPPED;
        } else if (info & QUSB_INFO_STREAMING_PAUSED) {
			// There is a difference between paused and pausing
			if (pendingRequests == 0) {
				*status = QUICKUSB_STREAM_STATUS_PAUSED;
			} else {
				*status = QUICKUSB_STREAM_STATUS_RUNNING;
			}
        } else if (info & (QUSB_INFO_STREAMING_READ | QUSB_INFO_STREAMING_WRITE)) {
            *status = QUICKUSB_STREAM_STATUS_RUNNING;
        } else {
            *status = QUICKUSB_STREAM_STATUS_UNKNOWN;
        }
    }

    // Get the last error that occurred on the stream
    if (error != NULL) {
        *error = stream->lastError;
        stream->lastError = QUICKUSB_ERROR_NO_ERROR;
    }

    // Return successfully
    return(TRUE);
}

#endif // IMPLEMENT_ASYNC



QRETURN QuickUsbOpenMemoryFile(PCQCHAR path, QULONG length, PQMEMFILEHANDLE hMemFile, PQBYTE *memory, QULONG flags) {
    QULONG bufLen = 256;
    QCHAR memFileName[256];
    PQCHAR pd;
    PCQCHAR ps; 
#if DRIVER==WindowsDriver
    DWORD winError;
#endif
    
    // Validate parameters
    if ((path == NULL) || (length == 0) || (hMemFile == 0) || (memory == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }
    
    // Create the file name
    pd = memFileName;
#if (DRIVER==LinuxDriver)
    *pd = '/';
    ++pd;
    --bufLen;
#endif
    for (--bufLen, ps = path; bufLen > 0 && *ps != '\0'; --bufLen, ++pd, ++ps) {
      *pd = *ps; 
    }
    *pd = '\0'; 
  

#if DRIVER==WindowsDriver
    if (flags & QUICKUSB_MEMORY_FILE_CREATE_ALWAYS) {
		// Create a file mapping
		*hMemFile = CreateFileMapping(
			INVALID_HANDLE_VALUE,		// File Handle - Use paging file
			NULL,						// Security Attributes - Default attributes
			PAGE_READWRITE,				// Page Protection - Read/write access
			0,							// Maximum Object Size (High-Word)
			length,     				// Maximum Object Size (Low-Word)
			path						// Name of Mapping Object
			);

        // If the mapping already exists, a handle to the existing mapping is returned
        if (GetLastError() == ERROR_ALREADY_EXISTS) {
            // The mapping already existed.  Note that it remains its original size and 
            // is not changed to the requested mapping size.
        }
    } else {
	    // Create a file mapping
	    *hMemFile = OpenFileMapping(
		    FILE_MAP_READ | FILE_MAP_WRITE,	// Access
		    FALSE,				            // Inherit handle
		    path				            // Name of Mapping Object
		    );
    }

	// Check for errors
	if ((*hMemFile == 0) || (*hMemFile == INVALID_HANDLE_VALUE)) {
		winError = GetLastError();

		// Check for the file not found here so we don't inadvertently return
		// QUICKUSB_ERROR_CANNOT_FIND_DEVICE with TranslateSystemError
		if (winError == ERROR_FILE_NOT_FOUND) {
			lastError = QUICKUSB_ERROR_FILE_NOT_FOUND;
		} else {
			lastError = TranslateSystemError(winError);
		}
		
		return(FALSE);
	}

	// Get a view of the file so that we may access the file memory through a pointer
	*memory = (PQBYTE) MapViewOfFile(
		*hMemFile,		    // Handle to mapping object
		FILE_MAP_READ | FILE_MAP_WRITE,		// Read and write access
		0,					// File offset (high)
		0,					// File offset (low)
		length  			// View size - entire memory file size
		);

	// Check that we have a valid view of the memory file
	if (*memory == NULL) {
        // Close the file
		fclose(*hMemFile);
        *hMemFile = 0;

		winError = GetLastError();
		lastError = TranslateSystemError(winError);
		return(FALSE);
	}
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    // Get access to shared memory
    if (flags & QUICKUSB_MEMORY_FILE_CREATE_ALWAYS) {
        *hMemFile = shm_open(path, O_RDWR | O_CREAT, 0666);
    } else {
        *hMemFile = shm_open(path, O_RDWR, 0666);
    }
    if (*hMemFile < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }

    // Resize the shared memory
    if (ftruncate(*hMemFile, length) < 0) {
        // ftruncate throws a EINVAL error at us if we try to resize
        // shared memory that was shm_open'ed by another process, so we'll
        // just ignore it
        if (errno != EINVAL) {
            lastError = TranslateSystemError(errno);
            return(FALSE);
        }
    }

    // Map the shared memory into our processes address space
    *memory = (PQBYTE) mmap(NULL, length, PROT_READ | PROT_WRITE, MAP_SHARED, *hMemFile, 0);
    if (*memory == MAP_FAILED) {
        // Close the handle to the shared memory
        close(*hMemFile);
        *hMemFile = 0;

        *memory = NULL;

        lastError = TranslateSystemError(errno);
        return(FALSE);
    }
#endif

    // Return successfully
    return(TRUE);
}



QRETURN QuickUsbCloseMemoryFile(PQMEMFILEHANDLE hMemFile, PQBYTE *memory, QULONG length, QULONG flags) {
    // Validate parameters
    if ((memory == 0) || (*memory == 0) || (length == 0)) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

#if DRIVER==WindowsDriver
    // Unmap the memory file view
    if (!UnmapViewOfFile(*memory)) {
        lastError = TranslateSystemError(GetLastError());
        return(FALSE);
    }

    // Close the file handle
    if (!CloseHandle(*hMemFile)) {
        lastError = TranslateSystemError(GetLastError());
        return(FALSE);
    }
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    // Unmap shared memory
    // NOTE: Shared memory is automatically unmapped when the process exits
    if (munmap(*memory, length) < 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }
    
    // Close the shared memory object
    if (close(*hMemFile) != 0) {
        lastError = TranslateSystemError(errno);
        return(FALSE);
    }

    // NOTE: Shared memory is automatically unlinked when the process exits
#endif

    // Return successfully
    return(TRUE);
}
