/*=============================================================================
 Title        : Console.cpp
 Description  : QuickUSB Console App Sample
 Notes        : None
 History      :

 Copyright © 2003,2004 Bitwise Systems.  All rights reserved.
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
#include "stdafx.h"
#include "windows.h"
#include "quickusb.h"


int main(int argc, char* argv[])
{
	char *namePtr;
	char nameList[120];
	unsigned short length, data[2];
	unsigned long count, len, errorCode;
	int result;
	HANDLE hDev;
   
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

	// Open the first device
	result = QuickUsbOpen(&hDev, nameList);
	if (result == FALSE) {
		printf("Cannot open %s\n", nameList);
		return 1;
	}

	// Read the Command register at address 0
   length = 2;
	result = QuickUsbReadCommand(hDev, 0, (unsigned char *) data, &length);
	if (result == FALSE) {
		printf("Cannot read %s command register\n", nameList);
		return 1;
	}
   printf("ReadCommand address 0 = %04X\n");

	// Make all the pins outputs for now
	result = QuickUsbWritePortDir(hDev, 0, (char) 0xFF);
	if (result == FALSE) {
		printf("Cannot write %s port A direction register\n", nameList);
		return 1;
	}

    // Loop through a pattern
	data[0] = 0xAAAA;
	data[1] = 0x5555;
	printf("Writing 128 cycles of 0xAAAA, 0x5555 to %s, port A...\n", nameList); 
	for (count = 0; count < 128; count++) {
		result = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[0], 2);
		if (result == FALSE) {
			printf("Cannot write %s port A data\n", nameList);
			return 1;
		}

		result = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[1], 2);
		if (result == FALSE) {
			printf("Cannot write %s port A data\n", nameList);
			return 1;
		}
	}

    QuickUsbGetLastError(&errorCode);
    
	// Close the port when you're done with it
	QuickUsbClose(hDev);

	printf("done");
	return 0;
}



