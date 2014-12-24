#include "Array.h"
#include "String.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern HANDLE Memory_hHeap;

using namespace SCFBase;

CArray::~CArray()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		RELEASE(*(m_ppValues[i]));
		delete m_ppValues[i];
	}
	
	if (m_ppValues) { HeapFree(Memory_hHeap, 0, m_ppValues); }
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

void CArray::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiCount);
}

void CArray::Deserialize(_INOUT IStreamRead& rStream)
{
	m_uiCount  = rStream.GetInt();

	if (m_ppValues) { m_ppValues = (CValue**)HeapReAlloc(Memory_hHeap, 0, m_ppValues, sizeof(CValue*) * ((m_uiCount / ALLOC_GRANULARITY_PTRS) + 1) * ALLOC_GRANULARITY_PTRS); }
	else            { m_ppValues = (CValue**)HeapAlloc  (Memory_hHeap, 0,             sizeof(CValue*) * ((m_uiCount / ALLOC_GRANULARITY_PTRS) + 1) * ALLOC_GRANULARITY_PTRS); }
}

void CArray::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		rStream.Next(*m_ppValues[i]);
	}
}

void CArray::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		rStream.Next();
		m_ppValues[i] = (CValue*)rStream.Current();
		ADDREF(*(m_ppValues[i]));
	}
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
		if (m_ppValues) { m_ppValues = (CValue**)HeapReAlloc(Memory_hHeap, 0, m_ppValues, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else            { m_ppValues = (CValue**)HeapAlloc  (Memory_hHeap, 0,             sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppValues[m_uiCount] = (CValue*)&rValue;
	m_uiCount++;
	ADDREF(rValue);
}