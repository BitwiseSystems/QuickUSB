// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICertAdmin wrapper class

class ICertAdmin : public COleDispatchDriver
{
public:
	ICertAdmin() {}		// Calls COleDispatchDriver default constructor
	ICertAdmin(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICertAdmin(const ICertAdmin& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long IsValidCertificate(LPCTSTR strConfig, LPCTSTR strSerialNumber);
	long GetRevocationReason();
	void RevokeCertificate(LPCTSTR strConfig, LPCTSTR strSerialNumber, long Reason, DATE Date);
	void SetRequestAttributes(LPCTSTR strConfig, long RequestId, LPCTSTR strAttributes);
	void SetCertificateExtension(LPCTSTR strConfig, long RequestId, LPCTSTR strExtensionName, long Type, long Flags, VARIANT* pvarValue);
	void DenyRequest(LPCTSTR strConfig, long RequestId);
	long ResubmitRequest(LPCTSTR strConfig, long RequestId);
	void PublishCRL(LPCTSTR strConfig, DATE Date);
	CString GetCRL(LPCTSTR strConfig, long Flags);
};
