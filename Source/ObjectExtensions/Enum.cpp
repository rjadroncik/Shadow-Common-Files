#include "Enum.h"
#include "String.h"
#include "Stack.h"
#include "Char.h"
#include "Int.h"

using namespace SCFBase;

SCF::ENUM CEnum::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	return CInt::Parse(rString, uipOutCharsParsed);
}

CString CEnum::Print(_IN SCF::ENUM eValue)
{
	return CInt::Print(eValue);
}

CEnum::CEnum(_IN SCF::ENUM eValue) { m_eValue = eValue; }
CEnum::CEnum(_IN CEnum& rValue)    { m_eValue = rValue.m_eValue; }
CEnum::CEnum(_IN CString& rString) { m_eValue = CEnum::Parse(rString, NULL); }

CEnum::~CEnum() {}

CString CEnum::ToString() _GET { return CEnum::Print(m_eValue); }

void CEnum::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_eValue);
}

void CEnum::Deserialize(_INOUT IStreamRead& rStream)
{
	m_eValue = rStream.GetInt();
}