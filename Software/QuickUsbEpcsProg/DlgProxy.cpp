// DlgProxy.cpp : implementation file
//

#include "stdafx.h"
#include "QuickUsbEpcsProg.h"
#include "DlgProxy.h"
#include "QuickUsbEpcsProgDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CQuickUsbEpcsProgDlgAutoProxy

IMPLEMENT_DYNCREATE(CQuickUsbEpcsProgDlgAutoProxy, CCmdTarget)

CQuickUsbEpcsProgDlgAutoProxy::CQuickUsbEpcsProgDlgAutoProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an automation 
	//	object is active, the constructor calls AfxOleLockApp.
	AfxOleLockApp();

	// Get access to the dialog through the application's
	//  main window pointer.  Set the proxy's internal pointer
	//  to point to the dialog, and set the dialog's back pointer to
	//  this proxy.
	ASSERT_VALID(AfxGetApp()->m_pMainWnd);
	if (AfxGetApp()->m_pMainWnd)
	{
		ASSERT_KINDOF(CQuickUsbEpcsProgDlg, AfxGetApp()->m_pMainWnd);
		if (AfxGetApp()->m_pMainWnd->IsKindOf(RUNTIME_CLASS(CQuickUsbEpcsProgDlg)))
		{
			m_pDialog = reinterpret_cast<CQuickUsbEpcsProgDlg*>(AfxGetApp()->m_pMainWnd);
			m_pDialog->m_pAutoProxy = this;
		}
	}
}

CQuickUsbEpcsProgDlgAutoProxy::~CQuickUsbEpcsProgDlgAutoProxy()
{
	// To terminate the application when all objects created with
	// 	with automation, the destructor calls AfxOleUnlockApp.
	//  Among other things, this will destroy the main dialog
	if (m_pDialog != NULL)
		m_pDialog->m_pAutoProxy = NULL;
	AfxOleUnlockApp();
}

void CQuickUsbEpcsProgDlgAutoProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}

BEGIN_MESSAGE_MAP(CQuickUsbEpcsProgDlgAutoProxy, CCmdTarget)
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CQuickUsbEpcsProgDlgAutoProxy, CCmdTarget)
END_DISPATCH_MAP()

// Note: we add support for IID_IQuickUsbEpcsProg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .IDL file.

// {D8F842D0-7907-4AA0-9E4C-7996E2F3EA4B}
static const IID IID_IQuickUsbEpcsProg =
{ 0xD8F842D0, 0x7907, 0x4AA0, { 0x9E, 0x4C, 0x79, 0x96, 0xE2, 0xF3, 0xEA, 0x4B } };

BEGIN_INTERFACE_MAP(CQuickUsbEpcsProgDlgAutoProxy, CCmdTarget)
	INTERFACE_PART(CQuickUsbEpcsProgDlgAutoProxy, IID_IQuickUsbEpcsProg, Dispatch)
END_INTERFACE_MAP()

// The IMPLEMENT_OLECREATE2 macro is defined in StdAfx.h of this project
// {B4B954F2-1E54-44D8-AB8F-602F97496340}
IMPLEMENT_OLECREATE2(CQuickUsbEpcsProgDlgAutoProxy, "QuickUsbEpcsProg.Application", 0xb4b954f2, 0x1e54, 0x44d8, 0xab, 0x8f, 0x60, 0x2f, 0x97, 0x49, 0x63, 0x40)


// CQuickUsbEpcsProgDlgAutoProxy message handlers
