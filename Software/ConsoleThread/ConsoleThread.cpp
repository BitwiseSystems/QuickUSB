/*=============================================================================
 Title        : ConsoleThread.cpp
 Description  : QuickUSB Console App Sample
 Notes        : None
 History      :
   Date       Name  J. T. Towson   5/07/04

 Copyright © 2004 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems.

 Bitwise Systems
 5385 Hollister Ave, Suite 215
 Santa Barbara, CA  93111
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

 ConsoleThread:
 This application uses the QuickUSB asynchronous bulk read/write functions to
 communicate with two QuickUSB devices.  The primary thread spawns three
 threads.  The primary and second thread communicate reads and writes to
 Device-0.  The third and fourth thread communicate with Device-1.  The primary
 and third thread will send reads to their devices while the second and fourth
 thread do constant asynchronous writes of 1048576 bytes to their devices. The
 second and fourth threads print their thread number (2, 4) every 100 times
 through their loop.  The primary and third threads generate asynchronous reads
 every 500th loop of the second and fourth threads and report the resulting
 throughput with their thread number attached (1, 3).  A query at program start
 asks whether or not the second device is attached.  If not, only the primary
 and second threads are active.
=============================================================================*/
#include "stdafx.h"
#include "windows.h"
#include "quickusb.h"
#include <conio.h>
#include <malloc.h>
//
// Thread Definitions
//
int Write_Async_Continuous2(HANDLE hDev0);
int Read_Async3(HANDLE hDev1);
int Write_Async_Continuous4(HANDLE hDev1);
//
// Function Definitions
//
int Exit(void);
int GetThroughput0(float *throughput, ULONG Size);
int GetThroughput1(float *throughput, ULONG Size);

#define MAX_LOW_LONG 0xFFFFFFFF

FILETIME Start, End;
BOOLEAN Fail = FALSE;         // Failure detected
BOOLEAN stopThread2 = FALSE;  // Exit the Dev0 write thread
BOOLEAN stopThread3 = FALSE;  // Exit the Dev1 read thread
BOOLEAN stopThread4 = FALSE;  // Exit the Dev1 write thread
BOOLEAN ActiveThread1 = FALSE;  // Dev0 read thread active 
BOOLEAN ActiveThread2 = FALSE;  // Dev0 write thread active 
BOOLEAN ActiveThread3 = FALSE;  // Dev1 read thread active
BOOLEAN ActiveThread4 = FALSE;  // Dev1 write thread active
HANDLE hThread2;              // Asynchronous continuous write thread handle
HANDLE hThread3;              // Asynchronous read thread handle
HANDLE hThread4;              // Asynchronous continuous write thread handle
HANDLE hEvent0;               // Device-0 Enable Asynchronous Read Event
HANDLE hEvent1;               // Device-1 Enable Asynchronous Read Event
HANDLE hDev0;
HANDLE hDev1;
UCHAR numDevices = 0;

int main(int argc, char* argv[])
{
	char *namePtr;
	char nameList[120];
	unsigned long result, tperr;
   char Char;
   size_t Size = 1048576;
   UCHAR *pAsyncRdBuf;
	ULONG error;
   float throughput;          //Asynchronous Read throughput
   ULONG bytecount = 0;       //Returned read bytecount
   ULONG timeout = 1;         //Initial timeout = 1-sec.
   UCHAR transaction = 0x00;  //Asynchronous transaction number
   DWORD threadId2;           //Asynchronous continuous write Dev-0 thread ID
   DWORD threadId3;           //Asynchronous read Dev-1 thread ID
   DWORD threadId4;           //Asynchronous continuous write Dev-1 thread ID


   printf("Hello QuickUSB World!\n");

	//  Find the QuickUSB modules in the system
	QuickUsbFindModules(nameList, 128);

	// Check for modules and exit if none found
	if (*nameList == '\0') {
		printf("Couldn't find any modules\n");
      Sleep(5000);
   	return 1;
	}
	// Print out the name of each module found
	namePtr = nameList;
	while (*namePtr != '\0') {
		printf("Found %s\n", namePtr);
		namePtr = namePtr + (strlen(namePtr) + 1);
	}

   namePtr = nameList;
   printf("Enter number of devices (1 or 2) on which to run test\n");
   while(!_kbhit())
      Sleep(0);  //Give up time slice
   Char = _getche();

   switch(Char)
   {
      case '1':
         numDevices = 1;
      	result = QuickUsbOpen(&hDev0, namePtr);
      	if (result == FALSE)
         {
      		printf("Cannot open %s\n", namePtr);
            Sleep(3000);
      		return 1;
      	}
         break;
      case '2':
         numDevices = 2;
      	result = QuickUsbOpen(&hDev0, namePtr);
      	if (result == FALSE)
         {
      		printf("Cannot open %s\n", namePtr);
            Sleep(3000);
      		return 1;
      	}
         namePtr += (strlen(namePtr) + 1);

      	result = QuickUsbOpen(&hDev1, namePtr);
      	if (result == FALSE)
         {
      		printf("Cannot open %s\n", namePtr);
            Sleep(3000);
      		return 1;
      	}
         break;
      default:
         printf("Entry not understood, Exiting\n");
         Sleep(3000);
         return(1);
   }
   //
   // Create asynchronous read event for device
   //
   hEvent0 = CreateEvent(NULL, FALSE, FALSE, NULL);
   if(numDevices == 2)
      hEvent1 = CreateEvent(NULL, FALSE, FALSE, NULL);

   printf("To set a timeout greater than 1 sec., enter number of seconds 2-9, else '0'\n");

   while(!_kbhit())
      Sleep(0);  //Give up time slice 
   if((Char = _getche()) != '0')
   {
      switch(Char)
      {
         case '2':
            timeout = 2000;
            break;
         case '3':
            timeout = 3000;
            break;
         case '4':
            timeout = 4000;
            break;
         case '5':
            timeout = 5000;
            break;
         case '6':
            timeout = 6000;
            break;
         case '7':
            timeout = 7000;
            break;
         case '8':
            timeout = 8000;
            break;
         case '9':
            timeout = 9000;
            break;
         default:
            printf("Unknown choice - Set 1 second timeout\n");
            timeout = 1000;
            break;
         }
         QuickUsbSetTimeout(hDev0, timeout);
         if(numDevices == 2)
            QuickUsbSetTimeout(hDev1, timeout);
   }
   printf("\n*** TYPE 'x' TO EXIT TEST ***\n\n");

   printf("To start the continuous asynch write thread on device 0, enter '1', else '0'.\n");
   printf("The threads send 1048576 (0x100000) bytes per transaction.\n");

   while(!_kbhit())
      Sleep(0);  //Give up time slice 
   Char = _getche();

   if(Char == '1')
   {
      ActiveThread1 = TRUE;
      ActiveThread2 = TRUE;
      //
      // Create a thread that does continuous writes, only halting for asynchronous waits.
      //
      hThread2 = CreateThread(NULL,
                              4000,
                              (LPTHREAD_START_ROUTINE)Write_Async_Continuous2,
                              hDev0,
                              0,
                              &threadId2);
   }
   if(numDevices == 2)
   {
      printf("To start the asynch read/write threads on device 1, enter '1', else '0'.\n");
      printf("The threads send 1048576 (0x100000) bytes per transaction.\n");

      while(!_kbhit())
         Sleep(0);  //Give up time slice 
      Char = _getche();

      if(Char == '1')
      {
         //
         // Create a thread that does reads, reporting throughput and bytecount.
         //
         ActiveThread3 = TRUE;
         hThread3 = CreateThread(NULL,
                                 4000,
                                 (LPTHREAD_START_ROUTINE)Read_Async3,
                                 hDev1,
                                 0,
                                 &threadId3);
         Sleep(100);
         //
         // Create a thread that does continuous writes, only halting for
         //asynchronous waits.
         //
         ActiveThread4 = TRUE;
         hThread4 = CreateThread(NULL,
                                 4000,
                                 (LPTHREAD_START_ROUTINE)Write_Async_Continuous4,
                                 hDev1,
                                 0,
                                 &threadId4);
      }
   }
   //
   //Async Bulk Read: Create a buffer of X bytes for the call.
   //
   if(!(pAsyncRdBuf = (UCHAR *)malloc(Size)))
   {
      printf("Memory Fault, primary thread\n");
      error = Exit();
   }
   while(1)
   {
      error = WaitForSingleObject(hEvent0, 10000);
      if(error == WAIT_TIMEOUT)
      {   
         printf("Primary Thread Wait failure\n");
         free(pAsyncRdBuf);
         Fail = TRUE;
         ActiveThread1 = FALSE;
         error = Exit();
         return(1);
      }
      if(Fail)
      {
         free(pAsyncRdBuf);
         ActiveThread1 = FALSE;
         Exit();
         return(1);
      }
      if(_kbhit())
         Char = _getche();

	   if(Char == 'x')
      {
         free(pAsyncRdBuf);
         ActiveThread1 = FALSE;
         Exit();
         return(1);
      }
      GetSystemTimeAsFileTime(&Start);
      result = QuickUsbReadDataAsync(hDev0, (PUCHAR)pAsyncRdBuf,
                                     (ULONG *)&Size, &transaction);
      if(!result)
      {
         printf("Primary Thread Async Read failure\n");
         free(pAsyncRdBuf);
         ActiveThread1 = FALSE;
         Fail = TRUE;
         error = Exit();
      }
      result = QuickUsbAsyncWait(hDev0, &bytecount, transaction);
      if(!result)
      {
         printf("Primary Thread Read Async Wait failure\n");
         free(pAsyncRdBuf);
         ActiveThread1 = FALSE;
         Fail = TRUE;
         error = Exit();
      }
      GetSystemTimeAsFileTime(&End);
      tperr = GetThroughput0(&throughput, Size);
      printf("Primary Thread Read Throughput = %3.3f MB/sec. Bytecount = %d\n",
              throughput, bytecount);
   } //end while
} //End main

int Exit()
{
   if(numDevices == 2)
   {
      stopThread4 = TRUE;
      while(ActiveThread4)
         Sleep(0);            //Give time for write thread to recognize stop
      CloseHandle(hThread4);
      
      stopThread3 = TRUE;
      while(ActiveThread3)
         Sleep(0);            //Give time for write thread to recognize stop
      CloseHandle(hThread3);

      CloseHandle(hEvent1);
   }
   stopThread2 = TRUE;
   while(ActiveThread2)
         Sleep(0);            //Give time for write thread to recognize stop
   CloseHandle(hThread2);

   CloseHandle(hEvent0);

   while(ActiveThread1)
         Sleep(0);            //Give time for write thread to recognize stop
   QuickUsbClose(hDev0);
   QuickUsbClose(hDev1);
   Sleep(1000);

   printf("Test Exited\n");
   Sleep(1000);
   return(1);
} //End Exit

int GetThroughput0(float *pThroughput, ULONG Size)
{
   DWORD deltaTime;   //End - Start time in 100ns interval
   float delTime;
   float size;

   if(End.dwHighDateTime > Start.dwHighDateTime)
      deltaTime = End.dwLowDateTime + ((MAX_LOW_LONG - Start.dwLowDateTime) + 1);
   else
      deltaTime = End.dwLowDateTime - Start.dwLowDateTime;

   delTime = float(deltaTime/10);   // Convert deltaTime to microseconds
   size = (float)Size;
   //
   // Throughput in MB/sec then equals number of bytes transferred
   // divided by the time in microseconds.
   //
   *pThroughput = size / delTime;

   return(1);
} //End GetThroughput0



int Write_Async_Continuous2(HANDLE hDev0)
{
   DWORD exitCode = 0xA5A5;      // thread exit code
   DWORD errorCode = 0x5A5A;     // thread exit error code
   UCHAR *pAsyncWrBuf;
   size_t Size = 1048576;
   UCHAR transaction = 0x00;    //Asynchronous transaction number
	ULONG result, bytecount;
   UCHAR loopCount = 0;
   //
   //Async Bulk Read: Create a buffer of X bytes for the call.
   //
   if(!(pAsyncWrBuf = (UCHAR *)malloc(Size)))
   {
      printf("Thread2 Memory Fault\n");
      SetEvent(hEvent0);
      Fail = TRUE;
      ActiveThread2 = FALSE;
      ExitThread(errorCode);
   }

   while(1)
   {
      if(loopCount++ == 100)
      {
         printf("2\n");
         SetEvent(hEvent0);
         loopCount = 0;
      }
      Sleep(1);
      result = QuickUsbWriteDataAsync(hDev0, (PUCHAR)pAsyncWrBuf, Size, &transaction);
      if(!result)
      {
         printf("Thread2 Async Write failure\n");
         SetEvent(hEvent0);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread2 = FALSE;
         ExitThread(errorCode);
      }
      Sleep(0);   //Give timeslice to other threads
      result = QuickUsbAsyncWait(hDev0, &bytecount, transaction);
      if(!result)
      {
         printf("Thread2 Async Wait failure\n");
         SetEvent(hEvent0);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread2 = FALSE;
         ExitThread(errorCode);
      }
      if(bytecount != Size)
      {
         printf("Thread2 Write bytecount failure\n");
      }
      if(stopThread2)
      {
         printf("Thread2 normal exit\n");
         SetEvent(hEvent0);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread2 = FALSE;
         ExitThread(exitCode);
      }
   } //end while
} //End Write_Async_Continuous2



int Read_Async3(HANDLE hDev1)
{
   DWORD exitCode = 0xA5A5;      // thread exit code
   DWORD errorCode = 0x5A5A;     // thread exit error code
   size_t Size = 1048576;
   UCHAR *pAsyncRdBuf;
	ULONG error, result;
   float throughput;          //Asynchronous Read throughput
   ULONG bytecount = 0;       //Returned read bytecount
   UCHAR transaction = 0x00;  //Asynchronous transaction number
   //
   //Async Bulk Read: Create a buffer of X bytes for the call.
   //
   if(!(pAsyncRdBuf = (UCHAR *)malloc(Size)))
   {
      printf("Thread3 Memory Fault\n");
      Fail = TRUE;
      ActiveThread3 = FALSE;
      ExitThread(errorCode);
   }
   while(1)
   {
      error = WaitForSingleObject(hEvent1, 10000);
      if(error == WAIT_TIMEOUT)
      {   
         printf("Thread3 Wait failure\n");
         free(pAsyncRdBuf);
         Fail = TRUE;
         ActiveThread3 = FALSE;
         ExitThread(errorCode);
      }
      if(stopThread3)
      {
         printf("Thread3 normal exit\n");
         free(pAsyncRdBuf);
         Fail = TRUE;
         ActiveThread3 = FALSE;
         ExitThread(exitCode);
      }
      GetSystemTimeAsFileTime(&Start);
      result = QuickUsbReadDataAsync(hDev1, (PUCHAR)pAsyncRdBuf,
                                     (ULONG *)&Size, &transaction);
      if(!result)
      {
         printf("Thread3 Async Read failure\n");
         free(pAsyncRdBuf);
         Fail = TRUE;
         ActiveThread3 = FALSE;
         ExitThread(errorCode);
      }
      Sleep(0);   //Give timeslice to other threads
      result = QuickUsbAsyncWait(hDev1, &bytecount, transaction);
      if(!result)
      {
         printf("Thread3 Read Async Wait failure\n");
         free(pAsyncRdBuf);
         Fail = TRUE;
         ActiveThread3 = FALSE;
         ExitThread(errorCode);
      }
      GetSystemTimeAsFileTime(&End);
      error = GetThroughput1(&throughput, Size);
      printf("Thread3 Read Throughput = %3.3f MB/sec. Bytecount = %d\n",
              throughput, bytecount);
   } //end while
} //End Read_Async3

int GetThroughput1(float *pThroughput, ULONG Size)
{
   DWORD deltaTime;   //End - Start time in 100ns interval
   float delTime;
   float size;

   if(End.dwHighDateTime > Start.dwHighDateTime)
      deltaTime = End.dwLowDateTime + ((MAX_LOW_LONG - Start.dwLowDateTime) + 1);
   else
      deltaTime = End.dwLowDateTime - Start.dwLowDateTime;

   delTime = float(deltaTime/10);   // Convert deltaTime to microseconds
   size = (float)Size;
   //
   // Throughput in MB/sec then equals number of bytes transferred
   // divided by the time in microseconds.
   //
   *pThroughput = size / delTime;

   return(1);
}



int Write_Async_Continuous4(HANDLE hDev1)
{
   DWORD exitCode = 0xA5A5;      // thread exit code
   DWORD errorCode = 0x5A5A;     // thread exit error code
   UCHAR *pAsyncWrBuf;
   size_t Size = 1048576;
   UCHAR transaction = 0x00;    //Asynchronous transaction number
	ULONG result, bytecount;
   UCHAR loopCount = 0;
   //
   //Async Bulk Read: Create a buffer of X bytes for the call.
   //
   if(!(pAsyncWrBuf = (UCHAR *)malloc(Size)))
   {
      printf("Thread2 Memory Fault\n");
      SetEvent(hEvent1);
      Fail = TRUE;
      ActiveThread4 = FALSE;
      ExitThread(errorCode);
   }

   while(1)
   {
      if(loopCount++ == 100)
      {
         printf("4\n");
         SetEvent(hEvent1);
         loopCount = 0;
      }
      Sleep(1);
      result = QuickUsbWriteDataAsync(hDev1, (PUCHAR)pAsyncWrBuf, Size, &transaction);
      if(!result)
      {
         printf("Thread4 Async Write failure\n");
         SetEvent(hEvent1);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread4 = FALSE;
         ExitThread(errorCode);
      }
      Sleep(0);   //Give timeslice to other threads
      result = QuickUsbAsyncWait(hDev1, &bytecount, transaction);
      if(!result)
      {
         printf("Thread4 Async Wait failure\n");
         SetEvent(hEvent1);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread4 = FALSE;
         ExitThread(errorCode);
      }
      if(bytecount != Size)
      {
         printf("Thread4 Write bytecount failure\n");
      }
      if(stopThread4)
      {
         printf("Thread4 normal exit\n");
         SetEvent(hEvent1);
         free(pAsyncWrBuf);
         Fail = TRUE;
         ActiveThread4 = FALSE;
         ExitThread(exitCode);
      }
   } //end while
} //End Write_Async_Continuous4
