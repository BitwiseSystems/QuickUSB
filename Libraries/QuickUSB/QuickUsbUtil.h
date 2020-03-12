/*=============================================================================
 Title        : QuickUsbUtil.h
 Description  : 
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
#ifndef __QUICKUSB_UTIL_HEADER__
#define __QUICKUSB_UTIL_HEADER__

// Other includes
#include "QuickUsbApi.h"
#include "ioctl.h"
#include "QuickUSB.h"
#include "QuickUsbEeprom.h"
#include "QuickUsbVersion.h"
#include "QuickUsbApiProgram.h"
#include "QuickUsbApiStream.h"
#include "ezusb.h"


#define QUICKUSB_LOG_FILE "QuickUSB.log"

#define QUICKUSB_LOG_LEVEL_0 0 // No reporting
#define QUICKUSB_LOG_LEVEL_1 1 // Report user log
#define QUICKUSB_LOG_LEVEL_2 2 // Report function calls
#define QUICKUSB_LOG_LEVEL_3 3 // Report parameters
#define QUICKUSB_LOG_LEVEL_4 4 // Report driver calls
#define QUICKUSB_LOG_LEVEL_5 5 // Reporting times

#define QUICKUSB_LOG_ENTER_FUNCTION QUICKUSB_LOG_LEVEL_2
#define QUICKUSB_LOG_EXIT_FUNCTION  QUICKUSB_LOG_LEVEL_2
#define QUICKUSB_LOG_IOCTL_CALL     QUICKUSB_LOG_LEVEL_4


QRESULT QuickUsbWriteLog(QBYTE level, const char *msg);
QRESULT QuickUsbSetLogLevel(QBYTE level);
QRESULT QuickUsbSetLogFile(const char *filepath);

#if DRIVER==WindowsDriver
    #define QUSB_LOCK(x) (((x)->lock != NULL) ? EnterCriticalSection((x)->lock) : 0)
    #define QUSB_UNLOCK(x) (((x)->lock != NULL) ? LeaveCriticalSection((x)->lock) : 0)
    #define SleepMS(ms) Sleep(ms)   // Windows 'Sleep' function takes milliseconds as argument

    typedef QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL QUICKUSB_VENDOR_REQUEST;
#endif

#if DRIVER==MacOSXDriver
    typedef IOUSBDevRequest QUICKUSB_VENDOR_REQUEST;

    #define SleepMS(ms) usleep((ms) * 1000)
    #define QUSB_LOCK(x) (((x)->lock != NULL) ? pthread_mutex_lock((x)->lock) : 0)
    #define QUSB_UNLOCK(x) (((x)->lock != NULL) ? pthread_mutex_unlock((x)->lock) : 0)

    #define gettid() (unsigned int)pthread_mach_thread_np(pthread_self())

    #define MAP_ANONYMOUS MAP_ANON
#endif

#if DRIVER==LinuxDriver
    #define SleepMS(ms) usleep((ms) * 1000)
    #define QUSB_LOCK(x) (((x)->lock != NULL) ? (int)pthread_mutex_lock((x)->lock) : 0)
    #define QUSB_UNLOCK(x) (((x)->lock != NULL) ? (int)pthread_mutex_unlock((x)->lock) : 0)

    #define gettid() (int)syscall(SYS_gettid)
    
    typedef QUSB_VENDOR_OR_CLASS_REQUEST_CONTROL QUICKUSB_VENDOR_REQUEST;
#endif

// Functions for internal use only (Not user API functions)
#define QUICKUSB_REQUEST_OUT 0
#define QUICKUSB_REQUEST_IN  1
QBOOL QuickUsbIoctl(QHANDLE hDevice, QDEVHANDLE hDevHandle, QULONG controlCode, PQVOID inBuffer, 
    QULONG inBufferSize, PQVOID outBuffer, QULONG outBufferSize, PQULONG bytesReturned);
QBOOL QuickUsbVendorRequest(QHANDLE hDevice, QDEVHANDLE hDevHandle, QBYTE direction, QBYTE request, 
    QWORD index, QWORD value, PQVOID data, QULONG length, QULONG *bytesReturned);

// QuickUSB Get Last Error Code -- Default to No Error
extern QULONG lastError;
extern QULONG lastDriverError;

// Variable
extern QBOOL initialized;
extern QUSB_DRIVER_VERSION DriverVersion;
extern QUSB_DEVICE_INFO QusbDeviceInfo[MAX_DEVICENUM];  // # of times device has been opened by this process
extern PQULONG QusbDeviceRefCount;                      // # of processes that have opened this device

// Define SWAPBYTES macro, per our operating system and endian
#if DRIVER==WindowsDriver
#define SWAPBYTES(source, dest, length) _swab(source, dest, length)
#else
#define SWAPBYTES(source, dest, length) swab(source, dest, length)
#endif // DRIVER==LinuxDriver || DRIVER==MacOSXDriver



extern int TestQusbOpen(QHANDLE *hDevice, char *deviceName);
extern int QuickUsbGetDeviceHandle(QHANDLE hDevice, QDEVHANDLE *hDevHandle);
extern int QuickUsbGetDeviceType(QHANDLE hDevice, QusbDriverType *pType);
extern QRESULT QuickUsbRetrieveDeviceHandle(QHANDLE hDevice, QDEVHANDLE *hDevHandle);
extern int QuickUsbGetError(QHANDLE hDevice, PQUSB_GET_DRIVER_ERROR pGetError);

#if DRIVER==WindowsDriver
extern QULONG TranslateSystemError(DWORD systemError);
#endif
#if (DRIVER==LinuxDriver) || (DRIVER==MacOSXDriver)
extern QULONG TranslateSystemError(int systemError);
#endif

#if DRIVER==WindowsDriver || DRIVER==LinuxDriver || DRIVER==MacOSXDriver
extern BOOLEAN CheckDriverVersion(unsigned short major, unsigned short minor, unsigned short build);
BOOLEAN IsAtLeastVersion(unsigned short minMajor, unsigned short minMinor, unsigned short minBuild,
                       unsigned short major, unsigned short minor, unsigned short build);
#endif  // DRIVER==WindowsDriver || DRIVER==LinuxDriver || DRIVER==MacOSXDriver

#ifdef __cplusplus
extern "C" {
#endif

// Exported by library, but hidden from user
extern QRETURN QuickUsbSetResetState(QHANDLE hDevice, QBYTE resetBit);

#if DRIVER==WindowsDriver
QUSBLIB QBOOL QuickUsbVendorRequestAsyncWait(QHANDLE hDevice, /*QDEVHANDLE hDevHandle,*/ OVERLAPPED *ovl, QULONG *bytesTransferred);
QUSBLIB QBOOL QuickUsbVendorRequestAsync(QHANDLE hDevice, /*QDEVHANDLE hDevHandle,*/ QBYTE direction, QBYTE request, QWORD index, QWORD value, PQVOID data, QULONG length, OVERLAPPED *ovl);
#endif

#ifdef __cplusplus
}
#endif


#endif //__QUICKUSB_UTIL_HEADER__

