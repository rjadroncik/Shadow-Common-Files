#include "Char.h"
#include "String.h"

using namespace SCFBase;

SCF::TCHAR CChar::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	if (rString.Length()) 
	{ 
		if (uipOutCharsParsed) { *uipOutCharsParsed = 1; }
		return rString[0];
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = 0; }
	return 0;
}

CString CChar::Print(_IN SCF::TCHAR cValue)
{
	SCF::TCHAR caString[] = { cValue, 0 };

	return CString(caString);
}

CChar::CChar(_IN SCF::TCHAR cValue)
{
	m_cValue = cValue;
}

CChar::CChar(_IN CChar& rValue)
{
	m_cValue = rValue.m_cValue;
}

CChar::~CChar()
{
}

CChar::CChar(_IN CString& rString) { m_cValue = CChar::Parse(rString, NULL); }

CString CChar::ToString() const { return CChar::Print(m_cValue); }	

bool CChar::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_cValue < ((const CChar&)rObject).m_cValue) { return TRUE; }

	return FALSE;
}

bool CChar::IsEqualTo(_IN CObject& rObject) const
{
	if (m_cValue == ((const CChar&)rObject).m_cValue) { return TRUE; }

	return FALSE;
}

