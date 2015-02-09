#include "ArrayFloat.h"
#include "Float.h"
#include "StringRange.h"

#include <malloc.h>

using namespace SCFBase;

SCF::UINT CArrayFloat::Parse(_IN CString& rString)
{
	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed      = 0;

	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		float fValue = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

		if (uiCharsParsed)
		{
			(*(CFloat*)m_ppValues[i]) = fValue;

			uiCharsParsedTotal += uiCharsParsed;

			if (uiCharsParsedTotal >= rString.Length()) { break; }
		}
		else { break; }
	}

	return uiCharsParsedTotal;
}

CArrayFloat& CArrayFloat::Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed)
{
	CArrayFloat* pArray = new CArrayFloat();

	SCF::UINT uiCharsParsedTotal = 0;
	SCF::UINT uiCharsParsed      = 0;

	for (;;)
	{
		float fValue = CFloat::Parse(CStringRange(rString, uiCharsParsedTotal), &uiCharsParsed);

		if (uiCharsParsed)
		{
			pArray->LastAdd(*(new CFloat(fValue)));

			uiCharsParsedTotal += uiCharsParsed;

			if (uiCharsParsedTotal >= rString.Length()) { break; }
		}
		else { break; }
	}

	if (uipOutCharsParsed) { *uipOutCharsParsed = uiCharsParsedTotal; }

	return *pArray;
}

CArrayFloat::CArrayFloat(_IN SCF::UINT uiSize)
{
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * uiSize);
	m_uiCount  = uiSize;

	for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppValues[i] = new CFloat(0); ADDREF(*(m_ppValues[i])); }
}

CArrayFloat::~CArrayFloat() { }

CArrayFloat::CArrayFloat(_IN CArrayFloat& rArray)
{
	m_ppValues = (CValue**)malloc(sizeof(CValue*) * rArray.Size());
	m_uiCount  = rArray.Size();

	for (SCF::UINT i = 0; i < m_uiCount; i++) 
	{ 
		m_ppValues[i] = new CFloat(*(CFloat*)rArray.m_ppValues[i]);
		ADDREF(*(m_ppValues[i]));
	}
}

void CArrayFloat::AtPut(_IN SCF::UINT uiIndex, _IN CValue& rValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CFloat*)m_ppValues[uiIndex]) = ((CFloat&)rValue);
}

float CArrayFloat::At(_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CFloat*)m_ppValues[uiIndex])->Value(); 
}

float CArrayFloat::operator [](_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return ((CFloat*)m_ppValues[uiIndex])->Value();
}

void CArrayFloat::AtPut(_IN SCF::UINT uiIndex, _IN float fValue) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	(*(CFloat*)m_ppValues[uiIndex]) = fValue;
}
