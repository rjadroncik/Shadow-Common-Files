#include "Int64.h"
#include "String.h"
#include "Stack.h"
#include "Char.h"

using namespace SCFBase;

SCF::UINT Int64_uiDigitsMin = 1;
SCF::BYTE Int64_ucBase = 10;
bool      Int64_bBasePrefix = TRUE;

void      CInt64::DigitsMin(_IN SCF::UINT uiMin) { Int64_uiDigitsMin = uiMin; }
SCF::UINT CInt64::DigitsMin()                    { return Int64_uiDigitsMin; }

void      CInt64::Base(_IN SCF::BYTE ucBase) { Int64_ucBase = ucBase; }
SCF::BYTE CInt64::Base()                     { return Int64_ucBase; }

SCF::INT64 CInt64::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	register SCF::TCHAR* szString = rString.Value();

	for (SCF::UINT i = 0; i < rString.Length(); i++)
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

SCF::INT64 CInt64::ParseBase10(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiValueEnd = rString.Length();

	register SCF::TCHAR* szString = rString.Value();

	for (SCF::UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] < '0') || (szString[i] > '9')) { uiValueEnd = i; break; }
	}

	SCF::UINT64 ui64Power = 1;
	SCF::INT64 i64Result = 0;

	for (SCF::UINT i = uiValueEnd; i > uiValueStart; i--)
	{
		i64Result += (szString[i - 1] - '0') * ui64Power;
		ui64Power *= 10;
	}

	if ((uiValueStart > 0) && (szString[uiValueStart - 1] == '-')) { i64Result *= -1; }

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return i64Result;
}

SCF::INT64 CInt64::ParseBase16(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiValueEnd = rString.Length();

	register SCF::TCHAR* szString = rString.Value();

	for (SCF::UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if (((szString[i] < '0') || (szString[i] > '9')) &&
			((szString[i] < 'a') || (szString[i] > 'f')) && 
			((szString[i] < 'A') || (szString[i] > 'F'))) { uiValueEnd = i; break; }
	}

	SCF::UINT64 ui64Result = 0;
	int iDigit = 0;

	for (SCF::UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) { iDigit = (szString[i] - '0'); }
		if ((szString[i] >= 'a') && (szString[i] <= 'f')) { iDigit = (szString[i] - 'a' + 10); }
		if ((szString[i] >= 'A') && (szString[i] <= 'F')) { iDigit = (szString[i] - 'A' + 10); }

		ui64Result <<= 4;
		ui64Result |= iDigit;
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return (SCF::INT64)ui64Result;
}

CString CInt64::Print(_IN SCF::INT64 i64Value)
{
	CString Result;
	CInt64::Print(i64Value, Result); 

	return CString(Result, FALSE, TRUE);
}

void CInt64::Print(_IN SCF::INT64 i64Value, _OUT CString& rResult)
{
	switch (Int64_ucBase)
	{
	case 10: { PrintBase10(i64Value, rResult); return; }
	case 16: { PrintBase16(i64Value, rResult); return; }
	default: { PrintBase10(i64Value, rResult); return; }
	}
}

void CInt64::PrintBase10(_IN SCF::INT64 i64Value, _OUT CString& rResult)
{
	static SCF::TCHAR caDigits[32];

	register SCF::BYTE ucDigitCurrent = 31;
	register SCF::UINT64 ui64ValueLeft = __abs(i64Value);

	while (ui64ValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (SCF::TCHAR)(ui64ValueLeft % 10);
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

void CInt64::PrintBase16(_IN SCF::INT64 i64Value, _OUT CString& rResult)
{
	static SCF::TCHAR caDigits[34];

	register SCF::BYTE ucDigitCurrent = 33;
	register SCF::UINT64 ui64ValueLeft = (SCF::UINT64)i64Value;

	while (ui64ValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (SCF::TCHAR)(ui64ValueLeft % 16);

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

CInt64::CInt64(_IN SCF::INT64 i64Value) { m_i64Value = i64Value; }
CInt64::CInt64(_IN CInt64& rValue)      { m_i64Value = rValue.m_i64Value; }
CInt64::CInt64(_IN CString& rString)    { m_i64Value = CInt64::Parse(rString, NULL); }

CInt64::~CInt64() {}
 
CString CInt64::ToString() _GET { return CInt64::Print(m_i64Value); }

CString CInt64::ToString(_IN CFormatInt& rFormat) _GET 
{
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(FALSE));
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

	return CString(Result, FALSE, TRUE);
}

bool CInt64::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_i64Value < ((const CInt64&)rObject).m_i64Value) { return TRUE; }

	return FALSE;
}

bool CInt64::IsEqualTo(_IN CObject& rObject) const
{
	if (m_i64Value == ((const CInt64&)rObject).m_i64Value) { return TRUE; }

	return FALSE;
}
