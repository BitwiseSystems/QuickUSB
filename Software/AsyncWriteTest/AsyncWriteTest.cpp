/*=============================================================================
 Title        : AsyncWriteTest.cpp
 Description  : QuickUSB Asynchronous I/O test console program
 Notes        : None
 History      :

 Copyright © 2012 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.

 Bitwise Systems
 6489 Calle Real, Suite E
 Goleta, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com
=============================================================================*/

// AsyncWriteTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>
#include "quickusb.h"

#define MEGA (1024 * 1024)

static void ProgramSyntax(void)
{
   printf("Program syntax is:\n");
   printf("AsyncWriteTest <file name> <block size> <buffer count>\n");
   printf("<block size> must be from 1 to 64 million\n");
   printf("<buffer count> must be from 2 to 253\n");
}


int main(int argc, char* argv[])
{
   char *            FileName;
   FILE *            inFile;
   unsigned char *   buffer[256];
   DWORD             start, finish;
   QHANDLE            hDevice;
   unsigned long     blocksize;
   unsigned long     bytesread;
   unsigned long     error;
   unsigned long     bytes2write[256];
   unsigned long     byteswritten[256];
   double            bytecount = 0;
   double            elapsed_time;
   int               bufcnt;                 // buffer count
   int               index;
   int               shutdown = FALSE;
   int               result;
   int               lastIndex;
   char              nameList[128];
   unsigned char     transaction[256];

   if (argc != 4) {
      ProgramSyntax();
      Sleep(5000);                        // let the user see the error
      return 1;
   }
   
   FileName = argv[1];
   blocksize = atoi(argv[2]);
   bufcnt = atoi(argv[3]);

   if ((blocksize < 1) || (blocksize > (64 * 1024 * 1024))) {
      ProgramSyntax();
      Sleep(5000);                        // let the user see the error
      return 1;
   }

   if ((bufcnt < 2) || (bufcnt > 253)) {
      ProgramSyntax();
      Sleep(5000);                        // let the user see the error
      return 1;
   }

   inFile = fopen(FileName, "r+b");

   if (inFile == NULL) {
      printf("Unable to open file %s for input\n", FileName);
      Sleep(5000);                        // let the user see the error
      return 1;
   }

   // Allocate buffers
   for (index = 0; index < bufcnt; index++) {
      buffer[index] = (unsigned char *)malloc(blocksize);

      if (buffer[index] == NULL) {
         printf("Unable to allocate buffer number %d\n", index);
         fclose(inFile);
         Sleep(5000);                        // let the user see the error
         return 1;
      }
   }

   // Find the QuickUSB modules in the system
	QuickUsbFindModules(&nameList[0], 128);

   // Open the first QuickUSB device
   result = QuickUsbOpen(&hDevice, (&nameList[0]));

   if (result == FALSE) {
      QuickUsbGetLastError(&error);
      printf("QuickUsbOpen error %d\n", error);
      fclose(inFile);
      Sleep(5000);                        // let the user see the error
      return 1;
   }

   // Set QuickUSB I/O timeout to 100 seconds
   // This is required with a 64 Meg block size and 253 buffers.
   // This value should probably be calculated to something reasonable.
   result = QuickUsbSetTimeout(hDevice, 1000000);

   if (result == FALSE) {
      QuickUsbGetLastError(&error);
      printf("QuickUsbSetTimeout error %d\n", error);
      fclose(inFile);
      QuickUsbClose(hDevice);
      Sleep(5000);                        // let the user see the error
      return 1;
   }

   // Get the current time
   start = GetTickCount();


   // Start up several asynchronous write commands
   printf("Starting up %d write requests.\n", bufcnt);

   for (index = 0; index < bufcnt; index++) {
      bytesread = (unsigned long)fread(buffer[index], 1, blocksize, inFile);

      if (bytesread == 0) {            /* end of file or read error */
         shutdown = TRUE;
         break;
      }
      
      bytes2write[index] = bytesread;
      result = QuickUsbWriteDataAsync(hDevice, buffer[index], bytes2write[index], &transaction[index]);

      if (result == FALSE) {
         QuickUsbGetLastError(&error);
         printf("QuickUsbWriteDataAsync error %d index %d\n", error, index);
         fclose(inFile);
         QuickUsbClose(hDevice);
         Sleep(5000);                        // let the user see the error
         return 1;
      }

      lastIndex = index;

      if (bytesread != blocksize) {    /* short last block read from disk??? */
         shutdown = TRUE;
         break;
      }
   }

   index = 0;

   while (TRUE) {
      // Wait for the next QuickUSB write operation to finish
      result = QuickUsbAsyncWait(hDevice, &byteswritten[index], transaction[index], FALSE);

      if (result == FALSE) {
         QuickUsbGetLastError(&error);
         printf("QuickUsbAsyncWait error %d index %d\n", error, index);
         fclose(inFile);
         QuickUsbClose(hDevice);
         Sleep(5000);                        // let the user see the error
         return 1;
      }

      // Did write function complete?
      if (byteswritten[index] == bytes2write[index]) {
         bytecount += bytes2write[index];
      }

      if ((shutdown == TRUE) && (index == lastIndex)) {
         break;
      }
      
      // Read another block from disk, and write to the QuickUSB
      if (shutdown == FALSE) {
         bytesread = (unsigned long)fread(buffer[index], 1, blocksize, inFile);

         if (bytesread == 0) {            /* end of file or read error */
            shutdown = TRUE;
            // Increment index
            index++;

            if (index == bufcnt) {
               index = 0;
            }

            continue;
         }
      
         bytes2write[index] = bytesread;
         result = QuickUsbWriteDataAsync(hDevice, buffer[index], bytes2write[index], &transaction[index]);

         if (result == FALSE) {
            QuickUsbGetLastError(&error);
            printf("QuickUsbWriteDataAsync error %d index %d\n", error, index);
            fclose(inFile);
            QuickUsbClose(hDevice);
            Sleep(5000);                        // let the user see the error
            return 1;
         }

         lastIndex = index;

         if (bytesread != blocksize) {    /* short last block read from disk??? */
            shutdown = TRUE;
         }
      }

      // Increment index
      index++;

      if (index == bufcnt) {
         index = 0;
      }
   }

   // Get the current time
   finish = GetTickCount();
   elapsed_time = (double)(finish - start) / 1000;
   fclose(inFile);
   QuickUsbClose(hDevice);

   for (index = 0; index < bufcnt; index++) {
      free(buffer[index]);
   }

   printf("Elapsed time %f seconds to transfer %f Megabytes.\n",
      elapsed_time, bytecount / MEGA);
   printf("Average throughput %f Megabytes per second.\n",
      (bytecount / MEGA) / elapsed_time);
   Sleep(5000);                        // let the user see the stats
   return 0;
}
