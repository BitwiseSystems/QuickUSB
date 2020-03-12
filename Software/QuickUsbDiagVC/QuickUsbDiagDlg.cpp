// QuickUsbDiagDlg.cpp : implementation file
//
/*=============================================================================
 Title        : QuickUsbDiagDlg.cpp
 Description  : QuickUSB Data, I2C, and FPGA I/O test program
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
 (1998), and FAR 12.212, as applicable.  Bitwise Systems, 6489 Calle Real, Suite E,
 Goleta, CA  93117.

 Bitwise Systems
 6489 Calle Real, Suite E
 Santa Barbara, CA  93117
 Voice: (805) 683-6469
 Fax  : (805) 683-4833
 Web  : www.bitwisesys.com
 email: support@bitwisesys.com

 $History: QuickUsbDiagDlg.cpp $
 
 *****************  Version 5  *****************
 User: Bob Jacobson Date: 2/14/06    Time: 2:43p
 Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagVC6
 Bump version number.
 Correct call to QuickUsbGetLastError()
 Add more property info.
 
 *****************  Version 4  *****************
 User: Bob Jacobson Date: 1/13/06    Time: 2:40p
 Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagVC6
 Correct VSS $History keyword


*/

#include "stdafx.h"                    /* for MFC library */
#include "QuickUsbDiag.h"
#include "QuickUsbDiagDlg.h"
#include "CQuickUsb.h"                 /* for CQuickUsb class */
#include <Mmsystem.h>                  /* for timeGetTime() */
#include <io.h>                        /* for _open(), _filelength() */
#include <fcntl.h>                     /* for _O_BINARY and _O_RDONLY */
#include <dbt.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define MEGA (1024 * 1024)

// This data structure is used by the async read and write routines.
typedef struct {
   unsigned long     bytesTransferred;    /* bytes actually transferred */
   unsigned char     transaction;         /* I/O transaction number */
   unsigned char *   inData;              /* buffer pointer */
} MemBlockType;

CQuickUsb * qusb = new CQuickUsb("QUSB-0");

// This program uses the following timers.
typedef enum {
   ModuleTimerNumber = 1,              /* QuickUSB modle combo box update */
   StatusTimerNumber,                  /* Status line erase */
}  TimerNumbers;

/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
		// No message handlers
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbDiagDlg dialog

CQuickUsbDiagDlg::CQuickUsbDiagDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickUsbDiagDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickUsbDiagDlg)
	m_AsyncCheckBox = FALSE;
	m_SerialNumberEditBox = _T("");
	m_ModuleDescriptionEditBox = _T("");
	m_HwRevisionEditBox = _T("");
	m_BusSpeedEditBox = _T("");
	m_DataFileTextBox = _T("");
	m_FpgaFileEditBox = _T("");
	m_IncrementAddressTextBox = TRUE;
	m_StatusMessage = _T("");
	m_BufferSize = 0;
	m_DataAddress = 0;
	m_DataLength = 65536;
	m_DataReadFileTextBox = _T("");
	m_DataWriteFileTextBox = _T("");
	m_BufferCount = 4;
	m_I2CAddress = 0;
	m_I2CLength = 64;
	m_I2CData = _T("");
	m_Rs232CharsWaiting = 0;
	m_Rs232Data = _T("");
	m_SpiData = _T("");
	m_SpiLength = 64;
	m_SpiPort = 0;
	m_Rs232Port = 0;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuickUsbDiagDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickUsbDiagDlg)
	DDX_Control(pDX, Rs232BaudComboBox, m_Rs232Baud);
	DDX_Control(pDX, BufferSizeTextBox, m_BufferSizeTextBox);
	DDX_Control(pDX, BufferCountTextBox, m_BufferCountTextBox);
	DDX_Control(pDX, FpgaLoadProgressBar, m_FpgaProgressBar);
	DDX_Control(pDX, ModuleComboBox, m_ModuleComboBox);
	DDX_Check(pDX, AsyncCheckBox, m_AsyncCheckBox);
	DDX_Text(pDX, SerialNumberEditBox, m_SerialNumberEditBox);
	DDX_Text(pDX, ModuleDescriptionEditBox, m_ModuleDescriptionEditBox);
	DDX_Text(pDX, HwRevEditBox, m_HwRevisionEditBox);
	DDX_Text(pDX, BusSpeedEditBox, m_BusSpeedEditBox);
	DDX_Text(pDX, DataFileTextBox, m_DataFileTextBox);
	DDV_MaxChars(pDX, m_DataFileTextBox, 128);
	DDX_Text(pDX, FpgaFileEditBox, m_FpgaFileEditBox);
	DDV_MaxChars(pDX, m_FpgaFileEditBox, 128);
	DDX_Check(pDX, IncrementAddressCheckBox, m_IncrementAddressTextBox);
	DDX_Text(pDX, StatusMessage, m_StatusMessage);
	DDX_Text(pDX, BufferSizeTextBox, m_BufferSize);
	DDV_MinMaxLong(pDX, m_BufferSize, 0, 16777216);
	DDX_Text(pDX, DataAddressTextBox, m_DataAddress);
	DDV_MinMaxLong(pDX, m_DataAddress, 0, 511);
	DDX_Text(pDX, DataLengthTextBox, m_DataLength);
	DDX_Text(pDX, DataReadFileTextBox, m_DataReadFileTextBox);
	DDX_Text(pDX, DataWriteFileTextBox, m_DataWriteFileTextBox);
	DDX_Text(pDX, BufferCountTextBox, m_BufferCount);
	DDV_MinMaxInt(pDX, m_BufferCount, 2, 253);
	DDX_Text(pDX, I2CAddressTextBox, m_I2CAddress);
	DDV_MinMaxInt(pDX, m_I2CAddress, 0, 127);
	DDX_Text(pDX, I2CLengthTextBox, m_I2CLength);
	DDV_MinMaxInt(pDX, m_I2CLength, 1, 64);
	DDX_Text(pDX, I2CDataTextBox, m_I2CData);
	DDV_MaxChars(pDX, m_I2CData, 1024);
	DDX_Text(pDX, Rs232CharsWaitingTextBox, m_Rs232CharsWaiting);
	DDX_Text(pDX, Rs232DataTextBox, m_Rs232Data);
	DDX_Text(pDX, SpiDataTextBox, m_SpiData);
	DDX_Text(pDX, SpiLengthTextBox, m_SpiLength);
	DDV_MinMaxInt(pDX, m_SpiLength, 1, 64);
	DDX_CBIndex(pDX, SpiPortComboBox, m_SpiPort);
	DDX_CBIndex(pDX, Rs232PortComboBox, m_Rs232Port);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQuickUsbDiagDlg, CDialog)
	//{{AFX_MSG_MAP(CQuickUsbDiagDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(ModuleComboBox, OnSelchangeModuleComboBox)
	ON_BN_CLICKED(AsyncCheckBox, OnAsyncCheckBox)
	ON_BN_CLICKED(DataReadButton, OnDataReadButton)
	ON_BN_CLICKED(DataWriteButton, OnDataWriteButton)
	ON_BN_CLICKED(FpgaBrowseButton, OnFpgaBrowseButton)
	ON_BN_CLICKED(FpgaStatusButton, OnFpgaStatusButton)
	ON_BN_CLICKED(FpgaUploadButton, OnFpgaUploadButton)
	ON_BN_CLICKED(I2CReadButton, OnI2CReadButton)
	ON_BN_CLICKED(I2CWriteButton, OnI2CWriteButton)
	ON_WM_TIMER()
	ON_BN_CLICKED(DataWriteBrowseButton, OnDataWriteBrowseButton)
	ON_BN_CLICKED(DataReadBrowseButton, OnDataReadBrowseButton)
	ON_BN_CLICKED(SpiReadButton, OnSpiReadButton)
	ON_BN_CLICKED(SpiWriteButton, OnSpiWriteButton)
	ON_BN_CLICKED(SpiWriteReadButton, OnSpiWriteReadButton)
	ON_BN_CLICKED(Rs232WriteButton, OnRs232WriteButton)
	ON_BN_CLICKED(Rs232ReadButton, OnRs232ReadButton)
	ON_BN_CLICKED(Rs232FlushButton, OnRs232FlushButton)
	ON_CBN_SELCHANGE(Rs232PortComboBox, OnSelchangeRs232PortComboBox)
	ON_CBN_SELCHANGE(Rs232BaudComboBox, OnSelchangeRs232BaudComboBox)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbDiagDlg message handlers

BOOL CQuickUsbDiagDlg::OnInitDialog()
{
	DEV_BROADCAST_DEVICEINTERFACE notificationFilter;
   HDEVNOTIFY result;

   CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon
	
	// TODO: Add extra initialization here
   UpdateModuleComboBox();                /* See what QuickUSB modules are connected */
   m_Rs232Baud.SetCurSel(1);              /* set to 9600 baud */
   UpdateData(FALSE);                     /* write data to form */
   ModuleTimerEvent = SetTimer(ModuleTimerNumber, 3000, NULL);

   ZeroMemory(&notificationFilter, sizeof(notificationFilter));
   notificationFilter.dbcc_size = sizeof(DBT_DEVTYP_DEVICEINTERFACE);
   notificationFilter.dbcc_classguid.Data1 = 0x36FC9E60;
   notificationFilter.dbcc_classguid.Data2 = 0x11CF;
   notificationFilter.dbcc_classguid.Data3 = 0xC465;
   notificationFilter.dbcc_classguid.Data4[0] = 0x45;
   notificationFilter.dbcc_classguid.Data4[1] = 0x44;
   notificationFilter.dbcc_classguid.Data4[2] = 0x56;
   notificationFilter.dbcc_classguid.Data4[3] = 0x80;
   notificationFilter.dbcc_classguid.Data4[4] = 0x00;
   notificationFilter.dbcc_classguid.Data4[5] = 0x00;
   notificationFilter.dbcc_classguid.Data4[6] = 0x54;
   notificationFilter.dbcc_classguid.Data4[7] = 0x53;
   result = RegisterDeviceNotification(this->m_hWnd, &notificationFilter, DEVICE_NOTIFY_WINDOW_HANDLE);
   if (result == NULL) {

   }

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQuickUsbDiagDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CQuickUsbDiagDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQuickUsbDiagDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


// This function is called when a new QuickUSB module is selected from the Combo Box.
void CQuickUsbDiagDlg::OnSelchangeModuleComboBox() 
{
   char devName[16];

   UpdateData(TRUE);                   // Read data from form

   if (m_ModuleComboBox.GetCount() == 0) {
      m_ModuleComboBox.SetCurSel(0);
   }

   m_ModuleComboBox.GetLBText(m_ModuleComboBox.GetCurSel(), devName);
   qusb->SetName(devName);
   UpdateStringDescriptor();
   UpdateData(FALSE);                  // Write data to form
}


// This function is called when the Async check box is clicked.  It enables or disables
// the async buffer size and count text boxes, and sets reasonable values for data length.
void CQuickUsbDiagDlg::OnAsyncCheckBox() 
{
   UpdateData(TRUE);                   // Read data from form
   if (m_AsyncCheckBox) {
      m_BufferSizeTextBox.SetReadOnly(FALSE);
      m_BufferCountTextBox.SetReadOnly(FALSE);
      m_BufferSize = 65536;
      m_BufferCount = 4;
      m_DataLength = 1048576;
   }
   else {
      m_BufferSizeTextBox.SetReadOnly(TRUE);
      m_BufferCountTextBox.SetReadOnly(TRUE);
      m_BufferSize = 0;
      m_BufferCount = 4;
      m_DataLength = 65536;
   }
   UpdateData(FALSE);                  // Write data to form
}


// This function selects an output file for the Data Read function.
void CQuickUsbDiagDlg::OnDataReadBrowseButton() 
{
   UpdateData(TRUE);                   // Read data from form

   // Display the file open dialog box
   CFileDialog dlg(TRUE, "bin", "*.bin", OFN_HIDEREADONLY | OFN_CREATEPROMPT | OFN_PATHMUSTEXIST, 
      "Binary Files | (*.bin)|*.bin|All Files (*.*)|*.*||", 
      this);
   
   dlg.m_ofn.lpstrTitle = "Open the Output Data File";
   if (dlg.DoModal() == IDOK) {
      m_DataReadFileTextBox = dlg.GetPathName();
   }
   UpdateData(FALSE);                  // Write data to form
}


// This function selects an input file for the Data Write function.
void CQuickUsbDiagDlg::OnDataWriteBrowseButton() 
{
   UpdateData(TRUE);                   // Read data from form

   // Display the file open dialog box
   CFileDialog dlg(TRUE, "bin", "*.bin", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
      "Binary Files | (*.bin)|*.bin|All Files (*.*)|*.*||", 
      this);
   
   dlg.m_ofn.lpstrTitle = "Open the Input Data File";
   if (dlg.DoModal() == IDOK) {
      m_DataWriteFileTextBox = dlg.GetPathName();
   }
   UpdateData(FALSE);                  // Write data to form
}


// This function is called when the Read Data button is pressed.
void CQuickUsbDiagDlg::OnDataReadButton() 
{
   unsigned long     length;
   int               result;
   int               address;
   unsigned char *   inData;
   FILE *            DataFile;

   UpdateData(TRUE);                   // Read data from form
   address = m_DataAddress;
   length = m_DataLength;
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }

   // Check for an empty filename
   if (strlen(m_DataReadFileTextBox) == 0) {
      OnDataReadBrowseButton();                     // Display the browse box
      if (strlen(m_DataReadFileTextBox) == 0) {     // The user selected Cancel
         return;
      }
   }

   // Check for asynchronous read request.
   if (m_AsyncCheckBox) {
      AsyncDataRead();
      return;
   }

   // Allocate a data buffer
   inData = (unsigned char *)malloc(length);
   if (inData == NULL) {
      ShowStatus("Unable to allocate a memory buffer!");
      return;
   }

   DataFile = fopen(m_DataReadFileTextBox, "wb");
   if (DataFile == NULL) {
      free(inData);
      MessageBox("Cannot open data output file!", "File Open Error");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      free(inData);
      fclose(DataFile);
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   // Set the data address
   if (m_IncrementAddressTextBox == FALSE) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }
   
   if (result == FALSE) {
      free(inData);
      fclose(DataFile);
      DisplayError("WriteSetting", -1);
      return;
   }

   ShowStatus("Data read starting.", TRUE);
   CWaitCursor wait;                   /* show hourglass cursor */
   startTime = timeGetTime();
   result = qusb->ReadData(inData, &length);
   if (result == FALSE) {
      free(inData);
      fclose(DataFile);
      DisplayError("ReadData", -1);
      return;
   }

   fwrite(inData, length, 1, DataFile);
   endTime = timeGetTime();
   free(inData);
   fclose(DataFile);
   qusb->Close();
   sprintf(messageBoxBuffer, "Data read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   ThroughputCalc(length);
}


// This function is called when the Write Data button is pressed.
void CQuickUsbDiagDlg::OnDataWriteButton() 
{
   unsigned long     length;
   int               result;
   int               address;
   unsigned char *   inData;
   FILE *            DataFile;
   int               fh;
      
   UpdateData(TRUE);                   // Read data from form
   address = m_DataAddress;

   // Check for an empty filename
   if (strlen(m_DataWriteFileTextBox) == 0) {
      OnDataWriteBrowseButton();                     // Display the browse box
      if (strlen(m_DataWriteFileTextBox) == 0) {     // The user selected Cancel
         return;
      }
   }

   // Check for asynchronous write request.
   if (m_AsyncCheckBox) {
      AsyncDataWrite();
      return;
   }

   // Calculate the data length
   if ((fh = _open(m_DataWriteFileTextBox, _O_BINARY | _O_RDONLY, 0))  != -1) {
      length = _filelength(fh);
      _close(fh);
   }
   else {
      ShowStatus("Unable to compute length of input file.");
      return;
   }

   if (length == 0) {
      ShowStatus("Attempt to read an empty file!");
      return;
   }

   DataFile = fopen(m_DataWriteFileTextBox, "rb");
   if (DataFile == NULL) {
      MessageBox("Cannot open data input file!", "File Open Error");
      return;
   }

   // Allocate a data buffer
   inData = (unsigned char *)malloc(length);
   if (inData == NULL) {
      fclose(DataFile);
      ShowStatus("Unable to allocate a memory buffer!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      fclose(DataFile);
      free(inData);
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   // Set the data address
   if (m_IncrementAddressTextBox == FALSE) {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address | 0x8000);
   }
   else {
      result = qusb->WriteSetting(SETTING_DATAADDRESS, address);
   }
   
   if (result == FALSE) {
      free(inData);
      fclose(DataFile);
      DisplayError("WriteSetting", -1);
      return;
   }

   ShowStatus("Data write starting.", TRUE);
   CWaitCursor wait;                   /* show hourglass cursor */
   startTime = timeGetTime();
   fread(inData, length, 1, DataFile);
   result = qusb->WriteData(inData, length);
   endTime = timeGetTime();
   if (result == FALSE) {
      free(inData);
      fclose(DataFile);
      DisplayError("WriteData", -1);
      return;
   }

   free(inData);
   fclose(DataFile);
   qusb->Close();
   m_DataLength = length;
   UpdateData(FALSE);
   sprintf(messageBoxBuffer, "Data write %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   ThroughputCalc(length);
}


// This code processes async data reads.
void CQuickUsbDiagDlg::AsyncDataRead() 
{
   unsigned long  BufSize;
   int            BufCount;
   int            address;
   long           TotalBlocks;
   FILE *         outputFile;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;

   UpdateData(TRUE);                   // Read data from form

   // This is not a limitation of the QuickUSB, but the I/O will be 
   // very slow with a small block size.
   if (m_BufferSize < 64) {
      ShowStatus("Buffer size must be at least 64 bytes for async I/O!");
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (m_BufferCount < 2) {
      ShowStatus("Buffer count must be at least 2 for async I/O!");
      return;
   }

   BufSize = m_BufferSize;
   BufCount = m_BufferCount;
   TotalBlocks = m_DataLength / BufSize;
   address = m_DataAddress;

   // This is not a limitation of the QuickUSB, but it makes this demo
   // code much simpler.
   if ((TotalBlocks * BufSize) != (unsigned long)m_DataLength) {
      ShowStatus("Data Length must be a multiple of Buffer Size");
      return;
   }
   
   outputFile = fopen(m_DataReadFileTextBox, "w+b");

   if (outputFile == NULL) {
      MessageBox("Cannot open data output file!", "File Open Error");
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
         MessageBox("Unable to allocate buffer memory", "Out of Memory");
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

      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }
   
   // Set the data address
   if (m_IncrementAddressTextBox == FALSE) {
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

      DisplayError("WriteSetting", -1);
      return;
   }

      // Get the current time
   ShowStatus("Data read async starting.", TRUE);
   CWaitCursor wait;                   /* show hourglass cursor */
   startTime = timeGetTime();

   // Start up several asynchronous read commands
   for (Index = 0; Index < BufCount; Index++) {
      result = qusb->ReadDataAsync(memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         fclose(outputFile);

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
         fclose(outputFile);

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

   sprintf(messageBoxBuffer, "Data read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   ThroughputCalc((unsigned long) length);
}


// This code processes async data writes.
void CQuickUsbDiagDlg::AsyncDataWrite() 
{
   unsigned long  BufSize;
   int            BufCount;
   int            address;
   long           TotalBlocks;
   unsigned long  fileLength = 0;
   FILE *         inputFile;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   int            fh;

   UpdateData(TRUE);

   // This is not a limitation of the QuickUSB, but the I/O will be 
   // very slow with a small block size.
   if (m_BufferSize < 64) {
      ShowStatus("Buffer size must be at least 64 bytes for async I/O!");
      return;
   }

   // This is not a limitation of the QuickUSB, but using only 1 buffer
   // negates the benefits of async I/O.
   if (m_BufferCount < 2) {
      ShowStatus("Buffer count must be at least 2 for async I/O!");
      return;
   }

   BufSize = m_BufferSize;
   BufCount = m_BufferCount;
   address = m_DataAddress;
   
   // Calculate the data length
   if ((fh = _open(m_DataWriteFileTextBox, _O_BINARY | _O_RDONLY, 0))  != -1) {
      fileLength = _filelength(fh);
      TotalBlocks = fileLength / BufSize;
      _close( fh );
   }
   else {
      ShowStatus("Unable to compute length of input file.");
      return;
   }
   
   // Open the input disk file for reading
   inputFile = fopen(m_DataWriteFileTextBox, "r+b");

   if ((inputFile == NULL) || (fileLength == 0)) {
      MessageBox("Cannot open data input file!", "File Open Error");
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
         MessageBox("Unable to allocate buffer memory", "Out of Memory");
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

      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }
   
   // Set the data address
   if (m_IncrementAddressTextBox == FALSE) {
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

      DisplayError("WriteSetting", -1);
      return;
   }

      // Do initial write commands
   ShowStatus("Data write async starting.", TRUE);
   CWaitCursor wait;                   /* show hourglass cursor */
   startTime = timeGetTime();
   
   for (Index = 0; Index < BufCount; Index++) {
      fread(memBlocks[Index].inData, 1, BufSize, inputFile);

      result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         fclose(inputFile);

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
         fclose(inputFile);

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
         fread(memBlocks[Index].inData, 1, BufSize, inputFile);

         result = qusb->WriteDataAsync(memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            fclose(inputFile);

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
   endTime = timeGetTime();
   fclose(inputFile);
   qusb->Close();
   
   // Free the memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }

   m_DataLength = (long) length;
   UpdateData(FALSE);                  // Write data to form
   sprintf(messageBoxBuffer, "Data write %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   ThroughputCalc((unsigned long) length);
}


// This function selects an input FPGA file.
void CQuickUsbDiagDlg::OnFpgaBrowseButton() 
{
   UpdateData(TRUE);                   // Read data from form

   // Display the file open dialog box
   CFileDialog dlg(TRUE, "rbf", "*.rbf", OFN_HIDEREADONLY | OFN_FILEMUSTEXIST, 
      "RBF Files | (*.rbf)|*.rbf|All Files (*.*)|*.*||", 
      this);
   
   // Load the FPGA
   dlg.m_ofn.lpstrTitle = "Open the FPGA File";
   if (dlg.DoModal() == IDOK) {
      m_FpgaFileEditBox = dlg.GetPathName();
   }

   UpdateData(FALSE);                  // Write data to form
}


// This function displays the FPGA configuration status.
void CQuickUsbDiagDlg::OnFpgaStatusButton() 
{
   unsigned short isConfigured = 0;
   int            result;

   // Check that the FPGA is configured
   result = qusb->IsFpgaConfigured(&isConfigured);
   
   // Notify the user
   if ((isConfigured == 0) || (result == 0)) {
      ShowStatus ("FPGA configuration failed");
   }
   else {
      ShowStatus ("FPGA was successfully configured");
   }

   UpdateData(FALSE);                  // Write data to form
}


// This function uploads an FPGA file to the QuickUSB device.
void CQuickUsbDiagDlg::OnFpgaUploadButton() 
{
   int            Index = 0;
   int            result;
   unsigned short length;
   int            fileLength;
   unsigned short blockLength = 64;
   unsigned char  fpgaData[64];
   FILE *         fpgaFile;
   int            fh;

   UpdateData(TRUE);                   // Read data from form

   // Check for an empty filename
   if (strlen(m_FpgaFileEditBox) == 0) {
      OnFpgaBrowseButton();                     // Display the browse box
      if (strlen(m_FpgaFileEditBox) == 0) {     // The user selected Cancel
         return;
      }
   }

   // Calculate the data length
   if ((fh = _open(m_FpgaFileEditBox, _O_BINARY | _O_RDONLY, 0))  != -1) {
      fileLength = _filelength(fh);
      _close( fh );
   }
   else {
      ShowStatus("Unable to compute length of input file.");
      return;
   }

   if (fileLength == 0) {
      ShowStatus("Invalid FPGA file.  File is zero length.");
      return;
   }

   fpgaFile = fopen(m_FpgaFileEditBox, "rb");
   if (fpgaFile == NULL) {
      MessageBox("Cannot open FPGA binary file!", "File Open Error");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      fclose(fpgaFile);
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   // Start configuring the FPGA
   result = qusb->StartFpgaConfiguration();
   if (result == 0) {
      fclose(fpgaFile);
      DisplayError("StartFpgaConfiguration", -1);
      return;
   }
   
   m_FpgaProgressBar.SetPos(0);
   ShowStatus("Configuring FPGA.", TRUE);
   CWaitCursor wait;                   /* show hourglass cursor */

   // Send the FPGA configuration data one block at a time
   while (Index < fileLength) {
      // Read the data
      if ((fileLength - Index) < blockLength) {
         length = fileLength - Index;
      }
      else {
         length = blockLength;
      }
      
      //Read the data from the file
      fread(fpgaData, length, 1, fpgaFile);
            
      // Write the data to the FPGA
      result = qusb->WriteFpgaData(&fpgaData[0], length);
      if (result == 0) {
         fclose(fpgaFile);
         DisplayError("WriteFpgaData", -1);
         return;
      }
      
      // Update the counters
      Index += length;
      
      // Update the progress bar
      m_FpgaProgressBar.SetPos((Index  * 100) / fileLength);
   }

   m_FpgaProgressBar.SetPos(0);

   OnFpgaStatusButton();               // Check that the FPGA is configured
   fclose(fpgaFile);
   qusb->Close();
   UpdateData(FALSE);                  // Write data to form
}


// This function reads from the I2C bus of the QuickUSB.
void CQuickUsbDiagDlg::OnI2CReadButton() 
{
   int            result;
   int            address;
   short          i;
   unsigned short length;
   unsigned char  inData[64];
   char           tempBuf[8];

   UpdateData(TRUE);                   // Read data from form
   address = m_I2CAddress;
   length = m_I2CLength;

   // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }
   if (length > 64) {
      ShowStatus("Length must not be greater than 64!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->ReadI2C(address, inData, &length);
   if (result == FALSE) {
      DisplayError("ReadI2C", -1);
      return;
   }

   qusb->Close();
   sprintf(messageBoxBuffer, "I2C read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   messageBoxBuffer[0] = 0;            /* clear the string */
   for (i = 0; i < length; i++) {
      sprintf(tempBuf, "0x%.2X ", inData[i]);
      strcat(messageBoxBuffer, tempBuf);
   }
   m_I2CData = messageBoxBuffer;
   UpdateData(FALSE);                  // Write data to form
}


// This function writes to the I2C bus of the QuickUSB.
void CQuickUsbDiagDlg::OnI2CWriteButton() 
{
   int            result;
   int            address;
   int            i = 0;
   unsigned short length;
   unsigned char  inData[64];
   char           tempBuf[8];
   char           seps[] = " ,\t\n";
   char *         token;

   UpdateData(TRUE);                   // Read data from form
   address = m_I2CAddress;
   strcpy(messageBoxBuffer, m_I2CData);
   token = strtok(messageBoxBuffer, seps);

   // Convert the data to binary form.
   while (token) {
      result = 0;

      // Is this a Hex number?
      if ((token[0] == '0') && (token[1] == 'x')) {
         sscanf(token, "0x%x", &result);
      }
      else {
         sscanf(token, "%d", &result);
      }

      inData[i] = result;
      i++;
      if (i >= sizeof(inData)) {
         break;
      }
      token = strtok(NULL, seps);
   }

   length = i;   

      // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->WriteI2C(address, inData, length);
   if (result == FALSE) {
      DisplayError("WriteI2C", -1);
      return;
   }

   qusb->Close();
   m_I2CLength = length;
   sprintf(messageBoxBuffer, "I2C write %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);

   // Reformat the user's data so it is neater.
   messageBoxBuffer[0] = 0;            /* clear the string */
   for (i = 0; i < length; i++) {
      sprintf(tempBuf, "0x%.2X ", inData[i]);
      strcat(messageBoxBuffer, tempBuf);
   }
   m_I2CData = messageBoxBuffer;
   UpdateData(FALSE);                  // Write data to form
}


// This function updates the read characters pending and sets the baud rate for
// the selected RS-232 port.
void CQuickUsbDiagDlg::UpdateRs232() 
{
   unsigned char  port;
   int            result;
   unsigned short charsWaiting;
   unsigned long  baudRate;
   int            curSel;
   char           baudString[10];
   
   UpdateData(TRUE);                   // Read data from form
   port = m_Rs232Port;
   curSel = m_Rs232Baud.GetCurSel();
   m_Rs232Baud.GetLBText(curSel, baudString);
   baudRate = atoi(baudString);
   result = qusb->Open();
   if (result == FALSE) {
      return;
   }

   result = qusb->GetNumRS232(port, &charsWaiting);
   if (result == FALSE) {
      ShowStatus("Cannot get RS-232 characters waiting!");
      return;
   }

   result = qusb->SetRS232BaudRate(baudRate);
   if (result == FALSE) {
      ShowStatus("Cannot set RS-232 baud rate!");
      return;
   }

   qusb->Close();
   m_Rs232CharsWaiting = charsWaiting;
   UpdateData(FALSE);                  // Write data to form
}


// This function is called when the RS-232 port is changed.
void CQuickUsbDiagDlg::OnSelchangeRs232PortComboBox() 
{
   unsigned char  port;
   int            result;
   unsigned short charsWaiting;
   
   UpdateData(TRUE);                   // Read data from form
   port = m_Rs232Port;
   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->GetNumRS232(port, &charsWaiting);
   if (result == FALSE) {
      DisplayError("GetNumRS232", -1);
      return;
   }

   qusb->Close();
   m_Rs232CharsWaiting = charsWaiting;
   UpdateData(FALSE);                  // Write data to form
}


// This function is called when the RS-232 baud rate is changed.
void CQuickUsbDiagDlg::OnSelchangeRs232BaudComboBox() 
{
   int            result;
   unsigned long  baudRate;
   int            curSel;
   char           baudString[10];
   
   UpdateData(TRUE);                   // Read data from form
   curSel = m_Rs232Baud.GetCurSel();
   m_Rs232Baud.GetLBText(curSel, baudString);
   baudRate = atoi(baudString);
   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->SetRS232BaudRate(baudRate);
   if (result == FALSE) {
      DisplayError("SetRS232BaudRate", -1);
      return;
   }

   qusb->Close();
   UpdateData(FALSE);                  // Write data to form
}


// This function reads all pending data from the selected RS-232 port.
void CQuickUsbDiagDlg::OnRs232ReadButton() 
{
   int            result;
   unsigned char  port;
   unsigned short length;
   unsigned char  inData[130];

   UpdateData(TRUE);                   // Read data from form
   port = m_Rs232Port;

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->GetNumRS232(port, &length);
   if (result == FALSE) {
      DisplayError("GetNumRS232", -1);
      return;
   }

   // Check for a valid length.
   if (length <= 0) {
      ShowStatus("No data available to read!");
      qusb->Close();
      return;
   }

   result = qusb->ReadRS232(port, inData, &length);
   if (result == FALSE) {
      DisplayError("ReadRS232", -1);
      return;
   }

   qusb->Close();
   sprintf(messageBoxBuffer, "RS-232 read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   inData[length] = 0;                 /* terminate the input string */
   m_Rs232Data = inData;
   UpdateData(FALSE);                  // Write data to form
}


// This function writes data to the selected RS-232 port.
void CQuickUsbDiagDlg::OnRs232WriteButton() 
{
   int            result;
   unsigned char  port;
   unsigned short length;
   unsigned char  inData[128];

   UpdateData(TRUE);                   // Read data from form
   port = m_Rs232Port;
   length = m_Rs232Data.GetLength();

   // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Data length must be greater than zero!");
      return;
   }
   if (length > 128) {
      length = 128;
   }

   memcpy(inData, m_Rs232Data, length);
   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->WriteRS232(port, inData, length);
   if (result == FALSE) {
      DisplayError("WriteRS232", -1);
      return;
   }

   qusb->Close();
   sprintf(messageBoxBuffer, "RS-232 write %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   UpdateData(FALSE);                  // Write data to form
}


// This function flushes the selected RS232 port buffer.
void CQuickUsbDiagDlg::OnRs232FlushButton() 
{
   unsigned char  port;
   int            result;
   
   UpdateData(TRUE);                   // Read data from form
   port = m_Rs232Port;
   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->FlushRS232(port);
   if (result == FALSE) {
      DisplayError("FlushRS232", -1);
      return;
   }

   qusb->Close();
   m_Rs232CharsWaiting = 0;
   ShowStatus("RS-232 flush successful.");
   UpdateData(FALSE);                  // Write data to form
}


// This function reads from the QuickUSB SPI port.
void CQuickUsbDiagDlg::OnSpiReadButton() 
{
   int            result;
   unsigned char  port;
   short          i;
   unsigned short length;
   unsigned char  inData[64];
   char           tempBuf[8];

   UpdateData(TRUE);                   // Read data from form
   port = m_SpiPort;
   length = m_SpiLength;

   // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }
   if (length > 64) {
      ShowStatus("Length must not be greater than 64!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->ReadSpi(port, inData, &length);
   if (result == FALSE) {
      DisplayError("ReadSpi", -1);
      return;
   }

   qusb->Close();
   sprintf(messageBoxBuffer, "SPI read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);
   messageBoxBuffer[0] = 0;            /* clear the string */
   for (i = 0; i < length; i++) {
      sprintf(tempBuf, "0x%.2X ", inData[i]);
      strcat(messageBoxBuffer, tempBuf);
   }
   m_SpiData = messageBoxBuffer;
   UpdateData(FALSE);                  // Write data to form
}


// This function writes to the QuickUSB SPI port.
void CQuickUsbDiagDlg::OnSpiWriteButton() 
{
   int            result;
   unsigned char  port;
   int            i = 0;
   unsigned short length;
   unsigned char  inData[64];
   char           tempBuf[8];
   char           seps[] = " ,\t\n";
   char *         token;

   UpdateData(TRUE);                   // Read data from form
   port = m_SpiPort;
   strcpy(messageBoxBuffer, m_SpiData);
   token = strtok(messageBoxBuffer, seps);

   // Convert the data to binary form.
   while (token) {
      result = 0;

      // Is this a Hex number?
      if ((token[0] == '0') && (token[1] == 'x')) {
         sscanf(token, "0x%x", &result);
      }
      else {
         sscanf(token, "%d", &result);
      }

      inData[i] = result;
      i++;
      if (i >= sizeof(inData)) {
         break;
      }
      token = strtok(NULL, seps);
   }

   length = i;   

      // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->WriteSpi(port, inData, length);
   if (result == FALSE) {
      DisplayError("WriteSpi", -1);
      return;
   }

   qusb->Close();
   m_SpiLength = length;
   sprintf(messageBoxBuffer, "SPI write %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);

   // Reformat the user's data so it is neater.
   messageBoxBuffer[0] = 0;            /* clear the string */
   for (i = 0; i < length; i++) {
      sprintf(tempBuf, "0x%.2X ", inData[i]);
      strcat(messageBoxBuffer, tempBuf);
   }
   m_SpiData = messageBoxBuffer;
   UpdateData(FALSE);                  // Write data to form
}


// This function writes and reads the QuickUSB SPI port.
void CQuickUsbDiagDlg::OnSpiWriteReadButton() 
{
   int            result;
   unsigned char  port;
   int            i = 0;
   unsigned short length;
   unsigned char  inData[64];
   char           tempBuf[8];
   char           seps[] = " ,\t\n";
   char *         token;

   UpdateData(TRUE);                   // Read data from form
   port = m_SpiPort;
   strcpy(messageBoxBuffer, m_SpiData);
   token = strtok(messageBoxBuffer, seps);

   // Convert the data to binary form.
   while (token) {
      result = 0;

      // Is this a Hex number?
      if ((token[0] == '0') && (token[1] == 'x')) {
         sscanf(token, "0x%x", &result);
      }
      else {
         sscanf(token, "%d", &result);
      }

      inData[i] = result;
      i++;
      if (i >= sizeof(inData)) {
         break;
      }
      token = strtok(NULL, seps);
   }

   length = i;   

      // Check for a valid length.
   if (length <= 0) {
      ShowStatus("Length must be greater than zero!");
      return;
   }

   result = qusb->Open();
   if (result == FALSE) {
      MessageBox("Cannot open QuickUSB module!", "QuickUSB Open Error");
      return;
   }

   result = qusb->WriteReadSpi(port, inData, length);
   if (result == FALSE) {
      DisplayError("WriteReadSpi", -1);
      return;
   }

   qusb->Close();
   m_SpiLength = length;
   sprintf(messageBoxBuffer, "SPI write read %d bytes successful.", length);
   ShowStatus(messageBoxBuffer);

   // Reformat the user's data so it is neater.
   messageBoxBuffer[0] = 0;            /* clear the string */
   for (i = 0; i < length; i++) {
      sprintf(tempBuf, "0x%.2X ", inData[i]);
      strcat(messageBoxBuffer, tempBuf);
   }
   m_SpiData = messageBoxBuffer;
   UpdateData(FALSE);                  // Write data to form
}


// This function updates the General section of the form.
void CQuickUsbDiagDlg::UpdateStringDescriptor(void)
{
   char           devName[16] = "";
   char           strbuf[128] = "";
   int            result;
   int            sel;
   unsigned short VersionSpeed = 0;

   UpdateData(TRUE);                   // Read data from form

   // Get device name
   sel = m_ModuleComboBox.GetCurSel();
   m_ModuleComboBox.GetLBText(sel, devName);
   qusb->SetName(devName); 

   // Get the string descriptor
   result = qusb->Open();
   if (result == FALSE) {
      m_ModuleDescriptionEditBox = "";
      m_SerialNumberEditBox = "";
      m_BusSpeedEditBox = "";
      m_HwRevisionEditBox = "";
      UpdateData(FALSE);                  // Write data to form
      return;
   }

   // Get the QuickUSB model name
   result = qusb->GetStringDescriptor(QUICKUSB_MODEL, strbuf, sizeof(strbuf));
   if (result == FALSE) {
      m_ModuleDescriptionEditBox = "";
   }
   else {
      m_ModuleDescriptionEditBox = strbuf;
   }

   // Get the QuickUSB serial number
   result = qusb->GetStringDescriptor(QUICKUSB_SERIAL, strbuf, sizeof(strbuf));
   if (result == FALSE) {
      m_SerialNumberEditBox = "";
   }
   else {
      m_SerialNumberEditBox = strbuf;
   }

   // Get USB bus speed and hardware revision.
   // NOTE This feature was implemented with QuickUSB firmware 2.11.
   result = qusb->ReadSetting(SETTING_VERSIONSPEED, &VersionSpeed);
   if (result == FALSE) {
      m_BusSpeedEditBox = "";
      m_HwRevisionEditBox = "";
   }
   else {
      // The low order byte of SETTING_VERSIONSPEED is zero for Full Speed,
      // and non zero for High Speed.
      if (VersionSpeed & 0xFF) {
         m_BusSpeedEditBox = "High Speed (480Mbps)";
      }
      else {
         m_BusSpeedEditBox = "Full Speed (12Mbps)";
      }

      // The high order byte of SETTING_VERSIONSPEED is the Cypress hardware version.
      // See the REVID register description in the TRM.  The following values are
      // defined as of this writing.
      switch ((VersionSpeed & 0xFF00) >> 8) {
      case 0:
         m_HwRevisionEditBox = "CY7C68013 Rev A/B (EZ-USB FX2)";
         break;
      case 1:
         m_HwRevisionEditBox = "CY7C68013A Rev A (EZ-USB FX2LP)";
         break;
      case 2:
         m_HwRevisionEditBox = "CY7C68013 Rev C/D (EZ-USB FX2)";
         break;
      case 4:
         m_HwRevisionEditBox = "CY7C68013 Rev E (EZ-USB FX2)";
         break;
      default:
         m_HwRevisionEditBox = "Unknown";
      }
   }

   qusb->Close();
   UpdateData(FALSE);                  // Write data to form
   UpdateRs232();                      /* set baud rate and chars waiting */
}


// This function is called at startup, and by the 3 second timer.  It
// repopulates the Module Combo Box with the QuickUSB modules currently
// connected to the system.
void CQuickUsbDiagDlg::UpdateModuleComboBox()
{
   char *         str;
   char           devName[16];
   char           nameList[512];
   int            curSel;
   int            oldNumModules;
   int            result;
   unsigned long  length = sizeof(nameList);

   UpdateData(TRUE);                   // Read data from form

   // Remember the last selection name
   oldNumModules = m_ModuleComboBox.GetCount();
   curSel = m_ModuleComboBox.GetCurSel();
   if (curSel == LB_ERR) {
      devName[0] = NULL;
   }
   else {
      m_ModuleComboBox.GetLBText(curSel, devName);
   }

   // Clear the list box and rescan the bus
   result = qusb->FindModules(nameList, length);
   str = nameList;
   if (str[0] == 0) {     // No modules present
      m_ModuleComboBox.ResetContent();
      UpdateStringDescriptor();
      return;
   }

   // Populate the list
   m_ModuleComboBox.ResetContent();
   while (*str != '\0') {
      m_ModuleComboBox.AddString(str);
      str = str + strlen(str) + 1;
   }

   // Reselect the old module name
   if (devName[0] != NULL) {
      curSel = m_ModuleComboBox.FindString(-1, devName);
      if (curSel != CB_ERR) {
         m_ModuleComboBox.SetCurSel(curSel);
      }
      else {
         m_ModuleComboBox.SetCurSel(0);
      }
   }
   else {
      m_ModuleComboBox.SetCurSel(0);
   }

   // If the number of modules changed, update the display and connection state
   if (oldNumModules != m_ModuleComboBox.GetCount()) {
      curSel = m_ModuleComboBox.GetCurSel();
      m_ModuleComboBox.GetLBText(curSel, devName);
      qusb->SetName(devName); 
      UpdateStringDescriptor();   
   }

   UpdateData(FALSE);                  // Write data to form
}


// This function writes a status message to the form, and starts a 5 second timer which will
// erase the message.
void CQuickUsbDiagDlg::ShowStatus(char * NewStatusMessage, boolean redrawWindow)
{
   if (StatusTimerEvent) {             /* kill previous status timer, if any */
      KillTimer(StatusTimerEvent);
   }

   m_StatusMessage = NewStatusMessage;
   StatusTimerEvent = SetTimer(StatusTimerNumber, 5000, NULL);
   UpdateData(FALSE);                  /* Write data to form */

   if (redrawWindow == TRUE) {
      UpdateWindow();
   }
}


// This function is called when any of the timers expire.
#ifdef _WIN64
void CQuickUsbDiagDlg::OnTimer(UINT_PTR nIDEvent) 
#else
void CQuickUsbDiagDlg::OnTimer(UINT nIDEvent) 
#endif
{
   if (nIDEvent == ModuleTimerEvent) {
      UpdateModuleComboBox();	            /* check current QuickUSB population */
      UpdateRs232();                      /* update RS-232 characters waiting */
      return;
   }

   if (nIDEvent == StatusTimerEvent) {
      UpdateData(TRUE);                   /* Read data from form */
      m_StatusMessage = "";               /* erase the status message */
      UpdateData(FALSE);                  /* Write data to form */
      KillTimer(StatusTimerEvent);        /* this timer is no longer needed */
      StatusTimerEvent = 0;               /* show timer not active */
      return;
   }
}


// This function computes the elapsed time and throughput for data reads and writes.
// NOTE See the Microsoft timeGetTime() function documentation for the precision of
// this calculation.
void CQuickUsbDiagDlg::ThroughputCalc(unsigned long dataLength)
{
   double   elapsed_time;
   double   floatLength;

   // Do the bandwidth calculation
   elapsed_time = (double)(endTime - startTime) / 1000;
   floatLength = dataLength;
   if (elapsed_time != 0) {
      sprintf(messageBoxBuffer, "Elapsed time %g seconds to transfer %g Megabytes.\nAverage throughput %g Megabytes per second.",
         elapsed_time, floatLength / MEGA, (floatLength / MEGA) / elapsed_time);
      MessageBox(messageBoxBuffer, "Throughput");
   }
}


// This function formats a QuickUSB error message.
void CQuickUsbDiagDlg::DisplayError(char * function, int index) {
   QuickUsbGetLastError(&errorCode);
   qusb->Close();

   if (index == -1) {
      sprintf(messageBoxBuffer, "QuickUsb%s error %d \n\r", function, errorCode);
   }
   else {
      sprintf(messageBoxBuffer, "QuickUsb%s error %d index %d\n\r", function, errorCode, index);
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
      strcat(messageBoxBuffer, "\nTry reloading the latest QuickUSB.sys driver file.");
      break;
   default:
      strcat(messageBoxBuffer, "Unknown error.");
      break;
   }

   MessageBox(messageBoxBuffer, "QuickUSB Error", MB_OK | MB_ICONSTOP);
}
