/*=============================================================================
 Title        : QuickUSB.c
 Description  : QuickUSB QUSB2 Module API
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
#ifdef __cplusplus
extern "C" {
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

extern int WINAPI QuickUsbReadTimerReg(HANDLE hDevice, 
                                       unsigned short address, 
                                       unsigned short *data);
extern int WINAPI QuickUsbWriteTimerReg(HANDLE hDevice, 
                                        unsigned short address, 
                                        unsigned short data);
extern int WINAPI QuickUsbReadTimerIsrRegs(HANDLE hDevice, 
                                           unsigned short *data, 
                                           unsigned short *length);

#ifdef __cplusplus
}
#endif
