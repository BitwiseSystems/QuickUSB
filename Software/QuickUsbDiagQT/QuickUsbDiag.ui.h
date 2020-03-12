/*=============================================================================
 Title        : QuickUsbDiag.ui.h
 Description  : GUI Unit for QuickUsbDiag
 Notes        : None

 Copyright  2003 Bitwise Systems.  All rights reserved.
 This software contains confidential information and trade secrets of
 Bitwise Systems and is protected by United States and international
 copyright laws.  Use, disclosure, or reproduction is prohibited without
 the prior express written permission of Bitwise Systems, except as agreed
 in the QuickUSB Plug-In Module license agreement.

 Use, duplication or disclosure by the U.S. Government is subject to
 restrictions as provided in DFARS 227.7202-1(a) and 227.7202-3(a)
 (1998), and FAR 12.212, as applicable. 

 Bitwise Systems
 5385 Hollister Ave, Suite 215
 Santa Barbara, CA  93111
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

=============================================================================*/

/****************************************************************************
** ui.h extension file, included from the uic-generated form implementation.
**
** If you wish to add, delete or rename functions or slots use
** Qt Designer which will update group file, preserving your code. Create an
** init() function in place of a constructor, and a destroy() function in
** place of a destructor.
*****************************************************************************/

/*------------------------------------------------------------------------------------------------------------------
  NOTES:
 For debugging with LIBUSB, QuickUsb.o must be compiled with the debug flag
 and you must uncomment one line in the init() function in  file which sets the 
 enviornment flag for debugging.
--------------------------------------------------------------------------------------------------------------------*/


#include <iostream>
#include <string>
#include <stdlib.h>
#include <fstream>

#include <qtextedit.h> 
#include <qtimer.h> 
#include <qcombobox.h>
#include <qstatusbar.h>
#include <qfiledialog.h>
#include <qbuttongroup.h> 
#include <qmessagebox.h>
#include <qapplication.h> 
#include <qwidget.h> 
#include <qevent.h> 
#include <qsizepolicy.h>
#include <qtooltip.h> 
#include <qsettings.h> 
#include <qlayout.h> 
#include <math.h>

#include "CQuickUsb.h"
//#include "outputForm.h"
#include "AboutForm.h"
#include "QuickUSB.h"

using namespace std;

// QT Pointers
QPushButton * lastButton = NULL;
QTimer * pauseTimer = NULL, * repeatTimer = NULL, 
* aboutTimer = NULL, * updateModulesTimer = NULL;
QStatusBar * statusBarPtr = NULL;
// Form pointers
AboutForm * aboutFormPtr = NULL;
// QuickUsb pointer
CQuickUsb *qusb = NULL;
// Diag variables
string currentDevice;

int interval;
unsigned char portIOSum[6]; //0 = portADir 1 =portAvalue 2 = portCdir 3 = portCVal 4 = portEdir 5 = portEval
unsigned short timerRegSum[11];
bool isRepeatOn = false, startUp = false;

// Helper functions
void displayString(QString str, int timeOut = 4000);
int StringToByteArray(unsigned char * &theData, string theString);
QString ByteArrayToString(const unsigned char * byteArray, const int length);
QString toHex(const int decimal, const int length);
QString toHex(const int decimal);
bool HexToDecimal (const char* HexNumber, int& Number);
bool HexToDecimal (const char* HexNumber, long& Number);
void setValue(unsigned char portData, QButtonGroup * group, QLineEdit * byteBox, bool setHint = true);
void getValue(unsigned char sum, QButtonGroup * group, QLineEdit * byteBox);
int GetFirmwareVersionString();


#define MEGA (1024 * 1024)

// This data structure is used by the async read and write routines.
typedef struct {
   unsigned long     bytesTransferred;    /* bytes actually transferred */
   unsigned char     transaction;         /* I/O transaction number */
   unsigned char *   inData;              /* buffer pointer */
} MemBlockType;

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::init( ) {
    int lastTab;
    bool okay, wordWide;
    QString previousRbfFile, previousPeriod;

    // This is for debugging LIBUSB. Uncomment this line when needed. 
    // Also QuickUsb.o must be compiled with the -g or -g2 flag  (which is not released this way to the public)
    // setenv("USB_DEBUG", "2", 1);

    // initialize variables
    for (int i = 0; i < 6; i++) {
       portIOSum[i] = 0;   
    }
    // initialize timers 
    updateModulesTimer = new QTimer(this); 
    pauseTimer = new QTimer(this);
    repeatTimer = new QTimer(this);
    aboutTimer = new QTimer(this);
    statusBarPtr = statusBar();
    connect( updateModulesTimer, SIGNAL(timeout()), this, SLOT( UpdateModuleComboBox() )); 
    connect( pauseTimer, SIGNAL(timeout()), this, SLOT( pause() ));   	
    connect( repeatTimer, SIGNAL(timeout()), this, SLOT( pressButton() ));  
    connect( aboutTimer, SIGNAL(timeout()), this, SLOT( aboutOnTop() ));  

    // Check if new command was successful
    if (updateModulesTimer == NULL || pauseTimer == NULL || repeatTimer == NULL || aboutTimer == NULL) {
       displayString("Could not allocate memory");
       cerr << "Could not allocate memory\n";
       return;
    }

   progressBar->setPercentageVisible(TRUE);
   progressBar->setProgress(0);
   bufferCountLabel->hide();
   bufferSizeLabel->hide();
   bufferCountTextBox->hide();
   bufferSizeTextBox->hide();
   DataLengthTextBox->setText("65536");

    // lock the gui size
    statusBar()->setSizeGripEnabled(false);
    this->setFixedSize( frameSize() );

    CommandFlexGrid->setColumnWidth(0, 80);
    CommandFlexGrid->setColumnWidth(1, 80);
    CommandFlexGrid->setTopMargin(0);
    CommandFlexGrid->setLeftMargin(0);

    // Initialize default values of the flex grid
    QString temp;
    for (int i = 0; i < 8; i++) {
       CommandFlexGrid->setText(i, 1, "0");	      
       temp.setNum(i+1);
       CommandFlexGrid->setText(i, 0, temp);
    }

    updateModulesTimer->start(2000, false);
    repeatTimer->start(100, false);

   // Retrieve user information from the last run of the QuckUSB Diag  (similar to windows registry)
   QSettings settings(QSettings::Native);
   settings.setPath( "BitwiseSys.com", "QuickUSBDiag", QSettings::User );
   //settings.insertSearchPath( QSettings::Unix, QDir::currentDirPath() );

   // Get the path of the last RBF file uploaded
   previousRbfFile = settings.readEntry( "/QuickUsb/uploadFile", "", &okay );
   if (okay) {
      UploadFilenameTextBox->setText(previousRbfFile);
   }
   else {
      UploadFilenameTextBox->setText("Please select a file to upload");
    }
   // Get the last interval for repeat functionality
   previousPeriod = settings.readEntry( "/QuickUsb/period","",&okay);
   if (okay) {
      intervalTextBox->setText(previousPeriod);
   }
   else {
      intervalTextBox->setText("500");
    }

   // Get status of wordWide setting
   wordWide = settings.readBoolEntry("/QuickUsb/wordWide", true, &okay);
   if (okay) {
      WordWideCheckBox->setChecked(wordWide);
   }
   else {
     WordWideCheckBox->setChecked(true);
   }

   // Get the last tab the user had active 
   lastTab = settings.readNumEntry("/QuickUsb/tab", 0, &okay);
   if (okay) {
      if (lastTab == 1) {
         startUp = true;
       }
      PageControl->showPage ( PageControl->page(lastTab) );
   }
   else {   
      PageControl->showPage (0);
   }
}

/*------------------------------------------------------------------------------------*/

void  QuickUsbDisplay::UpdateModuleComboBox() {
    char *str = NULL;
    char nameList[256];
    int result;
    unsigned long length = sizeof(nameList);

    // Get a list of the connected modules
    result = qusb->FindModules(nameList, length);

    // If we can't get the list or the list is empty then disable all the controls and exit. Also delete qusb object
    if ( (result == 0 || nameList[1] == ' ')  ) {
      // Disable the GUI tabs
      PageControl->setTabEnabled (PageControl->page (1), false);
      PageControl->setTabEnabled (PageControl->page (2), false);
      PageControl->setTabEnabled (PageControl->page (3), false); 
      PageControl->setTabEnabled (PageControl->page (4), false); 
      PageControl->setTabEnabled (PageControl->page (5), false); 
      PageControl->setTabEnabled (PageControl->page (6), false); 
      // disable page 0 last so page 0 will be shown on screen
      PageControl->setTabEnabled (PageControl->page (0), false); 
      // disable the repeat check box and interval text box
      repeatLastButtonCheckBox->setEnabled(false);
      intervalTextBox->setEnabled(false);

      //  Remove old data from the combo box and text field
      moduleComboBox->clear();
      stringDescriptorTextBox->setText("");
      displayString("Cannot find any QuickUSB modules");

      // Delete qusb object and set pointer to NULL
      if (qusb != NULL) {
         delete qusb;
         qusb = NULL;
     }

     // Since everything is being disabled startup should be false so when modules are found at the 
    // general tab the output form will not be shown
       startUp = false;
       return;
    }

    // If the list hasn't changed, just exit
    str = nameList;
    bool foundNew = foundNewModules( str );
    if (  ! ( foundNew ) && stringDescriptorTextBox->text() != "" ){
       return;
    }

    // Clear the list and start appending the item(s) to the empty list
    moduleComboBox->clear();
    str = nameList;
    while (*str != '\0') {
       moduleComboBox->insertItem(str, -1);
       str = str + strlen(str) + 1;
    }

    // Select the first  QUSB module from the combo box and attempt to open it
    currentDevice = (moduleComboBox->text(0)).ascii();
    stringDescriptorTextBox->setText("");
    qusb = new CQuickUsb(currentDevice.c_str());
    if (qusb == NULL) {
       displayString("Could not allocate memory");
       return;
    }

    // Enable the GUI tabs if they are disabled
    if (PageControl->isTabEnabled (PageControl->page (0) ) == false) {
       PageControl->setTabEnabled (PageControl->page (0), true ); 
       PageControl->setTabEnabled (PageControl->page (1), true );
       PageControl->setTabEnabled (PageControl->page (2), true );
       PageControl->setTabEnabled (PageControl->page (3), true ); 
       PageControl->setTabEnabled (PageControl->page (4), true ); 
       PageControl->setTabEnabled (PageControl->page (5), true );
       repeatLastButtonCheckBox->setEnabled(true);
       intervalTextBox->setEnabled(true);
    }

    // Get the string descriptor
    currentDevice = "";
    moduleComboBox_activated((moduleComboBox->text(0)).ascii());

    // Clean up parts of the GUI (clearing text boxes, fetching fresh data)
    cleanUp();
}


/*------------------------------------------------------------------------------------------*/  

int GetFirmwareVersionString() {
    int result, number;
    char description[256];
    string str;

    // Get the string descriptor from the module
    result = qusb->GetStringDescriptor(2, description, sizeof(description));

    // Check the transaction result
    if (result == 0) {
       displayString("Could not get firmware version");
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
/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PageControl_currentChanged( QWidget *) {
#if 0    
    // If the current tab selected is not Command and Data close the output form
    if ( PageControl->currentPageIndex() != 1) {
       if (outputFormPtr != NULL) {
          delete outputFormPtr;
          outputFormPtr = NULL;
       }

   // Just in case user quickly changes tabs on program initialization.
   startUp = false;
    }  
    // otherwise create it and fix its position to the right of the main window
    else { 
       outputFormPtr = new outputForm();
       if (outputFormPtr == NULL) {
          displayString("Could not allocate memory");
          return;
       }
       if (!startUp) {
          outputFormPtr->showForm(this->x() + this->width() + 7, this->y(), this->height(), &outputFormPtr);
       }
    }
#endif
}

/*------------------------------------------------------------------------------------
GENERAL TAB CODE
-------------------------------------------------------------------------------------*/

void  QuickUsbDisplay::fpgaUploadButton_clicked( ){ 
    int result, index = 0, length = 0, blockLength = 64, count = 1;
    unsigned char fpgaData[64];
    FILE * input;
    unsigned short isConfigured;
    long fileSize;
    QString str;

    if (UploadFilenameTextBox->text() == "" || UploadFilenameTextBox -> text() == "Please select a file to upload" ) {
       UploadFilenameTextBox -> setText("");
       browseButton_clicked();                         // Display the browse box
       if (UploadFilenameTextBox->text() == ""){ // The user selected Cancel
          UploadFilenameTextBox->setText("Please select a file to upload");
          return;
       }
    }

    // Open the input file for reading
    input = fopen( (UploadFilenameTextBox->text()).ascii() , "rb+");

    if (input == NULL) {
       displayString("There was an error opening the desired file");
       return;
    }

    // Get the file size
    fseek (input, 0L, SEEK_END);
    fileSize = ftell(input);
    fseek (input, 0L, SEEK_SET);

    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString( "Cannot open Device:" + currentDevice);
       return;
    }

    // Set the FPGA type for this download
    result = qusb->WriteSetting(SETTING_FPGATYPE, FpgaTypeComboBox->currentItem());
    if (result == 0) {
       displayString("Cannot set FPGA type");
       fclose(input);
       qusb->Close();
       return;
    }

    // Start configuring the FPGA
    result = qusb->StartFpgaConfiguration();
    if (result == 0) {
       displayString("Cannot start FPGA configuration");
       fclose(input);
       qusb->Close();
       return;
    }

    // Send the FPGA configuration data one block at a time
    progressBar->setProgress(0);

    do {
      // Read the data
      length = min(blockLength, (int) (fileSize - index) );

     // Read the data from the file and put it into fpgaData array
     result = fread(fpgaData, length, count, input);

     // Write the data to the EEPROM
     displayString("Configuring FPGA..." +  str.setNum(progressBar->progress()) + "%");
     result = qusb->WriteFpgaData(fpgaData, length);

     if (result == 0) {
        fclose(input);
        progressBar->setProgress(0);
        qusb->Close();
        displayString("FPGA configuration failed");
        return;
     }

     // Update the counters
     index = index + length;

     // Update the progress bar
    progressBar->setProgress((index * 100) / fileSize);	
    }
    while (index < fileSize);

    // Close the device when you're done
    qusb->Close();

    // Close the file
    fclose(input);

    // Start configuring the FPGA
    result = qusb->IsFpgaConfigured(&isConfigured);
    if (result == 0) {
       fclose(input);
       progressBar->setProgress(0);
       displayString("Attempt to get configuration status failed");
       return;
    }

    progressBar->setProgress(0);

    // Notify the user
    if (isConfigured == 0) {
       displayString("FPGA configuration failed");
    }
    else {
       displayString("FPGA was successfully configured");
    }

    return;
}
/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::moduleComboBox_activated( const QString & str) {
    QString temp;
    unsigned short length;
    int result = 0;
    char * deviceName = new char[ str.length() ];
    unsigned short VersionSpeed = 0;
    unsigned short fpgaType = 0;
    char deviceDescriptor[256];

    if (deviceName == NULL) {
       displayString("Could not allocate memory\n");
       return;
    }
    strcpy(deviceName, str.ascii() );

    // if the selected device has changed
    if (str != currentDevice) {
      qusb->SetName(deviceName);
      delete deviceName;

     currentDevice = str.ascii();

     // Open the device
     result = qusb->Open();	
     if (result == false) {
        displayString("QuickUsbOpen failed");
        return;
     }

     // Get the string descriptor
     qusb->GetStringDescriptor(QUICKUSB_MODEL, deviceDescriptor, sizeof(deviceDescriptor));
     if (result == 0) {
        displayString("Get string descriptor failed for:" + currentDevice);
        qusb->Close();
        return;
    }

     stringDescriptorTextBox->setText(deviceDescriptor);

     // Get the serial number
     qusb->GetStringDescriptor(QUICKUSB_SERIAL, deviceDescriptor, sizeof(deviceDescriptor));
     if (result == 0) {
        displayString("Get string descriptor failed for:" + currentDevice);
        qusb->Close();
        return;
    }

     SerialNumberTextBox->setText(deviceDescriptor);

      // Get USB bus speed and hardware revision.
      // NOTE This feature was implemented with QuickUSB firmware 2.11.
      result = qusb->ReadSetting(SETTING_VERSIONSPEED, &VersionSpeed);
      if (result == FALSE) {
         UsbSpeedTextBox->setText("");
         HwVersionTextBox->setText("");
      }
      else {
         // The low order byte of SETTING_VERSIONSPEED is zero for Full Speed,
         // and non zero for High Speed.
         if (VersionSpeed & 0xFF) {
            UsbSpeedTextBox->setText("High Speed (480Mbps)");
         }
         else {
            UsbSpeedTextBox->setText("Full Speed (12Mbps)");
         }

         // The high order byte of SETTING_VERSIONSPEED is the Cypress hardware version.
         // See the REVID register description in the TRM.  The following values are
         // defined as of this writing.
         switch ((VersionSpeed & 0xFF00) >> 8) {
         case 0:
            HwVersionTextBox->setText("CY7C68013 Rev A/B (EZ-USB FX2)");
            break;
         case 1:
            HwVersionTextBox->setText("CY7C68013A Rev A (EZ-USB FX2LP)");
            break;
         case 2:
            HwVersionTextBox->setText("CY7C68013 Rev C/D (EZ-USB FX2)");
            break;
         case 4:
            HwVersionTextBox->setText("CY7C68013 Rev E (EZ-USB FX2)");
            break;
         default:
            HwVersionTextBox->setText("Unknown");
         }
      }

     result = qusb->ReadSetting(SETTING_FPGATYPE, &fpgaType);
     if (result) {
        FpgaTypeComboBox->setCurrentItem(fpgaType);
     }

     RS232PortComboBox->setCurrentItem(0);
     result = qusb->GetNumRS232(0, &length);

     // Close the device when you're done
     qusb->Close();

     // Check the transaction result
     if (result == 0) {
        displayString("Cannot get number of RS232 bytes outstanding");
        return;
     }

     // Update the length
     temp.setNum(length);
     GetNumTextBox->setText(temp);
     cleanUp();
    }
    displayString("Selected " + currentDevice);    
    return;
}

/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::browseButton_clicked(){
    QString filename = "";
    filename = QFileDialog::getOpenFileName( "/home", "Raw Binary File(*.rbf);;All files(*.*)", this, "openfiledialog","Chooseafile");
    if (filename == "") { //user hit cancel
       return;
    }

    UploadFilenameTextBox->setText(filename);  
}

/*------------------------------------------------------------------------------------
  COMMAND AND DATA TAB
  ----------------------------------------------------------------------------------*/
void QuickUsbDisplay::WordWideCheckBox_clicked() {
    int result;
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Set the new setting value
    if (WordWideCheckBox->isChecked() == true) {
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

void QuickUsbDisplay::CommandReadButton_clicked() {
    unsigned short length;
    int commandAddress, result, commandLength;
    unsigned char * commandData;
    QString temp;
    bool wordWide, okay;
    
    // Remember the last buton clicked
    if ( lastButton != CommandReadButton) {
       if (isRepeatOn) {
          resetTimers(CommandReadButton);
        }
       else {           
          lastButton = CommandReadButton;
       }
    }
    
    // Set the command address increment flag if needed
    if (CommandAddressCheckBox->isChecked() == true) {
       commandAddress = commandAddress & 0x8000;
    }
    else {
       commandAddress = commandAddress & 0x7FFF;
    }
    
    // Handle wordwide
    if (WordWideCheckBox->isChecked() == true) {
       wordWide = true;
    }
    else {
      wordWide = false;
    }
    
    commandAddress = (CommandAddressTextBox->text()).toInt(&okay);
    // if it didn't return success, maybe the value is in HEX
    if ( !okay ) {
       okay =  HexToDecimal(CommandAddressTextBox->text(), commandAddress);     
    }   
    if ( !okay) {
       displayString("Invalid input entered in address box");
       return;
    }
    commandLength = (CommandLengthTextBox->text()).toInt(&okay);
    if ( !okay) {
       okay =  HexToDecimal(CommandLengthTextBox->text(), commandLength);
    }
    if ( !okay) {
       displayString("invalid input in length box");
       return;
    }
    
    // if wordwide is on set to proper length
    if (wordWide) {
       length = commandLength * 2;
    }
    else {
       length = commandLength;
    }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Allocate storage for incoming data
    commandData = new unsigned char[length];
    if (commandData == NULL) {
       displayString("Could not allocate memory\n");
       qusb->Close();
       return;
    }
    if (wordWide) {
       displayString("Reading " +  temp.setNum(commandLength) + " words of command...");  
    }
    else {
       displayString("Reading " +  temp.setNum(commandLength) + " bytes of command...");  
    }
    
    // read the data
    result = qusb->ReadCommand(commandAddress, commandData, &length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       delete [] commandData;
       return;
    }
    
    // Fill the array with data read from quickUSB
    for (int i = 0; i < commandLength; i++) {
       if (WordWideCheckBox->isChecked() ) {
          temp.setNum((commandData[i * 2] + (0x100 * commandData[ (i * 2) + 1] )));
          CommandFlexGrid->setText(i, 1, temp);
      }
      else{
          temp.setNum( commandData[i] );
          CommandFlexGrid->setText(i, 1, temp );
      }
    }

    delete [] commandData;
    return;
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::CommandWriteButton_clicked() {
    unsigned char * commandData;
    int commandAddress, result, length, commandLength, cellValue;
    bool wordWide, okay;
    QString temp;
    
    // Remember the last buton clicked
    if ( lastButton != CommandWriteButton) {
       if (isRepeatOn) {
          resetTimers(CommandWriteButton);
       }
      else {
         lastButton = CommandWriteButton;
      }
    }
    
    // Attempt to extract data from GUI
    commandAddress = (CommandAddressTextBox->text()).toInt( &okay );
    // If it failed, maybe the value is in HEX
    if (!okay) {
       okay = HexToDecimal(CommandAddressTextBox->text(), commandAddress);
    }   
    // Both attemps resulted in failure output message and return
    if ( !okay) {
       displayString("Invalid input in address box");
       return;
    }
    
    commandLength =  (CommandLengthTextBox->text() ).toInt(&okay);
    if (!okay) {
       okay =  HexToDecimal(CommandLengthTextBox->text(), commandLength);
    }
    if ( !okay) {
       displayString("Invalid input in length box");
       return;
    }
    
    // Set the command address increment flag if needed
    if (CommandAddressCheckBox->isChecked() ) {
       commandAddress = commandAddress & 0x7FFF;
    }
    else {
       commandAddress = commandAddress | 0x8000;
    }
    
    // Handle wordwide
    if (WordWideCheckBox->isChecked() ) {
       wordWide = true;
    }
    else {
       wordWide = false;
    }
    
    // If word wide is selected fix the length
    if (wordWide ) {
       length = commandLength * 2;
    }
    else {
       length = commandLength;
    }
    
    // Allocate storage
    commandData = new unsigned char[length];
    if (commandData == NULL) {
       displayString("Could not allocate memory");
       return;
    }
    
    
    // Fill the data array with the contents of the matrix
    for (int i = 0; i < commandLength; i++) {
       // Get the value from the matrix, 
       cellValue = (CommandFlexGrid->text(i, 1)).toInt(&okay);
       // If attempt to convert to int failed, check to see if it is in hex
       if (!okay) {
          okay = HexToDecimal( CommandFlexGrid->text(i, 1), cellValue);
       }
       // Both attemps resulted in failure output message and return
       if (!okay) {
          displayString("Invalid input detected");
          delete [] commandData;
          return;
       }
       if (wordWide) {
          commandData[i * 2] = cellValue & 0xFF;
          commandData[ (i * 2) + 1] = ((cellValue & 0xFF00) / 0x100);
       }
       else {
          commandData[i] = cellValue & 0xFF;
       }
    }
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       delete [] commandData;
       return;
   }
    
    // Write the data
    if (wordWide) {
       displayString("Writing " + temp.setNum(commandLength) + " words of command...");
    }
    else {
       displayString("Writing " + temp.setNum(commandLength) + " bytes of command...");
    }
    result = qusb->WriteCommand(commandAddress, commandData, length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot write command");
       delete [] commandData;
       return;
    }
    
    // Free the memory
    delete [] commandData;
}


/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::CommandLengthTextBox_textChanged( const QString & ) {
    bool okay;
    QString temp;
    int commandLength;

    if ( (CommandLengthTextBox->text()).length() > 0) {
       commandLength = (CommandLengthTextBox->text()).toInt(&okay);
       if (!okay) {
          okay = HexToDecimal(CommandLengthTextBox->text(), commandLength);
      }
      if (!okay) {
         displayString("Invalid input detected in length text box");
         return;
     }

     if ( commandLength > 0 && commandLength < 65 ) {
        // If the length is less then 7 then resize the columns in the flex grid 
        if (commandLength < 7) {
           CommandFlexGrid->setColumnWidth(0, 88);
           CommandFlexGrid->setColumnWidth(1, 88);
        }
        CommandFlexGrid->setNumRows(commandLength);
        for (int i = 0; i < commandLength; i++) {
           temp.setNum(i + 1);
           CommandFlexGrid->setText(i, 0, temp);
           CommandFlexGrid->setText(i, 1, "0");
       }
    }
    else {
       displayString("The maximum length is 64 bytes");
       CommandLengthTextBox->setText("64");
       commandLength = 64;
     }
  }
}

// This code processes async data reads.
void QuickUsbDisplay::AsyncDataRead()
{
   long           sBufSize;
   unsigned long  BufSize;
   int            BufCount;
   int            address;
   long           TotalBlocks;
   long           dataLength;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   bool           okay;

    // Get the data address datalength from GUI
    address = ( DataAddressTextBox->text() ).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(DataAddressTextBox->text(), address );
    }
    // Both attemps resulted in failure output message and return
    if (!okay) {
       displayString("Invalid input detected in address box");
       return;
    }

    dataLength = (DataLengthTextBox->text() ).toInt(&okay);
    if (!okay) {
       okay = HexToDecimal(DataLengthTextBox->text(), dataLength);
    }
    if (!okay) {
       displayString("Invalid input detected in length box");
       return;
    }

   if (dataLength <= 0) {
      displayString("Length must be greater than zero!");
      return;
   }

    sBufSize = (bufferSizeTextBox->text()).toInt(&okay);
    if (!okay) {
      okay = HexToDecimal(bufferSizeTextBox->text(), sBufSize);
    }
    if (!okay) {
      displayString("Invalid input detected in buffer size text box");
      return;
    }
    BufSize = sBufSize;

    BufCount = (bufferCountTextBox->text()).toInt(&okay);
    if (!okay) {
      okay = HexToDecimal(bufferCountTextBox->text(), BufCount);
    }
    if (!okay) {
      displayString("Invalid input detected in buffer count text box");
      return;
   }

   // This is not a limitation of the QuickUSB, but the I/O will be
   // very slow with a small block size.
   if (BufSize < 64) {
      displayString("Buffer size must be at least 64 bytes for async I/O!");
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (BufCount < 2) {
      displayString("Buffer count must be at least 2 for async I/O!");
      return;
   }

   TotalBlocks = dataLength / BufSize;

   // This is not a limitation of the QuickUSB, but it makes this demo
   // code much simpler.
   if ((TotalBlocks * sBufSize) != dataLength) {
      displayString("Data Length must be a multiple of Buffer Size");
      return;
   }

   QFile DataFile(ReadFileTextBox->text());
   okay = DataFile.open(IO_WriteOnly);
   if (okay == FALSE) {
      QMessageBox::critical(this, "File Open Error", "Cannot open data output file!");
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
         DataFile.close();
         QMessageBox::critical(this, "Out of Memory", "Unable to allocate buffer memory");
         return;
      }
   }

   // Open the QuickUSB device
   result = qusb->Open();

   if (result == 0) {
      DataFile.close();

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      QMessageBox::critical(this, "QuickUSB Open Error", "Cannot open QuickUSB module!");
      return;
   }

   // Set the data address
   if (DataAddressCheckBox->isChecked() == false) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }

   if (result == FALSE) {
      DataFile.close();

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      DisplayError("WriteSetting", -1);
      return;
   }

   displayString("Data read async starting.", TRUE);
   setCursor(WaitCursor);

   // Start up several asynchronous read commands
   for (Index = 0; Index < BufCount; Index++) {
      result = qusb->ReadDataAsync(memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         DataFile.close();
         setCursor(ArrowCursor);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

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
         DataFile.close();
         setCursor(ArrowCursor);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         DisplayError("AsyncWait", Index);
         return;
      }

      // Did read function complete?
      if (memBlocks[Index].bytesTransferred == BufSize) {
         length += BufSize;
      }

      // Write the block to disk
      DataFile.writeBlock((const char *)(memBlocks[Index].inData), BufSize);
      BlocksFinished++;

      if (BlocksFinished == TotalBlocks) {
         break;
      }

      // Start a new QuickUSB read
      if (BlocksQueued < TotalBlocks) {
         result = qusb->ReadDataAsync(memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            DataFile.close();
            setCursor(ArrowCursor);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

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
   DataFile.close();
   qusb->Close();
   setCursor(ArrowCursor);

   // Free the buffer memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }

   displayString("Data read successful.");
}


// This code processes async data writes.
void QuickUsbDisplay::AsyncDataWrite()
{
   long           BufSize;
   int            BufCount;
   int            address;
   long           TotalBlocks;
   unsigned long  fileLength = 0;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   bool           okay;
   QString        temp;

    // Get the data address datalength from GUI
    address = ( DataAddressTextBox->text() ).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(DataAddressTextBox->text(), address );
    }
    // Both attemps resulted in failure output message and return
    if (!okay) {
       displayString("Invalid input detected in address box");
       return;
    }

    BufSize = (bufferSizeTextBox->text()).toInt(&okay);
    if (!okay) {
      okay = HexToDecimal(bufferSizeTextBox->text(), BufSize);
    }
    if (!okay) {
      displayString("Invalid input detected in buffer size text box");
      return;
   }

    BufCount = (bufferCountTextBox->text()).toInt(&okay);
    if (!okay) {
      okay = HexToDecimal(bufferCountTextBox->text(), BufCount);
    }
    if (!okay) {
      displayString("Invalid input detected in buffer count text box");
      return;
   }

   // This is not a limitation of the QuickUSB, but the I/O will be
   // very slow with a small block size.
   if (BufSize < 64) {
      displayString("Buffer size must be at least 64 bytes for async I/O!");
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (BufCount < 2) {
      displayString("Buffer count must be at least 2 for async I/O!");
      return;
   }

   QFile DataFile(WriteFileTextBox->text());
   okay = DataFile.open(IO_ReadOnly);
   if (okay == FALSE) {
      QMessageBox::critical(this, "File Open Error", "Cannot open data input file!");
      return;
   }

   // Calculate the data length
   fileLength = DataFile.size();
   if (fileLength == 0) {
      DataFile.close();
      displayString("Attempt to read an empty file!");
      return;
   }
   TotalBlocks = fileLength / BufSize;

   // Don't use more buffers than the file size
   if (TotalBlocks < BufCount) {
      BufCount = TotalBlocks;
   }

   // Allocate buffers
   for (Index = 0; Index < BufCount; Index++) {
      memBlocks[Index].inData = (unsigned char *)malloc(BufSize);

      if (memBlocks[Index].inData == NULL) {
         DataFile.close();
         QMessageBox::critical(this, "Out of Memory", "Unable to allocate buffer memory");
         return;
      }
   }

   // Open the QuickUSB device
   result = qusb->Open();

   if (result == 0) {
      DataFile.close();

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      QMessageBox::critical(this, "QuickUSB Open Error", "Cannot open QuickUSB module!");
      return;
   }

   // Set the data address
   if (DataAddressCheckBox->isChecked() == false) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }

   if (result == FALSE) {
      DataFile.close();

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      DisplayError("WriteSetting", -1);
      return;
   }

      // Do initial write commands
   displayString("Data write async starting.");
   setCursor(WaitCursor);

   for (Index = 0; Index < BufCount; Index++) {
      DataFile.readBlock((char *)(memBlocks[Index].inData), BufSize);

      result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         DataFile.close();
         setCursor(ArrowCursor);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

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
         DataFile.close();
         setCursor(ArrowCursor);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

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
         DataFile.readBlock((char *)(memBlocks[Index].inData), BufSize);
         result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            DataFile.close();
            setCursor(ArrowCursor);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

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
   DataFile.close();
   qusb->Close();
   setCursor(ArrowCursor);

   // Free the memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }

   temp.setNum(fileLength);
   DataLengthTextBox->setText(temp);
   displayString("Data write successful.");
}



/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::DataReadButton_clicked() {
    int result, dataAddress, dataLength;
    unsigned long length;
    QString temp;
    bool okay;
    unsigned char *   inData;

    // Remember the last buton clicked
    if ( lastButton != DataReadButton) {
       if (isRepeatOn) {
          resetTimers(DataReadButton);
       }
       else {
          lastButton = DataReadButton;
      }
    }

    // Get the data address datalength from GUI
    dataAddress = ( DataAddressTextBox->text() ).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(DataAddressTextBox->text(), dataAddress );
    }
    // Both attemps resulted in failure output message and return
    if (!okay) {
       displayString("Invalid input detected in address box");
       return;
    }

    dataLength = (DataLengthTextBox->text() ).toInt(&okay);
    if (!okay) {
       okay = HexToDecimal(DataLengthTextBox->text(), dataLength);
    }
    if (!okay) {
       displayString("Invalid input detected in length box");
       return;
    }

   if (dataLength <= 0) {
      displayString("Length must be greater than zero!");
      return;
   }

   length = dataLength;

   // Check for an empty filename
   if (ReadFileTextBox->text() == "") {
      ReadFileBrowseButton_clicked();                     // Display the browse box
      if (ReadFileTextBox->text() == "") {     // The user selected Cancel
         return;
      }
   }

   // Check for asynchronous read request.
   if (asyncCheckBox->isChecked()) {
      AsyncDataRead();
      return;
   }

   // Allocate a data buffer
   inData = (unsigned char *)malloc(length);
   if (inData == NULL) {
      displayString("Unable to allocate a memory buffer!");
      return;
   }

   QFile DataFile(ReadFileTextBox->text());
   okay = DataFile.open(IO_WriteOnly);
   if (okay == FALSE) {
      free(inData);
      QMessageBox::critical(this, "File Open Error", "Cannot open data output file!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      free(inData);
      DataFile.close();
      displayString("Cannot open Device:" + currentDevice);
      return;
   }

    // Set the data address
    if (DataAddressCheckBox->isChecked() == false) {
       result = qusb->WriteSetting(SETTING_DATAADDRESS, (dataAddress | 0x8000) );
    }
    else{
       result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress);
    }

    // Check the transaction result
    if (result == 0) {
       displayString("Cannot write data address setting");
       qusb->Close();
      free(inData);
      DataFile.close();
       return;
    }

   displayString("Data read starting.");
   setCursor(WaitCursor);
   result = qusb->ReadData(inData, &length);
   if (result == FALSE) {
      free(inData);
      DataFile.close();
      qusb->Close();
      setCursor(ArrowCursor);
      displayString("QuickUsb ReadData error");
      return;
   }

   DataFile.writeBlock((const char *)inData, length);
   free(inData);
   DataFile.close();
   qusb->Close();
   setCursor(ArrowCursor);
   displayString("Data read successful.");


}

/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::DataWriteButton_clicked() {
    int result, dataAddress;
    QString temp;
    bool okay;
    unsigned char * inData;
    unsigned long length;

    // Remember the last buton clicked
    if ( lastButton != DataWriteButton) {
       if (isRepeatOn) {
          resetTimers(DataWriteButton);
       }
       else {
          lastButton = DataWriteButton;
      }
   }

    // Get the data address from the text box
    dataAddress = ( DataAddressTextBox->text() ).toInt(&okay);
    // Maybe it is in Hex
    if (!okay) {
       okay = HexToDecimal(DataAddressTextBox->text(), dataAddress );
    }
    // Both attemps resulted in failure output message and return
    if (!okay) {
       displayString("Invalid input detected in address box");
       return;
    }

   // Check for an empty filename
   if (WriteFileTextBox->text() == "") {
      WriteFileBrowseButton_clicked();                     // Display the browse box
      if (WriteFileTextBox->text() == "") {     // The user selected Cancel
         return;
      }
   }

   // Check for asynchronous write request.
   if (asyncCheckBox->isChecked()) {
      AsyncDataWrite();
      return;
   }

   QFile DataFile(WriteFileTextBox->text());
   okay = DataFile.open(IO_ReadOnly);
   if (okay == FALSE) {
      QMessageBox::critical(this, "File Open Error", "Cannot open data input file!");
      return;
   }

   // Calculate the data length
   length = DataFile.size();
   if (length == 0) {
      DataFile.close();
      displayString("Attempt to read an empty file!");
      return;
   }

   // Allocate a data buffer
   inData = (unsigned char *)malloc(length);
   if (inData == NULL) {
      DataFile.close();
      displayString("Unable to allocate a memory buffer!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      DataFile.close();
      free(inData);
      QMessageBox::critical(this, "QuickUSB Open Error", "Cannot open QuickUSB module!");
      return;
   }

   // Set the data address
   if (DataAddressCheckBox->isChecked() == false) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, dataAddress);
   }

   if (result == FALSE) {
      free(inData);
      DataFile.close();
      QMessageBox::critical(this, "QuickUSB Error", "Cannot set data address!");
      qusb->Close();
      return;
   }

   displayString("Data write starting.");
   setCursor(WaitCursor);
   DataFile.readBlock((char *)inData, length);
   result = qusb->WriteData(inData, length);
   if (result == FALSE) {
      free(inData);
      DataFile.close();
      setCursor(ArrowCursor);
      QMessageBox::critical(this, "QuickUSB Error", "Cannot write QuickUSB data!");
      qusb->Close();
      return;
   }

   free(inData);
   DataFile.close();
   qusb->Close();
   setCursor(ArrowCursor);
   temp.setNum(length);
   DataLengthTextBox->setText(temp);
   displayString("Data write successful.");
}




/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::DataCountButton_clicked() {
    int result, dataAddress;
    unsigned char * testData;
    long dataLength;
    QString temp;
    bool okay;

    // Remember the last buton clicked
    if ( lastButton != DataCountButton) {
       if (isRepeatOn) {
          resetTimers(DataCountButton);
       }
       else {
          lastButton = DataCountButton;
      }
   }

    // Get the datalength from the text box
    dataAddress = ( DataAddressTextBox->text() ).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(DataAddressTextBox->text(), dataAddress );
    }
    // Both attemps resulted in failure output message and return
    if (!okay) { 
       displayString("Invalid input detected in address box");
       return;
    }

    dataLength = (DataLengthTextBox->text() ).toInt(&okay);
    if (!okay) {
       okay = HexToDecimal( (DataLengthTextBox->text()).ascii() , dataLength);
    }
    if (!okay) {
       displayString("Invalid input detected in length box");
       return;
    }

    // Allocate storage for writeDataLength elements
    testData = new unsigned char[dataLength];
    if (testData == NULL) {
        displayString("Could not allocate memory");
        return;
    }

    // Fill the array with a count
    for (long i = 0; i < dataLength; i++) {
       testData[i] = i & 0xFF;
    }

    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       delete [] testData;
       return;
    }

    // Set the data address
    if (DataAddressCheckBox->isChecked() == true) {
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
    displayString("Writing " + temp.setNum(dataLength) + " words of data...");
    result = qusb->WriteData( testData, dataLength);

    // Close the device when you're done
    qusb->Close();

    // Check the transaction result
    if (result == 0) {
      displayString("Cannot write count data");
      delete [] testData;
      return;
    }

    // Free the memory
    delete [] testData;
}

/*------------------------------------------------------------------------------------
  PORT I/O TAB CODE
  ----------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortAReadButton_clicked() {
    int result;
    unsigned char portData = 0;
    unsigned short length = 1;
    
    // Remember the last buton clicked
    if ( lastButton != PortAReadButton ) {
       if (isRepeatOn) {
          resetTimers(PortAReadButton );
      }
     else {
        lastButton = PortAReadButton;
    }
 }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Read the port data
    displayString("Reading port A...");
    result = qusb->ReadPort( 0, &portData, &length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port A");
       return;
    }
    
    // Display the hex value
    portIOSum[1] = portData;
    setValue(portIOSum[1], PortAValueGroupBox, PortAValueBox);
}
/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortCReadButton_clicked() {
    int result;
    unsigned char portData = 0;
    unsigned short length = 1;
    
    // Remember the last buton clicked
    if ( lastButton != PortCReadButton ) {
       if (isRepeatOn) {
          resetTimers(PortCReadButton );
       }
       else {
          lastButton = PortCReadButton;
      }
   }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Read the port data
    displayString("Reading port C...");
    result = qusb->ReadPort( 2, &portData, &length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port C");
       return;
    }
    
    // Display the hex value
    portIOSum[3] = portData;
    setValue(portIOSum[3], PortCValueGroupBox,  PortCValueBox);
}

/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::PortEReadButton_clicked() {
    int result;
    unsigned char portData = 0;
    unsigned short length = 1;
    
    // Remember the last buton clicked
    if ( lastButton != PortEReadButton ) {
       if (isRepeatOn) {
          resetTimers(PortEReadButton );
       }
       else {
          lastButton = PortEReadButton;
       }
    }
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Read the port data
    displayString("Reading port E...");
    result = qusb->ReadPort( 4, &portData, &length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port E");
       return;
    }
    
    // Display the hex value and correct check box combination
    portIOSum[5] = portData;
    setValue(portIOSum[5], PortEValueGroupBox, PortEValueBox);
    
}

/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::ReadPortADirBits() {
    int result;
    unsigned char portData = 0;
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Read the port data   
    result = qusb->ReadPortDir(0, &portData);
    
    // Close the device when you are finished
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port A direction bits");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[0] = portData;
    setValue(portIOSum[0], PortADirGroupBox, PortADirBox);
    
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::ReadPortCDirBits() {
    int result;
    unsigned char portData = 0;
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Read the port data
    result = qusb->ReadPortDir(2, &portData);
    
    // Close the device when you are finished
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port C direction bits");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[2] = portData;
    setValue(portIOSum[2], PortCDirGroupBox, PortCDirBox);
}
/*------------------------------------------------------------------------------------*/
void QuickUsbDisplay::ReadPortEDirBits() {
    int result;
    unsigned char portData = 0;
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Read the port data
    result = qusb->ReadPortDir(4, &portData);
    
    // Close the device when you are finished
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot read port E direction bits");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[4] = portData;
    setValue(portIOSum[4], PortEDirGroupBox, PortEDirBox);   
}

/*------------------------------------------------------------------------------------------------
The following code is for the checking/unchecking of checkboxes in the port I/O tab
Each method finds out if the checkBox with id number buttonID is checked or not.
It then fixes the proper value, shows the value has hex in the proper text field,
 and then writes the value out to the qusb module using the temp open/close 
 functionality of the QuickUsb C++ class. (Open and Close are taken care for you)
-------------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortADirGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortADirGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[0] += (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[0] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the result out the QuickUSB module
    result = qusb->WritePortDir(0, portIOSum[0]);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port A direction");
       return;
    }   
    // Show the correct value in the GUI
    PortADirBox->setText( toHex(portIOSum[0]) );
    
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortAValueGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortAValueGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[1] += (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[1] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the port data
    result = qusb->WritePort(0, &portIOSum[1], 1);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port A value");
       return;
    }   
    // Show the hex value in the GUI
    PortAValueBox->setText( toHex( portIOSum[1]) );
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortCDirGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortCDirGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[2] += (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[2] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the result out the QuickUSB module
    result = qusb->WritePortDir(2, portIOSum[2]);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port C direction");
       return;
    }   
    // Show the hex value in the GUI
    PortCDirBox->setText( toHex(portIOSum[2]) );
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortCValueGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortCValueGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[3] += (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[3] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the port data
    result = qusb->WritePort(2, &portIOSum[3], 1);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port C value");
       return;
    }   
    PortCValueBox->setText( toHex(portIOSum[3]) );
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortEDirGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortEDirGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[4] += (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[4] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the result out the QuickUSB module
    result = qusb->WritePortDir(4, portIOSum[4]);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port E direction");
       return;
    }   
    PortEDirBox->setText( toHex(portIOSum[4]) );
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortEValueGroupBox_clicked( int buttonID) {
    int result;
    QString temp;
    QWidget * widget = PortEValueGroupBox->find(buttonID);
    
    // find out if the button with id number buttonID is checked or not and set the correct tooltip
    if ( ( (QCheckBox *) widget)->isChecked() ) { 
       portIOSum[5] += (char) pow(2.0,buttonID);
      QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Output" );
    }
    else {
       portIOSum[5] -= (char) pow(2.0,buttonID);
       QToolTip::add ( (QCheckBox *) widget, "Bit " + temp.setNum(buttonID) + " = Input" );
    }
    
    // Write the port data
    result = qusb->WritePort(4, &portIOSum[5], 1);
    
    // Check the transaction result
    if (result == false) {
       displayString("Cannot write port E value");
       return;
    }   
    PortEValueBox->setText( toHex( portIOSum[5]) );
}

/*------------------------------------------------------------------------------------*/
// For each bit check box find out whether portData has that specific bit on or off
// and then set the bit check box accordingly. Also change the hint message depending on whether or not
// that bit is an input or output
void setValue(unsigned char portData, QButtonGroup * group, QLineEdit * ByteBox, bool hint) {
    int sum = 0;
    
    if ( ( portData & 1 ) == 1) { 
       ( (QCheckBox *) (group->find(0) ) )->setChecked (true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(0) ), "Bit 0 = Output" ); }
          sum++;
    }
    else { 
       ( (QCheckBox *) (group->find(0) ) )->setChecked (false); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(0) ), "Bit 0 = Input" );  }
    }
    
    if ( (portData & 2) == 2) { 
       ( (QCheckBox *) (group->find(1) ) )->setChecked (true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(1) ), "Bit 1 = Output" );  }
       sum += 2;
    }
    else { 
       ( (QCheckBox *) (group->find(1) ) )->setChecked (false); 
      if (hint) { QToolTip::add ( (QCheckBox *) (group->find(1) ), "Bit 1 = Input" );  }
    }
    
    if ( (portData & 4) == 4){ 
       ( (QCheckBox *) (group->find(2) ) )->setChecked (true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(2) ), "Bit 2 = Output" );  }
       sum += 4;
    }
    else { 
       ( (QCheckBox *) (group->find(2) ) )->setChecked (false); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(2) ), "Bit 2 = Input" );  }
    }
    
    if ( (portData & 8) == 8) {
       ( (QCheckBox *) (group->find(3) ) )->setChecked (true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(3) ), "Bit 3 = Output" );  }
       sum += 8;
    }
    else { 
       ( (QCheckBox *) (group->find(3) ) )->setChecked (false); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(3) ), "Bit 3 = Input" );  }
    }
    
    if ( (portData & 16) == 16) { 
       ( (QCheckBox *) (group->find(4) ) )->setChecked(true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(4) ), "Bit 4 = Output" );  }
       sum+= 16;
    }
    else { 
       ( (QCheckBox *) (group->find(4) ) )->setChecked(false); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(4) ), "Bit 4 = Input" );  }
    }
    
    if ( (portData & 32) == 32) { 
       ( (QCheckBox *) (group->find(5) ) )->setChecked (true);
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(5) ), "Bit 5 = Output" );  }
       sum+=32;
    }
    else { 
       ( (QCheckBox *) (group->find(5) ) )->setChecked (false);
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(5) ), "Bit 5 = Input" );  }
    }
    
    if ( (portData & 64) == 64) { 
       ( (QCheckBox *) (group->find(6) ) )->setChecked (true);
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(6) ), "Bit 6 = Output" );  }
       sum += 64;
    }
    else { 
       ( (QCheckBox *) (group->find(6) ) )->setChecked(false);
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(6) ), "Bit 6 = Input" );  }
    }
    
    if ( (portData & 128) == 128 ) { 
       ( (QCheckBox *) (group->find(7) ) )->setChecked (true); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(7) ), "Bit 7 = Output" );  }
       sum += 128;
    }
    else { 
       ( (QCheckBox *) (group->find(7) ) )->setChecked(false); 
       if (hint) { QToolTip::add ( (QCheckBox *) (group->find(7) ), "Bit 7 = Input" );  }
    }
    ByteBox->setText( toHex(sum) );
}

/*------------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortAValueBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    oldValue =  portIOSum[1] ;
    
    portData = (PortAValueBox->text()).toInt( &okay );
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortAValueBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if (!okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
       // Put back the old value
       PortAValueBox->setText( toHex( oldValue) );
       return;
    }	 
    
    // Write the port data
    result = qusb->WritePort(0, (unsigned char *) &portData, 1);
    
    // Check the transaction result
    if (result == 0) {
      displayString("Cannot write value to port A");
      return;
    }
    
    // Display the hex value and remember it
    portIOSum[1] = portData;
    setValue(portIOSum[1], PortAValueGroupBox, PortAValueBox);
    
}

/*------------------------------------------------------------------------------------------------*/
void QuickUsbDisplay::PortCValueBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    // Remember the old value
    oldValue =  portIOSum[3];
    
    // Get the value form the GUI and then check for failure
    portData = (PortCValueBox->text()).toInt( &okay );
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortCValueBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if (!okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
       // Put back the old value
       PortCValueBox->setText( toHex( oldValue) );
       return;
    }	 
    
    // Write the port data
    result = qusb->WritePort(2, (unsigned char *) &portData, 1);
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot write value to port C");
       return;
    }
    
    // Display the hex value and remember it
    portIOSum[3] = portData;
    setValue(portIOSum[3], PortCValueGroupBox, PortCValueBox);
    
}

/*------------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortEValueBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    // remember the current value
    oldValue =  portIOSum[5];
    
    // Get the value form the GUI and then check for failure
    portData = (PortEValueBox->text()).toInt( &okay );
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortEValueBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if (!okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
       // Put back the old value
       PortEValueBox->setText( toHex( oldValue) );
       return;
    }	 
    
    // Write the port data
    result = qusb->WritePort(4, (unsigned char *) &portData, 1);
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot write value to port E");
       return;
    }
    
    // Display the hex value and remember it
    portIOSum[5] = portData;
    setValue(portIOSum[5], PortEValueGroupBox, PortEValueBox);
    
}

/*------------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortADirBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    // Remember the current value
    oldValue =  portIOSum[0];
    
    // Get the value form the GUI and then check for failure
    portData = (PortADirBox->text()).toInt(&okay);
    
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortADirBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if ( !okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
      // Put back the old value
      PortADirBox->setText( toHex( oldValue) );
      return;
    }	 
    
    // Write the port dirictions to the QuickUsb module and check the result
    result = qusb->WritePortDir(0, (unsigned char) portData);
    if (result == 0) {
       displayString("Cannot write to port A");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[0] = portData;
    setValue(portIOSum[0], PortADirGroupBox, PortADirBox);
}

/*------------------------------------------------------------------------------------------------*/
void QuickUsbDisplay::PortCDirBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    // Remember the current value
    oldValue =  portIOSum[2];
    
    // Get the value form the GUI and then check for failure
    portData = (PortCDirBox->text()).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortCDirBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if ( !okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
       // Put back the old value
       PortCDirBox->setText( toHex( oldValue) );
       return;
    }	 
    
    // Write the port dirictions to the QuickUsb module and check the result
    result = qusb->WritePortDir(2, (unsigned char) portData);
    if (result == 0) {
       displayString("Cannot write to port C");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[2] = portData;
    setValue(portIOSum[2], PortCDirGroupBox, PortCDirBox);
}
/*------------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::PortEDirBox_returnPressed() {
    int result, portData, oldValue;
    bool okay;
    
    // remember teh current value
    oldValue =  portIOSum[4];
    
    // Get the value form the GUI and check for failure
    portData = (PortEDirBox->text()).toInt(&okay);
    // Maybe it is in Hex?
    if (!okay) {
       okay = HexToDecimal(PortEDirBox->text(), portData );
    }
    // Both attemps resulted in failure or portData is out of range,  output message and return
    if ( !okay || portData > 255 || portData < 0) { 
       displayString("Invalid input detected only values between 0 to 255 accepted");
       // Put back the old value
       PortEDirBox->setText( toHex( oldValue) );
       return;
    }	 
    
    // Write the port directions to the QuickUsb module and check the result
    result = qusb->WritePortDir(4, (unsigned char) portData);
    if (result == 0) {
       displayString("Cannot write to port E");
       return;
    }
    
    // Set the port dir bit boxes
    portIOSum[4] = portData;
    setValue(portIOSum[4], PortEDirGroupBox, PortEDirBox);
}


/*------------------------------------------------------------------------------------
  RS232 TAB CODE
  -----------------------------------------------------------------------------------*/

void QuickUsbDisplay::RS232WriteButton_clicked() {
    char portNum;
    int result;
    string text;
    QString temp;
    
    // Remember the last buton clicked
    if ( lastButton != RS232WriteButton ) {
       if (isRepeatOn) {
          resetTimers(RS232WriteButton);
       }
      else {
         lastButton = RS232WriteButton;
      }
  }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    portNum = RS232PortComboBox->currentItem( );
    text =  (RS232WriteTextBox->text()).ascii();
    
    // Write to the RS-232 port
    displayString("Writing to RS232 port " +  temp.setNum(portNum));
    result = qusb->WriteRS232(portNum, (unsigned char *) text.c_str(), text.length() );
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot write string to RS232");
       return;
    }
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::RS232ReadButton_clicked(){
    unsigned char portNum, * inData;
    int result;
    unsigned short length = 0;
    QString temp;
    
    // Clear the RS232 Read text box
    RS232ReadTextBox->setText("");	
    
    // Remember the last buton clicked
    if ( lastButton != RS232ReadButton ) {
       if (isRepeatOn) {
          resetTimers(RS232ReadButton);
       }
       else {
          lastButton = RS232ReadButton;
       }
    }  
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    portNum = RS232PortComboBox->currentItem();
    
    // Find out how many characters are waiting for us
    result = qusb->GetNumRS232(portNum, &length);
    if (result == 0) { 
       qusb->Close();
       displayString("Cannot read number of RS232 characters waiting");
       return;
    }
    
    // If there is a valid amount of characters to be read
    if (length > 0) {
       inData = new unsigned char[length+1];
       if (inData == NULL) {
          displayString("Could not allocate memory");
          qusb->Close();     
          return;
       }
       inData[length] = '\0';
	
      // Read length characters from the RS232 port
      displayString("Reading RS232 port " + temp.setNum(portNum));
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
      RS232ReadTextBox->setText( (char *) inData);
      delete [] inData;
		
      // See if any more characters are waiting to be read. This uses the temp Open/Close functionality of the C++ class
      result = qusb->GetNumRS232(portNum, &length);
      if (result == 0) {
         displayString("Cannot read number of RS232 characters waiting");
         return;
      }
      
      temp.setNum(length);
      GetNumTextBox->setText(temp);
    }
    else{
       qusb->Close();
       displayString("There are no charachters waiting to be read");
    }
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::RS232PortComboBox_activated( int ){
    char portNum;
    int result;
    unsigned short length = 0;
    QString temp;
    
    portNum = RS232PortComboBox->currentItem();
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Get the number of characters waiting to be read
    result = qusb->GetNumRS232(portNum, &length);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot get number of RS232 bytes outstanding");
       return;
    }
    
    // Update the length   
    temp.setNum(length);
    GetNumTextBox->setText(temp);
    return;
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::RS232FlushButton_clicked(){
    unsigned char portNum;
    int result;
    QString temp;
    
    // Remember the last buton clicked
    if ( lastButton != RS232FlushButton ) {
       if (isRepeatOn) {
          resetTimers(RS232FlushButton);
      }
      else {
         lastButton = RS232FlushButton;
     }
  }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    portNum = RS232PortComboBox->currentItem();
    
    // Flush the RS232 port
    displayString("Flushing RS232 port " + temp.setNum(portNum));
    result = qusb->FlushRS232(portNum);
    
    // Close the device when you're done
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot flush RS232");
       return;
    }
    
    GetNumTextBox->setText("0");
}


/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::RS232BaudRateComboBox_activated( const QString & str ) {
    long baudRate; 
    int result;
    QString temp;
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Set the Qusb baudrate
    baudRate = str.toLong();
    displayString("Setting baudrate to " + temp.setNum(baudRate), 2000 );
    result = qusb->SetRS232BaudRate(baudRate);
    
    // Close the device when you are done   int result, portData, oldValue;
    qusb->Close();
    
    // Check the transaction result
    if (result == 0) {
       displayString("Cannot set RS232 baud rate");
       return;
    }     
}

/*----------------------------------------------------------------------------------------------
 SPI TAB CODE
 -----------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::SpiReadButton_clicked() {
    int result;
    unsigned short length;
    unsigned char portNum, * inData;
    QString str;
    
    // Remember the last buton clicked
    if ( lastButton != SpiReadButton ) {
       if (isRepeatOn) {
          resetTimers(SpiReadButton);
       }
       else {
          lastButton = SpiReadButton;
       }
    }
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString( "Cannot open Device:" + currentDevice);
       return;
    }
    
    // Calculate the length
    portNum = SpiReadPortNumComboBox->currentItem();
    length = (SpiReadLengthTextBox->text()).toUShort();
    
    // Allocate storage for writeDataLength elements
    inData = new unsigned char[length];
    if (inData == NULL) {
       qusb->Close();
       displayString("Could not allocate memory");
       return;
    }
    
    // Read the SPI data
    displayString("Reading SPI data from port: " + str.setNum(portNum));
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
    str = ByteArrayToString(inData, length);
    SpiReadTextBox->setText(str);
    
    delete [] inData;
    return;  
}

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::SpiWriteButton_clicked() { 
    int result;
    unsigned short length;
    unsigned char portNum, * outData;
    string tempString;
    QString temp;
    
    // Remember the last buton clicked
    if ( lastButton != SpiWriteButton ) {
       if (isRepeatOn) {
          resetTimers(SpiWriteButton);
       }
       else {
          lastButton = SpiWriteButton;
       }
    }  
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);;
       return;
    }
    
    // Get the port number and the length
    portNum = SpiWritePortNumComboBox->currentItem();
    
    // Convert the text string to a byte array
    tempString = (SpiWriteTextBox->text()).ascii();
    if ( tempString.length() <= 0 ){
       displayString("Please enter digits or hex values first");
       qusb->Close();
       return;
    }
    
    length = StringToByteArray(outData, tempString);
    if (length > 0) {
       // Write the data out to the SPI port
       displayString("Writing SPI data to port: " + temp.setNum(portNum));
       result = qusb->WriteSpi(portNum, outData, length);
    }
    else {
       displayString("Please enter decimal digits or Hex values only");
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

/*------------------------------------------------------------------------------------*/

void QuickUsbDisplay::SpiWriteReadButton_clicked() {
    int result;
    unsigned char portNum, * inOutData;
    unsigned short length;
    string tempString = "";
    QString inputString = "", temp;
    
    // Remember the last buton clicked
    if ( lastButton != SpiWriteReadButton ) {
       if (isRepeatOn) {
          resetTimers(SpiWriteReadButton);
       }
       else {
         lastButton = SpiWriteReadButton;
       }
    }  
    
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Get the port number and the length
    portNum = SpiWritePortNumComboBox->currentItem();
    
    // Get the string for the GUI
    tempString = (SpiWriteTextBox->text()).ascii();
    // Check to see if it has a proper length
    if (tempString.length() <= 0) {
       displayString("Please enter digits or hex values first");
       qusb->Close(); 
       return;
    }
    
    // Convert the text string to a byte array
    length = StringToByteArray(inOutData, tempString);
    
    if (length > 0) {
       // Write and read the SPI data
       displayString("Writing and reading SPI data from port:  " + temp.setNum(portNum));
       result = qusb->WriteReadSpi(portNum, inOutData, length);
    }
    else {
       displayString("Please enter decimal digits or Hex values only");
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
    inputString = ByteArrayToString( inOutData, length);
    SpiReadTextBox->setText(inputString);
    
    delete [] inOutData;
}

/*--------------------------------------------------------------------------------------------
  I2C TAB CODE
  --------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::I2cReadButton_clicked(){
    int result, address, int_length;
    unsigned short length;
    unsigned char * inData;
    QString str;
    bool okay;
    
    // Remember the last buton clicked
    if ( lastButton != I2cReadButton ) {
       if (isRepeatOn) {
          resetTimers(I2cReadButton);
       }
      else {
         lastButton = I2cReadButton;
      }
    }  
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device:" + currentDevice);
       return;
    }
    
    // Get the length from the GUI
    address = (I2cReadAddressTextBox->text()).toInt(&okay);
    // If attempt failed maybe the value is in hex format
    if (!okay) {
       okay = HexToDecimal(I2cReadAddressTextBox->text(), address);
    }
    //  failed again, output a message and return;
    if (!okay) {
       displayString("Invalid input detected in the address box");
       qusb->Close();
       return;
    }
    
    length = (I2cReadLengthTextBox->text()).toInt();
    if (!okay) {
       okay = HexToDecimal(I2cReadLengthTextBox->text(), int_length);
       length = int_length;
    }
    if (!okay) {
       displayString("Invalid input detected in the length box");
       qusb->Close();
       return;
    }
    
    // Allocate storage for writeDataLength elements
    inData = new unsigned  char[length];
    if (inData == NULL) {
       qusb->Close();
       displayString("Could not allocate memory");
       return;
    }
    
    // Read the I2c Data
    displayString("Reading I2C data from address " +  str.setNum(address) );
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
    I2cReadTextBox->setText(str);
    delete [] inData;
}

/*--------------------------------------------------------------------------------------------*/
void QuickUsbDisplay::I2cWriteButton_clicked() {
    int result, address;
    unsigned short length;
    unsigned char * outData;
    string tempString;
    bool okay;
    QString temp;
    
    // Remember the last buton clicked
    if ( lastButton != I2cWriteButton ) {
       if (isRepeatOn) {
          resetTimers(I2cWriteButton);
       }
       else {
          lastButton = I2cWriteButton;
       }
    }  
    
    // Open the device
    result = qusb->Open();
    if (result == 0) {
       displayString("Cannot open Device: " + currentDevice);
       return;
    }
    
    // Get the address and the length
    address = (I2cWriteAddressTextBox->text()).toInt(&okay);
    
    if (!okay) {
       okay = HexToDecimal(I2cWriteAddressTextBox->text(), address);
    }
    if (!okay) {
       displayString("Invalid input detected in address box");
       qusb->Close();
       return;
    }      
    // Convert the text string to a byte array
    tempString = (I2cWriteTextBox->text()).ascii();
    
    if (tempString.length() <= 0) {
       displayString("Please enter digits or hex values first");
       qusb->Close();
       return;
    }
    
    // Write the text out the I2C port
    length = StringToByteArray(outData, tempString);
    if ( length > 0 ) {
       displayString("Writing I2C data to address " + temp.setNum(address));
       result = qusb->WriteI2C(address, outData, length);
    }
    else {
       displayString("Cannot write to I2C");
       qusb->Close();
       return;
    }
    
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

/*----------------------------------------------------------------------------------------------
  HELPER FUNCTIONS
  --------------------------------------------------------------------------------------------*/

// Displays a string for a timeOut period in miliseconds in the status bar
void displayString(QString str, int timeOut) {

    statusBarPtr->message(str, timeOut);
}

/*------------------------------------------------------------------------------------------*/     

int StringToByteArray(unsigned char * &theData, string theString) {
    int dataIndex = 0, strLen, tempInt;
    long startStrIndex = 0, endStrIndex;
    unsigned char * tempData;
    string tempString, values = "1234567890ABCDEF";
    // Allocate space for the data
    // Guess that the data length is the maximum
    strLen = theString.length();
    if (strLen <= 0) {
       return -1;
    }
    
    tempData = new unsigned char[strLen];
    if (tempData == NULL) {
	cerr << "Could not allocate memory\n";
	return -1;
    }
    
    // Parse text and put it into the array
    
    while(startStrIndex < strLen){
       // Find the first non-space
       while (startStrIndex < strLen && theString.at(startStrIndex) == ' ') {
          startStrIndex++;
       }
      
       // Find the first space
       endStrIndex = startStrIndex + 1;
       while(endStrIndex < strLen && theString.at(endStrIndex) != ' ') {
          endStrIndex++;
       }
	
       // Get the text
       tempString = theString.substr(startStrIndex, endStrIndex - startStrIndex);
       if (tempString.find_first_not_of(values,0) >= tempString.length() ) {
          tempInt = atoi(  tempString.c_str() );
       }
       // Else attempt to do Hex to decimal conversion
       else if (HexToDecimal(tempString.c_str(), tempInt) == false) {
          delete [] tempData;
          return -1;
      }
    
      tempData[dataIndex++] = tempInt & 0xFF;
      startStrIndex = endStrIndex;
    }
    
    // Resize the data to the right length
    theData = new unsigned char[dataIndex];
    if (theData == NULL) {
       delete [] tempData;
       cerr << "Could not allocate memory\n";
       return -1;
    }
    
    for(int i = 0; i < dataIndex; i++){
       theData[i] = tempData[i];
    }
    
    // free memory
    delete [] tempData;
    
    // Return the actual length
    return dataIndex;
}

/*------------------------------------------------------------------------------------------*/

QString ByteArrayToString(const  unsigned char * byteArray, const int length){
    QString theString;
    
    for (int count = 0; count < length; count++) {
       if (byteArray[count] == ' ') {
          continue;
        }
        theString = theString + toHex(byteArray[count], 1) + " ";
    }
    return theString;
}


/*------------------------------------------------------------------------------------------*/
//This is for ints
bool HexToDecimal (const char* HexNumber, int& Number) {
    char* pStopString;
    Number = strtol(HexNumber, &pStopString, 16);
    return (bool) (Number != LONG_MAX);
}

/*------------------------------------------------------------------------------------------*/
//This is for longs
bool HexToDecimal (const char* HexNumber, long& Number) {
    char* pStopString;
    Number = strtol(HexNumber, &pStopString, 16);
    return (bool)(Number != LONG_MAX);
}

/*------------------------------------------------------------------------------------------*/
bool QuickUsbDisplay::foundNewModules(const char * str) {
    string tempString;
    int count, position = 0;
    count = moduleComboBox->count();  
    
    // Do while we havn't hit a double null terminal
    while(  *str != '\0' ) {
       // Check if there are more items in the new list then there are in the combo box. If there is return true
       if (position == count) {
          return true;
       }   
       
       tempString =  (moduleComboBox->text(position++)).ascii();
       // if the strings are not the same then we have found a different list return true
      if ( strcmp( str, tempString.c_str() ) != 0 ) {
         return true;
      }
      str += strlen(str) + 1;
    }
    
    // A qusb module has been removed from the computer
    if (position < count) {
       return true;
    }
    // otherwise nothing has changed return false;
    return false;
}

/*------------------------------------------------------------------------------------------*/

QString toHex(const int decimal) { 
   QString hex = "";
    for (int i=1; i>=0; i--) {
       hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
    }
    return ("0x" + hex);
}

/*------------------------------------------------------------------------------------------*/

QString toHex(const int decimal, const int length){
    QString hex = "";
    for (int i = length; i >= 0; i--) {
       hex += "0123456789ABCDEF"[((decimal >> i*4) & 0xF)];
    }  
    return ("0x" + hex);
}

/*------------------------------------------------------------------------------------------*/
// Used when switching between modules through the moduleComboBox
void QuickUsbDisplay::cleanUp() {
    ReadPortADirBits();     
    ReadPortCDirBits();
    ReadPortEDirBits();
    PortAReadButton_clicked();
    PortCReadButton_clicked();
    PortEReadButton_clicked();
    lastButton = NULL;
    displayString("");
    RS232ReadTextBox->setText("");
    RS232WriteTextBox->setText("");
    SpiReadTextBox->setText("");
    SpiWriteTextBox->setText("");
    I2cReadTextBox->setText("");
    I2cWriteTextBox->setText("");
}

/*----------------------------------------------------------------------------------------------
 REPEAT LAST BUTTON CODE
  --------------------------------------------------------------------------------------------*/
// Needed when switching between  buttons while the rpeat timer is on
void QuickUsbDisplay::resetTimers(QPushButton * button) {
    if ( isRepeatOn && repeatTimer->isActive() ) {
       repeatTimer->stop();
       pauseTimer->stop();
       lastButton = button;
       pauseTimer->start(100, true);
    } 
}

/*------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::repeatLastButtonCheckBox_clicked() {
    bool okay;
    if (lastButton == NULL && repeatLastButtonCheckBox->isChecked() == true) {
       return;
    }
    else if (repeatLastButtonCheckBox->isChecked() ) {
       interval = (intervalTextBox->text()).toInt(&okay);
       if ( !okay) {
          displayString("Invalid interval detected");
          repeatLastButtonCheckBox->setChecked(false);
          return;
       }
       isRepeatOn = true;
       repeatTimer->start(interval, false);
    }
    else {
       repeatTimer->stop();
       isRepeatOn = false;
    }
}

/*------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::intervalTextBox_textChanged( const QString & str) {
    int interval;
    bool okay;
    // If the repeat timer is active stop it and restart it with the new interval
    if ( repeatTimer->isActive() ) {
       repeatTimer->stop();
       interval = str.toInt(&okay);
       if ( !okay) {
          displayString("Invalid interval detected");
          repeatLastButtonCheckBox->setChecked(false);
          return;
       }
       repeatTimer->start(interval, false);
    }
    return;
}
/*------------------------------------------------------------------------------------------*/

void QuickUsbDisplay::pressButton() {
    // Function that the repeat timer calls when it times out
    if (lastButton != NULL && pauseTimer->isActive() == false && isRepeatOn)  {
       lastButton->animateClick();
    }
}


/*------------------------------------------------------------------------------------------*/

// This function is needed to switch between buttons while the
//repeat checkbox is clicked and the timer is activated
void QuickUsbDisplay::pause() {
    bool okay;
    // Attemp to get the interval
    interval = (intervalTextBox->text()).toInt(&okay);
    if ( !okay) {
       displayString("Invalid interval detected");
       repeatLastButtonCheckBox->setChecked(false);
       return;
    }
    // Start the timer
    repeatTimer->start(interval, false);
    return;
}

/*----------------------------------------------------------------------------------------------
 ABOUT FORM CODE
  --------------------------------------------------------------------------------------------*/

// When the user clicks on the about action in the toolbar
void QuickUsbDisplay::HelpAbout_activated() {
    int x, y;
    // Create the form
    aboutFormPtr = new AboutForm(); 
    if (aboutFormPtr == NULL) {
       displayString("Could not allocate memory");
       return;
    }
    
    // activate the timer at 25 miliseconds
    aboutTimer->start(25, false);
    
    // get the center coordinates of the main dialog window
    x = this->x() + this->width()/2;
    y = this->y() + this->height()/2;
    // Show the form 
    aboutFormPtr->showForm(x, y, &aboutFormPtr );
}        


/*------------------------------------------------------------------------------------------*/
// While the about form window is open, the aboutTimer calls this function
// and checks if the the about form is not ontop of the diag. If it isn't then it force
// it to be. Otherwise the aboutForm doesn't exist anymore so just stop the timer
void QuickUsbDisplay::aboutOnTop() {   
    if ( aboutFormPtr != NULL) {
       aboutFormPtr->raise();
       aboutFormPtr->setActiveWindow();
    }
    else {
       aboutTimer->stop();
    }
    return;
}

/*----------------------------------------------------------------------------------------------
 PROGRAM TERMINATION CODE
  --------------------------------------------------------------------------------------------*/
// When the user clicks on File->exit in the tool bar.
void QuickUsbDisplay::FileExit_activated() {
    
    // delete the QuickUSB object
    if (qusb != NULL) {
       delete qusb;
       qusb = NULL;
    }   
    
    // delete the aboutForm
    if (aboutFormPtr != NULL) {
       delete aboutFormPtr;
       aboutFormPtr = NULL;	
    }
    
    // delete all the timers which were created in the init function
    delete updateModulesTimer;
    delete repeatTimer;
    delete pauseTimer; 
    delete aboutTimer;
    
    // Exit the program
    QApplication::exit(0);
}


/*------------------------------------------------------------------------------------------*/
// Called when the user clicks on the X 
void QuickUsbDisplay::closeEvent( QCloseEvent *event ) {
    event->accept();
    // Delete the QuickUSB object
    if (qusb != NULL) {
       delete qusb;
       qusb = NULL;
    }   
    
    // Delete the aboutForm
    if (aboutFormPtr != NULL) {
       delete aboutFormPtr;
       aboutFormPtr = NULL;	
    }
    
    // Delete all three timers which were created in the init function
    delete updateModulesTimer;
    delete repeatTimer;
    delete pauseTimer; 
    delete aboutTimer;
    
    // Remeber specific information for the next time QuickUSB Diag starts up
    QSettings settings(QSettings::Native);
    settings.setPath( "BitwiseSys.com", "QuickUSBDiag", QSettings::User );
    //settings.insertSearchPath( QSettings::Unix, QDir::currentDirPath() );
    settings.writeEntry("/QuickUsb/uploadFile", UploadFilenameTextBox->text() );
    settings.writeEntry("/QuickUsb/wordWide", WordWideCheckBox->isChecked() );
    settings.writeEntry("/QuickUsb/period", intervalTextBox->text() );
    settings.writeEntry("/QuickUsb/tab", PageControl->currentPageIndex() );
    
    // Exit the program
    QApplication::exit(0);
}




void QuickUsbDisplay::asyncCheckBox_stateChanged( int )
{
   if (asyncCheckBox->isChecked() == FALSE) {
      bufferCountLabel->hide();
      bufferSizeLabel->hide();
      bufferCountTextBox->hide();
      bufferSizeTextBox->hide();
      DataLengthTextBox->setText("262144");
   }
   else {
      bufferCountLabel->show();
      bufferSizeLabel->show();
      bufferCountTextBox->show();
      bufferSizeTextBox->show();
      DataLengthTextBox->setText("1048576");
      bufferCountTextBox->setText("4");
      bufferSizeTextBox->setText("262144");
   }
}


void QuickUsbDisplay::ReadFileBrowseButton_clicked()
{
    QString filename = "";
    filename = QFileDialog::getSaveFileName( "/home", "All files(*.*)", this, "open file dialog","Choose a file");
    if (filename == "") { //user hit cancel
       return;
    }

    ReadFileTextBox->setText(filename);
}


void QuickUsbDisplay::WriteFileBrowseButton_clicked()
{
    QString filename = "";
    filename = QFileDialog::getOpenFileName( "/home", "All files(*.*)", this, "open file dialog","Choose a file");
    if (filename == "") { //user hit cancel
       return;
    }

    WriteFileTextBox->setText(filename);
}


// This function formats a QuickUSB error message.
void QuickUsbDisplay::DisplayError(const char * function, int index)
{
   unsigned long errorCode = 0;
   char messageBoxBuffer[500];

   QuickUsbGetLastError(&errorCode);
   qusb->Close();

   if (index == -1) {
      sprintf(messageBoxBuffer, "QuickUsb%s error %ld \n", function, errorCode);
   }
   else {
      sprintf(messageBoxBuffer, "QuickUsb%s error %ld index %d\n", function, errorCode, index);
   }

   switch(errorCode)
   {
   case QUICKUSB_ERROR_OUT_OF_MEMORY:
      strcat(messageBoxBuffer, "A memory allocation function failed while executing a function.");
      break;
   case QUICKUSB_ERROR_CANNOT_OPEN_MODULE:
      strcat(messageBoxBuffer, "The module was located, but was not able to be opened successfully.");
      break;
   case QUICKUSB_ERROR_CANNOT_FIND_DEVICE:
      strcat(messageBoxBuffer, "The module could not be located.");
      break;
   case QUICKUSB_ERROR_IOCTL_FAILED:
      strcat(messageBoxBuffer, "Unable to obtain necessary information from device.");
      break;
   case QUICKUSB_ERROR_INVALID_PARAMETER:
      strcat(messageBoxBuffer, "Attempting to read or write data of length 0.");
      break;
   case QUICKUSB_ERROR_TIMEOUT:
      strcat(messageBoxBuffer, "Timeout occured while trying to read or write data.");
      strcat(messageBoxBuffer, "\nTry calling QuickUsbSetTimeout() with a longer time out.");
      break;
   case QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED:
      strcat(messageBoxBuffer, "The function you are trying to execute is not supported by this driver version.");
      strcat(messageBoxBuffer, "\nTry reloading the latest libquickusb library file.");
      strcat(messageBoxBuffer, "\nNOTE libusb 0.1.11 does not support asynchronous I/O.");
      break;
   default:
      strcat(messageBoxBuffer, "Unknown error.");
      break;
   }

   QMessageBox::critical(this, "QuickUSB Error", messageBoxBuffer);
}
