/*=============================================================================
 Title        : QuickUsbStream.h
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
#include "QuickUsbUtil.h"

#ifdef IMPLEMENT_ASYNC

#define QUICKUSB_MEMORY_FILE_CREATE_ALWAYS       0x0001

#define QUICKUSB_STREAM_MEMORY_FILE              0x0004

#define QUICKUSB_CRTYPE_VOID_QBULKSTREAM 1

#if DRIVER==WindowsDriver
    #define QMEMFILEHANDLE HANDLE
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    #define QMEMFILEHANDLE int
#endif

#define PQMEMFILEHANDLE QMEMFILEHANDLE*
    
#define QUICKUSB_ASYNC_STREAM           0
#define QUICKUSB_LEGACY_ASYNC_STREAM    1
#define QUICKUSB_BULK_STREAM_START      2

QRESULT QuickUsbInitializeStream(QBYTE streamID, PQIOSTREAM stream, QHANDLE hDevice, QULONG numRequests, QWORD numThreads, QWORD threadConcurrency);
QRESULT QuickUsbStartStream(PQIOSTREAM stream);
QRESULT QuickUsbProcessSTStream(PQIOSTREAM stream, QULONG milliseconds);
QRESULT StopStream(PQIOSTREAM stream, QBOOL immediate);
QRESULT QuickUsbIssueStreamBulkRequest(QBYTE streamID, PQIOSTREAM stream, PQBULKSTREAM bulkStream, QBOOL acquireLock);
QVOIDRETURN CompleteFileStream(PQBULKSTREAM bulkStream);
QRETURN QuickUsbOpenMemoryFile(PCQCHAR path, QULONG length, PQMEMFILEHANDLE hMemFile, PQBYTE *memory, QULONG flags);
QRETURN QuickUsbCloseMemoryFile(PQMEMFILEHANDLE hMemFile, PQBYTE *memory, QULONG length, QULONG flags);
#ifdef IMPLEMENT_ASYNC
void CompleteBulkRequest(PQIOSTREAM stream, PQBULKSTREAM bulkStream, QULONG bytesTransferred, QULONG requestResult);
#endif

#if DRIVER==WindowsDriver
    DWORD WINAPI QuickUsbStreamThread(__in LPVOID lpParameter);

    // Memory file access API
    QUSBLIB QRETURN QuickUsbOpenMemoryFile(
        PCQCHAR         path, 
        QULONG          length,
        PQMEMFILEHANDLE hMemFile,
        PQBYTE         *memory,
        QULONG          flags
    );

    QUSBLIB QRETURN QuickUsbCloseMemoryFile(
        PQMEMFILEHANDLE hMemFile, 
        PQBYTE         *memory,
        QULONG          length,
        QULONG          flags
    );
#endif

#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
    PQVOID QuickUsbStreamThread(PQVOID param);
#endif

#if DRIVER==LinuxDriver
long io_setup(unsigned nr_reqs, aio_context_t *ctx);

long io_destroy(aio_context_t ctx);

long io_submit(aio_context_t ctx, long n, struct iocb **paiocb);

long io_cancel(aio_context_t ctx, struct iocb *aiocb, struct io_event *res);

long io_getevents(aio_context_t ctx, long min_nr, long nr, struct io_event *events, struct timespec *tmo);

void init_iocb(struct iocb *iocb, int fd, void const *buf, size_t nbytes, off_t offset, int opcode);
#endif

#if DRIVER==MacOSXDriver
    QULONG DispatchRequest(PQUSB_DEVICE_INFO qusb, PQBULKSTREAM bulkStream);
    void DispatchRequests(PQUSB_DEVICE_INFO qusb);
    void QueueRequestAndDispatch(PQUSB_DEVICE_INFO qusb, PQBULKSTREAM bulkStream);
    void MacStreamBulkCallback(void *refcon, IOReturn result, void *arg);
#endif

extern QULONG GlobalStreamFlags;

#endif

