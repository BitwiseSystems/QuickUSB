// QuickUsbVstudioDlg.h : header file
//

#if !defined(AFX_QUICKUSBVSTUDIODLG_H__CA59782A_E314_4388_9292_91BDC2BFC7DE__INCLUDED_)
#define AFX_QUICKUSBVSTUDIODLG_H__CA59782A_E314_4388_9292_91BDC2BFC7DE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CQuickUsb.h"

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbVstudioDlg dialog

class CQuickUsbVstudioDlg : public CDialog
{
// Construction
public:
	void UpdateDeviceDescriptor(void);
	void UpdateModuleComboBox(void);
	CQuickUsbVstudioDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQuickUsbVstudioDlg)
	enum { IDD = IDD_QUICKUSBVSTUDIO_DIALOG };
	CListBox	m_listBox;
	CEdit	m_descriptorEdit;
	CComboBox	m_moduleComboBox;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickUsbVstudioDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;
	CQuickUsb qusb;

	// Generated message map functions
	//{{AFX_MSG(CQuickUsbVstudioDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	virtual void OnOK();
	afx_msg void OnSelchangeModulecombobox();
	afx_msg void OnWritedata();
	afx_msg void OnReaddata();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKUSBVSTUDIODLG_H__CA59782A_E314_4388_9292_91BDC2BFC7DE__INCLUDED_)
