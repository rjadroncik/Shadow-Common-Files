#include "ArrayInt.h"
#include "Int.h"
#include "StringRange.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern HANDLE Memory_hHeap;

using namespace SCFBase;

SCF::UINT CArrayInt::Parse(_IN CString& rString)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed      = 0;

	for (SCF::UINT i = 0; i < m_uiCount; i++)
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

CArrayInt& CArrayInt::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	CArrayInt* pArray = new CArrayInt();

	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed      = 0;

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

CArrayInt::CArrayInt(_IN SCF::UINT uiSize)
{
	m_ppValues = (CValue**)HeapAlloc(Memory_hHeap, 0, sizeof(CValue*) * uiSize);
	m_uiCount  = uiSize;

	for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppValues[i] = new CInt(0); ADDREF(*(m_ppValues[i])); }
}

CArrayInt::~CArrayInt() { }

CArrayInt::CArrayInt(_IN CArrayInt& rArray)
{
	m_ppValues = (CValue**)HeapAlloc(Memory_hHeap, 0, sizeof(CValue*) * rArray.Size());
	m_uiCount  = rArray.Size();

	for (SCF::UINT i = 0; i < m_uiCount; i++) 
	{ 
		m_ppValues[i] = new CInt(*(CInt*)rArray.m_ppValues[i]);
		ADDREF(*(m_ppValues[i]));
	}
}

void CArrayInt::AtPut(_IN SCF::UINT uiIndex, _IN CValue& rValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CInt*)m_ppValues[uiIndex]) = ((CInt&)rValue);
}

int CArrayInt::At(_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CInt*)m_ppValues[uiIndex])->Value(); 
}

int CArrayInt::operator [](_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CInt*)m_ppValues[uiIndex])->Value();
}

void CArrayInt::AtPut(_IN SCF::UINT uiIndex, _IN int iValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CInt*)m_ppValues[uiIndex]) = iValue;
}