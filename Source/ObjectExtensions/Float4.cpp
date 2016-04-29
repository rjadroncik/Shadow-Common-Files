#include "Float.h"
#include "Float4.h"
#include "String.h"
#include "StringRange.h"

#include <SCFMathematics.h>

using namespace SCFBase;
using namespace SCFMathematics;

SCF::UINT CFloat4::Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; SCFMathematics::CopyVector4(m_Value, Parse(rString, &uiCharsParsed)); return uiCharsParsed; }

Float4& CFloat4::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	static Float4 Value;
	Parse(Value, rString, uipOutCharsParsed);
	return Value;
}

void CFloat4::Parse(_OUT Float4& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed = 0;

	rOutValue[0] = CFloat::Parse(rString, &uiCharsParsedTotal);
	rOutValue[1] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);
	uiCharsParsedTotal += uiCharsParsed;
	rOutValue[2] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);
	uiCharsParsedTotal += uiCharsParsed;
	rOutValue[3] = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CFloat4::Print(_IN Float4& rValue)
{
	CString RetVal;

	RetVal += CFloat::Print(rValue[0]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[1]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[2]);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue[3]);

	return RetVal;
}

CFloat4::CFloat4(_IN Float4& rValue)                                     { m_Value[0] = rValue[0];         m_Value[1] = rValue[1];         m_Value[2] = rValue[2];         m_Value[3] = rValue[3]; }
CFloat4::CFloat4(_IN float fX, _IN float fY, _IN float fZ, _IN float fW) { m_Value[0] = fX;                m_Value[1] = fY;                m_Value[2] = fZ;                m_Value[3] = fW; }
CFloat4::CFloat4(_IN CFloat4& rValue)                                    { m_Value[0] = rValue.m_Value[0]; m_Value[1] = rValue.m_Value[1]; m_Value[2] = rValue.m_Value[2]; m_Value[3] = rValue.m_Value[3]; }
CFloat4::CFloat4(_IN CString& rString)                                   { CFloat4::Parse(m_Value, rString, NULL); }

CFloat4::~CFloat4() {}

CString CFloat4::ToString() _GET { return CFloat4::Print(m_Value); }

float CFloat4::At        (_IN SCF::UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 4); return m_Value[uiIndex]; }
float CFloat4::operator[](_IN SCF::UINT uiIndex)                   _GET { _ASSERTE(uiIndex < 4); return m_Value[uiIndex]; }
void CFloat4::AtPut      (_IN SCF::UINT uiIndex, _IN float fValue) _SET { _ASSERTE(uiIndex < 4); m_Value[uiIndex] = fValue; }