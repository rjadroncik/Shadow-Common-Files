#include "Array.h"
#include "String.h"

#include <malloc.h>

using namespace SCFBase;

CArray::~CArray()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		RELEASE(*(m_ppValues[i]));
		delete m_ppValues[i];
	}
	
	if (m_ppValues) { free(m_ppValues); }
}

CValue& CArray::At(_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return *m_ppValues[uiIndex]; 
}

CValue& CArray::operator [](_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(uiIndex < m_uiCount);

	return *m_ppValues[uiIndex];
}

CString CArray::ToString() _GET 
{
	CString csValue;

	for (SCF::UINT i = 0; i < (m_uiCount - 1); i++)
	{
		csValue += m_ppValues[i]->ToString();
		csValue += STRING(" ");
	}

	if (m_uiCount) { csValue += m_ppValues[m_uiCount - 1]->ToString(); }

	return csValue; 
}

void CArray::LastAdd(_IN _REF CValue& rValue)
{
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppValues) { m_ppValues = (CValue**)realloc(m_ppValues, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else            { m_ppValues = (CValue**)malloc (            sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppValues[m_uiCount] = (CValue*)&rValue;
	m_uiCount++;
	ADDREF(rValue);
}