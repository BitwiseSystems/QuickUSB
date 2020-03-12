/*=============================================================================
 Title        : 
 Description  : 
 Notes        : 
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
#include <stdio.h>
#include <string.h>

#include "QuickUSB.h"



int main(int argc, char *argv[])
{
    PQCHAR namePtr;
    PQCHAR devName;
    QCHAR nameList[120];
    QWORD data[2];
    QBYTE bulkdata[4096];
    QWORD length;
    QULONG count, len;
    QRESULT ok;
    QHANDLE hDev;
    QULONG err;

    printf("Hello QuickUSB World!\n");

    //  Find the QuickUSB modules in the system
    len = 512;
    ok = QuickUsbFindModules(nameList, len);

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
    ok = QuickUsbOpen(&hDev, devName);
    if (!ok) {
        ok = QuickUsbGetLastError(&err);
        printf("QuickUsbOpen() return with error %li", err);
        return 1;
    }

    // Read the Command register at address 0
    length = 2;
    ok = QuickUsbReadCommand(hDev, 0, (unsigned char *) data, &length);
    if (!ok) {
        ok = QuickUsbGetLastError(&err);
        printf("QuickUsbReadCommand() return with error %li", err);
        ok = QuickUsbClose(hDev);
        return 1;
    }
    printf("ReadCommand address 0 = %04X %04X\n", (unsigned int) data[0], (unsigned int) data[1]);

    // Make all the pins outputs for now
    ok = QuickUsbWritePortDir(hDev, 0, (char) 0xFF);
    if (!ok) {
        ok = QuickUsbGetLastError(&err);
        printf("QuickUsbWritePortDir() return with error %li", err);
        ok = QuickUsbClose(hDev);
        return 1;
    }

    // Loop through a pattern
    data[0] = 0xAAAA;
    data[1] = 0x5555;
    printf("Writing 128 cycles of 0xAAAA, 0x5555 to %s, port A...\n", devName); 
    for (count = 0; count < 128; count++) {
        ok = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[0], 2);
        if (!ok) {
            ok = QuickUsbGetLastError(&err);
            printf("QuickUsbWritePort() return with error %li", err);
            ok = QuickUsbClose(hDev);
            return 1;
        }

        ok = QuickUsbWritePort(hDev, 0, (unsigned char *) &data[1], 2);
        if (!ok) {
            ok = QuickUsbGetLastError(&err);
            printf("QuickUsbWritePort() return with error %li", err);
            ok = QuickUsbClose(hDev);
            return 1;
        }
    }

    // Write a 4k block of data
    length = 4096;
    ok = QuickUsbWriteData(hDev, (unsigned char *) bulkdata, length);
    if (!ok) {
        ok = QuickUsbGetLastError(&err);
        printf("QuickUsbWriteData() return with error %li", err);
        ok = QuickUsbClose(hDev);
        return 1;
    }
    printf("QuickUsbWriteData wrote %d bytes to %s\n", length, devName);

    // Close the port when you're done with it
    ok = QuickUsbClose(hDev);
    if (!ok) {
        ok = QuickUsbGetLastError(&err);
        printf("QuickUsbClose() return with error %li", err);
        ok = QuickUsbClose(hDev);
        return 1;
    }

    return 0;
}
