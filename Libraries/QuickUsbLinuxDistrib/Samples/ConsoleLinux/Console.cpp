/*=============================================================================
 Title        : Console.cpp
 Description  : QuickUSB Console App Sample
 Notes        : None
 History      :

 Copyright � 2003,2004 Bitwise Systems.  All rights reserved.
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
#if defined _WIN32
#define DRIVER QuickUsb
#define QUICKUSB_LIBRARY
#pragma message ("Compiling for Windows with QuickUsb.sys")
#elif defined __linux__
#define DRIVER libusb
//#warning Compiling for Linux with libusb
#elif defined __APPLE__
#define DRIVER libusb
//#warning Compiling for Apple with libusb
#elif defined WindowsVista
#define DRIVER winusb
// #warn Compiling for Windows Vista with winusb
#else
#error Invalid operating system or driver detected
#endif

#if defined _WIN32
#include "stdafx.h"
#include "windows.h"
#endif

#ifdef __linux__
#include <stdio.h>
#include "usb.h"
#include "string.h"
#define FALSE                 0
#define TRUE                  1
#define HANDLE  usb_dev_handle *
#endif

#include "QuickUSB.h"


int main(int argc, char* argv[])
{
	char *namePtr, *devName;
	char nameList[120];
	unsigned short data[2];
        unsigned char bulkdata[4096];
	unsigned short length;
	unsigned long count, len;
	int result;
	QHANDLE hDev;
   
	printf("Hello QuickUSB World!\n");

	//  Find the QuickUSB modules in the system
   	len = 512;
	result = QuickUsbFindModules(nameList, len);

	// Check for no modules and bail if we don't find any
	if (*nameList == '\0') {
		printf("Couldn't find any modules\n");
      		return 0;
	}

	// Print out the name of each module found
	namePtr = nameList;
	while (*namePtr != '\0') {
		printf("Found %s\n", namePtr);
		namePtr = namePtr + strlen(namePtr) + 1;
	}
        
        // Just use the first device in the list because it's easy
        devName = nameList;

	// Open the first device
        result = QuickUsbOpen(&hDev, devName);
	if (result == FALSE) {
           printf("Cannot open %s\n", devName);
	   return 1;
	}

	// Read the Command register at address 0
        length = 2;
	result = QuickUsbReadCommand(hDev, 0, (unsigned char *) data, &length);
	if (result == FALSE) {
           printf("Cannot read %s command register: hDev = %p\n", devName, hDev);
	   return 1;
	}
        printf("ReadCommand address 0 = %04X\n", (unsigned int)*data);

	// Make all the pins outputs for now
	result = QuickUsbWritePortDir(hDev, 0, (char) 0xFF);
	if (result == FALSE) {
           printf("Cannot write %s port A direction register\n", devName);
	   return 1;
	}

    	// Loop through a pattern
	data[0] = 0xAAAA;
	data[1] = 0x5555;
        printf("Writing 128 cycles of 0xAAAA, 0x5555 to %s, port A...\n", devName); 
	for (count = 0; count < 128; count++) {
		result = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[0], 2);
		if (result == FALSE) {
                   printf("Cannot write %s port A data\n", devName);
		   return 1;
		}

		result = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[1], 2);
		if (result == FALSE) {
                   printf("Cannot write %s port A data\n", devName);
		   return 1;
		}
	}

	// Write a 4k block of data
        length = 4096;
        result = QuickUsbWriteData(hDev, (unsigned char *) bulkdata, length);
        if (result == FALSE) {
           printf("Cannot write data to %s : hDev = %p\n", devName, hDev);
           return 1;
        }
        printf("QuickUsbWriteData wrote %d bytes to %s\n", length, devName);

	// Close the port when you're done with it
	QuickUsbClose(hDev);

	printf("done");
	return 0;
}



