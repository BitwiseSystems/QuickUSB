; CLW file contains information for the MFC ClassWizard

[General Info]
Version=1
LastClass=CQuickUsbDiagDlg
LastTemplate=CDialog
NewFileInclude1=#include "stdafx.h"
NewFileInclude2=#include "QuickUsbDiag.h"

ClassCount=3
Class1=CQuickUsbDiagApp
Class2=CQuickUsbDiagDlg
Class3=CAboutDlg

ResourceCount=3
Resource1=IDD_ABOUTBOX
Resource2=IDR_MAINFRAME
Resource3=IDD_QUICKUSBDIAG_DIALOG

[CLS:CQuickUsbDiagApp]
Type=0
HeaderFile=QuickUsbDiag.h
ImplementationFile=QuickUsbDiag.cpp
Filter=N
LastObject=CQuickUsbDiagApp

[CLS:CQuickUsbDiagDlg]
Type=0
HeaderFile=QuickUsbDiagDlg.h
ImplementationFile=QuickUsbDiagDlg.cpp
Filter=D
BaseClass=CDialog
VirtualFilter=dWC
LastObject=AsyncCheckBox

[CLS:CAboutDlg]
Type=0
HeaderFile=QuickUsbDiagDlg.h
ImplementationFile=QuickUsbDiagDlg.cpp
Filter=D
LastObject=CAboutDlg

[DLG:IDD_ABOUTBOX]
Type=1
Class=CAboutDlg
ControlCount=4
Control1=IDC_STATIC,static,1342177283
Control2=IDC_STATIC,static,1342308480
Control3=IDC_STATIC,static,1342308352
Control4=IDOK,button,1342373889

[DLG:IDD_QUICKUSBDIAG_DIALOG]
Type=1
Class=CQuickUsbDiagDlg
ControlCount=63
Control1=IDC_STATIC,button,1342177287
Control2=ModuleComboBox,combobox,1344277763
Control3=IDC_STATIC,static,1342308352
Control4=IDC_STATIC,static,1342308352
Control5=SerialNumberEditBox,edit,1350568064
Control6=IDC_STATIC,static,1342308352
Control7=ModuleDescriptionEditBox,edit,1350568064
Control8=IDC_STATIC,static,1342308352
Control9=HwRevEditBox,edit,1350568064
Control10=IDC_STATIC,static,1342308352
Control11=BusSpeedEditBox,edit,1350568064
Control12=IDC_STATIC,button,1342177287
Control13=FpgaBrowseButton,button,1342242816
Control14=FpgaFileEditBox,edit,1350631552
Control15=FpgaUploadButton,button,1342242816
Control16=FpgaStatusButton,button,1342242816
Control17=FpgaLoadProgressBar,msctls_progress32,1350565889
Control18=IDC_STATIC,button,1342177287
Control19=IDC_STATIC,static,1342308352
Control20=I2CAddressTextBox,edit,1350639744
Control21=IDC_STATIC,static,1342308352
Control22=I2CLengthTextBox,edit,1350639744
Control23=I2CReadButton,button,1342242816
Control24=I2CWriteButton,button,1342242816
Control25=I2CDataTextBox,edit,1352728580
Control26=IDC_STATIC,button,1342177287
Control27=DataReadBrowseButton,button,1342242816
Control28=DataReadFileTextBox,edit,1350631552
Control29=IDC_STATIC,static,1342308352
Control30=DataAddressTextBox,edit,1350639744
Control31=IDC_STATIC,static,1342308352
Control32=DataLengthTextBox,edit,1350639744
Control33=DataReadButton,button,1342242816
Control34=DataWriteButton,button,1342242816
Control35=IncrementAddressCheckBox,button,1342242819
Control36=AsyncCheckBox,button,1342242819
Control37=BufferSizeLabel,static,1342308352
Control38=BufferSizeTextBox,edit,1350641792
Control39=BufferCountLabel,static,1342308352
Control40=BufferCountTextBox,edit,1350641792
Control41=StatusMessage,edit,1350568064
Control42=DataWriteBrowseButton,button,1342242816
Control43=DataWriteFileTextBox,edit,1350631552
Control44=IDC_STATIC,button,1342177287
Control45=SpiPortComboBox,combobox,1344339971
Control46=IDC_STATIC,static,1342308352
Control47=IDC_STATIC,static,1342308352
Control48=SpiLengthTextBox,edit,1350639744
Control49=SpiReadButton,button,1342242816
Control50=SpiWriteButton,button,1342242816
Control51=SpiWriteReadButton,button,1342242816
Control52=SpiDataTextBox,edit,1352728580
Control53=IDC_STATIC,button,1342177287
Control54=IDC_STATIC,static,1342308352
Control55=Rs232PortComboBox,combobox,1344339971
Control56=IDC_STATIC,static,1342308352
Control57=Rs232BaudComboBox,combobox,1344340995
Control58=Rs232ReadButton,button,1342242816
Control59=Rs232WriteButton,button,1342242816
Control60=Rs232FlushButton,button,1342242816
Control61=Rs232DataTextBox,edit,1350631428
Control62=IDC_STATIC,static,1342308352
Control63=Rs232CharsWaitingTextBox,edit,1350641792

