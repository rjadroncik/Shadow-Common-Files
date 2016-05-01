#include "RectInt.h"
#include "String.h"

using namespace SCFGraphics;

bool SCFGRAPHICS_API __stdcall SCFGraphics::RectanglesIntersect(_IN Rect4i& rRect4i01, _IN Rect4i& rRect4i02)
{
	if (__abs((rRect4i01.iX * 2 + rRect4i01.iWidth)  - (rRect4i02.iX * 2 + rRect4i02.iWidth )) > (rRect4i01.iWidth  + rRect4i02.iWidth))  { return FALSE; }
	if (__abs((rRect4i01.iY * 2 + rRect4i01.iHeight) - (rRect4i02.iY * 2 + rRect4i02.iHeight)) > (rRect4i01.iHeight	+ rRect4i02.iHeight)) { return FALSE; }

	return TRUE;
}

Rect4i& CRectInt::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	static Rect4i RetVal;

	Parse(RetVal, rString, uipOutCharsParsed);

	return RetVal;
}

void CRectInt::Parse(_OUT Rect4i& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed = 0;

	rOutValue.iX      = CInt::Parse(rString, &uiCharsParsedTotal);                              uiCharsParsedTotal++;
	rOutValue.iY      = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.iWidth  = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.iHeight = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CRectInt::Print(_IN Rect4i& rValue)
{
	CString RetVal;

	RetVal += CInt::Print(rValue.iX);
	RetVal += STRING(" ");
	RetVal += CInt::Print(rValue.iY);
	RetVal += STRING(" ");
	RetVal += CInt::Print(rValue.iWidth);
	RetVal += STRING(" ");
	RetVal += CInt::Print(rValue.iHeight);

	return RetVal;
}

CRectInt::CRectInt(_IN Rect4i& rValue)
{
	m_Value = rValue;
}

CRectInt::CRectInt(_IN CRectInt& rValue)
{
	m_Value = rValue.m_Value;
}

CRectInt::~CRectInt()
{
}

CRectInt::CRectInt(_IN CString& rString) { m_Value = CRectInt::Parse(rString, NULL); }

CString CRectInt::ToString() const { return CRectInt::Print(m_Value); }	

void CRectInt::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_Value.iX);
	rStream.PutInt(m_Value.iY);
	rStream.PutInt(m_Value.iWidth);
	rStream.PutInt(m_Value.iHeight);
}

void CRectInt::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Value.iX = rStream.GetInt();
	m_Value.iY = rStream.GetInt();
	m_Value.iWidth  = rStream.GetInt();
	m_Value.iHeight = rStream.GetInt();
}
