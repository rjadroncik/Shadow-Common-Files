#include "DateTime.h"
#include "String.h"
#include "StringRange.h"
#include "Int.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

SYSTEMTIME DateTime_SystemTime;

//2008-04-10 13:30:00
CDateTime& CDateTime::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	static CDateTime Value;
	Parse(Value, rString, uipOutCharsParsed);
	return Value;
}

//2008-04-10 13:30:00
void CDateTime::Parse(_OUT CDateTime& rOutValue, _IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed = 0;

	rOutValue.m_usYear  = (SCF::USHORT)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 4), &uiCharsParsedTotal); uiCharsParsedTotal++;
	rOutValue.m_ucMonth = (SCF::BYTE)  CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);      uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucDay   = (SCF::BYTE)  CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);      uiCharsParsedTotal += uiCharsParsed + 1;

	rOutValue.m_ucHour   = (SCF::BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucMinute = (SCF::BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucSecond = (SCF::BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);

	rOutValue.m_usMilliseconds = 0;

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

//2008-04-10 13:30:00
CString CDateTime::Print(_IN  CDateTime& rValue)
{
	CString RetVal;
	SCF::UINT uiDigitsMin = CInt::DigitsMin();

	CInt::DigitsMin(4);
	RetVal += CInt::Print(rValue.m_usYear);
	RetVal += STRING("-");
	CInt::DigitsMin(2);
	RetVal += CInt::Print(rValue.m_ucMonth);
	RetVal += STRING("-");
	RetVal += CInt::Print(rValue.m_ucDay);
	
	RetVal += STRING(" ");

	RetVal += CInt::Print(rValue.m_ucHour);
	RetVal += STRING(":");
	RetVal += CInt::Print(rValue.m_ucMinute);
	RetVal += STRING(":");
	RetVal += CInt::Print(rValue.m_ucSecond);

	CInt::DigitsMin(uiDigitsMin);
	return RetVal;
}

CDateTime::CDateTime()
{
	GetLocalTime(&DateTime_SystemTime);

	m_usYear         = (SCF::USHORT)DateTime_SystemTime.wYear;
	m_ucMonth        = (SCF::BYTE)  DateTime_SystemTime.wMonth;
	m_ucDay          = (SCF::BYTE)  DateTime_SystemTime.wDay;
	m_ucHour         = (SCF::BYTE)  DateTime_SystemTime.wHour;
	m_ucMinute       = (SCF::BYTE)  DateTime_SystemTime.wMinute;
	m_ucSecond       = (SCF::BYTE)  DateTime_SystemTime.wSecond;
	m_usMilliseconds = (SCF::USHORT)DateTime_SystemTime.wMilliseconds;
}

CDateTime::CDateTime(_IN CDateTime& rValue)
{
	m_usYear         = rValue.m_usYear;
	m_ucMonth        = rValue.m_ucMonth;
	m_ucDay          = rValue.m_ucDay;
	m_ucHour         = rValue.m_ucHour;
	m_ucMinute       = rValue.m_ucMinute;
	m_ucSecond       = rValue.m_ucSecond;
	m_usMilliseconds = rValue.m_usMilliseconds;
}

CDateTime::~CDateTime()
{
}

CDateTime::CDateTime(_IN CString& rString) { CDateTime::Parse(*this, rString, NULL); }

CString CDateTime::ToString() const { return CDateTime::Print(*this); }	

void CDateTime::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutWord(m_usYear);
	rStream.PutByte(m_ucMonth);
	rStream.PutByte(m_ucDay);
	rStream.PutByte(m_ucHour);
	rStream.PutByte(m_ucMinute);
	rStream.PutByte(m_ucSecond);
	rStream.PutWord(m_usMilliseconds);
}

void CDateTime::Deserialize(_INOUT IStreamRead& rStream)
{
	m_usYear         = rStream.GetWord();
	m_ucMonth        = rStream.GetByte();
	m_ucDay          = rStream.GetByte();
	m_ucHour         = rStream.GetByte();
	m_ucMinute       = rStream.GetByte();
	m_ucSecond       = rStream.GetByte();
	m_usMilliseconds = rStream.GetWord();
}

bool CDateTime::IsSmallerThen(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassDateTime))
	{
		if (m_usYear         < ((CDateTime&)rObject).m_usYear)         { return TRUE; }
		if (m_ucMonth        < ((CDateTime&)rObject).m_ucMonth)        { return TRUE; }
		if (m_ucDay          < ((CDateTime&)rObject).m_ucDay)          { return TRUE; }
		if (m_ucHour         < ((CDateTime&)rObject).m_ucHour)         { return TRUE; }
		if (m_ucMinute       < ((CDateTime&)rObject).m_ucMinute)       { return TRUE; }
		if (m_ucSecond       < ((CDateTime&)rObject).m_ucSecond)       { return TRUE; }
		if (m_usMilliseconds < ((CDateTime&)rObject).m_usMilliseconds) { return TRUE; }
	}

	return FALSE;
}

bool CDateTime::IsEqualTo(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassDateTime))
	{
		if (m_usYear         != ((CDateTime&)rObject).m_usYear)         { return FALSE; }
		if (m_ucMonth        != ((CDateTime&)rObject).m_ucMonth)        { return FALSE; }
		if (m_ucDay          != ((CDateTime&)rObject).m_ucDay)          { return FALSE; }
		if (m_ucHour         != ((CDateTime&)rObject).m_ucHour)         { return FALSE; }
		if (m_ucMinute       != ((CDateTime&)rObject).m_ucMinute)       { return FALSE; }
		if (m_ucSecond       != ((CDateTime&)rObject).m_ucSecond)       { return FALSE; }
		if (m_usMilliseconds != ((CDateTime&)rObject).m_usMilliseconds) { return FALSE; }

		return TRUE;
	}

	return FALSE;
}

