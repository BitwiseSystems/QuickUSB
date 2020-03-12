// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "certadm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICertAdmin properties

/////////////////////////////////////////////////////////////////////////////
// ICertAdmin operations

long ICertAdmin::IsValidCertificate(LPCTSTR strConfig, LPCTSTR strSerialNumber)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strConfig, strSerialNumber);
	return result;
}

long ICertAdmin::GetRevocationReason()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}

void ICertAdmin::RevokeCertificate(LPCTSTR strConfig, LPCTSTR strSerialNumber, long Reason, DATE Date)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_I4 VTS_DATE;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strConfig, strSerialNumber, Reason, Date);
}

void ICertAdmin::SetRequestAttributes(LPCTSTR strConfig, long RequestId, LPCTSTR strAttributes)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strConfig, RequestId, strAttributes);
}

void ICertAdmin::SetCertificateExtension(LPCTSTR strConfig, long RequestId, LPCTSTR strExtensionName, long Type, long Flags, VARIANT* pvarValue)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4 VTS_BSTR VTS_I4 VTS_I4 VTS_PVARIANT;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strConfig, RequestId, strExtensionName, Type, Flags, pvarValue);
}

void ICertAdmin::DenyRequest(LPCTSTR strConfig, long RequestId)
{
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strConfig, RequestId);
}

long ICertAdmin::ResubmitRequest(LPCTSTR strConfig, long RequestId)
{
	long result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x60020006, DISPATCH_METHOD, VT_I4, (void*)&result, parms,
		strConfig, RequestId);
	return result;
}

void ICertAdmin::PublishCRL(LPCTSTR strConfig, DATE Date)
{
	static BYTE parms[] =
		VTS_BSTR VTS_DATE;
	InvokeHelper(0x60020007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 strConfig, Date);
}

CString ICertAdmin::GetCRL(LPCTSTR strConfig, long Flags)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		strConfig, Flags);
	return result;
}
