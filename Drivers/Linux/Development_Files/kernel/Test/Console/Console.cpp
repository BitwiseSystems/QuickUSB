/*=============================================================================
 Title        : Console.cpp
 Description  : QuickUSB Console App Sample
 Notes        : None
 History      :

 Copyright (c) 2020 Bitwise Systems, Inc.

 Permission is hereby granted, free of charge, to any person obtaining 
 a copy of this software and associated documentation files (the "Software"), 
 to deal in the Software without restriction, including without limitation 
 the rights to use, copy, modify, merge, publish, distribute, sublicense, 
 and/or sell copies of the Software, and to permit persons to whom the 
 Software is furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included 
 in all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS 
 OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL 
 THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS 
 IN THE SOFTWARE.

=============================================================================*/
/*#if defined _WIN32
#define DRIVER QuickUsb
#define QUICKUSB_LIBRARY
#pragma message ("Compiling for Windows with QuickUsb.sys")
#elif defined __linux__
#define DRIVER libusb
#warning Compiling for Linux with libusb
#elif defined __APPLE__
#define DRIVER libusb
#warning Compiling for Apple with libusb
#elif defined WindowsVista
#define DRIVER winusb
// #warn Compiling for Windows Vista with winusb
#else
#error Invalid operating system or driver detected
#endif*/

#if defined _WIN32
#include "stdafx.h"
#include "windows.h"
#endif

#if defined __APPLE__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define FALSE 0
#define TRUE 1
#define LENGTH 1024
#endif

#ifdef __linux__
#include <stdio.h>
#include "usb.h"
#include "string.h"
#define FALSE                 0
#define TRUE                  1
#define HANDLE  usb_dev_handle *
#endif

#include "QuickUsb.h"


int main(int argc, char* argv[]) {
	QHANDLE hDev;
    QCHAR nameList[120];
    QCHAR *namePtr, *devName;
    QWORD isConfigured, fifoConfig, fpgaType, cpuConfig, mask;
    QBYTE command[1];
    QBYTE *fpgaBuffer;
    QULONG fpgaBufferSize;
    FILE *fp;
    QULONG length;
    QBYTE wdata[LENGTH], rdata[LENGTH], wdata2[LENGTH], rdata2[LENGTH];
    QULONG i;
	QRETURN result;
   
	printf("Hello QuickUSB World!\n");

	//  Find connected QuickUSB modules
    printf("Finding connected QuickUSB modules\n");
	result = QuickUsbFindModules(nameList, 512);

	// Check if no modules are connected
	if (*nameList == '\0') {
		printf("Could not find any modules\n");
        return 1;
	}

	// Print out the name of each module found
	namePtr = nameList;
	while (*namePtr != '\0') {
		printf("Found %s\n", namePtr);
		namePtr = namePtr + strlen((const char *)namePtr) + 1;
	}
        
    // Use the first connected device
    devName = nameList;

	// Open the first device
    printf("Opening %s\n", devName);
    result = QuickUsbOpen(&hDev, (QCHAR *)devName);
	if (result == FALSE) {
        printf("Cannot open %s\n", devName);
        return 1;
	}
    
    result = QuickUsbReadSetting(hDev, SETTING_CPUCONFIG, &cpuConfig);
    if (result == FALSE) {
        printf("Failed to read CPU setting\n");
        QuickUsbClose(hDev);
        return 1;
    }
    mask = 1<<15;
    result = QuickUsbWriteSetting(hDev, SETTING_CPUCONFIG, cpuConfig & (~mask));
    if (result == FALSE) {
        printf("Failed to write CPU setting\n");
        QuickUsbClose(hDev);
        return 1;
    }
    
    // Check if FPGA is configured
    /*printf("Checking if FPGA is configured...");
    result = QuickUsbIsFpgaConfigured(hDev, &isConfigured);
    if (result == FALSE) {
        printf("Failed to check FPGA configuration\n");
        QuickUsbClose(hDev);
        return 1;
    }
    
    if (!isConfigured) {
        printf("no\n");
    
        // Turn on FPGA power and reset FPGA logic
        printf("Turning on FPGA power and resetting FPGA logic\n");
        result = QuickUsbWriteSetting(hDev, SETTING_PORTA, 0x8180);
        if (result == FALSE) {
            printf("Failed to set FPGA setting\n");
            QuickUsbClose(hDev);
            return 1;
        }
        result = QuickUsbWriteSetting(hDev, SETTING_PORTA, 0x8181);
        if (result == FALSE) {
            printf("Failed to set FPGA setting\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Set wordwide mode
        printf("Setting wordwide mode\n");
        result = QuickUsbWriteSetting(hDev, SETTING_WORDWIDE, 0);
        if (result == FALSE) {
            printf("Failed to set wordwide mode\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Set FIFO configuration
        printf("Setting FIFO configuration\n");
        result = QuickUsbReadSetting(hDev, SETTING_FIFO_CONFIG, &fifoConfig);
        if (result == FALSE) {
            printf("Failed to read FIFO configuration\n");
            QuickUsbClose(hDev);
            return 1;
        }
        result = QuickUsbWriteSetting(hDev, SETTING_FIFO_CONFIG, fifoConfig & 0xFFFE);
        if (result == FALSE) {
            printf("Failed to write FIFO configuration\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Set loop back mode
        printf("Setting loop back mode\n");
        command[0] = 0;
        result = QuickUsbWriteCommand(hDev, 0, command, 1);
        if (result == FALSE) {
            printf("Failed to write command register\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Set FPGA type to Altera Passive Serial
        printf("Setting FPGA type to Altera Passive Serial\n");
        result = QuickUsbReadSetting(hDev, SETTING_FPGATYPE, &fpgaType);
        if (result == FALSE) {
            printf("Failed to read FPGA type\n");
            QuickUsbClose(hDev);
            return 1;
        }
        result = QuickUsbWriteSetting(hDev, SETTING_FPGATYPE, fpgaType & 0xFE);
        if (result == FALSE) {
            printf("Failed to write FPGA type\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Start FPGA configuration
        printf("Starting FPGA configuration\n");
        sleep(1);
        result = QuickUsbStartFpgaConfiguration(hDev);
        if (result == FALSE) {
            printf("Failed to start FPGA configuration\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Read FPGA file and write to FPGA
        printf("Reading FPGA file and writing FIFO to FPGA\n");
        fp = fopen("QUSBEVB_REVA_EP2C20_FIFO.rbf", "rb");
        if (fp == NULL) {
            printf("Failed to open FPGA FIFO file\n");
            QuickUsbClose(hDev);
            return 1;
        }
        fseek(fp, 0, SEEK_END);
        fpgaBufferSize = ftell(fp);
        rewind(fp);
        fpgaBuffer = (QBYTE *)malloc(sizeof(QBYTE)*fpgaBufferSize);
        if (fpgaBuffer == NULL) {
            printf("Failed to create buffer for FPGA data\n");
            QuickUsbClose(hDev);
            return 1;
        }
        result = fread(fpgaBuffer, 1, fpgaBufferSize, fp);
        if (result != (int)fpgaBufferSize) {
            printf("Failed to read FPGA data\n");
            QuickUsbClose(hDev);
            free(fpgaBuffer);
            fclose(fp);
            return 1;
        }
        fclose(fp);
        result = QuickUsbWriteFpgaData(hDev, fpgaBuffer, fpgaBufferSize);
        if (result == FALSE) {
            printf("Failed to write FPGA data\n");
            QuickUsbClose(hDev);
            free(fpgaBuffer);
            return 1;
        }
        free(fpgaBuffer);
        
        // Check if FPGA is configured
        printf("Checking if FPGA is configured...");
        result = QuickUsbIsFpgaConfigured(hDev, &isConfigured);
        if (result == FALSE) {
            printf("Failed to check FPGA configuration\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        isConfigured = 0; // Only write FPGA FIFO and do not write data to the FPGA
    }
    
    if (isConfigured) {
        printf("yes\n");
        
        length = LENGTH;
        
        // Write data to FPGA
        printf("Writing data to FPGA\n");
        for (i = 0; i < length; i++) {
            wdata[i] = i%256;
        }
        result = QuickUsbWriteData(hDev, (QBYTE *)wdata, length);
        if (result == FALSE) {
            printf("Failed to write data to the FPGA\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Read data from FPGA and display data
        printf("Reading data from FPGA\n");
        result = QuickUsbReadData(hDev, (QBYTE *)rdata, &length);
        if (result == FALSE) {
            printf("Failed to read data to the FPGA\n");
            QuickUsbClose(hDev);
            return 1;
        }
        printf("\n");
        for (i = 0; i < length; i++) {
            printf("%d ", rdata[i]);
        }
        printf("\n\n");
        
        // Write data to FPGA
        printf("Writing data to FPGA\n");
        for (i = 0; i < length; i++) {
            wdata2[i] = 255-i%256;
        }
        result = QuickUsbWriteData(hDev, (QBYTE *)wdata2, length);
        if (result == FALSE) {
            printf("Failed to write data to the FPGA\n");
            QuickUsbClose(hDev);
            return 1;
        }
        
        // Read data from FPGA and display data
        printf("Reading data from FPGA\n");
        result = QuickUsbReadData(hDev, (QBYTE *)rdata2, &length);
        if (result == FALSE) {
            printf("Failed to read data to the FPGA\n");
            QuickUsbClose(hDev);
            return 1;
        }
        printf("\n");
        for (i = 0; i < length; i++) {
            printf("%d ", rdata2[i]);
        }
        printf("\n\n");
    }
    else {
        printf("no\n");
    }*/
    
	QuickUsbClose(hDev);

	printf("Done\n");
    
	return 0;
}



