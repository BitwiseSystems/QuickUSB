/*=============================================================================
 Title        : QuickUsbApiStatistics.cpp
 Description  : QuickUSB QUSB2 Module Statistics API
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



/*---------------------------------------------------------------------------
 Purpose : 
 Input   : 
 Output  : 
 Notes   : 
 ---------------------------------------------------------------------------*/
QRETURN QuickUsbResetStatistic(QHANDLE hDevice, QULONG statistic) {
    QDEVHANDLE hDevHandle;
    QBOOL fallThrough = FALSE;

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    switch (statistic) {
        case QUICKUSB_STAT_ALL:
            // We'll perform an easy reset of all statistics by falling through every case statement below
            fallThrough = TRUE;

        case QUICKUSB_STAT_READ_THROUGHPUT:
            // Reset the bytes counter
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead = 0;

            // On Windows, we need to know the CPU frequency to convert clock ticks to time
#if DRIVER==WindowsDriver
            QueryPerformanceFrequency(&QusbDeviceInfo[(QULONG) hDevice].freq);
#endif

            // Get the current time
#if DRIVER==LinuxDriver
            clock_gettime(CLOCK_REALTIME, &QusbDeviceInfo[(QULONG) hDevice].tReadStart);
#endif
#if DRIVER==MacOSXDriver
            gettimeofday(&QusbDeviceInfo[(QULONG) hDevice].tReadStart, NULL);
#endif
#if DRIVER==WindowsDriver
            QueryPerformanceCounter(&QusbDeviceInfo[(QULONG) hDevice].tReadStart);
#endif
            if (!fallThrough) {
                break;
            }

        case QUICKUSB_STAT_WRITE_THROUGHPUT:
            // Reset the bytes counter
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten = 0;

            // On Windows, we need to know the CPU frequency to convert clock ticks to time
#if DRIVER==WindowsDriver
            QueryPerformanceFrequency(&QusbDeviceInfo[(QULONG) hDevice].freq);
#endif

            // Get the current time
#if DRIVER==LinuxDriver
            clock_gettime(CLOCK_REALTIME, &QusbDeviceInfo[(QULONG) hDevice].tWriteStart);
#endif
#if DRIVER==MacOSXDriver
            gettimeofday(&QusbDeviceInfo[(QULONG) hDevice].tWriteStart, NULL);
#endif
#if DRIVER==WindowsDriver
            QueryPerformanceCounter(&QusbDeviceInfo[(QULONG) hDevice].tWriteStart);
#endif
            if (!fallThrough) {
                break;
            }

        case QUICKUSB_STAT_TOTAL_THROUGHPUT:
            // Reset the bytes counters
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten = 0;
            QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead = 0;

            // On Windows, we need to know the CPU frequency to convert clock ticks to time
#if DRIVER==WindowsDriver
            QueryPerformanceFrequency(&QusbDeviceInfo[(QULONG) hDevice].freq);
#endif

            // Get the current time
#if DRIVER==LinuxDriver
            clock_gettime(CLOCK_REALTIME, &QusbDeviceInfo[(QULONG) hDevice].tWriteStart);
#endif
#if DRIVER==MacOSXDriver
            gettimeofday(&QusbDeviceInfo[(QULONG) hDevice].tWriteStart, NULL);
#endif
#if DRIVER==WindowsDriver
            QueryPerformanceCounter(&QusbDeviceInfo[(QULONG) hDevice].tWriteStart);
#endif
            QusbDeviceInfo[(QULONG) hDevice].tReadStart = QusbDeviceInfo[(QULONG) hDevice].tWriteStart;
            
            if (!fallThrough) {
                break;
            }

        case QUICKUSB_STAT_TOTAL_DATA_READ:
            QusbDeviceInfo[(QULONG) hDevice].totalBytesRead = 0;

            if (!fallThrough) {
                break;
            }

        case QUICKUSB_STAT_TOTAL_DATA_WRITTEN:
            QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten = 0;

            if (!fallThrough) {
                break;
            }
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
QRETURN QuickUsbGetStatistic(QHANDLE hDevice, QULONG statistic, QULONG unit, PQFLOAT value, QULONG flags) {
    QDEVHANDLE hDevHandle;
    QFLOAT floatValue, tElapsed, floatA, floatB;
#if DRIVER==LinuxDriver
    struct timespec tNow, temp;
#endif
#if DRIVER==MacOSXDriver
    timeval tNow, temp;
#endif
#if DRIVER==WindowsDriver
    LARGE_INTEGER tNow;
#endif

    // Get the device handle
    if (!QuickUsbRetrieveDeviceHandle(hDevice, &hDevHandle)) { // Sets lastError on failure
        return(FALSE);
    }

    // Validate parameters
    if (value == 0) {
        lastError = QUICKUSB_ERROR_INVALID_PARAMETER;
        return(FALSE);
    }

    // Determine which statistic the user is requesting
    switch (statistic) {
        case QUICKUSB_STAT_READ_THROUGHPUT:
#if DRIVER==LinuxDriver
            // Get the current time
            clock_gettime(CLOCK_REALTIME, &tNow);

            // Calculate the elasped time
            if ((tNow.tv_nsec-QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_nsec)<0) {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_sec-1;
                temp.tv_nsec = (long int)1e9 + tNow.tv_nsec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_nsec;
            } else {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_sec;
                temp.tv_nsec = tNow.tv_nsec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_nsec;
            }

            tElapsed = (QFLOAT)temp.tv_sec + (QFLOAT)temp.tv_nsec / (QFLOAT)1e9;
#endif
#if DRIVER==MacOSXDriver
            // Get the current time
            gettimeofday(&tNow, NULL);

            // Calculate the elasped time
            if ((tNow.tv_usec-QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_usec)<0) {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_sec-1;
                temp.tv_usec = 1e6 + tNow.tv_usec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_usec;
            } else {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_sec;
                temp.tv_usec = tNow.tv_usec - QusbDeviceInfo[(QULONG) hDevice].tReadStart.tv_usec;
            }
            
            tElapsed = (QFLOAT)temp.tv_sec + (QFLOAT)temp.tv_usec / (QFLOAT)1e6;
#endif
#if DRIVER==WindowsDriver
            // Get the current time
            QueryPerformanceCounter(&tNow);

            // Calculate the elasped time
            tElapsed = (QFLOAT) (tNow.QuadPart - QusbDeviceInfo[(QULONG) hDevice].tReadStart.QuadPart) / (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].freq.QuadPart;
#endif

            // Convert the time elapsed (in seconds) to the requested unit
            switch (unit & 0x03F0) {
                case QUICKUSB_STAT_UNIT_PER_NS:
                    tElapsed *= 1e9;
                    break;

                case QUICKUSB_STAT_UNIT_PER_US:
                    tElapsed *= 1e6;
                    break;

                case QUICKUSB_STAT_UNIT_PER_MS:
                    tElapsed *= 1e3;
                    break;

                default:
                case QUICKUSB_STAT_UNIT_PER_SEC:
                    // Already in seconds
                    break;

                case QUICKUSB_STAT_UNIT_PER_MIN:
                    tElapsed /= 60.0f;
                    break;

                case QUICKUSB_STAT_UNIT_PER_HOUR:
                    tElapsed /= (60.0f * 60.0f);
                    break;
            }

            // Convert data (in bytes) to the requested unit
            switch (unit & 0x000F) {
                case QUICKUSB_STAT_UNIT_BYTES:
                    // Already in bytes
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead;
                    break;
                    
                case QUICKUSB_STAT_UNIT_KB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead / 1024.0f;
                    break;
                   
                default:
                case QUICKUSB_STAT_UNIT_MB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead / (1024.0f * 1024.0f);
                    break;
                    
                case QUICKUSB_STAT_UNIT_GB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesRead / (1024.0f * 1024.0f * 1024.0f);
                    break;
            }
 
            // Calculate throughput            
            floatValue = floatA / tElapsed;
            break;

        case QUICKUSB_STAT_WRITE_THROUGHPUT:
#if DRIVER==LinuxDriver
            // Get the current time
            clock_gettime(CLOCK_REALTIME, &tNow);

            // Calculate the elasped time
            if ((tNow.tv_nsec-QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_nsec)<0) {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_sec-1;
                temp.tv_nsec = (long int)1e9 + tNow.tv_nsec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_nsec;
            } else {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_sec;
                temp.tv_nsec = tNow.tv_nsec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_nsec;
            }

            tElapsed = (QFLOAT)temp.tv_sec + (QFLOAT)temp.tv_nsec / (QFLOAT)1e9;
#endif
#if DRIVER==MacOSXDriver
            // Get the current time
            gettimeofday(&tNow, NULL);

            // Calculate the elasped time
            if ((tNow.tv_usec-QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_usec)<0) {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_sec-1;
                temp.tv_usec = 1e6 + tNow.tv_usec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_usec;
            } else {
                temp.tv_sec = tNow.tv_sec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_sec;
                temp.tv_usec = tNow.tv_usec - QusbDeviceInfo[(QULONG) hDevice].tWriteStart.tv_usec;
            }
            
            tElapsed = (QFLOAT)temp.tv_sec + (QFLOAT)temp.tv_usec / (QFLOAT)1e6;
#endif
#if DRIVER==WindowsDriver
            // Get the current time
            QueryPerformanceCounter(&tNow);

            // Calculate the elasped time
            tElapsed = ((QFLOAT) tNow.QuadPart - (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].tWriteStart.QuadPart) / (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].freq.QuadPart;
#endif
 
            // Convert the time elapsed (in seconds) to the requested unit
            switch (unit & 0x03F0) {
                case QUICKUSB_STAT_UNIT_PER_NS:
                    tElapsed *= (QFLOAT) 1e9;
                    break;

                case QUICKUSB_STAT_UNIT_PER_US:
                    tElapsed *= (QFLOAT) 1e6;
                    break;

                case QUICKUSB_STAT_UNIT_PER_MS:
                    tElapsed *= (QFLOAT) 1e3;
                    break;

                default:
                case QUICKUSB_STAT_UNIT_PER_SEC:
                    // Already in seconds
                    break;

                case QUICKUSB_STAT_UNIT_PER_MIN:
                    tElapsed /= 60.0f;
                    break;

                case QUICKUSB_STAT_UNIT_PER_HOUR:
                    tElapsed /= (60.0f * 60.0f);
                    break;
            }

            // Convert data (in bytes) to the requested unit
            switch (unit & 0x000F) {
                case QUICKUSB_STAT_UNIT_BYTES:
                    // Already in bytes
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten;
                    break;
                    
                case QUICKUSB_STAT_UNIT_KB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten / 1024.0f;
                    break;
                   
                default:
                case QUICKUSB_STAT_UNIT_MB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten / (1024.0f * 1024.0f);
                    break;
                    
                case QUICKUSB_STAT_UNIT_GB:
                    floatA = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].throughputBytesWritten / (1024.0f * 1024.0f * 1024.0f);
                    break;
            }
 
            // Calculate throughput            
            floatValue = floatA / tElapsed;
            break;

        case QUICKUSB_STAT_TOTAL_THROUGHPUT:
            if (!QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_READ_THROUGHPUT, unit, &floatA, flags)) { // Sets lastError on failure
                return(FALSE);
            }

            if (!QuickUsbGetStatistic(hDevice, QUICKUSB_STAT_WRITE_THROUGHPUT, unit, &floatB, flags)) { // Sets lastError on failure
                return(FALSE);
            }

            floatValue = (floatA + floatB);
            break;

        case QUICKUSB_STAT_TOTAL_DATA_READ:
            // Convert data (in bytes) to the requested unit
            switch (unit & 0x000F) {
                default:
                case QUICKUSB_STAT_UNIT_BYTES:
                    // Already in bytes
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesRead;
                    break;
                    
                case QUICKUSB_STAT_UNIT_KB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesRead / 1024.0f;
                    break;
                   
                case QUICKUSB_STAT_UNIT_MB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesRead / (1024.0f * 1024.0f);
                    break;
                    
                case QUICKUSB_STAT_UNIT_GB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesRead / (1024.0f * 1024.0f * 1024.0f);
                    break;
            }
            break;

        case QUICKUSB_STAT_TOTAL_DATA_WRITTEN:
            // Convert data (in bytes) to the requested unit
            switch (unit & 0x000F) {
                default:
                case QUICKUSB_STAT_UNIT_BYTES:
                    // Already in bytes
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten;
                    break;
                    
                case QUICKUSB_STAT_UNIT_KB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten / 1024.0f;
                    break;
                   
                case QUICKUSB_STAT_UNIT_MB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten / (1024.0f * 1024.0f);
                    break;
                    
                case QUICKUSB_STAT_UNIT_GB:
                    floatValue = (QFLOAT) QusbDeviceInfo[(QULONG) hDevice].totalBytesWritten / (1024.0f * 1024.0f * 1024.0f);
                    break;
            }
            break;

        default:
            // Unknown statistic
            return(FALSE);
    }

    // Reset the statistic if requested.  The total throughput statistic is already reset so don't reset it again.
    //if (statistic != QUICKUSB_STAT_TOTAL_THROUGHPUT) {
    //    if (!QuickUsbResetStatistic(hDevice, statistic)) { // Sets lastError on failure
    //        return(FALSE);
    //    }
    //}

    // Return the float statistic
    *value = floatValue;

    // Return successfully
    return(TRUE);
}
