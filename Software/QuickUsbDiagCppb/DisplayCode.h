/*=============================================================================
   Title        : DisplayCode.h
   Description  : GUI Unit for QuickUsbDiagCppb
   Notes        : None
   History      :

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

   $History: DisplayCode.h $

    =============================================================================*/

 //---------------------------------------------------------------------------

#ifndef DisplayCodeH
#define DisplayCodeH

//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Tabnotbk.hpp>
#include <DBCGrids.hpp>
#include <Grids.hpp>
#include <Dialogs.hpp>
#include "BitCheckBox.h"
#include <string>
#include <windows.h>
#include <Mmsystem.h>
#include <stdlib.h>

//---------------------------------------------------------------------------
class TDisplay : public TForm
{
__published:	// IDE-managed Components
   TStatusBar *StatusBar;
   TPanel *Panel;
   TMainMenu *MainMenu;
   TMenuItem *FileMenu;
   TMenuItem *FileExit;
   TMenuItem *HelpMenu;
   TMenuItem *HelpAbout;
   TTimer *ModuleScanTimer;
   TPageControl *PageControl;
   TTabSheet *GeneralTabSheet;
   TGroupBox *GeneralGroupBox;
   TComboBox *ModuleComboBox;
   TEdit *StringDescriptorEdit;
   TTabSheet *CmdDataTabSheet;
   TTabSheet *PortsTabSheet;
   TTabSheet *Rs232TabSheet;
   TTabSheet *TabSheet5;
   TTabSheet *TabSheet6;
   TLabel *Label1;
   TLabel *Label2;
   TButton *CycleButton;
   TButton *ResetButton;
   TGroupBox *GroupBox1;
   TButton *FpgaUploadButton;
   TButton *BrowseButton;
   TProgressBar *Progress;
   TEdit *UploadFilenameTextBox;
   TGroupBox *GroupBox2;
   TGroupBox *GroupBox3;
   TLabel *Label6;
   TLabel *Label7;
   TButton *ReadDataButton;
   TEdit *DataAddressTextBox;
   TEdit *DataLengthTextBox;
   TButton *WriteDataButton;
   TLabel *Label8;
   TCheckBox *NoIncDataAddressCheckBox;
   TButton *WriteCountDataButton;
   TLabel *Label3;
   TLabel *Label4;
   TLabel *Label5;
   TLabel *Label9;
   TEdit *CommandLengthTextBox;
   TLabel *Label10;
   TButton *WriteCommandButton;
   TLabel *Lablel1;
   TCheckBox *NoIncCommandAddressCheckBox;
   TButton *ReadCommandButton;
   TLabel *Label12;
   TLabel *Label13;
   TLabel *Label14;
   TLabel *Label15;
   TLabel *Label16;
   TLabel *Label17;
   TEdit *CommandAddressTextBox;
   TCheckBox *WordWideCheckBox;
   TLabel *Label19;
   TGroupBox *GroupBox4;
   TGroupBox *GroupBox5;
   TGroupBox *GroupBox6;
   TGroupBox *GroupBox7;
   TLabel *Label20;
   TLabel *Label21;
   TLabel *Label22;
   TLabel *Label23;
   TLabel *Label24;
   TLabel *Label25;
   TButton *ReadPortAButton;
   TButton *ReadPortEButton;
   TGroupBox *GroupBox8;
   TLabel *Label26;
   TComboBox *RS232PortComboBox;
   TLabel *Label27;
   TComboBox *RS232BaudRateComboBox;
   TButton *FlushRS232Button;
   TGroupBox *GroupBox9;
   TGroupBox *GroupBox10;
   TButton *ReadRS232Button;
   TButton *WriteRS232Button;
   TLabel *Label28;
   TEdit *GetNumTextBox;
   TGroupBox *GroupBox11;
   TLabel *Label29;
   TLabel *Label30;
   TComboBox *SpiReadPortNumComboBox;
   TEdit *SpiReadLengthTextBox;
   TButton *SpiReadButton;
   TGroupBox *GroupBox12;
   TLabel *Label31;
   TComboBox *SpiWritePortNumComboBox;
   TButton *SpiWriteButton;
   TGroupBox *GroupBox13;
   TLabel *Label32;
   TLabel *Label33;
   TEdit *I2cReadLengthTextBox;
   TButton *I2cReadButton;
   TEdit *I2cReadAddressTextBox;
   TGroupBox *GroupBox14;
   TLabel *Label34;
   TButton *I2cWriteButton;
   TEdit *I2cWriteAddressTextBox;
   TStringGrid *CommandFlexGrid;
   TOpenDialog *OpenDialog;
   TRichEdit *SpiReadTextBox;
   TRichEdit *SpiWriteTextBox;
   TRichEdit *RS232ReadTextBox;
   TRichEdit *RS232WriteTextBox;
   TRichEdit *I2cReadTextBox;
   TRichEdit *I2cWriteTextBox;
   TButton *SpiWriteReadButton;
   TBitCheckBoxFrame *PortADirBitCheckBox;
   TBitCheckBoxFrame *PortAValueBitCheckBox;
   TBitCheckBoxFrame *PortCDirBitCheckBox;
   TBitCheckBoxFrame *PortCValueBitCheckBox;
   TButton *ReadPortCButton;
   TBitCheckBoxFrame *PortEDirBitCheckBox;
   TBitCheckBoxFrame *PortEValueBitCheckBox;
   TButton *Button1;
   TTimer *StatusBarTimer;
   TTimer *RepeatTimer;
   TLabel *Label35;
   TCheckBox *RepeatCheckBox;
   TLabel *Label11;
   TEdit *RepeatTextBox;
   TLabel *Label37;
   TLabel *WordWideLabel;
   TCheckBox *AsyncCheckBox;
   TLabel *Label18;
   TButton *ReadDataBrowseButton;
   TButton *WriteDataBrowseButton;
   TEdit *ReadFileEditBox;
   TEdit *WriteFileEditBox;
   TLabel *Label36;
   TEdit *BufferCountEditBox;
   TLabel *Label38;
   TEdit *BufferSizeEditBox;
   TOpenDialog *DataOpenDialog;
   void __fastcall ModuleComboBoxClick(TObject *Sender);
   void __fastcall FileExitClick(TObject *Sender);
   void __fastcall ModuleScanTimerTimer(TObject *Sender);
   void __fastcall FpgaUploadButtonClick(TObject *Sender);
   void __fastcall BrowseButtonClick(TObject *Sender);
   void __fastcall ReadCommandButtonClick(TObject *Sender);
   void __fastcall SpiReadButtonClick(TObject *Sender);
   void __fastcall SpiWriteButtonClick(TObject *Sender);
   void __fastcall SpiWriteReadButtonClick(TObject *Sender);
   void __fastcall WriteCommandButtonClick(TObject *Sender);
   void __fastcall WordWideCheckBoxClick(TObject *Sender);
   void __fastcall ReadDataButtonClick(TObject *Sender);
   void __fastcall WriteDataButtonClick(TObject *Sender);
   void __fastcall ReadPortAButtonClick(TObject *Sender);
   void __fastcall ReadPortCButtonClick(TObject *Sender);
   void __fastcall ReadPortEButtonClick(TObject *Sender);
   void __fastcall WriteRS232ButtonClick(TObject *Sender);
   void __fastcall ReadRS232ButtonClick(TObject *Sender);
   void __fastcall FlushRS232ButtonClick(TObject *Sender);
   void __fastcall WriteCountDataButtonClick(TObject *Sender);
   void __fastcall PageControlChange(TObject *Sender);
   void __fastcall I2cReadButtonClick(TObject *Sender);
   void __fastcall I2cWriteButtonClick(TObject *Sender);
   void __fastcall RS232PortComboBoxChange(TObject *Sender);
   void __fastcall CommandLengthTextBoxChange(TObject *Sender);
   void __fastcall PortADirBitCheckBoxClick(TObject *Sender);
   void __fastcall PortAValueBitCheckBoxClick(TObject *Sender);
   void __fastcall PortCDirBitCheckBoxClick(TObject *Sender);
   void __fastcall PortCValueBitCheckBoxClick(TObject *Sender);
   void __fastcall PortEDirBitCheckBoxClick(TObject *Sender);
   void __fastcall PortEValueBitCheckBoxClick(TObject *Sender);
   void __fastcall HelpAboutClick(TObject *Sender);
   void __fastcall ModuleComboBoxChange(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall StatusBarTimerTimer(TObject *Sender);
   void __fastcall displayString(String str);
   void __fastcall UpdateWordWideCheckBox(TObject *Sender);
   int  __fastcall StringToByteArray(unsigned char * &theData, string theString);
   string __fastcall toHex(int decimal, int length);
   string __fastcall ByteArrayToString(char * byteArray, int length);
   bool __fastcall HexToDecimal(const char* HexNumber, int& Number);
   bool __fastcall foundNewModules(char * str);
   void __fastcall RepeatCheckBoxClick(TObject *Sender);
   void __fastcall RepeatTimerTimer(TObject *Sender);
   void __fastcall RepeatTextBoxChange(TObject *Sender);
   void __fastcall CycleButtonClick(TObject *Sender);
   void __fastcall ResetButtonClick(TObject *Sender);
   void __fastcall RS232BaudRateComboBoxChange(TObject *Sender);
   void __fastcall PortADirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall PortCDirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall PortEDirBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall PortAValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall PortCValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall PortEValueBitCheckBoxByte_Enter(TObject *Sender, WORD &Key);
   void __fastcall ReadDataBrowseButtonClick(TObject *Sender);
   void __fastcall WriteDataBrowseButtonClick(TObject *Sender);
   void __fastcall AsyncCheckBoxClick(TObject *Sender);
   void __fastcall ReadAsyncData();
   void __fastcall WriteAsyncData();
   void __fastcall DisplayError(char * function, int index);
   void __fastcall ThroughputCalc(unsigned long dataLength);

private:	// User declarations
   DWORD startTime;                    /* start of data transfer test */
   DWORD endTime;                      /* end of data transfer test */
   TFormOutputTextBox *FormOutputTextBox;

   public:		// User declarations
   int commandLength;
   void __fastcall UpdateModuleComboBox();
   __fastcall TDisplay(TComponent* Owner);
   void __fastcall TDisplay::ReadPortADirBits();
   void __fastcall TDisplay::ReadPortCDirBits();
   void __fastcall TDisplay::ReadPortEDirBits();
   int __fastcall TDisplay::GetFirmwareVersionString();
};
//---------------------------------------------------------------------------
extern PACKAGE TDisplay *Display;
//---------------------------------------------------------------------------
#endif
