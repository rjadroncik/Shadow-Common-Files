#include "FormatFloat.h"

using namespace SCFBase;
using namespace SCF;

CFormatFloat::CFormatFloat()
{
	m_uiFractionDigitsMin = 0;
	m_uiFractionDigitsMax = 4;

	m_usDecimalChar = '.';
}

CFormatFloat::CFormatFloat(_IN SCF::UINT uiFractionDigitsMin, _IN SCF::UINT uiFractionDigitsMax, _IN SCF::ENUM eDecimalSeparator)
{
	m_uiFractionDigitsMin = uiFractionDigitsMin;
	m_uiFractionDigitsMax = uiFractionDigitsMax;

	m_usDecimalChar = (SCF::TCHAR)eDecimalSeparator;
}

CFormatFloat::~CFormatFloat()
{
}


//SCF::UINT uiCharsParsedTotal = 0;
//SCF::UINT uiCharsParsed = 0;

//SCF::UINT uiFractionDigitsMin = (SCF::UINT)CInt::Parse(rFormat, &uiCharsParsedTotal);
//SCF::UINT uiFractionDigitsMax = (SCF::UINT)CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

//SCF::TCHAR usDecimalSeparator = rFormat[uiCharsParsedTotal + uiCharsParsed + 1];
