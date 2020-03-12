// QuickUsbEpcsProgDlg.h : header file
//

#pragma once
#include "CQuickUSB.h"
#include "afxwin.h"


// CQuickUsbEpcsProgDlg dialog
class CQuickUsbEpcsProgDlg : public CDialog
{
	DECLARE_DYNAMIC(CQuickUsbEpcsProgDlg);

// Construction
public:
	CQuickUsbEpcsProgDlg(CWnd* pParent = NULL);	// standard constructor
	virtual ~CQuickUsbEpcsProgDlg();

// Dialog Data
	enum { IDD = IDD_QUICKUSBEPCSPROG_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


// Implementation
protected:
	HICON m_hIcon;
	CQuickUsb *qusb;

	BOOL CanExit();

	// Generated message map functions
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	DECLARE_MESSAGE_MAP()
public:
   afx_msg void OnBnClickedProgEpcsButton();
   CEdit m_statusBar;
   CEdit m_fileNameEdit;
   afx_msg void OnBnClickedIsconfigButton();
   afx_msg void OnBnClickedButton1();
   afx_msg void OnBnClickedVerifyEpcsButton();
};

