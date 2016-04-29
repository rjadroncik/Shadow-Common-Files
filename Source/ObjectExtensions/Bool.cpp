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

bool CBool::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	if (uipOutCharsParsed)
	{
		if (rString == STRING("1"))    { *uipOutCharsParsed = 1; return TRUE; } 
		if (rString == STRING("true")) { *uipOutCharsParsed = 4; return TRUE; } 
		if (rString == STRING("True")) { *uipOutCharsParsed = 4; return TRUE; } 
		if (rString == STRING("TRUE")) { *uipOutCharsParsed = 4; return TRUE; } 

		if (rString == STRING("0"))     { *uipOutCharsParsed = 1; return FALSE; } 
		if (rString == STRING("false")) { *uipOutCharsParsed = 5; return FALSE; } 
		if (rString == STRING("False")) { *uipOutCharsParsed = 5; return FALSE; } 
		if (rString == STRING("FALSE")) { *uipOutCharsParsed = 5; return FALSE; } 

		*uipOutCharsParsed = 0;
		return FALSE;
	}
	else
	{
		if ((rString == STRING("1"))    ||
			(rString == STRING("true")) ||
			(rString == STRING("True")) ||
			(rString == STRING("TRUE"))) { return TRUE; }
	
		return FALSE;
	}
}

CString CBool::Print(_IN bool bValue)
{
	if (bValue) { return STRING("true"); } 
	else        { return STRING("false"); }
}

CBool::CBool(_IN CString& rString) { m_bValue = CBool::Parse(rString, NULL); }

CString CBool::ToString() _GET { return CBool::Print(m_bValue); }
