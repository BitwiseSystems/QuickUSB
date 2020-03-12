#if !defined(AFX__COLLECTION_H__3C92D80C_0BFE_4E51_8A2B_F754096CC34C__INCLUDED_)
#define AFX__COLLECTION_H__3C92D80C_0BFE_4E51_8A2B_F754096CC34C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// C_Collection wrapper class

class C_Collection : public COleDispatchDriver
{
public:
	C_Collection() {}		// Calls COleDispatchDriver default constructor
	C_Collection(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	C_Collection(const C_Collection& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	VARIANT Item(VARIANT* Index);
	void Add(VARIANT* Item, VARIANT* Key, VARIANT* Before, VARIANT* After);
	long Count();
	void Remove(VARIANT* Index);
	LPUNKNOWN _NewEnum();
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX__COLLECTION_H__3C92D80C_0BFE_4E51_8A2B_F754096CC34C__INCLUDED_)
