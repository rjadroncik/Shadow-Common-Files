#include "Pointer.h"
#include "String.h"

#include "Int.h"
#include "Int64.h"

using namespace SCFBase;

void* CPointer::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	return (void*)CInt64::Parse(rString, uipOutCharsParsed);
}

CString CPointer::Print(_IN void* vpValue)
{
	#ifdef X64
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(false));
	static CFormatInt Format(16, 16, 1);
	BETAONLY(CObject::Tracing(bTracing));
	return CInt64((INT64)vpValue).ToString(Format);
	#else
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(false));
	static CFormatInt Format(8, 16, 1);
	BETAONLY(CObject::Tracing(bTracing));
	return CInt64((UINT64)vpValue & 0xFFFFFFFF).ToString(Format);
	#endif
}

CPointer::CPointer(_IN CString&  rValue) { m_vpValue = CPointer::Parse(rValue, nullptr); }
CPointer::CPointer(_IN void*    vpValue) { m_vpValue = (void*)vpValue; }
CPointer::CPointer(_IN CPointer& rValue) { m_vpValue = (void*)rValue.m_vpValue; }

CPointer::~CPointer() {}

CString CPointer::ToString() _GET { return CPointer::Print(m_vpValue); }

bool CPointer::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_vpValue < ((const CPointer&)rObject).m_vpValue) { return true; }

	return false;
}

bool CPointer::IsEqualTo(_IN CObject& rObject) const
{
	if (m_vpValue == ((const CPointer&)rObject).m_vpValue) { return true; }

	return false;
}
