// AsyncTestDlg.cpp : implementation file
//
/*=============================================================================
 Title        : AsyncTestDlg.cpp
 Description  : QuickUSB Asynchronous I/O test program
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

 $ History: AsyncTestDlg.cpp $


*/

#include "stdafx.h"
#include "AsyncTest.h"
#include "AsyncTestDlg.h"
#include "quickusb.h"
#include <io.h>
#include <fcntl.h>

#define MEGA (1024 * 1024)

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

typedef struct {
   unsigned long     bytesTransferred;
   unsigned char     transaction;
   unsigned char *   inData;
} MemBlockType;

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
// CAsyncTestDlg dialog

CAsyncTestDlg::CAsyncTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CAsyncTestDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CAsyncTestDlg)
	m_BufferSize = 1024;
	m_ByteCount = 2048;
	m_FileName = _T("");
	m_BufferCount = 2;
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CAsyncTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAsyncTestDlg)
	DDX_Text(pDX, BufferSizeEditBox, m_BufferSize);
	DDV_MinMaxLong(pDX, m_BufferSize, 1, 67108864);
	DDX_Text(pDX, ByteCountEditBox, m_ByteCount);
	DDV_MinMaxLong(pDX, m_ByteCount, 1, 2147483647);
	DDX_Text(pDX, FileNameEditBox, m_FileName);
	DDV_MaxChars(pDX, m_FileName, 80);
	DDX_Text(pDX, BufferCountEditBox, m_BufferCount);
	DDV_MinMaxInt(pDX, m_BufferCount, 2, 253);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAsyncTestDlg, CDialog)
	//{{AFX_MSG_MAP(CAsyncTestDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(ReadButton, OnReadButton)
	ON_BN_CLICKED(WriteButton, OnWriteButton)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CAsyncTestDlg message handlers

BOOL CAsyncTestDlg::OnInitDialog()
{
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
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CAsyncTestDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CAsyncTestDlg::OnPaint() 
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
HCURSOR CAsyncTestDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}


void DisplayError(char * MsgBoxBuffer, char * function, int index, int errorcode) {
   sprintf(MsgBoxBuffer, "QuickUsb%s error %d index %d\n\r", function, errorcode, index);

   switch(errorcode)
   {
   case QUICKUSB_ERROR_OUT_OF_MEMORY:
      strcat(MsgBoxBuffer, "A memory allocation function failed while executing a function.");
      break;
   case QUICKUSB_ERROR_CANNOT_OPEN_MODULE:
      strcat(MsgBoxBuffer, "The module was located, but was not able to be opened successfully.");
      break;
   case QUICKUSB_ERROR_CANNOT_FIND_DEVICE:
      strcat(MsgBoxBuffer, "The module could not be located.");
      break;
   case QUICKUSB_ERROR_IOCTL_FAILED:
      strcat(MsgBoxBuffer, "Unable to obtain necessary information from device.");
      break;
   case QUICKUSB_ERROR_INVALID_PARAMETER:
      strcat(MsgBoxBuffer, "Attempting to read or write data of length 0.");
      break;
   case QUICKUSB_ERROR_TIMEOUT:
      strcat(MsgBoxBuffer, "Timeout occured while trying to read or write data.");
      strcat(MsgBoxBuffer, "\nTry calling QuickUsbSetTimeout() with a longer time out.");
      break;
   case QUICKUSB_ERROR_FUNCTION_NOT_SUPPORTED:
      strcat(MsgBoxBuffer, "The function you are trying to execute is not supported by this driver version.");
      strcat(MsgBoxBuffer, "\nTry reloading the latest QuickUSB.sys driver file.");
      break;
   default:
      strcat(MsgBoxBuffer, "Unknown error.");
      break;
   }
}


void CAsyncTestDlg::OnReadButton() 
{
   unsigned long  BufSize;
   int            BufCount;
   long           TotalBlocks;
   unsigned long  startTime;
   unsigned long  endTime;
   FILE *         outputFile;
   QHANDLE         hDevice;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   unsigned long  errorCode;
   const char *   FileName;
   char           devName[128];
   char           messageBoxBuffer[500];
   double         elapsed_time;

   UpdateData(TRUE);
   BufSize = m_BufferSize;
   BufCount = m_BufferCount;
   FileName = (LPCTSTR)m_FileName;
   TotalBlocks = m_ByteCount / BufSize;

   if ((TotalBlocks * BufSize) != (unsigned long)m_ByteCount) {
      MessageBox ("Read Length must be a multiple of Buffer Size");
      return;
   }
   
   outputFile = fopen(FileName, "w+b");

   if (outputFile == NULL) {
      sprintf(messageBoxBuffer, "Unable to open file %s for output", FileName);
      MessageBox(messageBoxBuffer);
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
         sprintf(messageBoxBuffer, "Unable to allocate buffer memory");
         MessageBox(messageBoxBuffer);
         return;
      }
   }
      
   // Find the QuickUSB modules in the system
	QuickUsbFindModules(&devName[0], 128);

   // Open the first QuickUSB device
   result = QuickUsbOpen(&hDevice, devName);

   if (result == 0) {
      sprintf(messageBoxBuffer, "Cannot open Device: %s", devName);
      MessageBox(messageBoxBuffer);
      fclose(outputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      return;
   }
   
   // Get the current time
   CWaitCursor wait;
   startTime = GetTickCount();

   // Start up several asynchronous read commands
   for (Index = 0; Index < BufCount; Index++) {
      result = QuickUsbReadDataAsync(hDevice, memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         QuickUsbGetLastError (&errorCode);
         DisplayError(messageBoxBuffer, "ReadDataAsync",Index, errorCode);
         MessageBox(messageBoxBuffer);
         fclose(outputFile);
         QuickUsbClose(hDevice);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         return;
      }
   
      BlocksQueued++;
   }

   Index = 0;

   while (TRUE) {
      // Wait for the next QuickUSB read operation to finish
      memBlocks[Index].bytesTransferred = 0;
      result = QuickUsbAsyncWait(hDevice, &memBlocks[Index].bytesTransferred, memBlocks[Index].transaction, FALSE);

      if (result == FALSE) {
         QuickUsbGetLastError (&errorCode);
         DisplayError(messageBoxBuffer, "AsyncWait",Index, errorCode);
         MessageBox(messageBoxBuffer);
         fclose (outputFile);
         QuickUsbClose (hDevice);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

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
         result = QuickUsbReadDataAsync(hDevice, memBlocks[Index].inData, &BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            QuickUsbGetLastError (&errorCode);
            DisplayError(messageBoxBuffer, "ReadDataAsync",Index, errorCode);
            MessageBox(messageBoxBuffer);
            fclose(outputFile);
            QuickUsbClose(hDevice);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

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
   endTime = GetTickCount();
   fclose(outputFile);
   QuickUsbClose(hDevice);
   
   // Do the bandwidth calculation
   elapsed_time = (double)(endTime - startTime) / 1000;
   if (elapsed_time != 0) {
      sprintf(messageBoxBuffer, "Elapsed time %f seconds to transfer %f Megabytes.\nAverage throughput %f Megabytes per second.",
         elapsed_time, length / MEGA, (length / MEGA) / elapsed_time);
      MessageBox(messageBoxBuffer);
   }
   
   // Free the memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }
}

void CAsyncTestDlg::OnWriteButton() 
{
   unsigned long  BufSize;
   int            BufCount;
   long           TotalBlocks;
   unsigned long  startTime;
   unsigned long  endTime;
   unsigned long  fileLength = 0;
   FILE *         inputFile;
   QHANDLE         hDevice;
   int            Index;
   long           result;
   double         length = 0;
   MemBlockType   memBlocks[253];
   long           BlocksQueued = 0;
   long           BlocksFinished = 0;
   unsigned long  errorCode;
   const char *   FileName;
   char           devName[128];
   char           messageBoxBuffer[500];
   double         elapsed_time;
   int            fh;

   UpdateData(TRUE);
   BufSize = m_BufferSize;
   BufCount = m_BufferCount;
   FileName = (LPCTSTR)m_FileName;
   
   // Calculate the data length
   if ((fh = _open(FileName, _O_BINARY | _O_RDONLY, 0))  != -1) {
      fileLength = _filelength(fh);
      TotalBlocks = fileLength / BufSize;
      _close( fh );
   }
   
   // Open the input disk file for reading
   inputFile = fopen(FileName, "r+b");

   if ((inputFile == NULL) || (fileLength == 0)) {
      sprintf(messageBoxBuffer, "Unable to open file %s for input", FileName);
      MessageBox(messageBoxBuffer);
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
         sprintf(messageBoxBuffer, "Unable to allocate buffer memory");
         MessageBox(messageBoxBuffer);
         return;
      }
   }
      
   // Find the QuickUSB modules in the system
	QuickUsbFindModules(&devName[0], 128);

   // Open the first QuickUSB device
   result = QuickUsbOpen(&hDevice, devName);

   if (result == 0) {
      sprintf(messageBoxBuffer, "Cannot open Device: %s", devName);
      MessageBox(messageBoxBuffer);
      fclose(inputFile);

      for (Index = 0; Index < BufCount; Index++) {
         free(memBlocks[Index].inData);
      }

      return;
   }
   
   // Do initial write commands
   CWaitCursor wait;
   startTime = GetTickCount();
   
   for (Index = 0; Index < BufCount; Index++) {
      fread(memBlocks[Index].inData, 1, BufSize, inputFile);

      result = QuickUsbWriteDataAsync(hDevice, memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

      if (result == FALSE) {
         QuickUsbGetLastError(&errorCode);
         DisplayError(messageBoxBuffer, "WriteDataAsync",Index, errorCode);
         MessageBox(messageBoxBuffer);
         fclose(inputFile);
         QuickUsbClose(hDevice);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

         return;
      }

      BlocksQueued++;
   }

   Index = 0;
   
   // Wait for current writes to finish, and start up more writes to the QuickUSB
   while (TRUE) {

      // Wait for the next QuickUSB write operation to finish
      result = QuickUsbAsyncWait(hDevice, &memBlocks[Index].bytesTransferred, memBlocks[Index].transaction, FALSE);

      if (result == FALSE) {
         QuickUsbGetLastError(&errorCode);
         DisplayError(messageBoxBuffer, "AsyncWait",Index, errorCode);
         MessageBox(messageBoxBuffer);
         fclose(inputFile);
         QuickUsbClose(hDevice);

         for (Index = 0; Index < BufCount; Index++) {
            free(memBlocks[Index].inData);
         }

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

         result = QuickUsbWriteDataAsync(hDevice, memBlocks[Index].inData, BufSize, &memBlocks[Index].transaction);

         if (result == FALSE) {
            QuickUsbGetLastError(&errorCode);
            DisplayError(messageBoxBuffer, "ReadDataAsync",Index, errorCode);
            MessageBox(messageBoxBuffer);
            fclose(inputFile);
            QuickUsbClose(hDevice);

            for (Index = 0; Index < BufCount; Index++) {
               free(memBlocks[Index].inData);
            }

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
   endTime = GetTickCount();
   fclose(inputFile);
   QuickUsbClose(hDevice);
   
   // Do the bandwidth calculation
   elapsed_time = (double)(endTime - startTime) / 1000;
   if (elapsed_time != 0) {
      sprintf(messageBoxBuffer, "Elapsed time %f seconds to transfer %f Megabytes.\nAverage throughput %f Megabytes per second.",
         elapsed_time, length / MEGA, (length / MEGA) / elapsed_time);
      MessageBox(messageBoxBuffer);
   }
   
   // Free the memory
   for (Index = 0; Index < BufCount; Index++) {
      free(memBlocks[Index].inData);
   }
}
