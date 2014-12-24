#include "Pointer.h"
#include "String.h"

#include "int.h"
#include "int64.h"

using namespace SCFBase;

void* CPointer::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	return (void*)CInt64::Parse(rString, uipOutCharsParsed);
}

CString CPointer::Print(_IN void* vpValue)
{
	#ifdef X64
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(FALSE));
	static CFormatInt Format(16, 16, 1);
	BETAONLY(CObject::Tracing(bTracing));
	return CInt64((SCF::INT64)vpValue).ToString(Format);
	#else
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(FALSE));
	static CFormatInt Format(8, 16, 1);
	BETAONLY(CObject::Tracing(bTracing));
	return CInt64((SCF::UINT64)vpValue & 0xFFFFFFFF).ToString(Format);
	#endif
}

CPointer::CPointer(_IN CString&  rValue) { m_vpValue = CPointer::Parse(rValue, NULL); }
CPointer::CPointer(_IN void*    vpValue) { m_vpValue = (void*)vpValue; }
CPointer::CPointer(_IN CPointer& rValue) { m_vpValue = (void*)rValue.m_vpValue; }

CPointer::~CPointer() {}

CString CPointer::ToString() _GET { return CPointer::Print(m_vpValue); }

void CPointer::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt64((SCF::UINT64)m_vpValue);
}

void CPointer::Deserialize(_INOUT IStreamRead& rStream)
{
	m_vpValue = (void*)rStream.GetInt64();
}

bool CPointer::IsSmallerThen(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassInt) && (m_vpValue < ((const CPointer&)rObject).m_vpValue)) { return TRUE; }

	return FALSE;
}

bool CPointer::IsEqualTo(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassInt) && (m_vpValue == ((const CPointer&)rObject).m_vpValue)) { return TRUE; }

	return FALSE;
}
