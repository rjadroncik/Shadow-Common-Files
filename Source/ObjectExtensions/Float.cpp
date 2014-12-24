#include "Float.h"
#include "String.h"
#include "Int.h"
#include "Memory.h"

using namespace SCFBase;

SCF::UINT  Float_uiFractionDigitsMin = 1;
SCF::UINT  Float_uiFractionDigitsMax = 4;

float Float_fMultiplierForPrint = 10000; 

SCF::TCHAR Float_usDecimalSeparator = '.';

void      CFloat::DecimalSeparator(_IN SCF::ENUM eChar) { Float_usDecimalSeparator = (SCF::TCHAR)eChar; }
SCF::ENUM CFloat::DecimalSeparator()                    { return (SCF::ENUM)Float_usDecimalSeparator; }

void CFloat::FractionDigits(_IN SCF::UINT uiMin, _IN SCF::UINT uiMax) 
{ 
	Float_uiFractionDigitsMin = uiMin;
	Float_uiFractionDigitsMax = uiMax;

	Float_fMultiplierForPrint = 1;

	for (SCF::UINT i = 0; i < Float_uiFractionDigitsMax; i++)
	{
		Float_fMultiplierForPrint *= 10;
	}
}


SCF::UINT CFloat::FractionDigitsMin() { return Float_uiFractionDigitsMin; }
SCF::UINT CFloat::FractionDigitsMax() { return Float_uiFractionDigitsMax; }

float CFloat::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	SCF::UINT uiValueStart   = rString.Length();
	SCF::UINT uiValueEnd     = rString.Length();
	SCF::UINT uiDecimalPoint = rString.Length();

	SCF::TCHAR* szString = rString.Value();

	for (SCF::UINT i = 0; i < uiValueEnd; i++)
	{
		if ((szString[i] >= '0') && (szString[i] <= '9')) { uiValueStart = i; break; }
	}

	for (SCF::UINT i = uiValueStart; i < uiValueEnd; i++)
	{
		if ((szString[i] < '0') || (szString[i] > '9')) 
		{
			if ((szString[i] == '.') || (szString[i] == ',')) { uiDecimalPoint = i; }
			else                                              { uiValueEnd = i; break; }
		}
	}
 
	float fResult = 0;
	float fPower  = 0.1f;

	for (SCF::UINT j = uiDecimalPoint + 1; j < uiValueEnd; j++)
	{
		fResult += ((float)(szString[j] - '0')) * fPower;
		fPower *= 0.1f;
	}

	SCF::UINT uiPower = 1;

	for (int i = __min(uiDecimalPoint, uiValueEnd) - 1; i >= (int)uiValueStart; i--)
	{
		fResult += (float)((szString[i] - '0') * uiPower);
		uiPower *= 10;
	}

	if (uiValueStart && (szString[uiValueStart - 1] == '-')) { fResult *= -1; }
 
	if (uipOutCharsParsed) { *uipOutCharsParsed = uiValueEnd; }
	return fResult;
}

CString CFloat::Print(float fValue)
{
	CString Result;
	CFloat::Print(fValue, Result); 

	return CString(Result, FALSE, TRUE);
}

void CFloat::Print(_IN float fValue, _OUT CString& rResult)
{
	static SCF::TCHAR caDigits[16];
	
	register SCF::BYTE ucDigitCurrent = 15;
	register float fCurrentValue = __abs(fValue * Float_fMultiplierForPrint) + 0.5f;

	for (SCF::UINT i = 0 ; i < Float_uiFractionDigitsMax; i++)
	{
		caDigits[ucDigitCurrent] = '0' + (SCF::TCHAR)((int)(fCurrentValue) % 10);

		//Skip digit if it is '0' & is on a fraction position greater then the specified minimum of fraction digits
		if ((caDigits[ucDigitCurrent] != '0') || ((Float_uiFractionDigitsMax - i) <= Float_uiFractionDigitsMin))
		{
			ucDigitCurrent--;
		}

		fCurrentValue /= 10;
	}

	//Write decimal point char
	caDigits[ucDigitCurrent] = Float_usDecimalSeparator;
	ucDigitCurrent--;

	if (fCurrentValue < 1)
	{
		caDigits[ucDigitCurrent] = '0';
		ucDigitCurrent--;
	}
	else
	{
		while (fCurrentValue >= 1)
		{
			caDigits[ucDigitCurrent] = '0' + (SCF::TCHAR)((int)(fCurrentValue) % 10);
			ucDigitCurrent--;

			fCurrentValue /= 10;
		}
	}

	if (fValue < 0)
	{
		caDigits[ucDigitCurrent] = '-';
		ucDigitCurrent--;
	}

	rResult.Assign(&caDigits[ucDigitCurrent + 1], 15 - (SCF::UINT)ucDigitCurrent);
}

CFloat::CFloat(_IN float fValue)     { m_fValue = fValue; }
CFloat::CFloat(_IN CFloat& rValue)   { m_fValue = rValue.m_fValue; }
CFloat::CFloat(_IN CString& rString) { m_fValue = CFloat::Parse(rString, NULL); }

CFloat::~CFloat() {}

CString CFloat::ToString() _GET { return CFloat::Print(m_fValue); }
 
CString CFloat::ToString(_IN CFormatFloat& rFormat) _GET 
{
	static CFormatFloat FormatBefore;
	FormatBefore.FractionDigitsMax(Float_uiFractionDigitsMax);
	FormatBefore.FractionDigitsMin(Float_uiFractionDigitsMin);
	FormatBefore.DecimalSeparator (Float_usDecimalSeparator);

	FractionDigits(rFormat.FractionDigitsMin(), rFormat.FractionDigitsMax());
	Float_usDecimalSeparator = (SCF::TCHAR)rFormat.DecimalSeparator();

	CString Result;
	CFloat::Print(m_fValue, Result); 

	FractionDigits(FormatBefore.FractionDigitsMin(), FormatBefore.FractionDigitsMax());
	Float_usDecimalSeparator = (SCF::TCHAR)FormatBefore.DecimalSeparator();

	return CString(Result, FALSE, TRUE);
}


void CFloat::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutFloat(m_fValue);
}

void CFloat::Deserialize(_INOUT IStreamRead& rStream)
{
	m_fValue = rStream.GetFloat();
}

bool CFloat::IsSmallerThen(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassFloat) && (m_fValue < ((const CFloat&)rObject).m_fValue)) { return TRUE; }

	return FALSE;
}

bool CFloat::IsEqualTo(_IN CObject& rObject) const
{
	if ((rObject.ClassKey() == ClassFloat) && (m_fValue == ((const CFloat&)rObject).m_fValue)) { return TRUE; }

	return FALSE;
}
