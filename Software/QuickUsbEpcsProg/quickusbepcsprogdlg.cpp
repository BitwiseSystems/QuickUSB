// QuickUsbEpcsProgDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuickUsbEpcsProg.h"
#include "QuickUsbEpcsProgDlg.h"
#include ".\quickusbepcsprogdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};


CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}


void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CQuickUsbEpcsProgDlg dialog



IMPLEMENT_DYNAMIC(CQuickUsbEpcsProgDlg, CDialog);
CQuickUsbEpcsProgDlg::CQuickUsbEpcsProgDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickUsbEpcsProgDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
   qusb = new CQuickUsb("QUSB-0");
}


CQuickUsbEpcsProgDlg::~CQuickUsbEpcsProgDlg()
{
}


void CQuickUsbEpcsProgDlg::DoDataExchange(CDataExchange* pDX)
{
   CDialog::DoDataExchange(pDX);
   DDX_Control(pDX, IDC_STATUS_EDIT, m_statusBar);
   DDX_Control(pDX, IDC_FILENAME_EDIT, m_fileNameEdit);
}


BEGIN_MESSAGE_MAP(CQuickUsbEpcsProgDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_PROG_EPCS_BUTTON, OnBnClickedProgEpcsButton)
   ON_BN_CLICKED(IDC_ISCONFIG_BUTTON, OnBnClickedIsconfigButton)
   ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
	ON_BN_CLICKED(IDC_VERIFY_EPCS_BUTTON, OnBnClickedVerifyEpcsButton)
END_MESSAGE_MAP()


/*
BEGIN_MESSAGE_MAP(QuickUsbEpcsProgDlg, CDialog)
	//{{AFX_MSG_MAP(QuickUsbEpcsProgDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
   ON_BN_CLICKED(IDC_PROG_EPCS_BUTTON, OnBnClickedProgEpcsButton)
   ON_BN_CLICKED(IDC_ISCONFIG_BUTTON, OnBnClickedIsconfigButton)
   ON_BN_CLICKED(IDC_BUTTON1, OnBnClickedButton1)
END_MESSAGE_MAP()
*/

// CQuickUsbEpcsProgDlg message handlers

BOOL CQuickUsbEpcsProgDlg::OnInitDialog()
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

void CQuickUsbEpcsProgDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQuickUsbEpcsProgDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

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

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CQuickUsbEpcsProgDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CQuickUsbEpcsProgDlg::OnClose() 
{
	if (CanExit())
		CDialog::OnClose();
}

void CQuickUsbEpcsProgDlg::OnOK() 
{
	if (CanExit())
		CDialog::OnOK();
}

void CQuickUsbEpcsProgDlg::OnCancel() 
{
	if (CanExit())
		CDialog::OnCancel();
}

BOOL CQuickUsbEpcsProgDlg::CanExit()
{
	return TRUE;
}


void CQuickUsbEpcsProgDlg::OnBnClickedProgEpcsButton()
{
	char filename[256];
	unsigned short isConfigured;
	char namelist[1024];
   int result;
		
	qusb->FindModules(namelist, 1024);
	qusb->SetName(namelist);   // Just use the first device in the list
	
	m_fileNameEdit.GetWindowText((LPTSTR) &filename, 256);
	if (strlen(filename) == 0) {
	   AfxMessageBox("Please enter a fully qualified pathname to an RBF or RPD file");
	   return;
	}
	
	m_statusBar.SetWindowText("Programming EPCS device...please wait");
	UpdateWindow();
	
	// Erase and program the EPCS device 
	result = qusb->ConfigureEpcs(0, filename, 0, NULL, NULL);
   if (result == FALSE) {
	   m_statusBar.SetWindowText("EPCS programming failed");
	   return;
   } else {
	   m_statusBar.SetWindowText("EPCS successfully configured");
   }

	UpdateWindow();
   Sleep(1500);
	
	// Check to see if the FPGA is configured
	qusb->IsFpgaConfigured(&isConfigured);
	if (isConfigured) {
	   m_statusBar.SetWindowText("FPGA successfully configured");
	}
	else {
	   m_statusBar.SetWindowText("FPGA configuration failed");
	}
}



void CQuickUsbEpcsProgDlg::OnBnClickedButton1()
{
	char namelist[1024];
		
	qusb->FindModules(namelist, 1024);
	qusb->SetName(namelist);   // Just use the first device in the list

	m_statusBar.SetWindowText("Erasing EPCS device...please wait");
	UpdateWindow();
	
	// Erase and program the EPCS device 
    if (qusb->EraseEpcs(0, 0, NULL, NULL)) {
	   m_statusBar.SetWindowText("EPCS successfully erased");
    } else {
	   m_statusBar.SetWindowText("EPCS erase failed");
    }

	UpdateWindow();
}



void CQuickUsbEpcsProgDlg::OnBnClickedVerifyEpcsButton()
{
	char namelist[1024];
	char filename[256];
   int result;
		
	qusb->FindModules(namelist, 1024);
	qusb->SetName(namelist);   // Just use the first device in the list

	m_fileNameEdit.GetWindowText((LPTSTR) &filename, 256);
	if (strlen(filename) == 0) {
	   AfxMessageBox("Please enter a fully qualified pathname to an RBF or RPD file");
	   return;
	}
	
	m_statusBar.SetWindowText("Verifying EPCS device...please wait");
	UpdateWindow();
	
	// Erase and program the EPCS device 
    result = qusb->VerifyEpcs(0, filename, 0, NULL, NULL);
   if (result) {
	   m_statusBar.SetWindowText("Verify passed");
   }
   else {
	   m_statusBar.SetWindowText("Verify failed");
   }
	UpdateWindow();
}



void CQuickUsbEpcsProgDlg::OnBnClickedIsconfigButton()
{
	char namelist[1024];
	unsigned short isConfigured;
		
	qusb->FindModules(namelist, 1024);
	qusb->SetName(namelist);   // Just use the first device in the list


  // Check to see if the FPGA is configured
	qusb->IsFpgaConfigured(&isConfigured);
	if (isConfigured) {
	   m_statusBar.SetWindowText("FPGA Successfully configured");
	}
	else {
	   m_statusBar.SetWindowText("FPGA configuration failed");
	}
}


