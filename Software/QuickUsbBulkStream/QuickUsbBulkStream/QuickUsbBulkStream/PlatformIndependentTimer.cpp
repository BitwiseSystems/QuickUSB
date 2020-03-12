/*=============================================================================
 Title        : PlatformIndependentTimer.h
 Description  : Implementation of a very accurate platform independent timer
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
#include "PlatformIndependentTimer.h"



PlatformIndependentTimer::PlatformIndependentTimer() : running(false) {
}



void PlatformIndependentTimer::Start() {
#if defined(__linux__)
    clock_gettime(CLOCK_REALTIME, &tStart);
#endif
#if defined(__APPLE__)
    gettimeofday(&tStart, NULL);
#endif
#if defined(_WIN32)
    QueryPerformanceFrequency(&freq);
    QueryPerformanceCounter(&tStart);
#endif
    running = true;
}



void PlatformIndependentTimer::Stop() {
#if defined(__linux__)
    clock_gettime(CLOCK_REALTIME, &tEnd);
#endif
#if defined(__APPLE__)
    gettimeofday(&tEnd, NULL);
#endif
#if defined(_WIN32)
    QueryPerformanceCounter(&tEnd);
#endif
    running = false;
}



double PlatformIndependentTimer::GetElapsedTimeInSeconds() {
#if defined(__APPLE__)
    timeval temp;
#endif
#if defined(__linux__)
    timespec temp;
#endif
    
    if (running) {
        Stop();
        running = true;
    }
    
#if defined(__linux__)
    if ((tEnd.tv_nsec-tStart.tv_nsec)<0) {
        temp.tv_sec = tEnd.tv_sec - tStart.tv_sec-1;
        temp.tv_nsec = (long int)1e9 + tEnd.tv_nsec - tStart.tv_nsec;
    } else {
        temp.tv_sec = tEnd.tv_sec - tStart.tv_sec;
        temp.tv_nsec = tEnd.tv_nsec - tStart.tv_nsec;
    }
    
    return (double)temp.tv_sec + (double)temp.tv_nsec / (double)1e9;
#endif
    
#if defined(__APPLE__)
    if ((tEnd.tv_usec-tStart.tv_usec)<0) {
        temp.tv_sec = tEnd.tv_sec - tStart.tv_sec-1;
        temp.tv_usec = 1e6 + tEnd.tv_usec - tStart.tv_usec;
    } else {
        temp.tv_sec = tEnd.tv_sec - tStart.tv_sec;
        temp.tv_usec = tEnd.tv_usec - tStart.tv_usec;
    }
    
    return (double)temp.tv_sec + (double)temp.tv_usec / (double)1e6;
#endif
    
#if defined(_WIN32)
    return ((double) tEnd.QuadPart - (double) tStart.QuadPart) / (double) freq.QuadPart;
#endif
}
