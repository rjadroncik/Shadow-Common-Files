#include "DateTime.h"
#include "String.h"
#include "StringRange.h"
#include "Int.h"

using namespace SCFBase;

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

SYSTEMTIME DateTime_SystemTime;

#else

#include <time.h>

#endif

//2008-04-10 13:30:00
CDateTime& CDateTime::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	static CDateTime Value;
	Parse(Value, rString, uipOutCharsParsed);
	return Value;
}

//2008-04-10 13:30:00
void CDateTime::Parse(_OUT CDateTime& rOutValue, _IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed = 0;

	rOutValue.m_usYear  = (USHORT)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 4), &uiCharsParsedTotal); uiCharsParsedTotal++;
	rOutValue.m_ucMonth = (BYTE)  CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);      uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucDay   = (BYTE)  CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);      uiCharsParsedTotal += uiCharsParsed + 1;

	rOutValue.m_ucHour   = (BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucMinute = (BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed); uiCharsParsedTotal += uiCharsParsed + 1;
	rOutValue.m_ucSecond = (BYTE)CInt::Parse(CStringRange(rString, uiCharsParsedTotal, 2), &uiCharsParsed);

	rOutValue.m_usMilliseconds = 0;

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal + uiCharsParsed; }
}

//2008-04-10 13:30:00
CString CDateTime::Print(_IN  CDateTime& rValue)
{
	CString RetVal;
	UINT uiDigitsMin = CInt::DigitsMin();

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
    #ifdef WIN32

	GetLocalTime(&DateTime_SystemTime);

	m_usYear         = (USHORT)DateTime_SystemTime.wYear;
	m_ucMonth        = (BYTE)  DateTime_SystemTime.wMonth;
	m_ucDay          = (BYTE)  DateTime_SystemTime.wDay;
	m_ucHour         = (BYTE)  DateTime_SystemTime.wHour;
	m_ucMinute       = (BYTE)  DateTime_SystemTime.wMinute;
	m_ucSecond       = (BYTE)  DateTime_SystemTime.wSecond;
	m_usMilliseconds = (USHORT)DateTime_SystemTime.wMilliseconds;

	#else

	time_t unixTime;
	time(&unixTime);

    struct tm* pDateTime = localtime(&unixTime);

   	m_usYear         = (USHORT)pDateTime->tm_year;
   	m_ucMonth        = (BYTE)  pDateTime->tm_mon;
	m_ucDay          = (BYTE)  pDateTime->tm_mday;
	m_ucHour         = (BYTE)  pDateTime->tm_hour;
	m_ucMinute       = (BYTE)  pDateTime->tm_min;
	m_ucSecond       = (BYTE)  pDateTime->tm_sec;
	m_usMilliseconds = (USHORT)0;

	#endif
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

CDateTime::CDateTime(_IN CString& rString) { CDateTime::Parse(*this, rString, nullptr); }

CString CDateTime::ToString() const { return CDateTime::Print(*this); }

bool CDateTime::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_usYear         < ((CDateTime&)rObject).m_usYear)         { return true; }
	if (m_ucMonth        < ((CDateTime&)rObject).m_ucMonth)        { return true; }
	if (m_ucDay          < ((CDateTime&)rObject).m_ucDay)          { return true; }
	if (m_ucHour         < ((CDateTime&)rObject).m_ucHour)         { return true; }
	if (m_ucMinute       < ((CDateTime&)rObject).m_ucMinute)       { return true; }
	if (m_ucSecond       < ((CDateTime&)rObject).m_ucSecond)       { return true; }
	if (m_usMilliseconds < ((CDateTime&)rObject).m_usMilliseconds) { return true; }

	return false;
}

bool CDateTime::IsEqualTo(_IN CObject& rObject) const
{
	if (m_usYear         != ((CDateTime&)rObject).m_usYear)         { return false; }
	if (m_ucMonth        != ((CDateTime&)rObject).m_ucMonth)        { return false; }
	if (m_ucDay          != ((CDateTime&)rObject).m_ucDay)          { return false; }
	if (m_ucHour         != ((CDateTime&)rObject).m_ucHour)         { return false; }
	if (m_ucMinute       != ((CDateTime&)rObject).m_ucMinute)       { return false; }
	if (m_ucSecond       != ((CDateTime&)rObject).m_ucSecond)       { return false; }
	if (m_usMilliseconds != ((CDateTime&)rObject).m_usMilliseconds) { return false; }

	return true;
}

