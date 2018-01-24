#include "PointInt.h"
#include "String.h"

using namespace SCFGraphics;

bool SCFGRAPHICS_API __stdcall SCFGraphics::PointInRect(_IN Point2i& rPoint, _IN Rect4i& rRect)
{
	if ((rPoint.iX > rRect.iX) && (rPoint.iX < (rRect.iX + rRect.iWidth)) && (rPoint.iY > rRect.iY) && (rPoint.iY < (rRect.iY + rRect.iHeight))) { return true; }

	return false;
}

bool SCFGRAPHICS_API __stdcall SCFGraphics::PointInRect(_IN int iX, _IN int iY, _IN Rect4i& rRect)
{
	if ((iX > rRect.iX) && (iX < (rRect.iX + rRect.iWidth)) && (iY > rRect.iY) && (iY < (rRect.iY + rRect.iHeight))) { return true; }

	return false;
}

Point2i& CPointInt::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	static Point2i RetVal;

	Parse(RetVal, rString, uipOutCharsParsed);

	return RetVal;
}

void CPointInt::Parse(_OUT Point2i& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed = 0;

	rOutValue.iX = CInt::Parse(rString, &uiCharsParsedTotal); uiCharsParsedTotal++;
	rOutValue.iY = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

CString CPointInt::Print(_IN Point2i& rValue)
{
	CString RetVal;

	RetVal += CInt::Print(rValue.iX);
	RetVal += STRING(" ");
	RetVal += CInt::Print(rValue.iY);

	return RetVal;
}

CPointInt::CPointInt(_IN Point2i& rValue)
{
	m_Value = rValue;
}

CPointInt::CPointInt(_IN CPointInt& rValue)
{
	m_Value = rValue.m_Value;
}

CPointInt::~CPointInt()
{
}

CPointInt::CPointInt(_IN CString& rString) { m_Value = CPointInt::Parse(rString, nullptr); }

CString CPointInt::ToString() const { return CPointInt::Print(m_Value); }	

void CPointInt::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_Value.iX);
	rStream.PutInt(m_Value.iY);
}

void CPointInt::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Value.iX = rStream.GetInt();
	m_Value.iY = rStream.GetInt();
}
