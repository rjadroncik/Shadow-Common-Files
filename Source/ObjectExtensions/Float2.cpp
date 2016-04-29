#include "Float.h"
#include "Float2.h"
#include "String.h"
#include "StringRange.h"

#include <SCFMathematics.h>

using namespace SCFBase;
using namespace SCFMathematics;

SCF::UINT CFloat2::Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; SCFMathematics::CopyVector2(m_Value, Parse(rString, &uiCharsParsed)); return uiCharsParsed; }

Float2& CFloat2::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	static Float2 Value;
	Parse(Value, rString, uipOutCharsParsed);
	return Value;
}

void CFloat2::Parse(_OUT Float2& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed = 0;

	rOutValue[0] = CFloat::Parse(rString, &uiCharsParsedTotal);
	rOutValue[1] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CFloat2::Print(_IN Float2& rValue)
{
	CString RetVal;

	RetVal += CFloat::Print(rValue[0]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[1]);

	return RetVal;
}

CFloat2::CFloat2(_IN Float2& rValue)         { m_Value[0] = rValue[0];         m_Value[1] = rValue[1]; }
CFloat2::CFloat2(_IN float fX, _IN float fY) { m_Value[0] = fX;                m_Value[1] = fY; }
CFloat2::CFloat2(_IN CFloat2& rValue)        { m_Value[0] = rValue.m_Value[0]; m_Value[1] = rValue.m_Value[1]; }
CFloat2::CFloat2(_IN CString& rString)       { CFloat2::Parse(m_Value, rString, NULL); }

CFloat2::~CFloat2() {}

CString CFloat2::ToString() _GET { return CFloat2::Print(m_Value); }

float CFloat2::At        (_IN SCF::UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 2); return m_Value[uiIndex]; }
float CFloat2::operator[](_IN SCF::UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 2); return m_Value[uiIndex]; }
void CFloat2::AtPut      (_IN SCF::UINT uiIndex, _IN float fValue) _SET { _ASSERTE(uiIndex < 2); m_Value[uiIndex] = fValue; }