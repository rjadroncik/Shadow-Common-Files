#include "PointFloat.h"
#include "String.h"

using namespace SCFGraphics;

bool SCFGRAPHICS_API __stdcall SCFGraphics::PointInRect(_IN Point2f& rPoint2f, _IN Rect4f& rRect4f)
{
	if ((rPoint2f.fX > rRect4f.fX) && (rPoint2f.fX < (rRect4f.fX + rRect4f.fWidth)) && (rPoint2f.fY > rRect4f.fY) && (rPoint2f.fY < (rRect4f.fY + rRect4f.fHeight))) { return TRUE; }

	return FALSE;
}

bool SCFGRAPHICS_API __stdcall SCFGraphics::PointInRect(_IN float fX, _IN float fY, _IN Rect4f& rRect4f)
{
	if ((fX > rRect4f.fX) && (fX < (rRect4f.fX + rRect4f.fWidth)) && (fY > rRect4f.fY) && (fY < (rRect4f.fY + rRect4f.fHeight))) { return TRUE; }

	return FALSE;
}

Point2f& CPointFloat::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	static Point2f RetVal;

	Parse(RetVal, rString, uipOutCharsParsed);

	return RetVal;
}

void CPointFloat::Parse(_OUT Point2f& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed = 0;

	rOutValue.fX = CFloat::Parse(rString, &uiCharsParsedTotal); uiCharsParsedTotal++;
	rOutValue.fY = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CPointFloat::Print(_IN Point2f& rValue)
{
	CString RetVal;

	RetVal += CFloat::Print(rValue.fX);
	RetVal += STRING(" ");
	RetVal += CFloat::Print(rValue.fY);

	return RetVal;
}

CPointFloat::CPointFloat(_IN Point2f& rValue)
{
	m_Value = rValue;
}

CPointFloat::CPointFloat(_IN CPointFloat& rValue)
{
	m_Value = rValue.m_Value;
}

CPointFloat::~CPointFloat()
{
}

CPointFloat::CPointFloat(_IN CString& rString) { m_Value = CPointFloat::Parse(rString, NULL); }

CString CPointFloat::ToString() const { return CPointFloat::Print(m_Value); }	

void CPointFloat::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutFloat(m_Value.fX);
	rStream.PutFloat(m_Value.fY);
}

void CPointFloat::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Value.fX = rStream.GetFloat();
	m_Value.fY = rStream.GetFloat();
}
