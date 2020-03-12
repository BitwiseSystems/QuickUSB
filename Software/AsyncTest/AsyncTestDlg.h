// AsyncTestDlg.h : header file
//

#if !defined(AFX_ASYNCTESTDLG_H__F32863D1_E6E8_4A7C_AE2A_D0CD4D493A59__INCLUDED_)
#define AFX_ASYNCTESTDLG_H__F32863D1_E6E8_4A7C_AE2A_D0CD4D493A59__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CAsyncTestDlg dialog

class CAsyncTestDlg : public CDialog
{
// Construction
public:
	CAsyncTestDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CAsyncTestDlg)
	enum { IDD = IDD_ASYNCTEST_DIALOG };
	long	m_BufferSize;
	long	m_ByteCount;
	CString	m_FileName;
	int		m_BufferCount;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAsyncTestDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CAsyncTestDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnReadButton();
	afx_msg void OnWriteButton();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ASYNCTESTDLG_H__F32863D1_E6E8_4A7C_AE2A_D0CD4D493A59__INCLUDED_)
