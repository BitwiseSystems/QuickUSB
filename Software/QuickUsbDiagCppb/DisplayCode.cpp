/*=============================================================================
 Title        : DisplayCode.cpp
 Description  : GUI Unit for QuickUsbDiagCppb
 Notes        : None

 Copyright © 2012 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

   Use, duplication or disclosure by the U.S. Government is subject to
   restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
   (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, 
   Suite E, Goleta, CA  93117.

   Bitwise Systems
   6489 Calle Real, Suite E
   Goleta, CA  93117
   Voice: (805) 683-6469
   Fax  : (805) 683-4833
   Web  : www.bitwisesys.com
   email: support@bitwisesys.com

 $History: DisplayCode.cpp $

=============================================================================*/
//---------------------------------------------------------------------------
#include <fstream>
#include <vcl.h>
#pragma hdrstop

#include "CQuickUsb.h"
#include "OutputTextBox.h"
#include "AboutForm.h"
#include <Registry.hpp>


#define SETTING_WORDWIDE 1
#define SETTING_DATAADDRESS 2

#define MEGA (1024 * 1024)

// This data structure is used by the async read and write routines.
typedef struct {
   unsigned long     bytesTransferred;    /* bytes actually transferred */
   unsigned char     transaction;         /* I/O transaction number */
   unsigned char *   inData;              /* buffer pointer */
} MemBlockType;


//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "BitCheckBox"
#pragma resource "*.dfm"

using namespace std;

#include "DisplayCode.h"

TDisplay *Display;
String oldItem = "no Item";
AnsiString devName;
TButton * lastButton = NULL;
TStrings * oldList;
CQuickUsb *qusb = NULL;


//---------------------------------------------------------------------------
__fastcall TDisplay::TDisplay(TComponent* Owner) : TForm(Owner) {
   AnsiString setting;
   bool wordWide;
   TRegistry *Registry = new TRegistry;
   FormOutputTextBox = new TFormOutputTextBox(Owner);




   // Select the first item if the list isn't empty
   if (ModuleComboBox->Items->Count != 0) {
      ModuleComboBox->ItemIndex = 0;
	  ModuleComboBoxClick(Owner);


      // Update the port states
	  ReadPortADirBits();
	  ReadPortCDirBits();
	  ReadPortEDirBits();
	  ReadPortAButtonClick(Owner);
	  ReadPortCButtonClick(Owner);
	  ReadPortEButtonClick(Owner);

	  UpdateWordWideCheckBox(Owner);

      // Disable timers depending on the firmware version
      if (GetFirmwareVersionString() >= 2) {
        // (PageControl->Pages[6])->Enabled = true;
        // TimerReadAllButton->Click();
      }
      else {
         // Disable the tab
         //(PageControl->Pages[6])->Enabled = false;
         //PageControl->Pages[6]->Font->Color = clInactiveCaption;
      }
   }


   // Set defaults
   RS232PortComboBox->ItemIndex = 0;
   RS232BaudRateComboBox->ItemIndex = 3;
   SpiReadPortNumComboBox->ItemIndex = 0;
   SpiWritePortNumComboBox->ItemIndex = 0;


   // Set text box defaults
   CommandLengthTextBox->Text = "8";
   DataLengthTextBox->Text = "65536";

   // Fill the grid with zeroes
   commandLength = 8;
   CommandFlexGrid->RowCount = 8;

   for (int i = 0; i < CommandFlexGrid->RowCount; i++) {
      CommandFlexGrid->Cells[1][i] = 0;
      CommandFlexGrid->Cells[0][i] = IntToStr(i+1);
   }


   // Read in settings form the Windows registry
   try {
      // Attempt to open the registry key for QuickUsb diag, if it doesn't exist create it
      Registry->OpenKey("\\Software\\BitwiseSys\\QuickUsb Diag",true);


      // look up which tab the program used last;
      setting = Registry->ReadString("Tab");
      if ( setting != "" ) {
         PageControl->ActivePageIndex = setting.ToInt();
      }

      // Look up the last file uploaded
      setting = Registry->ReadString("UploadFileName");
      if ( setting != "" ) {
         UploadFilenameTextBox->Text = setting;
      }

      // Look up the last repeat interval
      setting = Registry->ReadString("Interval");
      if ( setting != "" ) {
         RepeatTextBox->Text = setting;
      }

      // Get the last state of the wordWide check box
      wordWide = Registry->ReadBool("wordWide");
      if ( setting != "" ) {
         WordWideCheckBox->Checked = wordWide;
      }
    }

    catch ( const Exception &e ) {
       displayString ( "Registry error" );
    }

    delete Registry;


}


//---------------------------------------------------------------------------

void __fastcall TDisplay::PageControlChange(TObject *Sender) {
   if (PageControl->ActivePage == CmdDataTabSheet) {
      FormOutputTextBox->Left = Left + Width;
      FormOutputTextBox->Top = Top;
      FormOutputTextBox->Height = Height;
      FormOutputTextBox->Visible = true;
      FormOutputTextBox->Enabled = true;
   }
   else {
      FormOutputTextBox->Visible = false;
   }
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::UpdateModuleComboBox() {
   char *str, nameList[256];
   int result;
   unsigned long length = 256;

   // Get a list of the connected modules
   result = qusb->FindModules(nameList, length);

   // If we can't get the list or the list is empty then disable all the controls and exit. Also delete qusb object
   if ( (result == 0 || nameList[1] == ' ') ) {
      PageControl->Enabled = false;
      PageControl->ActivePage->Font->Color = clInactiveCaption;


      // Clear the list of devices
      oldList = NULL;
      ModuleComboBox->Clear();
      StringDescriptorEdit->Text = "";
       StatusBar->SimpleText =  "Cannot find any QuickUSB modules" ;

      // Delete qusb object and set pointer to NULL
      if (qusb != NULL) {
         delete qusb;
         qusb = NULL;
      }

      return;
   }

   // If the list has items and it hasn't changed, just exit
   str = nameList;
   bool foundNew = foundNewModules( str );
   if ( !foundNew ) {
      return;

   }

   // Clear the combo box
   ModuleComboBox->Clear();
   StringDescriptorEdit->Clear();

   str = nameList;
   StatusBar->SimpleText= "";
   while (*str != '\0') {
      ModuleComboBox->Items->Add(str);
      str += strlen(str) + 1;
   }

   // If we found an object reactivate the tabs
   if (oldList == NULL) {

      PageControl->Enabled = true;
      PageControl->ActivePage->Font->Color = clWindowText;
      devName = ModuleComboBox->Items->Strings[0];

	  if (qusb != NULL) {
         displayString("Program error");
      }

	  qusb = new CQuickUsb(AnsiString(devName).c_str());

      if (qusb == NULL) {
         displayString("Not enough memory");
         PageControl->Enabled = false;
         PageControl->ActivePage->Font->Color = clInactiveCaption;
         CmdDataTabSheet->Font->Color = clInactiveCaption;

         // Clear the list of devices
         ModuleComboBox->Clear();
         StringDescriptorEdit->Text = "";
         return;
      }

      // Disable timers depending on the firmware version
      if (GetFirmwareVersionString() >= 2) {
         //(PageControl->Pages[6])->Enabled = true;
         //TimerReadAllButton->Click();
      }
      else {
         // Disable the tab
         //(PageControl->Pages[6])->Enabled = false;
         //PageControl->Pages[6]->Font->Color = clInactiveCaption;
      }
   }
      // remember the current devices for next time
      oldList =  ModuleComboBox->Items;
}

//---------------------------------------------------------------------------

int __fastcall TDisplay::GetFirmwareVersionString() {
   int result, number;
   char description[128];
   string str;

   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open device:" + devName);
      return -1;
   }

   // Get the string descriptor from the module
   result = qusb->GetStringDescriptor(2, description, sizeof(description));

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read timer register");
      return -1;
   }
   str = description;
   // Extract the version
   result = str.find_first_of("0123456789", 15);
   if ( result > 0 ) {
      number =  atoi( (char *) &str.at(result) );
      return number;
   }

   return -1;
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::ModuleComboBoxClick(TObject *Sender) {
   char description[128];
   int result;

   if (ModuleComboBox->ItemIndex != -1) {
      devName = ModuleComboBox->Items->Strings[ModuleComboBox->ItemIndex];
      displayString( "Selected " + devName );

      result = qusb->Open();
      if (result == 0) {
        displayString("Cannot open device:" + devName);
        return;
      }

      // Get the string descriptor from the module
      result = qusb->GetStringDescriptor(2, description, sizeof(description));

      // Close the device
      qusb->Close();

      // Check transaction result
      if (result == 0) {
        displayString("Get string descriptor failed for:" + devName);
        return;
      }

      // Fill the string descriptor text box with the string
      StringDescriptorEdit->Text = (String) description;
   }
}


//---------------------------------------------------------------------------

void __fastcall TDisplay::ModuleComboBoxChange(TObject *Sender) {
   char description[128];
   int result;

   if (ModuleComboBox->ItemIndex != -1) {
      devName = ModuleComboBox->Items->Strings[ModuleComboBox->ItemIndex];

      // Check to see if a new Qusb item has been selected
      if (devName == oldItem) {
         return;
	  }

	  qusb->SetName(devName.c_str());

      displayString("Selected " + devName);

      result = qusb->Open();
      if (result == 0) {
        displayString("Cannot open device: " + devName);
        return;
      }

      result = qusb->GetStringDescriptor(2, description, sizeof(description));
      if (result == 0) {
        displayString("Get string descriptor failed for:" + devName);
        qusb->Close();
        return;
      }

      qusb->Close();

      // Fill the string descriptor text box with the string
      StringDescriptorEdit->Text = description;

      oldItem = devName;
	  ReadPortADirBits();
	  ReadPortCDirBits();
	  ReadPortEDirBits();
	  ReadPortAButtonClick(Sender);
	  ReadPortCButtonClick(Sender);
	  ReadPortEButtonClick(Sender);
      displayString("");
      RS232ReadTextBox->Text = "";
      RS232WriteTextBox->Text = "";
      SpiReadTextBox->Text = "";
      SpiWriteTextBox->Text = "";
      I2cReadTextBox->Text = "";
      I2cWriteTextBox->Text = "";
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::UpdateWordWideCheckBox(TObject *Sender) {
   int result;
   unsigned short value;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      Display->displayString("Cannot open device: " + devName);
      return;
   }

   // Set the new setting value
   result = qusb->ReadSetting(SETTING_WORDWIDE, &value);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      Display->displayString("Cannot read WordWide setting");
      return;
   }

   // Set the checkbox state
   if (value == 1) {
      Display->WordWideCheckBox->Checked = true;
   }
   else {
      Display->WordWideCheckBox->Checked = false;
   }

   return;
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::FileExitClick(TObject *Sender) {
   Application->Terminate();
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::ModuleScanTimerTimer(TObject *Sender) {
   UpdateModuleComboBox();

   // Select the first item if the list isn't empty and no other item is selected
   if (ModuleComboBox->Items->Count != 0) {
      ModuleComboBox->Enabled = True;
      StringDescriptorEdit->Enabled = True;

      // if no item is selected then choose the first item
      if (ModuleComboBox->ItemIndex == -1) {
         ModuleComboBox->ItemIndex = 0;
         devName = ModuleComboBox->Items->Strings[0];
		 ModuleComboBoxClick(Sender);
      }
   }
   else {
      ModuleComboBox->Enabled = False;
      StringDescriptorEdit->Enabled = False;
   }
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::FpgaUploadButtonClick(TObject *Sender) {
   int result, index = 0, length = 0, blockLength = 64, count = 1;
   char fpgaData[64];
   FILE * input;
   unsigned short isConfigured;
   long fileSize;

   lastButton = NULL;

   if (UploadFilenameTextBox->Text == "" || UploadFilenameTextBox -> Text == "Please select a file to upload" ) {
      UploadFilenameTextBox -> Text = "";
	  BrowseButtonClick(Sender);                // Display the browse box
      if (UploadFilenameTextBox->Text == "") { // The user selected Cancel
         UploadFilenameTextBox->Text = "Please select a file to upload";
         return;
      }
   }

   // Open the input file for reading
   input = fopen( AnsiString(UploadFilenameTextBox->Text).c_str() , "rb+");
   if (input == NULL) {
      displayString( "There was an error opening the disired file");
      return;
   }


   fseek(input, 0L, SEEK_END);
   fileSize = ftell(input);
   fseek(input, 0L, SEEK_SET);

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Start configuring the FPGA
   result = qusb->StartFpgaConfiguration();
   if (result == 0) {
      displayString( "Cannot start FPGA configuration");
      fclose(input);
      qusb->Close();
      return;
   }

   // Send the FPGA configuration data one block at a time
   Progress->Position = 0;

   do {
      // Read the data
      length = min(blockLength, (int) (fileSize - index) );

      // Read the data from the file and put it into fpgaData array
      fread(fpgaData,length, count, input);

      // Write the data to the EEPROM
      displayString("Configuring FPGA..." + (String) Progress->Position + "%");
      result = qusb->WriteFpgaData(fpgaData, length);

      if (result == 0) {
         fclose(input);
         Progress->Position = 0;
         displayString("FPGA configuration failed");
         qusb->Close();
         return;
      }

      // Update the counters
      index = index + length;

      // Update the progress bar
      Progress->Position = ((index * 100) / fileSize);
      Progress->Update();
   }
   while (index < fileSize);

   // Close the file
   fclose(input);

   // check if FPGA is configuring
   result = qusb->IsFpgaConfigured(&isConfigured);

   // Close the device when you're done
   qusb->Close();

   // Notify the user
   if (isConfigured == 0) {
      displayString( "FPGA configuration failed");
   }
   else{
      displayString("FPGA was successfully configured");
   }

   Progress->Position = 0;
 return;
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::BrowseButtonClick(TObject *Sender) {
   String filename = "";

   lastButton = NULL;

   if (OpenDialog->Execute()) {
     filename = OpenDialog->FileName;
   }
   // check if the user hit cancel
   if (filename == "") {
     return;
   }

   UploadFilenameTextBox->Text = filename;
}


//---------------------------------------------------------------------------



void __fastcall TDisplay::ReadCommandButtonClick(TObject *Sender) {
   unsigned short length, commandAddress;
   int result, commandLength;
   unsigned char * commandData;
   bool wordWide;

   // Remember the last button used
   lastButton = ReadCommandButton;

   // Attempt to get the address from the GUI
  try {
      commandAddress = (CommandAddressTextBox->Text).ToInt();
      commandLength = (CommandLengthTextBox->Text).ToInt();
  }
  catch(const Exception &EConvertError) {
     displayString("Invalid input detected.");
     return;
  }

  // Set the command address increment flag if needed
  if (NoIncCommandAddressCheckBox->Checked == true) {
     commandAddress = commandAddress & 0x7FFF;
  }
  else {
     commandAddress = commandAddress | 0x8000;
  }

  // Handle wordwide
  if (WordWideCheckBox->Checked == true) {
     wordWide = true;
  }
  else {
     wordWide = false;
  }

  // if wordwide is on set length to the proper length
  if (wordWide) {
     length = commandLength * 2;
  }
  else {
     length = commandLength;
  }

  // Allocate storage for incoming data
  commandData = new unsigned char[length];
  if (commandData == NULL) {
     displayString("Not enough memory");
     return;
  }

  // Open the device
  result = qusb->Open();
  if (result == 0) {
     displayString("Cannot open Device:" + devName);
     return;
  }


  if (wordWide) {
     displayString("Reading " +  (String) commandLength + " words of command...");
  }
  else {
     displayString("Reading " +  (String) commandLength + " bytes of command...");
  }

  // Read data from the module
  result = qusb->ReadCommand(commandAddress, commandData, &length);

  // Close the device when you're done
  qusb->Close();

  // Check the transaction result
  if (result == 0) {
    displayString("Cannot read command from device: " + devName);
    delete [] commandData;
    return;
  }

  // Fill the array with data read from quickUSB
  for (int i = 0; i < commandLength; i++) {
     if (wordWide) {
        CommandFlexGrid->Cells[1][i] = commandData[i * 2] + (0x100 * commandData[ (i * 2) + 1] );
     }
     else {
        CommandFlexGrid->Cells[1][i] = commandData[i];
     }
  }

  // Display the data if the form is up
  if ( FormOutputTextBox->Visible == true ) {
     FormOutputTextBox->ShowByteData(commandData, length);
  }

  delete [] commandData;
  return;

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::WriteCommandButtonClick(TObject *Sender) {
   unsigned char * commandData;
   int result, length;
   unsigned short commandAdress;
   bool wordWide;

   // Remember the last button clicked by user
   lastButton = WriteCommandButton;

   // Attempt to extract data from GUI
   try {
      commandAdress = (CommandAddressTextBox->Text).ToInt();
      commandLength =  (CommandLengthTextBox->Text).ToInt();
   }
   catch (const EConvertError &e) {
      displayString("Invalid input detected");
      return;
   }

   // Set the command address increment flag if needed
   if (NoIncCommandAddressCheckBox->Checked == true) {
      commandAdress = commandAdress & 0x7FFF;
   }
   else {
      commandAdress = commandAdress | 0x8000;
   }

   // Handle wordwide
   if (WordWideCheckBox->Checked == true) {
      wordWide = true;
   }
   else {
      wordWide = false;
   }

   // If word wide is selected fix the length
   if (WordWideCheckBox->Checked == true) {
     length = commandLength * 2;
   }
   else {
      length = commandLength;
   }

   // Allocate storage
   commandData = new unsigned char[length];
   if (commandData == NULL) {
      displayString("Not enough memory");
      return;
   }

   // Fill the data array with the contents of the matrix
   for (int i = 0; i < commandLength; i++) {
      try {
         if (wordWide) {
            commandData[i * 2] = (CommandFlexGrid->Cells[1][i]).ToInt() & 0xFF;
            commandData[ (i * 2) + 1] = (((CommandFlexGrid->Cells[1][i]).ToInt() & 0xFF00) / 0x100);
         }
         else {
            commandData[i] = (CommandFlexGrid->Cells[1][i]).ToInt() & 0xFF;
         }
      }
      catch (const EConvertError &e) {
         displayString("Invalid input detected");
         return;
      }
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
     displayString("Cannot open Device:" + devName);
     delete [] commandData;
     return;
   }

   // Write the data
   if (wordWide) {
      displayString("Writing " + (String) commandLength + " words of command...");
   }
   else {
      displayString("Writing " + (String) commandLength + " bytes of command...");
   }

   result = qusb->WriteCommand(commandAdress, commandData, length);


   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write command");
      delete [] commandData;
      return;
   }

   // Display the data if the form is up
   if (FormOutputTextBox->Visible == true) {
      FormOutputTextBox->ShowByteData(commandData, length);
   }

   // Free the memory
   delete [] commandData;

}


//---------------------------------------------------------------------------

void __fastcall TDisplay::WriteDataButtonClick(TObject *Sender) {
   int result, dataAddress;
   long dataLength;
   char * testData;
   FILE * DataFile;
   FILE * fh;

   lastButton = WriteDataButton;

   // Get the dataAddress from the text box
   try {
      dataAddress = (DataAddressTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   // Check for an empty filename
   if (WriteFileEditBox->Text == "") {
	  WriteDataBrowseButtonClick(Sender);       // Display the browse box
	  if (WriteFileEditBox->Text == "") {     // The user selected Cancel
         return;
      }
   }

   // Is this an async request?
   if (AsyncCheckBox->Checked) {
	  WriteAsyncData();
      return;
   }

   // Calculate the data length
   fh = _wfopen(WriteFileEditBox->Text.c_str(), _T("rb"));
   if (fh != NULL) {
	  fseek(fh, 0L, SEEK_END);
	  dataLength = ftell(fh);
	  fclose(fh);
   }
   else {
      displayString("Unable to compute length of input file.");
      return;
   }

   if (dataLength == 0) {
      displayString("Attempt to read an empty file!");
      return;
   }

   DataFile = _wfopen(WriteFileEditBox->Text.c_str(), _T("rb"));
   if (DataFile == NULL) {
      displayString("Cannot open data input file!");
      return;
   }

   // Allocate storage for writeDataLength elements
   testData = new char[dataLength];
   if (testData == NULL) {
      displayString("Not enough memory");
      fclose(DataFile);
      return;
   }

   TCursor Save_Cursor = Screen->Cursor;
   Screen->Cursor = crHourGlass;    // Show hourglass cursor
   startTime = timeGetTime();

   // Read the input file
   fread(testData, dataLength, 1, DataFile);
   fclose(DataFile);

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      delete [] testData;
      Screen->Cursor = Save_Cursor;
      return;
   }

   // Set the data address
   if (NoIncDataAddressCheckBox->Checked == true) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress | 0x8000);
   }

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write data address setting");
      qusb->Close();
      delete [] testData;
      Screen->Cursor = Save_Cursor;
      return;
   }

   // Write the data
   displayString("Writing " + (String) dataLength + " bytes of data...");
   result = qusb->WriteData(testData, dataLength);
   endTime = timeGetTime();

   // Close the device when you're done
   qusb->Close();
   Screen->Cursor = Save_Cursor;

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write data");
	  Application->MessageBox(_T("Please disconnect the QuickUSB module now to avoid a system lockup"), _T("Write Data"), MB_OK);
      delete [] testData;
	  return;
   }

   // Free the memory
   delete [] testData;
   DataLengthTextBox->Text = dataLength;
   displayString("Write " + (String) dataLength + " bytes of data successful.");
   ThroughputCalc(dataLength);
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::WriteCountDataButtonClick(TObject *Sender) {
   int result, dataAddress;
   unsigned char * testData;
   unsigned long dataLength;

   // Get the datalength from the text box

   lastButton = WriteCountDataButton;

   try {
      dataAddress = (DataAddressTextBox->Text).ToInt();
      dataLength = (DataLengthTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   // Allocate storage for writeDataLength elements
   testData = new char[dataLength];
   if (testData == NULL) {
      displayString("Not enough memory");
      return;
   }

   // Fill the array with a count
   for (unsigned long i = 0; i < dataLength; i++) {
     testData[i] = i & 0xFF;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      delete [] testData;
      return;
   }

   // Set the data address
   if (NoIncDataAddressCheckBox->Checked == true) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress | 0x8000);
   }

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write data address setting");
      qusb->Close();
      delete [] testData;
      return;
   }

   // Write the data
   displayString("Writing " + (String)  dataLength + " words of data...");
   result = qusb->WriteData(testData, dataLength);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write count data");
      delete [] testData;
      return;
   }

   // Display the data if the form is up
    if (FormOutputTextBox->Visible == true) {
     FormOutputTextBox->ShowByteData(testData, dataLength);
    }

   // Free the memory
   delete [] testData;
}
//---------------------------------------------------------------------------


void __fastcall TDisplay::WordWideCheckBoxClick(TObject *Sender) {
   int result;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Set the new setting value
   if (WordWideCheckBox->Checked == true) {
      result = qusb->WriteSetting(SETTING_WORDWIDE, 1);
   }
   else {
     result = qusb->WriteSetting(SETTING_WORDWIDE, 0);
   }

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write WORDWIDE setting");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::ReadDataButtonClick(TObject *Sender) {
   int result, dataAddress, dataLength;
   char * testData;
   unsigned long length;
   FILE * DataFile;

   lastButton = ReadDataButton;

   // Get the datalength from the text box
   try {
      dataAddress = (DataAddressTextBox->Text).ToInt();
      dataLength = (DataLengthTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   if (dataLength <= 0) {
      displayString("Length must be greater than zero!");
      return;
   }

   // Check for an empty filename
   if (ReadFileEditBox->Text == "") {
	  ReadDataBrowseButtonClick(Sender);                     // Display the browse box
      if (ReadFileEditBox->Text == "") {     // The user selected Cancel
         return;
      }
   }

   // Is this an async request?
   if (AsyncCheckBox->Checked) {
	  ReadAsyncData();
      return;
   }

   // Allocate storage for writeDataLength elements
   length = dataLength;
   testData = new char[length];
   if (testData == NULL) {
      displayString("Not enough memory");
      return;
   }

   DataFile = _wfopen(ReadFileEditBox->Text.c_str(), _T("wb"));
   if (DataFile == NULL) {
      delete [] testData;
      displayString("Cannot open data output file!");
      return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      delete [] testData;
      fclose(DataFile);
      return;
   }

   TCursor Save_Cursor = Screen->Cursor;
   Screen->Cursor = crHourGlass;    // Show hourglass cursor
   startTime = timeGetTime();

   // Set the data address
   if (NoIncDataAddressCheckBox->Checked == true) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress);
   }
   else{
      result = qusb->WriteSetting(SETTING_DATAADDRESS, (dataAddress | 0x8000));
   }

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write data address setting");
      qusb->Close();
      delete [] testData;
      fclose(DataFile);
      Screen->Cursor = Save_Cursor;
      return;
   }

   // Read the data
   displayString("Read " + (String) dataLength + " bytes of data...");
   result = qusb->ReadData(testData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read data");
      Application->MessageBox(_T("Please disconnect the QuickUSB module now to avoid a system lockup"), _T("Read Data"), MB_OK);
      delete [] testData;
      fclose(DataFile);
      Screen->Cursor = Save_Cursor;
      return;
   }

   fwrite(testData, length, 1, DataFile);
   endTime = timeGetTime();
   fclose(DataFile);
   Screen->Cursor = Save_Cursor;

   // Free the memory
   delete [] testData;
   displayString("Read " + (String) dataLength + " bytes of data successful.");
   ThroughputCalc(dataLength);
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::SpiReadButtonClick(TObject *Sender) {
   int result;
   unsigned short length;
   unsigned char portNum;
   char * inData;
   string str;

   lastButton = SpiReadButton;


   // Calculate the length
   portNum = SpiReadPortNumComboBox->ItemIndex;
   try {
      length =  (SpiReadLengthTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   // Allocate storage for writeDataLength elements
   inData = new char[length];
   if (inData == NULL) {
      displayString("Not enough memory");
      return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      delete [] inData;
	  displayString("Cannot open Device:" + (String) qusb->GetDeviceName());
      return;
   }

   // Read the SPI data
   result = qusb->ReadSpi(portNum, inData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read SPI data");
      delete [] inData;
      return;
   }

   // Convert the byte array to a string
   str = ByteArrayToString( inData, length);
   SpiReadTextBox->Text = str.c_str();

   delete [] inData;
   return;
}


//---------------------------------------------------------------------------


void __fastcall TDisplay::SpiWriteButtonClick(TObject *Sender) {
   int result;
   size_t length;
   char outData[64];
   unsigned char portNum;
   AnsiString tempString;

   lastButton = SpiWriteButton;

   // Get the port number and the length
   portNum = SpiWritePortNumComboBox->ItemIndex;

   // Convert the text string to a byte array
   length = wcstombs(outData, SpiWriteTextBox->Text.c_str(), 64);

   if (length <= 0) {
	 displayString("Please enter digits or hex values first");
	 return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" +  (String) qusb->GetDeviceName());
      return;
   }

   // If it is a valid length write out the SPI data
   if (length > 0 ) {
      result = qusb->WriteSpi(portNum, outData, length);
   }
   else {
      displayString("Please enter decimal digits or Hex values only.");
      qusb->Close();
      return;
   }

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write SPI data");
      delete [] outData;
      return;
   }

  delete [] outData;
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::SpiWriteReadButtonClick(TObject *Sender) {
   int result, length;
   unsigned char portNum, * inOutData;
   string tempString = "", inputString = "";

   lastButton = SpiWriteReadButton;


   // Get the port number and the length
   portNum = SpiWritePortNumComboBox->ItemIndex;

   // Convert the text string to a byte array
   length = wcstombs(inOutData, SpiWriteTextBox->Text.c_str(), 64);

   if (length <= 0) {
	 displayString("Please enter digits or hex values first");
	 return;
   }

   // Attempt to open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Write the data out if the length is > 0
   if (length > 0) {
     result = qusb->WriteReadSpi(portNum, inOutData, length);
   }
   else {
      displayString("Please enter decimal digits or Hex values only.");
      qusb->Close();
      return;
   }

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write SPI data");
      delete [] inOutData;
      return;
  }

   // Convert the byte array to a string
   inputString = ByteArrayToString(inOutData, length);
   SpiReadTextBox->Text = inputString.c_str();
   delete [] inOutData;
}


//---------------------------------------------------------------------------

void __fastcall TDisplay::ReadPortAButtonClick(TObject *Sender) {
   int result;
   unsigned char portData;
   unsigned short length = 1;

   lastButton = ReadPortAButton;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" +devName);
      return;
   }

   // Read the port data
   displayString("Reading port A...");
   result = qusb->ReadPort(0, &portData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read port A");
      return;
   }

   // Display the hex value
   PortAValueBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::ReadPortCButtonClick(TObject *Sender) {
   int result;
   unsigned char portData;
   unsigned short length = 1;

   lastButton = ReadPortCButton;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Read the port data
   displayString("Reading port C...");
   result = qusb->ReadPort(2, &portData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read port C");
      return;
   }

   // Display the hex value
   PortCValueBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::ReadPortEButtonClick(TObject *Sender) {
   int result;
   unsigned char portData;
   unsigned short length = 1;

   lastButton = ReadPortEButton;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" +devName);
      return;
   }

   // Read the port data
   displayString("Reading port E...");
   result = qusb->ReadPort(4, &portData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read port E");
      return;
   }

   // Display the hex value
  PortEValueBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::ReadPortADirBits() {
   int result;
   unsigned char portData;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Read the port data
   result = qusb->ReadPortDir(0, &portData);

   // Close the device
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read direction of port A");
      return;
   }

   // Set the port dir bit boxes
   PortADirBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::ReadPortCDirBits() {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Read the port data
   result = qusb->ReadPortDir(2, &portData);

   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read direction of port C");
      return;
   }

   // Set the port dir bit boxes
   PortCDirBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::ReadPortEDirBits() {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Read the port data
   result = qusb->ReadPortDir(4, &portData);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot read direction of port E");
      return;
   }

   // Set the port dir bit boxes
   PortEDirBitCheckBox->setValue(portData);
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortADirBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the value of the checked off bits
   portData = PortADirBitCheckBox->getValue();

   // write the port data
   result = qusb->WritePortDir(0, portData);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write direction of port A");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortAValueBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the value of the checked off bits
   portData = PortAValueBitCheckBox->getValue();

   // Write the port data
   result = qusb->WritePort(0, &portData, 1);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write value of port A");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortCDirBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the value from the checked bits
   portData = PortCDirBitCheckBox->getValue();

   // Write the port data
   result = qusb->WritePortDir(2, portData);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write direction of port C");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortCValueBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the hex value from the text box
   portData = PortCValueBitCheckBox->getValue();

   // Write the port data
   result = qusb->WritePort(2, &portData, 1);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write value of port C");
      return;
   }

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortEDirBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result =qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the hex value from the text box
   portData = PortEDirBitCheckBox->getValue();

   // Read the port data
   result = qusb->WritePortDir(4, portData);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write direction of port E");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortEValueBitCheckBoxClick(TObject *Sender) {
   int result;
   unsigned char portData;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Get the hex value from the text box
   portData = PortEValueBitCheckBox->getValue();

   // write port data
   result = qusb->WritePort(4, &portData, 1);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write value of port E");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::RS232BaudRateComboBoxChange(TObject *Sender) {
   int result, baudRate;

   // Attemp to retrieve the value from the combo box
   try {
      baudRate = (RS232BaudRateComboBox->Text).ToInt();
   }
   catch (const Exception &EConvertError) {
      displayString("Invalid input detected");
      return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   result = qusb->SetRS232BaudRate(baudRate);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot set RS232 baud rate");
      return;
   }

}
//---------------------------------------------------------------------------


void __fastcall TDisplay::FlushRS232ButtonClick(TObject *Sender) {
   unsigned char portNum;
   int result;

   lastButton = FlushRS232Button;

   portNum = RS232PortComboBox->ItemIndex;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Read length characters from the RS232 port
   displayString("Flushing RS232 port");
   result = qusb->FlushRS232(portNum);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
    displayString("Could not flush RS232");
    return;
   }

   GetNumTextBox->Text =  0;
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::ReadRS232ButtonClick(TObject *Sender) {
   char portNum, * inData;
   int result;
   unsigned short length = 0;

   lastButton = ReadRS232Button;

   portNum = RS232PortComboBox->ItemIndex;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Find out how many characters are waiting for us
   result = qusb->GetNumRS232(portNum, &length);
   if (result == 0) {
      qusb->Close();
      displayString("Cannot read number of RS232 characters waiting");
      return;
   }

   if (length > 0) {

      // Make the size one larger to accomodate the terminating NULL
      inData = new char[length+1];
      if (inData == NULL) {
         qusb->Close();
         displayString("Not enough memory");
         return;
      }
      // Appending the NULL
      inData[length] = '\0';

      // Read length characters from the RS232 port
      result = qusb->ReadRS232(portNum, inData, &length);

      // Close the device when you're done
      qusb->Close();

      // Check the transaction result
      if (result == 0) {
         displayString("Cannot read string from RS232");
         delete [] inData;
         return;
      }

      // Put the string into the text box
      RS232ReadTextBox->Text = inData;
      delete [] inData;

      // See if any more characters are waiting to be read
      result = qusb->GetNumRS232(portNum, &length);

      if (result == 0) {
        // Do not need a close because I closed the qusb device above and qusb->GetNumRS232(portNum, &length)
        // uses the tempOpen/tempClose functionality of the CQuickUsb class
        displayString("Cannot read number of RS232 characters waiting");
        return;
      }

       GetNumTextBox->Text = length;
   }
   else {
      displayString("There are no characters waiting to be read");
      qusb->Close();
   }
}


//---------------------------------------------------------------------------


void __fastcall TDisplay::WriteRS232ButtonClick(TObject *Sender) {
   char portNum;
   int result;
   AnsiString text;

   lastButton = WriteRS232Button;

   // Retrieve information from GUI
   portNum = RS232PortComboBox->ItemIndex;
   text =  RS232WriteTextBox->Text;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device: " + devName);
      return;
   }

   // Write to the RS-232 port
   result = qusb->WriteRS232(portNum, text.c_str(), text.Length() );

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write string to RS232");
      return;
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::RS232PortComboBoxChange(TObject *Sender) {
  char portNum;
  int result;
  unsigned short length;

   if (RS232PortComboBox->ItemIndex == -1) {
     return;
   }

   portNum = RS232PortComboBox->ItemIndex;

   // Open the device
   result = qusb->Open();
   if (result == 0) {
       displayString("Cannot open Device:" + devName);
       return;
   }

   // Get the number of RS232 characters waiting
   result = qusb->GetNumRS232(portNum, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot get number of RS232 bytes outstanding");
      return;
   }

   // Update the length
   GetNumTextBox->Text = length;
   return;
}


//---------------------------------------------------------------------------

void __fastcall TDisplay::I2cReadButtonClick(TObject *Sender) {
   int result;
   unsigned short address, length;
   unsigned char * inData;
   string str;

   lastButton = I2cReadButton;

   // Calculate the length
   try {
      address = (I2cReadAddressTextBox->Text).ToInt();
      length = (I2cReadLengthTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device: " + devName);
      return;
   }

   // Allocate storage for writeDataLength elements
   inData = new unsigned char[length];
   if (inData == NULL) {
      displayString("Not enough memory");
      qusb->Close();
      return;
   }

   // Read I2C data
   displayString("Reading I2C Data...");
   result = qusb->ReadI2C(address, inData, &length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot Read from I2C");
      delete [] inData;
      return;
   }

   // Convert the byte array to a string
   str = ByteArrayToString(inData, length);
   I2cReadTextBox->Text = str.c_str();

   delete [] inData;
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::I2cWriteButtonClick(TObject *Sender) {
   int result;
   unsigned short length, address;
   unsigned char * outData;
   string tempString;

   lastButton = I2cWriteButton;

   // Get the address and the length
   try {
      address = (I2cWriteAddressTextBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected");
      return;
   }

   // Convert the text string to a byte array
   length = wcstombs(outData, I2cWriteTextBox->Text.c_str(), 64);

   if (length <= 0) {
	 displayString("Please enter digits or hex values first");
	 return;
   }

   // Open the device
   result = qusb->Open();
   if (result == 0) {
      displayString("Cannot open Device:" + devName);
      return;
   }

   // Write the text out the I2C port
   displayString("Writing I2C Data...");
   result = qusb->WriteI2C(address, outData, length);

   // Close the device when you're done
   qusb->Close();

   // Check the transaction result
   if (result == 0) {
      displayString("Cannot write to I2C");
      delete [] outData;
      return;
   }

   delete [] outData;
}


//---------------------------------------------------------------------------

void __fastcall TDisplay::CommandLengthTextBoxChange(TObject *Sender) {

   if ( (CommandLengthTextBox->Text).Length() > 0) {
      try {
         commandLength = (CommandLengthTextBox->Text).ToInt();
      }
      catch (const EConvertError &e) {
         displayString("Invalid input detected");
         CommandLengthTextBox->Text = "8";
         commandLength = 8;
         return;
      }
      if ( commandLength > 0 && commandLength < 65 ) {
         CommandFlexGrid->RowCount = commandLength;
         for (int i = 0; i < commandLength; i++) {
           CommandFlexGrid->Cells[0][i] = i + 1;
           CommandFlexGrid->Cells[1][i] = 0;
         }
      }
      else {
       displayString("The maximum length is 64 bytes");
       CommandLengthTextBox->Text = "64";
       commandLength = 64;
      }
   }
}

//---------------------------------------------------------------------------


int __fastcall TDisplay::StringToByteArray(unsigned char * &theData, string theString) {
   int dataIndex = 0, strLen, tempInt;
   long startStrIndex = 0, endStrIndex;
   unsigned char * tempData;
   String tempString;

   // Exit for an empty string
   strLen = theString.length();
   if (strLen <=0) {
     return false;
   }

   // Allocate space for the data  and guess that the data is the longest
   tempData = new char[strLen];
   if (tempData == NULL) {
      displayString("Not enough memory");
      return false;
   }

   // Parse text and put it into the array

   while (startStrIndex < strLen) {
      // Find the first non-space
      while (startStrIndex < strLen && theString[startStrIndex] == ' ') {
         startStrIndex++;
      }

      // Find the first space
      endStrIndex = startStrIndex + 1;
      while (endStrIndex < strLen && theString[endStrIndex] != ' ') {
         endStrIndex++;
      }

     // Get the text
     tempString = (theString.substr(startStrIndex, endStrIndex - startStrIndex) ).c_str();
     try {
        tempInt = tempString.ToInt();
     }
     catch(const Exception &EConvertError) {
        displayString("Invalid input detected");
        delete [] tempData;
        return false;
     }

     tempData[dataIndex++] = tempInt & 0xFF;
     startStrIndex = endStrIndex;
  }

   // Resize the data to the right length
   theData = new unsigned char[dataIndex];
   if (theData == NULL) {
      delete tempData;
      displayString("Not enough memory");
      delete [] tempData;
      return false;
   }

   for (int i = 0; i < dataIndex; i++) {
    theData[i] = tempData[i];
   }

   // free memory
   delete [] tempData;

   // Return the actual length
   return dataIndex;
}

//---------------------------------------------------------------------------


string __fastcall TDisplay::ByteArrayToString(char * byteArray, int length) {
   string theString;

   for (int count = 0; count < length; count++) {
      if (byteArray[count] == ' ')
        continue;
      theString = theString + toHex(byteArray[count], 1) + " ";
   }

   return theString;
}

//---------------------------------------------------------------------------

string __fastcall TDisplay::toHex(int decimal, int length) {
   string hex = "";
   for (int i=length; i>=0; i--) {
      hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
   }

   return ("0x" + hex);
}

//---------------------------------------------------------------------------

bool __fastcall TDisplay::HexToDecimal (const char* HexNumber, int& Number) {
    char* pStopString;
    Number = strtol(HexNumber, &pStopString, 16);
    return (bool)(Number != LONG_MAX);

}

//---------------------------------------------------------------------------


void __fastcall TDisplay::HelpAboutClick(TObject *Sender) {
   About->ShowModal();

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::StatusBarTimerTimer(TObject *Sender) {
 StatusBar->SimpleText = "";
 StatusBarTimer->Enabled = false;

}
//---------------------------------------------------------------------------

void __fastcall TDisplay::displayString(String str) {
   // Stop the timer, display the message, and then restart it
   StatusBarTimer->Enabled = false;
   StatusBar->SimpleText = str;
   StatusBarTimer->Enabled = true;

}

//===========================================================

bool __fastcall TDisplay::foundNewModules(char * str) {

   String tempString;
   int position = 0;

   // Do while we havn't hit a double null terminal
   do {
      tempString =  (ModuleComboBox->Items->Strings[position++]);

      // if the strings are not the same then we have found a different list return true
	  if ( wcscmp( String(str).c_str(), tempString.c_str() ) != 0 ) {
         return true;
      }
      str += strlen(str) + 1;
   }
   while( *str != '\0' );

   //if the length has changed return true
   if (position < ModuleComboBox->Items->Count) {
	  return true;
   }

   // otherwise nothing has changed return false;
   return false;
}

//---------------------------------------------------------------------------


void __fastcall TDisplay::RepeatCheckBoxClick(TObject *Sender) {

   if (RepeatCheckBox->Checked == true) {
      RepeatTimer->Enabled = true;
   }
   else {
      RepeatTimer->Enabled = false;
   }


}

//---------------------------------------------------------------------------

void __fastcall TDisplay::RepeatTimerTimer(TObject *Sender) {

   if ( lastButton != NULL ) {
      lastButton->Click();
   }
}

//---------------------------------------------------------------------------

void __fastcall TDisplay::RepeatTextBoxChange(TObject *Sender) {
   int interval;

   if ( RepeatTextBox->Text.Length() < 1) {
      return;
   }

   try {
      interval =  (RepeatTextBox->Text).ToInt() ;
   }
   catch (const EConvertError &e) {
      displayString("Value for repeat interval is invalid" );
      RepeatTextBox->Text = "500";
      RepeatTimer->Interval = 500;
      RepeatTimer->Enabled = false;
      return;
   }

   RepeatTimer->Interval = interval;


}

//---------------------------------------------------------------------------


void __fastcall TDisplay::FormDestroy(TObject *Sender) {

   AnsiString setting;

   // Delete Qusb object and set pointer to NULL
   delete qusb;
   qusb = NULL;

   TRegistry *Registry = new TRegistry;
   if (Registry == NULL) {
      displayString("Not enough memory");
   }
   else {
      try {
         // Attempt to open the registry key for QuickUsb diag, if it doesn't exist create it
         Registry->OpenKey("\\Software\\BitwiseSys\\QuickUsb Diag",true);

         // look up which tab the program used last;
         Registry->WriteString("Tab", PageControl->ActivePage->PageIndex );
         // Save the last uploaded file
         Registry->WriteString("UploadFileName", UploadFilenameTextBox->Text );

         // Save interval
         Registry->WriteString("Interval", RepeatTextBox->Text);

         // Save wordWide setting
         Registry->WriteBool("wordWide", WordWideCheckBox->Checked );

       }
     catch ( const Exception &e ) {
        return;
     }

    delete Registry;
  }
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::CycleButtonClick(TObject *Sender) {
   lastButton = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::ResetButtonClick(TObject *Sender) {
   lastButton = NULL;

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortADirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortADirBitCheckBox->getValue();

   try {
      portData = (PortADirBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port A
      if (portData > 255 || portData < 0) {
         displayString("Invalid input detected only numbers between 0 to 255");
         PortADirBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port A
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected only numbers between 0 to 255");
      PortADirBitCheckBox->setBoxValue(oldValue);
      return;
   }
   result = qusb->WritePortDir(0, (unsigned char) portData);

   if (result == 0) {
      displayString("Cannot write to port A");
      return;
   }

   // Display the hex value
   PortADirBitCheckBox->setValue( (unsigned char) portData);

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortCDirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortCDirBitCheckBox->getValue();

   try {
      portData = (PortCDirBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port C
      if (portData > 255 || portData < 0) {
         displayString("Invalid input detected only numbers between 0 to 255");
         PortCDirBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port C
   catch ( const EConvertError &e ) {
      displayString("Invalid input detected only numbers between 0 to 255");
      PortCDirBitCheckBox->setBoxValue(oldValue);
      return;
   }
   result = qusb->WritePortDir(2, (unsigned char) portData);

   if (result == 0) {
      displayString("Cannot write to port C");
      return;
   }

   // Display the hex value
   PortCDirBitCheckBox->setValue( (unsigned char) portData);

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortEDirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortEDirBitCheckBox->getValue();

   try {
      portData = (PortEDirBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port E
      if (portData > 255 || portData < 0) {
		 displayString(_T("Invalid input detected only numbers between 0 to 255"));
         PortEDirBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port E
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected only numbers between 0 to 255"));
      PortEDirBitCheckBox->setBoxValue(oldValue);
      return;
   }
   result = qusb->WritePortDir(4, (unsigned char) portData);

   if (result == 0) {
	  displayString(_T("Cannot write to port E"));
      return;
   }

   // Display the hex value
   PortEDirBitCheckBox->setValue( (unsigned char) portData);

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortAValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortAValueBitCheckBox->getValue();

   try {
      portData = (PortAValueBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port A
      if (portData > 255 || portData < 0) {
		 displayString(_T("Invalid input detected only numbers between 0 to 255"));
         PortAValueBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port A
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected only numbers between 0 to 255"));
      PortAValueBitCheckBox->setBoxValue(oldValue);
      return;
   }
   // Write the port data
   result = qusb->WritePort(0, (unsigned char *) &portData, 1);

   // Check the transaction result
   if (result == 0) {
	  displayString(_T("Cannot write value of port A"));
      return;
   }

   // Display the hex value
   PortAValueBitCheckBox->setValue( (unsigned char) portData);

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortCValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortCValueBitCheckBox->getValue();

   try {
      portData = (PortCValueBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port C
      if (portData > 255 || portData < 0) {
		 displayString(_T("Invalid input detected only numbers between 0 to 255"));
         PortCValueBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port C
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected only numbers between 0 to 255"));
      PortCValueBitCheckBox->setBoxValue(oldValue);
      return;
   }
   // Write the port data
   result = qusb->WritePort(2, (unsigned char *) &portData, 1);

   // Check the transaction result
   if (result == 0) {
	  displayString(_T("Cannot write value of port C"));
      return;
   }

   // Display the hex value
   PortCValueBitCheckBox->setValue( (unsigned char) portData);

}

//---------------------------------------------------------------------------

void __fastcall TDisplay::PortEValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key) {
   int result, portData, oldValue;

   if (Key != VK_RETURN	 ) {
      return;
   }

   oldValue = PortEValueBitCheckBox->getValue();

   try {
      portData = (PortEValueBitCheckBox->getText()).ToInt();
      // if there was an error revert to the current value of port E
      if (portData > 255 || portData < 0) {
		 displayString(_T("Invalid input detected only numbers between 0 to 255"));
         PortEValueBitCheckBox->setBoxValue(oldValue);
         return;
      }
   }
   // if there was an error revert to the current value of port E
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected only numbers between 0 to 255"));
      PortEValueBitCheckBox->setBoxValue(oldValue);
      return;
   }
   // Write the port data
   result = qusb->WritePort(4, (unsigned char *) &portData, 1);

   // Check the transaction result
   if (result == 0) {
	  displayString(_T("Cannot write value of port E"));
      return;
   }

   // Display the hex value
   PortEValueBitCheckBox->setValue( (unsigned char) portData);
}


void __fastcall TDisplay::ReadDataBrowseButtonClick(TObject *Sender)
{
   String filename = "";

   lastButton = NULL;

   if (DataOpenDialog->Execute()) {
     filename = DataOpenDialog->FileName;
   }
   // check if the user hit cancel
   if (filename == "") {
     return;
   }

   ReadFileEditBox->Text = filename;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::WriteDataBrowseButtonClick(TObject *Sender)
{
   String filename = "";

   lastButton = NULL;

   if (DataOpenDialog->Execute()) {
     filename = DataOpenDialog->FileName;
   }
   // check if the user hit cancel
   if (filename == "") {
     return;
   }

   WriteFileEditBox->Text = filename;
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::AsyncCheckBoxClick(TObject *Sender)
{
   if (AsyncCheckBox->Checked) {
      BufferSizeEditBox->Visible = TRUE;
      BufferCountEditBox->Visible = TRUE;
      BufferSizeEditBox->ReadOnly = FALSE;
      BufferCountEditBox->ReadOnly = FALSE;
      BufferSizeEditBox->Text = "65536";
      BufferCountEditBox->Text = "4";
      DataLengthTextBox->Text = "1048576";
   }
   else {
      BufferSizeEditBox->Visible = FALSE;
      BufferCountEditBox->Visible = FALSE;
      BufferSizeEditBox->ReadOnly = TRUE;
      BufferCountEditBox->ReadOnly = TRUE;
      BufferSizeEditBox->Text = "";
      BufferCountEditBox->Text = "";
      DataLengthTextBox->Text = "65536";
   }
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::ReadAsyncData()
{
   unsigned long  BufSize;
   int            BufCount;
   int            address;
   int            dataLength;
   long           TotalBlocks;
   FILE *         outputFile;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;

   // Get the datalength from the text box
   try {
      address = (DataAddressTextBox->Text).ToInt();
      dataLength = (DataLengthTextBox->Text).ToInt();
      BufSize = (BufferSizeEditBox->Text).ToInt();
      BufCount = (BufferCountEditBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected"));
      return;
   }

   if (dataLength <= 0) {
	  displayString(_T("Length must be greater than zero!"));
      return;
   }

   // This is not a limitation of the QuickUSB, but the I/O will be
   // very slow with a small block size.
   if (BufSize < 64) {
	  displayString(_T("Buffer size must be at least 64 bytes for async I/O!"));
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (BufCount < 2) {
	  displayString(_T("Buffer count must be at least 2 for async I/O!"));
      return;
   }

   TotalBlocks = dataLength / BufSize;

   // This is not a limitation of the QuickUSB, but it makes this demo
   // code much simpler.
   if ((TotalBlocks * BufSize) != (unsigned long)dataLength) {
	  displayString(_T("Data Length must be a multiple of Buffer Size"));
      return;
   }

   outputFile = _wfopen(ReadFileEditBox->Text.c_str(), _T("w+b"));

   if (outputFile == NULL) {
	  Application->MessageBox(_T("Cannot open data output file!"), _T("File Open Error"), MB_OK);
      return;
   }

   // Don't use more buffers than the file size
   if (TotalBlocks < BufCount) {
      BufCount = TotalBlocks;
   }

   // Allocate buffers
   for (Index = 0; Index < BufCount; Index++) {
      memBlocks[Index].inData = (unsigned char *)malloc(BufSize);

      if (memBlocks[Index].inData == NULL) {
         fclose(outputFile);
		 Application->MessageBox(_T("Unable to allocate buffer memory"), _T("Out of Memory"), MB_OK);
         return;
      }
   }

   // Open the QuickUSB device
   result = qusb->Open();

   if (result == 0) {
      fclose(outputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

	  Application->MessageBox(_T("Cannot open QuickUSB module!"), _T("QuickUSB Open Error"), MB_OK);
      return;
   }

   TCursor Save_Cursor = Screen->Cursor;
   Screen->Cursor = crHourGlass;    // Show hourglass cursor
   startTime = timeGetTime();

   // Set the data address
   if (NoIncDataAddressCheckBox->Checked == FALSE) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }

   if (result == FALSE) {
      fclose(outputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      Screen->Cursor = Save_Cursor;
      DisplayError("WriteSetting", -1);
      return;
   }

   displayString("Data read async starting.");

   // Start up several asynchronous read commands
   for (Index = 0; Index < BufCount; Index++) {
      result = qusb->ReadDataAsync(memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         fclose(outputFile);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         Screen->Cursor = Save_Cursor;
         DisplayError("ReadDataAsync", Index);
         return;
      }

      BlocksQueued++;
   }

   Index = 0;

   while (TRUE) {
      // Wait for the next QuickUSB read operation to finish
      memBlocks[Index].bytesTransferred = 0;
      result = qusb->AsyncWait(&memBlocks[Index].bytesTransferred, memBlocks[Index].transaction, FALSE);

      if (result == FALSE) {
         fclose(outputFile);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         Screen->Cursor = Save_Cursor;
         DisplayError("AsyncWait", Index);
         return;
      }

      // Did read function complete?
      if (memBlocks[Index].bytesTransferred == BufSize) {
         length += BufSize;
      }

      // Write the block to disk
      fwrite(memBlocks[Index].inData, 1, BufSize, outputFile);
      BlocksFinished++;

      if (BlocksFinished == TotalBlocks) {
         break;
      }

      // Start a new QuickUSB read
      if (BlocksQueued < TotalBlocks) {
         result = qusb->ReadDataAsync(memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            fclose(outputFile);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

            Screen->Cursor = Save_Cursor;
            DisplayError("ReadDataAsync", Index);
            return;
         }

         BlocksQueued++;
      }

      // Increment index
      Index++;

      if (Index == BufCount) {
         Index = 0;
      }
   }

   // End of Async test
   endTime = timeGetTime();
   fclose(outputFile);
   qusb->Close();

   // Free the buffer memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }

   Screen->Cursor = Save_Cursor;
   displayString("Read " + (String) length + " bytes of data successful.");
   ThroughputCalc(length);
}
//---------------------------------------------------------------------------

void __fastcall TDisplay::WriteAsyncData()
{
   unsigned long  BufSize;
   int            BufCount;
   int            address;
   int            dataLength;
   long           TotalBlocks;
   unsigned long  fileLength = 0;
   FILE *         inputFile;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   FILE *         fh;

   // Get the datalength from the text box
   try {
      address = (DataAddressTextBox->Text).ToInt();
      BufSize = (BufferSizeEditBox->Text).ToInt();
      BufCount = (BufferCountEditBox->Text).ToInt();
   }
   catch ( const EConvertError &e ) {
	  displayString(_T("Invalid input detected"));
      return;
   }

   // This is not a limitation of the QuickUSB, but the I/O will be
   // very slow with a small block size.
   if (BufSize < 64) {
	  displayString(_T("Buffer size must be at least 64 bytes for async I/O!"));
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (BufCount < 2) {
	  displayString(_T("Buffer count must be at least 2 for async I/O!"));
      return;
   }

   // Calculate the data length
   fh = _wfopen(WriteFileEditBox->Text.c_str(), _T("rb"));
   if (fh != NULL) {
	  fseek(fh, 0L, SEEK_END);
	  fileLength = ftell(fh);
	  TotalBlocks = fileLength / BufSize;
	  fclose( fh );
   }
   else {
	  displayString(_T("Unable to compute length of input file."));
      return;
   }

   // Open the input disk file for reading
   inputFile = _wfopen(WriteFileEditBox->Text.c_str(), _T("r+b"));

   if ((inputFile == NULL) || (fileLength == 0)) {
	  Application->MessageBox(_T("Cannot open data input file!"), _T("File Open Error"), MB_OK);
      return;
   }

   // Don't use more buffers than the file size
   if (TotalBlocks < BufCount) {
      BufCount = TotalBlocks;
   }

   // Allocate buffers
   for (Index = 0; Index < BufCount; Index++) {
      memBlocks[Index].inData = (unsigned char *)malloc(BufSize);

      if (memBlocks[Index].inData == NULL) {
         fclose(inputFile);
		 Application->MessageBox(_T("Unable to allocate buffer memory"), _T("Out of Memory"), MB_OK);
         return;
      }
   }

   // Open the QuickUSB device
   result = qusb->Open();

   if (result == 0) {
      fclose(inputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

	  Application->MessageBox(_T("Cannot open QuickUSB module!"), _T("QuickUSB Open Error"), MB_OK);
      return;
   }

   TCursor Save_Cursor = Screen->Cursor;
   Screen->Cursor = crHourGlass;    // Show hourglass cursor
   startTime = timeGetTime();

   // Set the data address
   if (NoIncDataAddressCheckBox->Checked == FALSE) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }

   if (result == FALSE) {
      fclose(inputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      Screen->Cursor = Save_Cursor; // always restore the cursor
      DisplayError("WriteSetting", -1);
      return;
   }

   // Do initial write commands
   displayString("Data write async starting.");

   for (Index = 0; Index < BufCount; Index++) {
      fread(memBlocks[Index].inData, 1, BufSize, inputFile);

      result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         fclose(inputFile);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         Screen->Cursor = Save_Cursor; // always restore the cursor
         DisplayError("WriteDataAsync", Index);
         return;
      }

      BlocksQueued++;
   }

   Index = 0;

   // Wait for current writes to finish, and start up more writes to the QuickUSB
   while (TRUE) {

      // Wait for the next QuickUSB write operation to finish
      result = qusb->AsyncWait(&memBlocks[Index].bytesTransferred, memBlocks[Index].transaction, FALSE);

      if (result == FALSE) {
         fclose(inputFile);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         Screen->Cursor = Save_Cursor; // always restore the cursor
         DisplayError("AsyncWait", Index);
         return;
      }

      length += memBlocks[Index].bytesTransferred;
      BlocksFinished++;

      if (BlocksFinished == TotalBlocks) {
         break;
      }

      // Read another block from disk, and write to the QuickUSB
      if (BlocksQueued < TotalBlocks) {
         fread(memBlocks[Index].inData, 1, BufSize, inputFile);

         result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            fclose(inputFile);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

            Screen->Cursor = Save_Cursor; // always restore the cursor
            DisplayError("ReadDataAsync",Index);
            return;
         }

         BlocksQueued++;
      }

      // Increment index
      Index++;

      if (Index == BufCount) {
         Index = 0;
      }
   }

   // End of Async test
   endTime = timeGetTime();
   fclose(inputFile);
   qusb->Close();

   // Free the memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }

   Screen->Cursor = Save_Cursor; // always restore the cursor
   DataLengthTextBox->Text = length;
   displayString("Write " + (String) length + " bytes of data successful.");
   ThroughputCalc(length);
}
//---------------------------------------------------------------------------


// This function computes the elapsed time and throughput for data reads and writes.
// NOTE See the Microsoft timeGetTime() function documentation for the precision of
// this calculation.
void __fastcall TDisplay::ThroughputCalc(unsigned long dataLength)
{
   double   elapsed_time;
   double   floatLength;
   TCHAR     messageBoxBuffer[500];

   // Do the bandwidth calculation
   elapsed_time = (double)(endTime - startTime) / 1000;
   floatLength = dataLength;
   if (elapsed_time != 0) {
      swprintf(messageBoxBuffer, _T("Elapsed time %g seconds to transfer %g Megabytes.\nAverage throughput %g Megabytes per second."),
         elapsed_time, floatLength / MEGA, (floatLength / MEGA) / elapsed_time);
	  Application->MessageBox(messageBoxBuffer, _T("Throughput"), MB_OK);
   }
}
//---------------------------------------------------------------------------


// This function formats a QuickUSB error message.
void __fastcall TDisplay::DisplayError(char * function, int index) {
   unsigned long errorCode;
   TCHAR messageBoxBuffer[500];

   errorCode = qusb->GetLastError();
   qusb->Close();

   if (index == -1) {
	  swprintf(messageBoxBuffer, _T("QuickUsb%s error %d \n\r"), function, errorCode);
   }
   else {
	  swprintf(messageBoxBuffer, _T("QuickUsb%s error %d index %d\n\r"), function, errorCode, index);
   }

   switch(errorCode)
   {
   case QUICKUSB_ERROR_OUT_OF_MEMORY:
	  wcscat(messageBoxBuffer, _T("A memory allocation function failed while executing a function."));
      break;
   case QUICKUSB_ERROR_CANNOT_OPEN_MODULE:
	  wcscat(messageBoxBuffer, _T("The module was located, but was not able to be opened successfully."));
      break;
   case QUICKUSB_ERROR_CANNOT_FIND_DEVICE:
	  wcscat(messageBoxBuffer, _T("The module could not be located."));
      break;
   case QUICKUSB_ERROR_IOCTL_FAILED:
	  wcscat(messageBoxBuffer, _T("Unable to obtain necessary information from device."));
      break;
   case QUICKUSB_ERROR_INVALID_PARAMETER:
	  wcscat(messageBoxBuffer, _T("Attempting to read or write data of length 0."));
      break;
   case QUICKUSB_ERROR_TIMEOUT:
	  wcscat(messageBoxBuffer, _T("Timeout occured while trying to read or write data."));
	  wcscat(messageBoxBuffer, _T("\nTry calling QuickUsbSetTimeout() with a longer time out."));
      break;
   case QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED:
	  wcscat(messageBoxBuffer, _T("The function you are trying to execute is not supported by this driver version."));
	  wcscat(messageBoxBuffer, _T("\nTry reloading the latest QuickUSB.sys driver file."));
      break;
   default:
	  wcscat(messageBoxBuffer, _T("Unknown error."));
      break;
   }

   Application->MessageBox(messageBoxBuffer, _T("QuickUSB Error"), MB_OK);
}

