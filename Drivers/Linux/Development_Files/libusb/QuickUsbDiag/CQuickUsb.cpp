/*=============================================================================
 Title        : CQuickUSB.cpp
 Description  : QuickUSB Class
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
#include "CQuickUsb.h"


using namespace std;

// Constructor to open a specific QuickUsb device
CQuickUsb::CQuickUsb(const char * devName, unsigned short length){
   isOpen = false;
   tempOpen = false;
   m_devName = new char[length+1];
   strcpy(m_devName, devName);
   lastError = NO_PREVIOUS_ERROR;
}

int CQuickUsb::SetDeviceName(char * newDevName, unsigned short length){

  // if the current object is already open then force a close
  if(isOpen){
     result = Close();
     if(result == 0){
       lastError = QUSB_COULD_NOT_CLOSE;
       cerr << "Could not close device\n";
       return FALSE;
     }
  }

  delete [] m_devName;
  m_devName = new char[length+1];
  strcpy(m_devName, newDevName);
  isOpen = false;
  tempOpen = false;
  lastError = NO_PREVIOUS_ERROR;
  return TRUE;

}



CQuickUsb::~CQuickUsb(){
  if (tempOpen || isOpen) {
    Close();
  }
  delete [] m_devName;
}

bool CQuickUsb::IsOpen(){
    return isOpen;
}


int CQuickUsb::GetLastError(){
   return lastError;
}


char * CQuickUsb::GetDeviceName(){
   return m_devName;
}



int CQuickUsb::Open(void){
   // Open the device if it isn't already open
   if (!isOpen) {
      if (QuickUsbOpen(&hDevice, m_devName)) {
         isOpen = true;
         return TRUE;
      }
      lastError = QUSB_ERROR_COULD_NOT_OPEN;
      cerr << "Could not open device\n";
      return FALSE;
   }
   lastError = QUSB_ERROR_ALREADY_OPEN;
   cerr << "Device already open\n";
   return FALSE;
}


int CQuickUsb::Close(void){
   // Open the device if it isn't already open
   if (tempOpen || isOpen) {
     if (QuickUsbClose(hDevice)) {
         isOpen = false;
         tempOpen = false;
         return TRUE;
      }
      cerr << "Could not close device";
      return FALSE;
   }
   lastError = QUSB_ERROR_NOT_OPEN;
   cerr << "Device not open\n";
   return FALSE;
}


int CQuickUsb::GetStringDescriptor(unsigned char index, char * &data ) {      
   char strbuf[256];
 
   // Open the device if it isn't already open
   if (!isOpen){
      tempOpen = true;
      result = Open();
      if (result == FALSE){
         Close();
	 lastError = QUSB_TEMP_OPEN_FAILED;
	 cerr << "Temp open failed";
         return FALSE;
      }
   }

   // Get the string descriptor
   result = QuickUsbGetStringDescriptor(hDevice, index, strbuf, 256);
   if (result == FALSE) {
      return FALSE;
   }

   // Print the device descriptor string into the string
   data = new char[ strlen(strbuf) + 1];
   strcpy(data, strbuf);

   // Close the device if we opened it temporarily
   if (tempOpen) {
      Close();
   }
   return TRUE;
}


int CQuickUsb::ReadSetting(unsigned short address, unsigned short *value) {
   // Open the device if it isn't already open
   if (!isOpen) {
      tempOpen = true;
      result = Open();
      if (result == FALSE) {
         Close();
         lastError = QUSB_TEMP_OPEN_FAILED;
	      cerr << "Temp open failed\n";
         return FALSE;
      }
   }
   result = QuickUsbReadSetting(hDevice,address, value);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::WriteSetting(unsigned short address, unsigned short value) {
   // Open the device if it isn't already open
   if (!isOpen){
      tempOpen = true;
      result = Open();
      if (result == FALSE) {
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWriteSetting(hDevice,address, value);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::StartFpgaConfiguration(void) {
   // Open the device if it isn't already open
   if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	cerr << "Temp open failed\n";
        return FALSE;
      }
    }
   result = QuickUsbStartFpgaConfiguration(hDevice);

   if (tempOpen) {
      Close();
   }

    return result;
}


int CQuickUsb::WriteFpgaData(unsigned char *fpgadata, unsigned long datalength) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }
   result = QuickUsbWriteFpgaData(hDevice, fpgadata, datalength);

   if (tempOpen) {
      Close();
   }

    return result;
}


int CQuickUsb::IsFpgaConfigured(unsigned short *isConfigured) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbIsFpgaConfigured(hDevice, isConfigured);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::WriteCommand(unsigned short address, unsigned char *data, unsigned short length) {
   if(!isOpen){
     tempOpen = true;
     result = Open();
     if(result == FALSE){
       Close();
       lastError = QUSB_TEMP_OPEN_FAILED;
       cerr << "Temp open failed\n";
       return FALSE;
     }
   }
   result = QuickUsbWriteCommand(hDevice, address, data, length);

   if (tempOpen) {
     Close();
   }

   return result;
}


int CQuickUsb::ReadCommand(unsigned short address, unsigned char *data, unsigned short *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadCommand(hDevice, address, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WriteData(unsigned char *data, unsigned long length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWriteData(hDevice, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadData(unsigned char *data, unsigned long *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

    result = QuickUsbReadData(hDevice, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadPortDir(unsigned short address, unsigned char *data) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadPortDir(hDevice, address, data);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WritePortDir(unsigned short address, unsigned char data) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWritePortDir(hDevice, address, data);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadPort(unsigned short address, unsigned char *data, unsigned short *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadPort(hDevice, address, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WritePort(unsigned short address, unsigned char *data, unsigned short length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWritePort(hDevice, address, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}

int CQuickUsb::SetRS232BaudRate(unsigned long baudRate) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbSetRS232BaudRate(hDevice, baudRate);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadRS232(unsigned char portNum, unsigned char *data, unsigned short *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadRS232(hDevice, portNum, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WriteRS232(unsigned char portNum, unsigned char *data, unsigned short length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

    result = QuickUsbWriteRS232(hDevice, portNum, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}

int CQuickUsb::FlushRS232(unsigned char portNum){
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbFlushRS232(hDevice, portNum);

   if (tempOpen) {
      Close();
   }
   return result;
}

int CQuickUsb::GetNumRS232(unsigned char portNum, unsigned short *length){
   if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbGetNumRS232(hDevice, portNum, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadI2C(unsigned short address, unsigned char *data, unsigned short *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadI2C(hDevice, address, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WriteI2C(unsigned short address, unsigned char *data, unsigned short length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

    result = QuickUsbWriteI2C(hDevice, address, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::ReadSpi(unsigned char portNum, unsigned char *data, unsigned short *length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadSpi(hDevice, portNum, data, length);

   if (tempOpen) {
      Close();
   }
   return result;
}


int CQuickUsb::WriteSpi(unsigned char portNum, unsigned char *data, unsigned short length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
        cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWriteSpi(hDevice, portNum, data, length);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::WriteReadSpi(unsigned char portNum, unsigned  char *data, unsigned short length) {
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWriteReadSpi(hDevice, portNum, data, length);

   if (tempOpen) {
      Close();
   }

   return result;
}

int CQuickUsb::ReadTimerReg(unsigned short address, unsigned short *data){
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadTimerReg(hDevice, address, data);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::WriteTimerReg(unsigned short address, unsigned short data){
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbWriteTimerReg(hDevice, address, data);

   if (tempOpen) {
      Close();
   }

   return result;
}


int CQuickUsb::ReadTimerIsrRegs(unsigned short *data, unsigned short *length){
    if(!isOpen){
      tempOpen = true;
      result = Open();
      if(result == FALSE){
        Close();
        lastError = QUSB_TEMP_OPEN_FAILED;
	     cerr << "Temp open failed\n";
        return FALSE;
      }
    }

   result = QuickUsbReadTimerIsrRegs(hDevice, data, length);

   if (tempOpen) {
      Close();
   }

   return result;
}


//-------------------------Static Methods---------------------------------------

int CQuickUsb::GetNumOfModules(char * nameList){
   if(nameList == NULL){
      return 0;
   }
   char * str = nameList;
   int count = 0;

   while (*str != '\0') {
      str += strlen(str) + 1;
      count++;
   }
   return count;
}

//----------------------------------------------------------------------------


int CQuickUsb::GetNameListLength(char * nameList){
   if(nameList == NULL){
      return 0;
   }
   int count = 0;
   char * str = nameList;
   count += strlen(str) + 1;

   while (*str != '\0') {
      str += strlen(str) + 1;
      count += strlen(str) + 1;
   }

   return count;
}

//----------------------------------------------------------------------------

int CQuickUsb::GetModuleName( char * nameList, unsigned char index, char *&name ) {
   int numOfItems;
   char * str;

   if(nameList == NULL){
      return 0;
   }

   numOfItems = GetNumOfModules(nameList);
   // if no items have been found or index > then items found, return an error flag
   if(numOfItems == 0 || index >= numOfItems ) {
      return FALSE;
   }

   str = nameList;
   for(int i = 0; i < index; i++){
      str += strlen(str) + 1;
   }

   name = str;
   return TRUE;
}

//----------------------------------------------------------------------------

int CQuickUsb::FindModules(char *nameList, unsigned long length) {
   return QuickUsbFindModules(nameList, length);
}

//----------------------------------------------------------------------------

#ifdef QuickUSB_TEST
// =================================================================
// This method will check all functionality of the QuickUSB module.
// Module will need to be connected to the evaluation board with
// firmare version "v1.30rc5.iic" and above.
// To use the test method simply call the UnitTest function
// File "memory.rbf" must be in present working directory
// This will only work for Windows platform machines
// RS232 ports need to be connected to eachother on the evaluation board
// =================================================================

int CQuickUsb::UnitTest(){

   int index;
   unsigned short isConfigured, length;
   unsigned char testData[64], readData[64], outdata[1] , indata[1];

   // ============================
   // Configuring FPGA and testing   *******PASSED*********
   // ============================

   // Upload the file to the FPGA if its not configured already
   result = IsFpgaConfigured(&isConfigured);

   if(isConfigured == 0){
      int blockLength = 64, fileSize = 0,length, index; //progress = 0
      char fpgaData[64];
      index = 0;
      length = 0;
      ifstream input;

      // Open the input file for reading
      input.open( "memory.rbf", ifstream::binary);

      if(input.fail() ){
      	QUSB_ERROR("Failed to open memory.rbf. Make sure its in the present directory");
      }

      input.seekg(0,ifstream::end);
      fileSize=input.tellg();
      input.seekg(0, ifstream::beg);

      // Open the device
      result = Open();
      if(result == 0){
         QUSB_ERROR("Cannot open Device:" +  *m_devName );
         return FALSE;
      }

      // Start configuring the FPGA
      result = StartFpgaConfiguration();
      if(result == 0){
	      QUSB_ERROR("Cannot start FPGA configuration");
	      input.close();
         Close();
         return FALSE;
      }

      // Send the FPGA configuration data one block at a time

      do {
         // Read the data
         length = min(blockLength, fileSize - index);

         // Read the data from the file and put it into fpgaData array
         input.read(fpgaData,length);

         // Write the data to the EEPROM
         result = WriteFpgaData(fpgaData, length);

         if(result == 0){
            input.close();
	         QUSB_ERROR("FPGA configuration failed");
            return FALSE;
         }

         // Update the counters
         index = index + length;

      }
      while(index < fileSize);

      // Start configuring the FPGA
      result = IsFpgaConfigured(&isConfigured);

      // Close the device when you're done
      result = Close();
      if(result == 0){
         QUSB_ERROR("Cannot close device:" + *m_devName);
         return FALSE;
      }

      // Close the file
      input.close();

      // Notify the user
      if(isConfigured == 0){
	      QUSB_ERROR("FPGA configuration failed");
         return FALSE;
      }
      else{
         QUSB_ERROR("FPGA was successfully configured");
      }
   }

  // ==============================================
  // Testing ReadSetting and WriteSetting functions     *******PASSED*********
  // ==============================================

   unsigned short int value, memory;

   result = ReadSetting(SETTING_WORDWIDE, &value);
   if(result == 0){
      QUSB_ERROR("ReadSettings failed");
      return FALSE;
   }

   memory = value;
   // If high speed port data width is 16 bits, change it to 8 bits
   if( value == 1 ){
     result =  WriteSetting(SETTING_WORDWIDE, 0);
      if(result == 0){
         QUSB_ERROR("WriteSetting failed");
         return FALSE;
      }
   }
   // If high speed port data width is 8 bits, change it to 16 bits
   else if(value == 0){
   result =  WriteSetting(SETTING_WORDWIDE, 1);
      if(result == 0){
         QUSB_ERROR("WriteSetting failed");
         return FALSE;
      }
   }
   // Else an unidentified number was written to valule
   else{
      QUSB_ERROR("value returned from ReadSettings contained an unidentified value");
      return FALSE;
   }

   value = 445566;
   // Now ReadSettings again to see if writeSettings actually changed the value
   result = ReadSetting(SETTING_WORDWIDE, &value);
   if(result == 0){
      QUSB_ERROR("ReadSettings second attempt failed");
      return FALSE;
   }

   if(value == memory){
      QUSB_ERROR("WriteSettings did not change the settings");
      return FALSE;
   }

  // ====================================
  // Testing WriteCommand and ReadCommand       ********PASSED*********
  // =====================================

  unsigned short temp, address;  
  srand( time(NULL) );
  address = rand() % 512;

  for(int i=0; i < 64; i++){
     testData[i] = rand() % 127;
  }

   // Test a block transfer
   temp = 64;
   result = WriteCommand( address, (char *) testData, temp);
   if(result == 0){
      QUSB_ERROR( "WriteCommand failed" );
      return FALSE;
   }

   result = ReadCommand( address, (char *) readData, &temp );
   if(result == 0){
      QUSB_ERROR("Cannot read a command block" );
      return FALSE;
   }

   // Verify that we got the same values back

   for(index = 0; index < 64; index++){
      if(readData[index] != testData[index] ){
         QUSB_ERROR("Didn't get same values in ReadCommand" );
         return FALSE;
      }
   }

   // ===============================
   // Testing WriteData and ReadData    *******PASSED*********
   // ===============================

   char writeDataArray[512];
   char readDataArray[512];
   unsigned long writeReadDataLength = 512, temp2 = 512;

   for(int i=0; i < 512; i++){
     writeDataArray[i] = rand() % 127;
   }

   result = WriteData(writeDataArray, writeReadDataLength);
   if(result == 0){
      QUSB_ERROR("WriteData failed");
      return FALSE;
   }

   result = ReadData(readDataArray, &temp2);
   if(result == 0){
      QUSB_ERROR("ReadData failed");
      return FALSE;
   }

   if(temp2 != writeReadDataLength){
      QUSB_ERROR("did not read the same length written");
      return FALSE;
   }

   for(int i = 0; i < 512; i++){
    if( writeDataArray[i] != readDataArray[i] ){
         QUSB_ERROR("writeData and readData arrays do not match" );
         return FALSE;
      }
   }



   // ====================================
   // Testing WritePortDir and ReadPortDir    ********PASSED**********
   // =====================================

   unsigned char bitsIn, bitsOut;
   // for all port combination
     for(int i = 0; i < 4; i++){
       for(int j = 0; j < 16; j++){
         bitsOut = j;

         result = WritePortDir(i, bitsOut);
         if(result == 0){
           QUSB_ERROR("writePortDir failed");
           return FALSE;
         }

         // now read the data to see if it matches
         result = ReadPortDir(i, &bitsIn);
         if(result == 0){
           QUSB_ERROR("ReadPortDir failed");
           return FALSE;
         }

         if(bitsIn != bitsOut){
            QUSB_ERROR("ReadPortDir bits did not match with writePortDir bits");
            return FALSE;
        }
      }
     }

  // ===============================
  // Testing WritePort and ReadPort    ********Passed***********
  // ===============================

   // For ports A-E
   for(int i = 0; i < 5; i++){
      //make the port writable
      result = WritePortDir(i, 0xFF);
         if(result == 0){
           QUSB_ERROR("writePortDir failed");
           return FALSE;
         }

      outdata[0] = rand() % 256;

      // Write
      result = WritePort(i, outdata, 1);
      if(result == 0){
         QUSB_ERROR("WritePort failed");
         return FALSE;
      }

      temp = 1;
      result = ReadPort(i, indata, &temp);
      if(result == 0){
         QUSB_ERROR("ReadPort failed");
         return FALSE;
      }

      if(temp != 2){
         QUSB_ERROR( "ReadPort did not read the correct number of bytes");
         return FALSE;
      }

      if(outdata[0] != indata[0] ){
           QUSB_ERROR( "Data sent out and aquired does not mach for ReadPort and WritePort functions");
           return FALSE;
      }
   }

   // This method needs to be tested with a terminal type program
   // Make sure there is an RS232 cable connected to Port one on the
   // evaluation board, which is then connected to the computer
   // =========================
   // Testing  SetRS232BaudRate               ************PASSED****************
   // =========================

   char ourProduct[] = "Our main product is the QuickUSB module";
   temp = ( (string) ourProduct).length() + 1; //+1 for \0 character

   result = SetRS232BaudRate(38400);
   QUSB_ERROR("RS232 Baud Rate set to 38400");
   if(result == 0){
     QUSB_ERROR("SetRS2323BaudRate failed");
     return FALSE;
   }

   result = WriteRS232(0, ourProduct, temp );
   if(result == 0){
     QUSB_ERROR("WriteRS232 port 0 failed");
     return FALSE;

   }

   // This method needs to be tested with a loop back cable
   // ===============================
   // Testing  ReadRS232   WriteRS232         ************PASSED****************
   // ===============================

   // Flush the ports to initialize test
   result = FlushRS232(0);
   if(result == 0){
      QUSB_ERROR("FlushRS232 port 0 failed");
      return FALSE;
   }

   result = FlushRS232(1);
   if(result == 0){
      QUSB_ERROR("FlushRS232 port 1 failed");
      return FALSE;
   }

   char welcome[] =  "Welcome to BitWise Systems";  //length is 26 + \0 chracter= 27
   char testOne [27];
   char testTwo [40];
   temp = ( (string) welcome).length() + 1; //+1 for \0 character

   result = WriteRS232(0, welcome , temp );
   if(result == 0){
      QUSB_ERROR("WriteRS232 port 0 failed");
      return FALSE;

   }

   result = ReadRS232( 1, testOne, &temp );
   if(result == 0){
      QUSB_ERROR("ReadRS232 port 1 failed");
      return FALSE;

   }

  if(welcome != (string) testOne){
     QUSB_ERROR("String written to and recieved from RS232 does not match on port 0 to port 1");
     return FALSE;
  }

   temp = ( (string) ourProduct).length() + 1; //+1 for \0 character

   result = WriteRS232(1, ourProduct, temp );
   if(result == 0){
      QUSB_ERROR("WriteRS232 port 1 failed");
      return FALSE;

   }
   result = ReadRS232( 0, testTwo, &temp );
   if(result == 0){
      QUSB_ERROR("ReadRS232 port 0 failed");
      return FALSE;

   }

   if(ourProduct != (string) testTwo){
      QUSB_ERROR("String written to and recieved from RS232 does not match on port 1 to port 0");
      return FALSE;
   }


   // ==================================
   // Testing  RS232Flush   Rs232GetNum   **************PASSED***************
   // ===================================

    // Using welcome char string from above test
    temp = ( (string) welcome).length() + 1; //+1 for \0 character
    result = WriteRS232(0, welcome, temp );
    if(result == 0){
      QUSB_ERROR("WriteRS232 port 0 failed");
      return FALSE;
   }

   length = 0;
   // Now checking RS232 port 1 to see how long the string was
   result = GetNumRS232(1, &length);
   if(result == 0){
      QUSB_ERROR("GetNumRS232 port 1 failed");
      return FALSE;
   }

   if(length != 27){
      QUSB_ERROR("String written to and recieved from RS232 had different length RS232GetNum failed");
      return FALSE;
   }

   result = FlushRS232(1);
   if(result == 0){
      QUSB_ERROR("FlushRS232 port 1 failed");
      return FALSE;
   }

   // Length should now be 0 or flush didn't work
   result = GetNumRS232(1, &length);
   if(result == 0){
      QUSB_ERROR("GetNumRS232 port 1 failed");
      return FALSE;
   }

   if(length != 0){
      QUSB_ERROR("RS232Flush Failed");
      return FALSE;
   }


   // ==============================
   // Testing  WriteSpi and ReadSpi    ***************PASSED*****************
   // ==============================

   char outData[2] = { 0x12, 0x34 };
   char inData[2];
   char inAndOutData[2];

   temp = 2;
   result = WriteSpi(0, outData, temp);
   if(result == 0){
      QUSB_ERROR("Write SPI failed");
      return FALSE;

   }

   result = ReadSpi(0, inData, &temp);
   if(result == 0){
      QUSB_ERROR("Read SPI failed");
      return FALSE;

   }

   // If the arrays do not match
   if( !( (outData[0] == inData[0]) && (outData[1] == inData[1]) ) ){
       QUSB_ERROR("outData and inData arrays do not match in WriteSpi and ReadSpi test" );
       return false;
   }


    // Get rid of old values
   outData[0] = 0x66;
   outData[1] = 0x39;
   // Filling up the buffer

   WriteSpi(0, outData, temp);

   // Now send new values and retrieve old values
   WriteReadSpi(1, inAndOutData, temp);
   if(result == 0){
      QUSB_ERROR("ReadWrite SPI failed");
      return FALSE;

   }

   // InData should match with the old outData values which are 0x12 0x34
   if( !( (inAndOutData[0]== 0x66) && (0x39 == inAndOutData[1]) ) ) {
       QUSB_ERROR("wrong values in array for WriteReadSpi test" );
       return false;
   }
   return 1;
}
#endif




