/*=============================================================================
 Title        : QuickUSB.c
 Description  : QuickUSB QUSB2 Module API
 Notes        : None
 History      :

 Copyright © 2003-2010 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, Suite E,
 Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/
#ifdef __cplusplus
extern "C" {
#endif

#ifdef __linux__
#include <stdio.h>
#include "usb.h"
#include "string.h"
#define HANDLE  usb_dev_handle *
#endif

// QuickUSB String Descriptor Indices
#define QUICKUSB_MAKE                           1
#define QUICKUSB_MODEL                          2
#define QUICKUSB_SERIAL                         3


// QuickUSB settings
#define SETTING_EP26CONFIG                      0
#define SETTING_WORDWIDE                        1
#define SETTING_DATAADDRESS                     2
#define SETTING_FIFO_CONFIG                     3
#define SETTING_FPGATYPE                        4
#define SETTING_CPUCONFIG                       5
#define SETTING_SPICONFIG                       6
#define SETTING_SLAVEFIFOFLAGS                  7
#define SETTING_I2CTL                           8
#define SETTING_PORTA                           9
#define SETTING_PORTB                           10
#define SETTING_PORTC                           11
#define SETTING_PORTD                           12
#define SETTING_PORTE                           13
#define SETTING_PORTACCFG                       14
#define SETTING_PINFLAGS                        15
#define SETTING_VERSIONBUILD                    16
#define SETTING_VERSIONSPEED                    17

// FPGA type settings
#define FPGATYPE_ALTERAPS                       0
#define FPGATYPE_XILINXSS                       1

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB Error Codes
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define QUICKUSB_ERROR_NO_ERROR                 0
#define QUICKUSB_ERROR_OUT_OF_MEMORY            1     
#define QUICKUSB_ERROR_CANNOT_OPEN_MODULE       2
#define QUICKUSB_ERROR_CANNOT_FIND_DEVICE       3
#define QUICKUSB_ERROR_IOCTL_FAILED             4
#define QUICKUSB_ERROR_INVALID_PARAMETER        5
#define QUICKUSB_ERROR_TIMEOUT                  6
#define QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED   7
#define QUICKUSB_ERROR_I2C_BUS_ERROR            8
#define QUICKUSB_ERROR_I2C_NO_ACK               9
#define QUICKUSB_ERROR_I2C_SLAVE_WAIT           10
#define QUICKUSB_ERROR_I2C_TIMEOUT              11
#define QUICKUSB_ERROR_UNKNOWN_DRIVER_TYPE      12

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  QuickUSB parameter type defintions
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
#define QBYTE	unsigned char
#define PQBYTE	*QBYTE
#define QCHAR	char
#define PQCHAR	*QCHAR
#define QWORD   unsigned short
#define PQWORD	*QWORD
#define QLONG	signed long
#define PQLONG	*QLONG
#define QULONG	unsigned long
#define PQULONG *QULONG
#ifdef _WIN32
#define QHANDLE	QLONG
#elif defined __linux__
#define QHANDLE usb_dev_handle *
#endif
#define PQHANDLE *QHANDLE
#define QRESULT	QLONG
#ifdef _WIN32
#define QRETURN QRESULT WINAPI
#elif defined __linux__
#define QRETURN QLONG
#endif

/*- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
  External function declarations
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -*/
// General purpose calls
extern QRETURN QuickUsbGetLastError(QULONG *error);
extern QRETURN QuickUsbFindModules(QCHAR *nameList,
                                   QULONG length);
extern QRETURN QuickUsbOpen(QHANDLE *hDevice, 
                            QCHAR *deviceName);

extern QRETURN QuickUsbClose(QHANDLE hDevice);

extern QRETURN QuickUsbGetStringDescriptor(QHANDLE hDevice, 
                                           QBYTE index,
                                           QCHAR *buffer, 
                                           QWORD length);

extern QRETURN QuickUsbReadSetting(QHANDLE hDevice, 
                                   QWORD address, 
                                   QWORD *value);
extern QRETURN QuickUsbWriteSetting(QHANDLE hDevice,
                                    QWORD address, 
                                    QWORD value);

extern QRETURN QuickUsbStartFpgaConfiguration(QHANDLE hDevice);

extern QRETURN QuickUsbWriteFpgaData(QHANDLE hDevice, 
                                     QBYTE *fpgadata, 
                                     QULONG datalength);

extern QRETURN QuickUsbIsFpgaConfigured(QHANDLE hDevice, 
              	                        QWORD *isConfigured);


extern QRETURN QuickUsbReadCommand(QHANDLE hDevice, 
                                   QWORD address, 
                                   QBYTE *data, 
                                   QWORD *length);
extern QRETURN QuickUsbWriteCommand(QHANDLE hDevice, 
                                    QWORD address, 
                                    QBYTE *data, 
                                    QWORD length);


extern QRETURN QuickUsbReadData(QHANDLE hDevice, 
                                QBYTE *data, 
                                QULONG *length);

extern QRETURN QuickUsbWriteData(QHANDLE hDevice, 
                                 QBYTE *data, 
                                 QULONG length);

extern QRETURN QuickUsbReadPortDir(QHANDLE hDevice, 
                                   QWORD address, 
                                   QBYTE *data);

extern QRETURN QuickUsbWritePortDir(QHANDLE hDevice, 
                                    QWORD address, 
                                    QBYTE data);

extern QRETURN QuickUsbReadPort(QHANDLE hDevice, 
                                QWORD address, 
                                QBYTE *data, 
                                QWORD *length);

extern QRETURN QuickUsbWritePort(QHANDLE hDevice, 
                                 QWORD address, 
                                 QBYTE *data, 
                                 QWORD length);


extern QRETURN QuickUsbSetRS232BaudRate(QHANDLE hDevice, 
                                        QULONG baudRate);

extern QRETURN QuickUsbGetNumRS232(QHANDLE hDevice, 
                                   QBYTE portNum, 
                                   QWORD *length);

extern QRETURN QuickUsbFlushRS232(QHANDLE hDevice, 
                                  QBYTE portNum);

extern QRETURN QuickUsbReadRS232(QHANDLE hDevice, 
                                 QBYTE portNum, 
                                 QBYTE *data, 
                                 QWORD *length);

extern QRETURN QuickUsbWriteRS232(QHANDLE hDevice, 
                                  QBYTE portNum, 
                                  QBYTE *data, 
                                  QWORD length);

extern QRETURN QuickUsbReadSpi(QHANDLE hDevice, 
                               QBYTE portNum,
                               QBYTE *data, 
                               QWORD *length);

extern QRETURN QuickUsbWriteSpi(QHANDLE hDevice, 
                                QBYTE portNum,
                                QBYTE *data, 
                                QWORD length);

extern QRETURN QuickUsbWriteReadSpi(QHANDLE hDevice, 
                                    QBYTE portNum, 
                                    QBYTE *data, 
                                    QWORD length);

extern QRETURN QuickUsbReadI2C(QHANDLE hDevice, 
                               QWORD address, 
                               QBYTE *data, 
                               QWORD *length);

extern QRETURN QuickUsbWriteI2C(QHANDLE hDevice, 
                                QWORD address, 
                                QBYTE *data,
                                QWORD length);

extern QRETURN QuickUsbSetTimeout(QHANDLE hDevice,
                                  QULONG timeOut);

extern QRETURN QuickUsbGetDriverVersion(QWORD * MajorDriverVersion,
                                        QWORD * MinorDriverVersion,
                                        QWORD * BuildDriverVersion);

extern QRETURN QuickUsbGetDllVersion(QWORD * MajorDllVersion,
                                     QWORD * MinorDllVersion,
                                     QWORD * BuildDllVersion);

extern QRETURN QuickUsbGetFirmwareVersion(QHANDLE hDevice, 
                                          QWORD * MajorFirmwareVersion,
                                          QWORD * MinorFirmwareVersion,
                                          QWORD * BuildFirmwareVersion);

extern QRETURN QuickUsbWriteDataAsync(QHANDLE hDevice, 
                                      QBYTE *data, 
                                      QULONG length, 
                                      QBYTE *transaction);

extern QRETURN QuickUsbReadDataAsync(QHANDLE hDevice, 
                                     QBYTE *data, 
                                     QULONG *length, 
                                     QBYTE *transaction);

extern QRETURN QuickUsbAsyncWait(QHANDLE hDevice, 
                                 QULONG *bytecount,
                                 QBYTE transaction,
                                 QBYTE immediate);

#ifndef __linux__
extern QRETURN QuickUsbReadDefault(QHANDLE hDevice,
                                   QWORD address, 
                                   QWORD *data);

extern QRETURN QuickUsbWriteDefault(QHANDLE hDevice,
                                    QWORD address, 
                                    QWORD data);

extern QRETURN QuickUsbReadStorage(QHANDLE hDevice, 
                               QWORD address, 
                               QBYTE *data, 
                               QWORD bytes);

extern QRETURN QuickUsbWriteStorage(QHANDLE hDevice, 
                                QWORD address, 
                                QBYTE *data,
                                QWORD bytes);
#endif

#ifdef __cplusplus
}
#endif
