// QuickUsbVstudioDlg.cpp : implementation file
//

#include "stdafx.h"
#include "QuickUsbVstudio.h"
#include "QuickUsbVstudioDlg.h"
#include "QuickUsb.h"
#include ".\quickusbvstudiodlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

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
// CQuickUsbVstudioDlg dialog

CQuickUsbVstudioDlg::CQuickUsbVstudioDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CQuickUsbVstudioDlg::IDD, pParent)
{
	//{{AFX_DATA_INIT(CQuickUsbVstudioDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CQuickUsbVstudioDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CQuickUsbVstudioDlg)
	DDX_Control(pDX, IDC_LISTBOX, m_listBox);
	DDX_Control(pDX, IDC_DESCRIPTOREDIT, m_descriptorEdit);
	DDX_Control(pDX, IDC_MODULECOMBOBOX, m_moduleComboBox);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CQuickUsbVstudioDlg, CDialog)
	//{{AFX_MSG_MAP(CQuickUsbVstudioDlg)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_MODULECOMBOBOX, OnSelchangeModulecombobox)
	ON_BN_CLICKED(IDC_WRITEDATA, OnWritedata)
	ON_BN_CLICKED(IDC_READDATA, OnReaddata)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbVstudioDlg message handlers

BOOL CQuickUsbVstudioDlg::OnInitDialog()
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
   UpdateModuleComboBox();	
   m_moduleComboBox.SetCurSel(0);
   UpdateDeviceDescriptor();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CQuickUsbVstudioDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CQuickUsbVstudioDlg::OnPaint() 
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
HCURSOR CQuickUsbVstudioDlg::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}

void CQuickUsbVstudioDlg::OnOK() 
{
	// TODO: Add extra validation here
	
	CDialog::OnOK();
}

void CQuickUsbVstudioDlg::UpdateModuleComboBox()
{
   char *str, nameList[512];
   int result;
   unsigned long length = 512;

   result = qusb.FindModules(nameList, length);
   str = nameList;
   while (*str != '\0') {
      m_moduleComboBox.AddString(str);
      str = str + strlen(str) + 1;
   }
}

void CQuickUsbVstudioDlg::UpdateDeviceDescriptor(void)
{
   char devName[16];
   char strbuf[128];
   int result;
   int sel;

   // Get device name
   sel = m_moduleComboBox.GetCurSel();
   m_moduleComboBox.GetLBText(sel, devName);
   qusb.SetDeviceName(devName);
   qusb.GetStringDescriptor(2, strbuf, 128);
   m_descriptorEdit.SetWindowText(strbuf);
}


void CQuickUsbVstudioDlg::OnSelchangeModulecombobox() 
{
   UpdateDeviceDescriptor();
}


void CQuickUsbVstudioDlg::OnWritedata() 
{
   unsigned short index;
   unsigned short data[1024];

   // Initialize the array with a count sequence
   for (index = 0; index < 1024; index++) {
      data[index] = index;
   }

   qusb.WriteData((unsigned char *) data, 1024 * sizeof(unsigned short));
}

void CQuickUsbVstudioDlg::OnReaddata() 
{
   unsigned short index;
   unsigned short data[1024];
   unsigned long length;
   CString str;

	length = 1024 * sizeof(unsigned short);
   qusb.ReadData((unsigned char *) data, &length);

   // Populate the listbox with the results
   m_listBox.ResetContent();
   for (index=0; index < 1024; index++) {
      str.Format("Item %d = %04X", index, data[index]);
      m_listBox.AddString(str);
   }
}

