#include "Int64.h"
#include "String.h"
#include "Stack.h"
#include "Char.h"

using namespace SCFBase;

UINT Int64_uiDigitsMin = 1;
BYTE Int64_ucBase = 10;
bool      Int64_bBasePrefix = true;

void      CInt64::DigitsMin(_IN UINT uiMin) { Int64_uiDigitsMin = uiMin; }
UINT CInt64::DigitsMin()                    { return Int64_uiDigitsMin; }

void      CInt64::Base(_IN BYTE ucBase) { Int64_ucBase = ucBase; }
BYTE CInt64::Base()                     { return Int64_ucBase; }

INT64 CInt64::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	register TCHAR* szString = rString.Value();

	for (UINT i = 0; i < rString.Length(); i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) 
		{ 
			if (((i + 1) < rString.Length()) && (szString[i + 1] == 'x'))
			{
				return ParseBase16(rString, i + 2, uipOutCharsParsed);
			}
			else
			{
				return ParseBase10(rString, i, uipOutCharsParsed);
			}
		}
	}

	return 0;
}

INT64 CInt64::ParseBase10(_IN CString& rString, _IN UINT uiValueStart, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiValueEnd = rString.Length();

	register TCHAR* szString = rString.Value();

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] < '0') || (szString[i] > '9')) { uiValueEnd = i; break; }
	}

	UINT64 ui64Power = 1;
	INT64 i64Result = 0;

	for (UINT i = uiValueEnd; i > uiValueStart; i--)
	{
		i64Result += (szString[i - 1] - '0') * ui64Power;
		ui64Power *= 10;
	}

	if ((uiValueStart > 0) && (szString[uiValueStart - 1] == '-')) { i64Result *= -1; }

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return i64Result;
}

INT64 CInt64::ParseBase16(_IN CString& rString, _IN UINT uiValueStart, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiValueEnd = rString.Length();

	register TCHAR* szString = rString.Value();

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if (((szString[i] < '0') || (szString[i] > '9')) &&
			((szString[i] < 'a') || (szString[i] > 'f')) && 
			((szString[i] < 'A') || (szString[i] > 'F'))) { uiValueEnd = i; break; }
	}

	UINT64 ui64Result = 0;
	int iDigit = 0;

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) { iDigit = (szString[i] - '0'); }
		if ((szString[i] >= 'a') && (szString[i] <= 'f')) { iDigit = (szString[i] - 'a' + 10); }
		if ((szString[i] >= 'A') && (szString[i] <= 'F')) { iDigit = (szString[i] - 'A' + 10); }

		ui64Result <<= 4;
		ui64Result |= iDigit;
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return (INT64)ui64Result;
}

CString CInt64::Print(_IN INT64 i64Value)
{
	CString Result;
	CInt64::Print(i64Value, Result); 

	return CString(Result, false, true);
}

void CInt64::Print(_IN INT64 i64Value, _OUT CString& rResult)
{
	switch (Int64_ucBase)
	{
	case 10: { PrintBase10(i64Value, rResult); return; }
	case 16: { PrintBase16(i64Value, rResult); return; }
	default: { PrintBase10(i64Value, rResult); return; }
	}
}

void CInt64::PrintBase10(_IN INT64 i64Value, _OUT CString& rResult)
{
	static TCHAR caDigits[32];

	register BYTE ucDigitCurrent = 31;
	register UINT64 ui64ValueLeft = __abs(i64Value);

	while (ui64ValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (TCHAR)(ui64ValueLeft % 10);
		ucDigitCurrent--;

		ui64ValueLeft /= 10;
	}

	while (ucDigitCurrent > 31 - Int64_uiDigitsMin)
	{
		caDigits[ucDigitCurrent] = '0';
		ucDigitCurrent--;
	}

	if (ucDigitCurrent == 31) { caDigits[ucDigitCurrent] = '0'; }
	else
	{
		if (i64Value < 0) { caDigits[ucDigitCurrent] = '-'; }
		else              { ucDigitCurrent++; }
	}

	rResult.Assign(&caDigits[ucDigitCurrent], 32 - ucDigitCurrent);
}

void CInt64::PrintBase16(_IN INT64 i64Value, _OUT CString& rResult)
{
	static TCHAR caDigits[34];

	register BYTE ucDigitCurrent = 33;
	register UINT64 ui64ValueLeft = (UINT64)i64Value;

	while (ui64ValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (TCHAR)(ui64ValueLeft % 16);

		if (caDigits[ucDigitCurrent] > '9') { caDigits[ucDigitCurrent] += 'A' - '9' - 1; }

		ucDigitCurrent--;

		ui64ValueLeft /= 16;
	}

	while (ucDigitCurrent > 33 - Int64_uiDigitsMin)
	{
		caDigits[ucDigitCurrent] = '0';
		ucDigitCurrent--;
	}

	if (ucDigitCurrent == 33) { caDigits[ucDigitCurrent] = '0'; }
	else
	{
		if (i64Value < 0) { caDigits[ucDigitCurrent] = '-'; }
		else              { ucDigitCurrent++; }
	}

	if (Int64_bBasePrefix) 
	{
		caDigits[ucDigitCurrent - 1] = 'x';
		caDigits[ucDigitCurrent - 2] = '0';
		ucDigitCurrent -= 2;
	}

	rResult.Assign(&caDigits[ucDigitCurrent], 34 - ucDigitCurrent);
}

CInt64::CInt64(_IN INT64 i64Value) { m_i64Value = i64Value; }
CInt64::CInt64(_IN CInt64& rValue)      { m_i64Value = rValue.m_i64Value; }
CInt64::CInt64(_IN CString& rString)    { m_i64Value = CInt64::Parse(rString, nullptr); }

CInt64::~CInt64() {}
 
CString CInt64::ToString() _GET { return CInt64::Print(m_i64Value); }

CString CInt64::ToString(_IN CFormatInt& rFormat) _GET 
{
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(false));
	static CFormatInt FormatBefore;
	BETAONLY(CObject::Tracing(bTracing));

	FormatBefore.DigitsMin (Int64_uiDigitsMin);
	FormatBefore.Base      (Int64_ucBase);
	FormatBefore.BasePrefix(Int64_bBasePrefix);

	Int64_uiDigitsMin = rFormat.DigitsMin();
	Int64_ucBase      = rFormat.Base();
	Int64_bBasePrefix = rFormat.BasePrefix();

	CString Result;
	CInt64::Print(m_i64Value, Result); 

	Int64_uiDigitsMin = FormatBefore.DigitsMin();
	Int64_ucBase      = FormatBefore.Base();
	Int64_bBasePrefix = FormatBefore.BasePrefix();

	return CString(Result, false, true);
}

bool CInt64::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_i64Value < ((const CInt64&)rObject).m_i64Value) { return true; }

	return false;
}

bool CInt64::IsEqualTo(_IN CObject& rObject) const
{
	if (m_i64Value == ((const CInt64&)rObject).m_i64Value) { return true; }

	return false;
}
