// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.


#include "stdafx.h"
#include "repository.h"

// Dispatch interfaces referenced by this interface
#include "RepositoryObject.h"
#include "RepositoryTransaction.h"


/////////////////////////////////////////////////////////////////////////////
// CRepository properties

/////////////////////////////////////////////////////////////////////////////
// CRepository operations

CRepositoryObject CRepository::Create(LPCTSTR Connect, LPCTSTR User, LPCTSTR Password, long fFlags)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x1, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Connect, User, Password, fFlags);
	return CRepositoryObject(pDispatch);
}

CRepositoryObject CRepository::Open(LPCTSTR Connect, LPCTSTR User, LPCTSTR Password, long fFlags)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BSTR VTS_I4;
	InvokeHelper(0x2, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		Connect, User, Password, fFlags);
	return CRepositoryObject(pDispatch);
}

CRepositoryObject CRepository::CreateObject(const VARIANT& sTypeID, const VARIANT& sObjID)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT VTS_VARIANT;
	InvokeHelper(0x3, DISPATCH_METHOD, VT_DISPATCH, (void*)&pDispatch, parms,
		&sTypeID, &sObjID);
	return CRepositoryObject(pDispatch);
}

CRepositoryObject CRepository::GetRootObject()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CRepositoryObject(pDispatch);
}

CRepositoryObject CRepository::GetObject(const VARIANT& sObjID)
{
	LPDISPATCH pDispatch;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, parms,
		&sObjID);
	return CRepositoryObject(pDispatch);
}

void CRepository::Refresh(long iMilliseconds)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 iMilliseconds);
}

VARIANT CRepository::ObjectIDToInternalID(const VARIANT& sObjID)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x7, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&sObjID);
	return result;
}

VARIANT CRepository::InternalIDToObjectID(const VARIANT& sIntID)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x8, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&sIntID);
	return result;
}

CRepositoryTransaction CRepository::GetTransaction()
{
	LPDISPATCH pDispatch;
	InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&pDispatch, NULL);
	return CRepositoryTransaction(pDispatch);
}
