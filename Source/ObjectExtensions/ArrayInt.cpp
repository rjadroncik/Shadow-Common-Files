#include "ArrayInt.h"
#include "Int.h"
#include "StringRange.h"

#include <malloc.h>

using namespace SCFBase;

UINT CArrayInt::Parse(_IN CString& rString)
{
	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed      = 0;

	for (UINT i = 0; i < m_uiCount; i++)
	{
		int iValue = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

		if (uiCharsParsed)
		{
			(*(CInt*)m_ppValues[i]) = iValue;

			uiCharsParsedTotal += uiCharsParsed;

			if (uiCharsParsedTotal >= rString.Length()) { break; }
		}
		else { break; }
	}

	return uiCharsParsedTotal;
}

CArrayInt& CArrayInt::Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed)
{
	CArrayInt* pArray = new CArrayInt();

	UINT uiCharsParsedTotal = 0;
	UINT uiCharsParsed      = 0;

	for (;;)
	{
		int iValue = CInt::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

		if (uiCharsParsed)
		{
			pArray->LastAdd(*(new CInt(iValue)));

			uiCharsParsedTotal += uiCharsParsed;

			if (uiCharsParsedTotal >= rString.Length()) { break; }
		}
		else { break; }
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal; }

	return *pArray;
}

CArrayInt::CArrayInt(_IN UINT uiSize)
{
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * uiSize);
	m_uiCount  = uiSize;

	for (UINT i = 0; i < m_uiCount; i++) { m_ppValues[i] = new CInt(0); ADDREF(*(m_ppValues[i])); }
}

CArrayInt::~CArrayInt() { }

CArrayInt::CArrayInt(_IN CArrayInt& rArray)
{
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * rArray.Size());
	m_uiCount  = rArray.Size();

	for (UINT i = 0; i < m_uiCount; i++) 
	{ 
		m_ppValues[i] = new CInt(*(CInt*)rArray.m_ppValues[i]);
		ADDREF(*(m_ppValues[i]));
	}
}

void CArrayInt::AtPut(_IN UINT uiIndex, _IN CValue& rValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CInt*)m_ppValues[uiIndex]) = ((CInt&)rValue);
}

int CArrayInt::At(_IN UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CInt*)m_ppValues[uiIndex])->Value(); 
}

int CArrayInt::operator [](_IN UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CInt*)m_ppValues[uiIndex])->Value();
}

void CArrayInt::AtPut(_IN UINT uiIndex, _IN int iValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CInt*)m_ppValues[uiIndex]) = iValue;
}