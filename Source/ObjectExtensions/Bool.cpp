#include "Bool.h"
#include "String.h"

using namespace SCFBase;

CBool::CBool(_IN bool bValue)
{
	m_bValue = bValue;
}

CBool::CBool(_IN CBool& rValue)
{
	m_bValue = rValue.m_bValue;
}

CBool::~CBool()
{
}

bool CBool::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	if (uipOutCharsParsed)
	{
		if (rString == STRING("1"))    { *uipOutCharsParsed = 1; return true; } 
		if (rString == STRING("true")) { *uipOutCharsParsed = 4; return true; } 
		if (rString == STRING("True")) { *uipOutCharsParsed = 4; return true; } 
		if (rString == STRING("true")) { *uipOutCharsParsed = 4; return true; } 

		if (rString == STRING("0"))     { *uipOutCharsParsed = 1; return false; } 
		if (rString == STRING("false")) { *uipOutCharsParsed = 5; return false; } 
		if (rString == STRING("False")) { *uipOutCharsParsed = 5; return false; } 
		if (rString == STRING("false")) { *uipOutCharsParsed = 5; return false; } 

		*uipOutCharsParsed = 0;
		return false;
	}
	else
	{
		if ((rString == STRING("1"))    ||
			(rString == STRING("true")) ||
			(rString == STRING("True")) ||
			(rString == STRING("true"))) { return true; }
	
		return false;
	}
}

CString CBool::Print(_IN bool bValue)
{
	if (bValue) { return STRING("true"); } 
	else        { return STRING("false"); }
}

CBool::CBool(_IN CString& rString) { m_bValue = CBool::Parse(rString, nullptr); }

CString CBool::ToString() _GET { return CBool::Print(m_bValue); }
