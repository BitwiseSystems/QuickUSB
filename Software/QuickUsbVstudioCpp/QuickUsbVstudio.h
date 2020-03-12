// QuickUsbVstudio.h : main header file for the QUICKUSBVSTUDIO application
//

#if !defined(AFX_QUICKUSBVSTUDIO_H__DF738E16_64FE_4E26_B0AA_A8B6D9607249__INCLUDED_)
#define AFX_QUICKUSBVSTUDIO_H__DF738E16_64FE_4E26_B0AA_A8B6D9607249__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CQuickUsbVstudioApp:
// See QuickUsbVstudio.cpp for the implementation of this class
//

class CQuickUsbVstudioApp : public CWinApp
{
public:
	CQuickUsbVstudioApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CQuickUsbVstudioApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CQuickUsbVstudioApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_QUICKUSBVSTUDIO_H__DF738E16_64FE_4E26_B0AA_A8B6D9607249__INCLUDED_)
