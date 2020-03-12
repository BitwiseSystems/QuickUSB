// QuickUsbDiagDlg.h : header file
//
/*=============================================================================
 Title        : QuickUsbDiagDlg.h
 Description  : QuickUSB Data, I2C, and FPGA I/O test program
 Notes        : None
 History      :

 Copyright © 2003 Bitwise Systems.  All rights reserved.
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

 $History: QuickUsbDiagDlg.h $
// 
// *****************  Version 4  *****************
// User: Bob Jacobson Date: 1/13/06    Time: 2:41p
// Updated in $/Projects/Bitwise/QuickUSB/Library/Software/QuickUsbDiagVC6
// Correct VSS $History keyword


*/

#if !defined(AFX_QUICKUSBDIAGDLG_H__0FF8F0E4_5A95_4220_8B9C_2FAC548CD607__INCLUDED_)
#define AFX_QUICKUSBDIAGDLG_H__0FF8F0E4_5A95_4220_8B9C_2FAC548CD607__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbDiagDlg dialog

class CQuickUsbDiagDlg : public CDialog
{
// Construction
public:
	CQuickUsbDiagDlg(CWnd* pParent = NULL);	// standard constructor

// Dialog Data
	//{{AFX_DATA(CQuickUsbDiagDlg)
	enum { IDD = IDD_QUICKUSBDIAG_DIALOG };
	CComboBox	m_Rs232Baud;
	CEdit	m_BufferSizeTextBox;
	CEdit	m_BufferCountTextBox;
	CProgressCtrl	m_FpgaProgressBar;
	CComboBox	m_ModuleComboBox;
	BOOL	m_AsyncCheckBox;
	CString	m_SerialNumberEditBox;
	CString	m_ModuleDescriptionEditBox;
	CString	m_HwRevisionEditBox;
	CString	m_BusSpeedEditBox;
	CString	m_DataFileTextBox;
	CString	m_FpgaFileEditBox;
	BOOL	m_IncrementAddressTextBox;
	CString	m_StatusMessage;
	long	m_BufferSize;
	long	m_DataAddress;
	long	m_DataLength;
	CString	m_DataReadFileTextBox;
	CString	m_DataWriteFileTextBox;
	int		m_BufferCount;
	int		m_I2CAddress;
	int		m_I2CLength;
	CString	m_I2CData;
	int		m_Rs232CharsWaiting;
	CString	m_Rs232Data;
	CString	m_SpiData;
	int		m_SpiLength;
	int		m_SpiPort;
	int		m_Rs232Port;
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickUsbDiagDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
   void UpdateStringDescriptor(void);
   void AsyncDataRead(void);
   void AsyncDataWrite(void);
   void ThroughputCalc(unsigned long dataLength);
   void DisplayError(char * function, int index);
   void UpdateModuleComboBox(void);
   void ShowStatus(char * NewStatusMessage, boolean redrawWindow=FALSE);
   void UpdateRs232(void);
   unsigned long errorCode;
   char messageBoxBuffer[500];
#ifdef _WIN64
   UINT_PTR ModuleTimerEvent;
   UINT_PTR StatusTimerEvent;
#else
   UINT ModuleTimerEvent;
   UINT StatusTimerEvent;
#endif
   DWORD startTime;                    /* start of data transfer test */
   DWORD endTime;                      /* end of data transfer test */
   HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(CQuickUsbDiagDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSelchangeModuleComboBox();
	afx_msg void OnAsyncCheckBox();
	afx_msg void OnDataBrowseButton();
	afx_msg void OnDataReadButton();
	afx_msg void OnDataWriteButton();
	afx_msg void OnFpgaBrowseButton();
	afx_msg void OnFpgaStatusButton();
	afx_msg void OnFpgaUploadButton();
	afx_msg void OnI2CBrowseButton();
	afx_msg void OnI2CReadButton();
	afx_msg void OnI2CWriteButton();
#ifdef _WIN64
	afx_msg void OnTimer(UINT_PTR nIDEvent);
#else
    afx_msg void OnTimer(UINT nIDEvent);
#endif
	afx_msg void OnDataWriteBrowseButton();
	afx_msg void OnDataReadBrowseButton();
	afx_msg void OnSpiReadButton();
	afx_msg void OnSpiWriteButton();
	afx_msg void OnSpiWriteReadButton();
	afx_msg void OnRs232WriteButton();
	afx_msg void OnRs232ReadButton();
	afx_msg void OnRs232FlushButton();
	afx_msg void OnSelchangeRs232PortComboBox();
	afx_msg void OnSelchangeRs232BaudComboBox();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKUSBDIAGDLG_H__0FF8F0E4_5A95_4220_8B9C_2FAC548CD607__INCLUDED_)
