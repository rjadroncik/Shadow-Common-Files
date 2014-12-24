#include "RectFloat.h"
#include "String.h"

using namespace SCFGraphics;

bool SCFGRAPHICS_API __stdcall SCFGraphics::RectanglesIntersect(_IN Rect4f& rRect4i01, _IN Rect4f& rRect4i02)
{
	if (__abs((rRect4i01.fX * 2 + rRect4i01.fWidth)  - (rRect4i02.fX * 2 + rRect4i02.fWidth )) > (rRect4i01.fWidth  + rRect4i02.fWidth))  { return FALSE; }
	if (__abs((rRect4i01.fY * 2 + rRect4i01.fHeight) - (rRect4i02.fY * 2 + rRect4i02.fHeight)) > (rRect4i01.fHeight	+ rRect4i02.fHeight)) { return FALSE; }

	return TRUE;
}

Rect4f& CRectFloat::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	static Rect4f RetVal;

	Parse(RetVal, rString, uipOutCharsParsed);

	return RetVal;
}

void CRectFloat::Parse(_OUT Rect4f& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed = 0;

	rOutValue.fX      = CFloat::Parse(rString, &uiCharsParsedTotal);                              uiCharsParsedTotal++;
	rOutValue.fY      = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.fWidth  = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.fHeight = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CRectFloat::Print(_IN Rect4f& rValue)
{
	CString RetVal;

	RetVal += CFloat::Print(rValue.fX);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue.fY);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue.fWidth);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue.fHeight);

	return RetVal;
}

CRectFloat::CRectFloat(_IN Rect4f& rValue)
{
	m_Value = rValue;
}

CRectFloat::CRectFloat(_IN CRectFloat& rValue)
{
	m_Value = rValue.m_Value;
}

CRectFloat::~CRectFloat()
{
}

CRectFloat::CRectFloat(_IN CString& rString) { m_Value = CRectFloat::Parse(rString, NULL); }

CString CRectFloat::ToString() const { return CRectFloat::Print(m_Value); }	

void CRectFloat::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutFloat(m_Value.fX);
	rStream.PutFloat(m_Value.fY);
	rStream.PutFloat(m_Value.fWidth);
	rStream.PutFloat(m_Value.fHeight);
}

void CRectFloat::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Value.fX = rStream.GetFloat();
	m_Value.fY = rStream.GetFloat();
	m_Value.fWidth  = rStream.GetFloat();
	m_Value.fHeight = rStream.GetFloat();
}
