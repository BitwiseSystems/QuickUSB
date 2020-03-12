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
#include <ctime>
#if defined(__APPLE__)
    #include <sys/time.h>
#endif

#if defined(__linux__) || defined(__APPLE__)
    #include <sys/syscall.h>
#endif

#if defined(__linux__)
    #define SleepMS(x) usleep((x) * 1000)
#endif
#if defined(__APPLE__)
    #define SleepMS(x) usleep((x) * 1000)
#endif
#if defined(_WIN32)
    #include <windows.h>
    #define SleepMS(x) Sleep(x)
#endif


class PlatformIndependentTimer {
public:
    PlatformIndependentTimer();    
    void Start();    
    void Stop();    
    double GetElapsedTimeInSeconds();
    
private:
    bool running;
#if defined(__linux__)
    timespec tStart, tEnd;
#endif
#if defined(__APPLE__)
    timeval tStart, tEnd;
#endif
#if defined(_WIN32)
    LARGE_INTEGER freq, tStart, tEnd;
#endif
};
