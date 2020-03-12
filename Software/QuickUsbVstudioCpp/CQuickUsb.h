/*=============================================================================
 Title        : CQuickUSB.h
 Description  : QuickUSB Class Header File
 Notes        : None

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

 $ History $

=============================================================================*/
#include <time.h>
//#include <fstream>
#include "QuickUsb.h"

#define NO_PREVIOUS_ERROR 0
#define QUSB_ERROR_NOT_OPEN 1
#define QUSB_ERROR_ALREADY_OPEN 2
#define QUSB_ERROR_COULD_NOT_OPEN 3
#define QUSB_TEMP_OPEN_FAILED 4
#define QUSB_COULD_NOT_CLOSE 5


#ifdef  WIN32
#include <windows.h>
//#include <vcl.h>
#define QUSB_ERROR(str) OutputDebugString(str);
#endif


#ifdef  __linux__
#include <string>
#include <iostream>
#define AnsiString string
#define QUSB_ERROR(str) cerr << str;
#define FALSE 0
#define TRUE 1
#endif

//using namespace std;

class CQuickUsb {
public:
   CQuickUsb();
   CQuickUsb(const char *devName);
   ~CQuickUsb();
   int Open(void);
   int Close(void);
   bool IsOpen();
   int GetLastError();
   int GetStringDescriptor(unsigned char index, char *str, unsigned short maxlen); 
   char *GetDeviceName();
   int SetDeviceName(char * newDevName);
   int ReadSetting(unsigned short address, unsigned short *value);
   int WriteSetting(unsigned short address, unsigned short value);
   int StartFpgaConfiguration(void);
   int WriteFpgaData(unsigned char *fpgadata, unsigned long datalength);
   int IsFpgaConfigured(unsigned short  *isConfigured);
   int WriteCommand(unsigned short address, unsigned char *data, unsigned short length);
   int ReadCommand(unsigned short address, unsigned char *data, unsigned short *length);
   int WriteData(unsigned char *data, unsigned long length);
   int ReadData(unsigned char *data, unsigned long *length);
   int ReadPortDir(unsigned short address, unsigned char *data);
   int WritePortDir(unsigned short address, unsigned char data);
   int ReadPort(unsigned short address, unsigned char *data, unsigned short *length);
   int WritePort(unsigned short address, unsigned char *data, unsigned short length);
   int SetRS232BaudRate(unsigned long baudRate);
   int ReadRS232(unsigned char portNum, unsigned char *data, unsigned short *length);
   int WriteRS232(unsigned char portNum, unsigned char *data, unsigned short length);
   int FlushRS232(unsigned char portNum);
   int GetNumRS232(unsigned char portNum, unsigned short *length);
   int ReadI2C(unsigned short address, unsigned char *data, unsigned short *length);
   int WriteI2C(unsigned short address, unsigned char *data, unsigned short length);
   int ReadSpi(unsigned char portNum, unsigned char *data, unsigned short *length);
   int WriteSpi(unsigned char portNum, unsigned char *data, unsigned short length);
   int WriteReadSpi(unsigned char portNum, unsigned char *data, unsigned short length);
   int UploadFpga(char * filename);
   int UnitTest(void);

   // Static methods
   static int GetNumModules(char * nameList);
   static int GetModuleName(char * nameList, unsigned char index, char *&name);
   static int GetNameListLength(char * nameList);
   static int FindModules(char *nameList, unsigned long length);

private:
   int SetLastErrorString(char *errorStr);
};
