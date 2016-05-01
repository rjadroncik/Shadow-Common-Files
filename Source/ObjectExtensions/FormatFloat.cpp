#include "FormatFloat.h"

using namespace SCFBase;

CFormatFloat::CFormatFloat()
{
	m_uiFractionDigitsMin = 0;
	m_uiFractionDigitsMax = 4;

	m_usDecimalChar = '.';
}

CFormatFloat::CFormatFloat(_IN UINT uiFractionDigitsMin, _IN UINT uiFractionDigitsMax, _IN ENUM eDecimalSeparator)
{
	m_uiFractionDigitsMin = uiFractionDigitsMin;
	m_uiFractionDigitsMax = uiFractionDigitsMax;

	m_usDecimalChar = (TCHAR)eDecimalSeparator;
}

CFormatFloat::~CFormatFloat()
{
}


//UINT uiCharsParsedTotal = 0;
//UINT uiCharsParsed = 0;

//UINT uiFractionDigitsMin = (UINT)CInt::Parse(rFormat, &uiCharsParsedTotal);
//UINT uiFractionDigitsMax = (UINT)CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

//TCHAR usDecimalSeparator = rFormat[uiCharsParsedTotal + uiCharsParsed + 1];
