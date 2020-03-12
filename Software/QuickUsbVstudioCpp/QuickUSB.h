/*=============================================================================
 Title        : QuickUSB.c
 Description  : QuickUSB QUSB2 Module API
 Notes        : None
 History      :

 Copyright © 2003 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 697 Via Miguel
 Santa Barbara, CA  93111.

 Bitwise Systems
 5385 Hollister Ave, Suite 215
 Santa Barbara, CA  93111
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

 $History: QuickUSB.h $
// 
// *****************  Version 1  *****************
// User: Blake Henry  Date: 4/08/05    Time: 7:13p
// Created in $/Projects/Trex/RIB/Software/RIBDIAG
// 
// *****************  Version 9  *****************
// User: Blake Henry  Date: 8/21/03    Time: 9:00a
// Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
// Added timer constants
 
 *****************  Version 8  *****************
 User: Blake Henry  Date: 8/07/03    Time: 7:13p
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Added timer support
 Bumped rev to 2.00
 
 *****************  Version 7  *****************
 User: Alon Dakik   Date: 8/07/03    Time: 8:32a
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Changed some of the prototypes from having a char * parameter to
 unsigned char *.
 
 Also added a single line "bytesReceived = length" to allow for longer
 string descriptors
 
 *****************  Version 5  *****************
 User: Blake Henry  Date: 6/11/03    Time: 9:37a
 Updated in $/Projects/Bitwise/QuickUSB/Software/QuickUsb
 Fixed header to correctly display the History

=============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef WIN32
#include <windows.h>
#endif

#ifdef __linux__
#include <stdio.h>
#include "usb.h"
#include "string.h"
#define WINAPI
#define HANDLE  usb_dev_handle *
#endif

// QuickUSB String Descriptor Indices
#define QUICKUSB_MAKE         0x01
#define QUICKUSB_MODEL        0x02

// QuickUSB vendor specific requests
#define QUICKUSB_SETTING      0xB0
#define QUICKUSB_FPGA         0xB1
#define QUICKUSB_COMMAND      0xB2
#define QUICKUSB_PORT         0xB3
#define QUICKUSB_RS232        0xB4
#define QUICKUSB_I2C          0xB5
#define QUICKUSB_SPI          0xB6
#define QUICKUSB_READDATALEN  0xB7
#define QUICKUSB_TIMER  		0xB8


// QuickUSB settings (wIndex values for QUICKUSB_SETTING
#define SETTING_WORDWIDE      1           // *wbuf = 1 is 16 bit transfers, 0 is 8 bits
#define SETTING_DATAADDRESS   2           // *wbuf = Data transfer address.  
                                          // If the high bit is set, don't increment addresses
#define SETTING_FIFOCONFIG    3           // *wbuf: 0xFB=Slave, 0xFA=Master mode
#define SETTING_FPGATYPE      4           // FPGA configuration type
#define SETTING_CPUCONFIG     5           // Sets/Reads CPUCS

// FIFO config settings
#define FIFOCONFIG_GPIF       0xFA
#define FIFOCONFIG_SLAVE      0x03

// FPGA type settings
#define FPGATYPE_ALTERAPS     0        // Altera passive serial

// QuickUSB Timer register addresses
#define TIMER_TCON				0
#define TIMER_TMOD				1
#define TIMER_T0					2
#define TIMER_T1					3
#define TIMER_CKCON				4
#define TIMER_IE					5
#define TIMER_T2CON				6
#define TIMER_RCAP2				7
#define TIMER_T2					8
#define TIMER_PORTACFG			9
#define TIMER_PORTECFG			10


/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  External function declarations
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// General purpose calls
extern int WINAPI QuickUsbFindModules(char *nameList,
                                     unsigned long length);
extern int WINAPI QuickUsbOpen(HANDLE *hDevice, 
			       char *deviceName);

extern int WINAPI QuickUsbClose(HANDLE hDevice);

extern int WINAPI QuickUsbGetStringDescriptor(HANDLE hDevice, 
                                              unsigned char index,
                                              char *buffer, 
                                              unsigned short length);

extern int WINAPI QuickUsbReadSetting(HANDLE hDevice, 
                                      unsigned short address, 
                                      unsigned short *value);
extern int WINAPI QuickUsbWriteSetting(HANDLE hDevice,
                                       unsigned short address, 
                                       unsigned short value);

extern int WINAPI QuickUsbStartFpgaConfiguration(HANDLE hDevice);

extern int WINAPI QuickUsbWriteFpgaData(HANDLE hDevice, 
                                        unsigned char *fpgadata, 
                                        unsigned long datalength);

extern int WINAPI QuickUsbIsFpgaConfigured(HANDLE hDevice, 
              	                          unsigned short *isConfigured);


extern int WINAPI QuickUsbReadCommand(HANDLE hDevice, 
                                      unsigned short address, 
                                      unsigned char *data, 
                                      unsigned short *length);
extern int WINAPI QuickUsbWriteCommand(HANDLE hDevice, 
                                       unsigned short address, 
                                       unsigned char *data, 
                                       unsigned short length);


extern int WINAPI QuickUsbReadData(HANDLE hDevice, 
                                   unsigned char *data, 
                                   unsigned long *length);

extern int WINAPI QuickUsbWriteData(HANDLE hDevice, 
                                    unsigned char *data, 
                                    unsigned long length);


extern int WINAPI QuickUsbReadPortDir(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data);

extern int WINAPI QuickUsbWritePortDir(HANDLE hDevice, 
                                    unsigned short address, 
                                    unsigned char data);

extern int WINAPI QuickUsbReadPort(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data, 
                                   unsigned short *length);

extern int WINAPI QuickUsbWritePort(HANDLE hDevice, 
                                    unsigned short address, 
                                    unsigned char *data, 
                                    unsigned short length);


extern int WINAPI QuickUsbSetRS232BaudRate(HANDLE hDevice, 
                                           unsigned long baudRate);

extern int WINAPI QuickUsbGetNumRS232(HANDLE hDevice, 
                                      unsigned char portNum, 
                                      unsigned short *length);

extern int WINAPI QuickUsbFlushRS232(HANDLE hDevice, 
                                     unsigned char portNum);

extern int WINAPI QuickUsbReadRS232(HANDLE hDevice, 
                                    unsigned char portNum, 
                                    unsigned char *data, 
                                    unsigned short *length);

extern int WINAPI QuickUsbWriteRS232(HANDLE hDevice, 
                                     unsigned char portNum, 
                                     unsigned char *data, 
                                     unsigned short length);

extern int WINAPI QuickUsbReadSpi(HANDLE hDevice, 
                                  unsigned char portNum,
                                  unsigned char *data, 
                                  unsigned short *length);

extern int WINAPI QuickUsbWriteSpi(HANDLE hDevice, 
                                   unsigned char portNum,
                                   unsigned char *data, 
                                   unsigned short length);

extern int WINAPI QuickUsbWriteReadSpi(HANDLE hDevice, 
                                       unsigned char portNum, 
                                       unsigned char *data, 
                                       unsigned short length);

extern int WINAPI QuickUsbReadI2C(HANDLE hDevice, 
                                  unsigned short address, 
                                  unsigned char *data, 
                                  unsigned short *length);

extern int WINAPI QuickUsbWriteI2C(HANDLE hDevice, 
                                   unsigned short address, 
                                   unsigned char *data,
                                   unsigned short length);

#ifdef __cplusplus
}
#endif
