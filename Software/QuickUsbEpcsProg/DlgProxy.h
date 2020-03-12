// DlgProxy.h: header file
//

#pragma once

class CQuickUsbEpcsProgDlg;


// CQuickUsbEpcsProgDlgAutoProxy command target

class CQuickUsbEpcsProgDlgAutoProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CQuickUsbEpcsProgDlgAutoProxy)

	CQuickUsbEpcsProgDlgAutoProxy();           // protected constructor used by dynamic creation

// Attributes
public:
	CQuickUsbEpcsProgDlg* m_pDialog;

// Operations
public:

// Overrides
	public:
	virtual void OnFinalRelease();

// Implementation
protected:
	virtual ~CQuickUsbEpcsProgDlgAutoProxy();

	// Generated message map functions

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CQuickUsbEpcsProgDlgAutoProxy)

	// Generated OLE dispatch map functions

	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

