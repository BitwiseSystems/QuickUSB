/*=============================================================================
Title        : AsyncReadTestMulti.cpp
Description  : QuickUSB Asynchronous I/O console program for multiple modules
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

// AsyncReadTest.cpp : Defines the entry point for the console application.
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
	printf("AsyncReadTest <file name> <block size> <buffer count> <num modules>\n");
	printf("<block size> must be from 1 to 64 million\n");
	printf("<buffer count> must be from 2 to 253\n");
	printf("<num modules> must be from 1 to 127\n");
}

// Global variables
char *            FileName;
FILE *            outFile;
unsigned char *   buffer[127][256];
DWORD             start, finish;
QHANDLE            hDevice[127];
unsigned long     blocksize;
unsigned long     byteswritten;
unsigned long     error;
unsigned long     bytesread[127][256];
double            bytecount = 0;
double            elapsed_time;
int               bufcnt;                 // buffer count
int               index;
int               shutdownAsync = FALSE;
int               result;
int               lastIndex;
char              nameList[1024];
unsigned char     transaction[127][256];
int	              moduleIndex;
int               numModules;
int               numAttachedModules;
char *            moduleName;
int	              lastModule;

// Function prototypes
void CloseModules();
int AllocateBuffers();
void FreeBuffers();

// Main entry point
int main(int argc, char* argv[])
{
	if (argc != 5) {
		ProgramSyntax();
		Sleep(5000);                        // let the user see the error
		return 1;
	}

	FileName = argv[1];
	blocksize = atoi(argv[2]);
	bufcnt = atoi(argv[3]);
	numModules = atoi(argv[4]);

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

	if ((numModules < 1) || (bufcnt > 127)) {
		ProgramSyntax();
		Sleep(5000);                        // let the user see the error
		return 1;
	}

	outFile = fopen(FileName, "w+b");

	if (outFile == NULL) {
		printf("Unable to open file %s for output\n", FileName);
		Sleep(5000);                        // let the user see the error
		return 1;
	}

	// Find the QuickUSB modules in the system
	QuickUsbFindModules(&nameList[0], 1024);

	// Parse the module name-list
	numAttachedModules = 0;
	if (nameList[0] != 0) {
		moduleName = nameList;
		do {
			// Open the QuickUSB module
			if (numAttachedModules < numModules) {
				result = QuickUsbOpen(&hDevice[numAttachedModules], moduleName);

				if (result == FALSE) {
					CloseModules();
					FreeBuffers();
					QuickUsbGetLastError(&error);
					printf("QuickUsbOpen error %d for module %s\n", error, moduleName);
					fclose(outFile);
					Sleep(5000);                        // let the user see the error
					return 1;
				}

				// Set QuickUSB I/O timeout to 100 seconds
				// This is required with a 64 Meg block size and 253 buffers.
				// This value should probably be calculated to something reasonable.
				result = QuickUsbSetTimeout(hDevice[numAttachedModules], 1000000);

				if (result == FALSE) {
					CloseModules();
					FreeBuffers();
					QuickUsbGetLastError(&error);
					printf("QuickUsbSetTimeout error %d for module %s\n", error, moduleName);
					fclose(outFile);
					Sleep(5000);                        // let the user see the error
					return 1;
				}
			}

			// Move to next module name
			++numAttachedModules;
			while (*moduleName != 0)
				++moduleName;
		} while (*(++moduleName) != 0);
	}

	// Verify enough modules are attached
	if (numAttachedModules < numModules)
	{
		CloseModules();
		printf("Not enough modules attached (%d of %d attached)\n", numAttachedModules, numModules);
		fclose(outFile);
		Sleep(5000);                        // let the user see the error
		return 1;
	}

	// Allocate buffers
	if (AllocateBuffers() != 0) {
		return 1;
	}

	// Get the current time
	start = GetTickCount();

	// Start up several asynchronous read commands
	lastIndex = 0;
	lastModule = 0;
	printf("Starting up %d read requests per module (for %d modules).\n", bufcnt, numModules);
	for (moduleIndex = 0; moduleIndex < numModules; ++moduleIndex) {
		for (index = 0; index < bufcnt; index++) {
			result = QuickUsbReadDataAsync(hDevice[moduleIndex], buffer[moduleIndex][index], &blocksize, &transaction[moduleIndex][index]);

			if (result == FALSE) {
				QuickUsbGetLastError(&error);
				printf("QuickUsbReadDataAsync error %d index %d module %d\n", error, index, moduleIndex);
				shutdownAsync = TRUE;
				break;
			}
			else {
				// Track the indicies of the last QuickUsbReadDataAsync call to help with the shutdown process
				lastIndex = index;
				lastModule = moduleIndex;
			}
		}
		if (result == FALSE) {
			break;
		}
	}

	moduleIndex = 0;
	index = 0;
	printf("Acquiring data...\n");
	while (TRUE) {
		// Wait for the next QuickUSB read operation to finish
		result = QuickUsbAsyncWait(hDevice[moduleIndex], &bytesread[moduleIndex][index], transaction[moduleIndex][index], FALSE);

		if (result == FALSE) {
			QuickUsbGetLastError(&error);
			printf("QuickUsbAsyncWait error %d index %d module %d\n", error, index, moduleIndex);
			shutdownAsync = TRUE;
		}

		// Did write function complete?
		if (bytesread[moduleIndex][index] == blocksize) {
			bytecount += blocksize;
		}

		// Write the block to disk
		byteswritten = (unsigned long)fwrite(buffer[moduleIndex][index], 1, blocksize, outFile);

		if (byteswritten != blocksize) {
			printf("Disk write error\n");
			shutdownAsync = TRUE;
		}

		if ((shutdownAsync == TRUE) && (moduleIndex == lastModule) && (index == lastIndex)) {
			break;
		}

		// Check if any key was pressed
		if (shutdownAsync == FALSE) {
			if (_kbhit()) {
				printf("Program is shutting down, waiting for queued I/O to finish.\n");
				shutdownAsync = TRUE;
			}
		}

		// Start a new QuickUSB read
		if (shutdownAsync == FALSE) {
			result = QuickUsbReadDataAsync(hDevice[moduleIndex], buffer[moduleIndex][index], &blocksize, &transaction[moduleIndex][index]);

			if (result == FALSE) {
				QuickUsbGetLastError(&error);
				printf("QuickUsbReadDataAsync error %d index %d module %d\n", error, index, moduleIndex);
				shutdownAsync = TRUE;
			}
			else {
				// Track the indicies of the last QuickUsbReadDataAsync call to help with the shutdown process
				lastIndex = index;
				lastModule = moduleIndex;
			}
		}

		// Increment index
		index++;

		if (index == bufcnt) {
			index = 0;
			
			// Increment module index
			++moduleIndex;

			if (moduleIndex == numModules) {
				moduleIndex = 0;
			}
		}
	}

	// Get the current time
	finish = GetTickCount();
	elapsed_time = (double)(finish - start) / 1000;

	// Free up resources
	CloseModules();
	FreeBuffers();
	fclose(outFile);

	// Display time and data rate
	printf("Elapsed time %f seconds to transfer %f Megabytes.\n",
		elapsed_time, bytecount / MEGA);
	printf("Average throughput %f Megabytes per second.\n",
		(bytecount / MEGA) / elapsed_time);
	Sleep(5000);                        // let the user see the stats
	return 0;
}

void CloseModules()
{
	// Close all opened modules
	for (moduleIndex = 0; moduleIndex < min(numModules, numAttachedModules); ++moduleIndex) {
		QuickUsbClose(hDevice[moduleIndex]);
	}
}

void FreeBuffers()
{
	// Free buffers
	for (moduleIndex = 0; moduleIndex < min(numModules, numAttachedModules); ++moduleIndex) {
		for (index = 0; index < bufcnt; index++) {
			free(buffer[moduleIndex][index]);
		}
	}
}
int AllocateBuffers()
{
	// Allocate buffers
	for (moduleIndex = 0; moduleIndex < numModules; moduleIndex++) {
		for (index = 0; index < bufcnt; index++) {
			buffer[moduleIndex][index] = (unsigned char *)malloc(blocksize);

			if (buffer[moduleIndex][index] == NULL) {
				//FreeBuffers();
				printf("Unable to allocate buffer number %d for module %d\n", index, moduleIndex);
				fclose(outFile);
				Sleep(5000);                        // let the user see the error
				return 1;
			}
		}
	}
	return 0;
}