#include "Float.h"
#include "Float3.h"
#include "String.h"
#include "StringRange.h"

#include <SCFMathematics.h>

using namespace SCFBase;
using namespace SCFMathematics;

UINT CFloat3::Parse(_IN CString& rString) { UINT uiCharsParsed = 0; SCFMathematics::CopyVector3(m_Value, Parse(rString, &uiCharsParsed)); return uiCharsParsed; }

Float3& CFloat3::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	static Float3 Value;
	Parse(Value, rString, uipOutCharsParsed);
	return Value;
}

void CFloat3::Parse(_OUT Float3& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed = 0;

	rOutValue[0] = CFloat::Parse(rString, &uiCharsParsedTotal);
	rOutValue[1] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);
	uiCharsParsedTotal += uiCharsParsed;
	rOutValue[2] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CFloat3::Print(_IN Float3& rValue)
{
	CString RetVal;

	RetVal += CFloat::Print(rValue[0]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[1]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[2]);

	return RetVal;
}

CFloat3::CFloat3(_IN Float3& rValue)                       { m_Value[0] = rValue[0];         m_Value[1] = rValue[1];         m_Value[2] = rValue[2]; }
CFloat3::CFloat3(_IN float fX, _IN float fY, _IN float fZ) { m_Value[0] = fX;                m_Value[1] = fY;                m_Value[2] = fZ; }
CFloat3::CFloat3(_IN CFloat3& rValue)                      { m_Value[0] = rValue.m_Value[0]; m_Value[1] = rValue.m_Value[1]; m_Value[2] = rValue.m_Value[2]; }
CFloat3::CFloat3(_IN CString& rString)                     { CFloat3::Parse(m_Value, rString, nullptr); }

CFloat3::~CFloat3() {}

CString CFloat3::ToString() _GET { return CFloat3::Print(m_Value); }

float CFloat3::At        (_IN UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 3); return m_Value[uiIndex]; }
float CFloat3::operator[](_IN UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 3); return m_Value[uiIndex]; }
void CFloat3::AtPut      (_IN UINT uiIndex, _IN float fValue) _SET { _ASSERTE(uiIndex < 3); m_Value[uiIndex] = fValue; }