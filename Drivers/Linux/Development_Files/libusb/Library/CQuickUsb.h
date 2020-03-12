/*=============================================================================
 Title        : CQuickUSB.h
 Description  : QuickUSB Class Header File
 Notes        : None

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
#include <time.h>
#include <fstream>
#include "QuickUsb.h"

#define NO_PREVIOUS_ERROR 0
#define QUSB_ERROR_NOT_OPEN 1
#define QUSB_ERROR_ALREADY_OPEN 2
#define QUSB_ERROR_COULD_NOT_OPEN 3
#define QUSB_TEMP_OPEN_FAILED 4
#define QUSB_COULD_NOT_CLOSE 5

#ifdef  __WIN32__ 
#include <windows.h>
#include <vcl.h>
#define QUSB_ERROR OutputDebugString;
#endif


#ifdef  __linux__
#include <string>
#include <iostream>
#define AnsiString string
#define QUSB_ERROR(str) cerr << str;
#define FALSE 0
#define TRUE 1
#endif

using namespace std;

class CQuickUsb {
public:
   CQuickUsb(const char *devName, unsigned short length);
   ~CQuickUsb();
   int Open(void);
   int Close(void);
   bool IsOpen();
   int GetLastError();
   int GetStringDescriptor(unsigned char index, char * &data); 
   char * GetDeviceName();
   int SetDeviceName(char * newDevName, unsigned short length);
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
   int ReadTimerReg(unsigned short address, unsigned short *data);
   int WriteTimerReg(unsigned short address, unsigned short data);
   int ReadTimerIsrRegs(unsigned short *data, unsigned short *length);
   int UnitTest(void);

   // Static methods
   static int GetNumOfModules(char * nameList);
   static int GetModuleName(char * nameList, unsigned char index, char *&name);
   static int GetNameListLength(char * nameList);
   static int FindModules(char *nameList, unsigned long length);

private:
   char * m_devName;
   HANDLE hDevice;
   bool isOpen;     // False = closed  true = open used for user
   bool tempOpen;
   int result;
   int lastError;
};
