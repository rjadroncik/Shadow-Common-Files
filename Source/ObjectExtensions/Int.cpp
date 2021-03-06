#include "Int.h"
#include "String.h"
#include "Stack.h"
#include "Char.h"

using namespace SCFBase;

UINT Int_uiDigitsMin = 1;
BYTE Int_ucBase = 10;
bool      Int_bBasePrefix = true;

void CInt::DigitsMin(_IN UINT uiMin) { Int_uiDigitsMin = uiMin; }
UINT CInt::DigitsMin()               { return Int_uiDigitsMin; }

void CInt::Base(_IN BYTE ucBase) { Int_ucBase = ucBase; }
BYTE CInt::Base()                { return Int_ucBase; }

int CInt::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	register TCHAR* szString = rString.Value();

	for (UINT i = 0; i < rString.Length(); i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) 
		{ 
			if (((i + 1) < rString.Length()) && ((szString[i + 1] == 'x') || (szString[i + 1] == 'X')))
			{
				return ParseBase16(rString, i + 2, uipOutCharsParsed);
			}
			else
			{
				return ParseBase10(rString, i, uipOutCharsParsed);
			}
		}

		if ((szString[i] == 'x') || (szString[i] == 'X'))
		{
			return ParseBase16(rString, i + 1, uipOutCharsParsed);
		}
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = 0; }
	return 0;
}

int CInt::ParseBase10(_IN CString& rString, _IN UINT uiValueStart, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiValueEnd = rString.Length();

	register TCHAR* szString = rString.Value();

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] < '0') || (szString[i] > '9')) { uiValueEnd = i; break; }
	}

	UINT uiPower = 1;
	int iResult = 0;

	for (UINT i = uiValueEnd; i > uiValueStart; i--)
	{
		iResult += (szString[i - 1] - '0') * uiPower;
		uiPower *= 10;
	}

	if ((uiValueStart > 0) && (szString[uiValueStart - 1] == '-')) { iResult *= -1; }

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return iResult;
}

int CInt::ParseBase16(_IN CString& rString, _IN UINT uiValueStart, _OUT _OPT UINT* uipOutCharsParsed)
{
	UINT uiValueEnd = rString.Length();

	register TCHAR* szString = rString.Value();

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if (((szString[i] < '0') || (szString[i] > '9')) &&
			((szString[i] < 'a') || (szString[i] > 'f')) && 
			((szString[i] < 'A') || (szString[i] > 'F'))) { uiValueEnd = i; break; }
	}

	UINT uiResult = 0;
	int iDigit = 0;

	for (UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) { iDigit = (szString[i] - '0'); }
		if ((szString[i] >= 'a') && (szString[i] <= 'f')) { iDigit = (szString[i] - 'a' + 10); }
		if ((szString[i] >= 'A') && (szString[i] <= 'F')) { iDigit = (szString[i] - 'A' + 10); }

		uiResult <<= 4;
		uiResult |= iDigit;
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return (int)uiResult;
}

CString CInt::Print(_IN int iValue)
{
	CString Result;
	CInt::Print(iValue, Result); 

	return CString(Result, false, true);
}

void CInt::Print(_IN int iValue, _OUT CString& rResult)
{
	switch (Int_ucBase)
	{
	case 10: { PrintBase10(iValue, rResult); return; }
	case 16: { PrintBase16(iValue, rResult); return; }
	default: { PrintBase10(iValue, rResult); return; }
	}
}

void CInt::PrintBase10(_IN int iValue, _OUT CString& rResult)
{
	static TCHAR caDigits[32];

	register BYTE ucDigitCurrent = 31;
	register UINT uiValueLeft = __abs(iValue);

	while (uiValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (TCHAR)(uiValueLeft % 10);
		ucDigitCurrent--;

		uiValueLeft /= 10;
	}

	while (ucDigitCurrent > 31 - Int_uiDigitsMin)
	{
		caDigits[ucDigitCurrent] = '0';
		ucDigitCurrent--;
	}

	if (ucDigitCurrent == 31) { caDigits[ucDigitCurrent] = '0'; }
	else
	{
		if (iValue < 0) { caDigits[ucDigitCurrent] = '-'; }
		else            { ucDigitCurrent++; }
	}

	rResult.Assign(&caDigits[ucDigitCurrent], 32 - ucDigitCurrent);
}

void CInt::PrintBase16(_IN int iValue, _OUT CString& rResult)
{
	static TCHAR caDigits[34];

	register BYTE ucDigitCurrent = 33;
	register UINT uiValueLeft = (UINT)iValue;

	while (uiValueLeft > 0)
	{
		caDigits[ucDigitCurrent] = '0' + (TCHAR)(uiValueLeft % 16);
		
		if (caDigits[ucDigitCurrent] > '9') { caDigits[ucDigitCurrent] += 'A' - '9' - 1; }

		ucDigitCurrent--;

		uiValueLeft /= 16;
	}

	while (ucDigitCurrent > 33 - Int_uiDigitsMin)
	{
		caDigits[ucDigitCurrent] = '0';
		ucDigitCurrent--;
	}

	if (ucDigitCurrent == 33) { caDigits[ucDigitCurrent] = '0'; }
	else
	{
		if (iValue < 0) { caDigits[ucDigitCurrent] = '-'; }
		else            { ucDigitCurrent++; }
	}

	if (Int_bBasePrefix) 
	{
		caDigits[ucDigitCurrent - 1] = 'x';
		caDigits[ucDigitCurrent - 2] = '0';
		ucDigitCurrent -= 2;
	}

	rResult.Assign(&caDigits[ucDigitCurrent], 34 - ucDigitCurrent);
}

CInt::CInt(_IN int iValue)       { m_iValue = iValue; }
CInt::CInt(_IN CInt& rValue)     { m_iValue = rValue.m_iValue; }
CInt::CInt(_IN CString& rString) { m_iValue = CInt::Parse(rString, nullptr); }

CInt::~CInt() {}
 
CString CInt::ToString() _GET { return CInt::Print(m_iValue); }

CString CInt::ToString(_IN CFormatInt& rFormat) _GET 
{
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(false));
	static CFormatInt FormatBefore;
	BETAONLY(CObject::Tracing(bTracing));

	FormatBefore.DigitsMin (Int_uiDigitsMin);
	FormatBefore.Base      (Int_ucBase);
	FormatBefore.BasePrefix(Int_bBasePrefix);
	
	Int_uiDigitsMin = rFormat.DigitsMin();
	Int_ucBase      = rFormat.Base();
	Int_bBasePrefix = rFormat.BasePrefix();

	CString Result;
	CInt::Print(m_iValue, Result); 

	Int_uiDigitsMin = FormatBefore.DigitsMin();
	Int_ucBase      = FormatBefore.Base();
	Int_bBasePrefix = FormatBefore.BasePrefix();

	return CString(Result, false, true);
}

bool CInt::IsSmallerThen(_IN CObject& rObject) const
{
	if (m_iValue < ((const CInt&)rObject).m_iValue) { return true; }

	return false;
}

bool CInt::IsEqualTo(_IN CObject& rObject) const
{
	if (m_iValue == ((const CInt&)rObject).m_iValue) { return true; }

	return false;
}
