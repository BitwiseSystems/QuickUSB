/*=============================================================================
 Title        : QuickUsbApiLog.cpp
 Description  : QuickUSB QUSB2 Module Logging API
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
#include "QuickUsbUtil.h"

QRESULT QuickUsbWriteLog(QBYTE level, const char *msg) {
    FILE *hFile;
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    errno_t status;
#endif
#endif
    LARGE_INTEGER freq;
    LARGE_INTEGER tick;
    float time;

    return FALSE;

    // Open the input file for writing
#if DRIVER==WindowsDriver
#ifdef USE_SAFE_FUNCTIONS
    status = fopen_s(&hFile, QUICKUSB_LOG_FILE, "a");
    if (status != 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#else
    hFile = fopen(QUICKUSB_LOG_FILE, "a");
    if (hFile == 0) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif
#elif DRIVER==LinuxDriver
    hFile = fopen(QUICKUSB_LOG_FILE, "a");
    if (hFile == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#elif DRIVER==MacOSXDriver
    hFile = fopen((const char *)QUICKUSB_LOG_FILE, "a");
    if (hFile == NULL) {
        lastError = QUICKUSB_ERROR_CANNOT_OPEN_FILE;
        return(FALSE);
    }
#endif

    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tick);
    time = (float)tick.QuadPart / (float)freq.QuadPart;

    // Log
    fprintf(hFile, "%.3f: %s\n", time, msg);

    // Close the file
    fclose(hFile);

    return(TRUE);
}



QRESULT QuickUsbSetLogLevel(QBYTE level) {
    return(FALSE);
}



QRESULT QuickUsbSetLogFile(const char *filepath) {
    return(FALSE);
}
