#include "List.h"

#include "StreamReadObject.h"
#include "StreamWriteObject.h"

#include <SCFStandardUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

extern HANDLE Memory_hHeap;

CList::CList()
{
	m_ppObjects = NULL;
	m_uiCount = 0;
}

CList::~CList()
{
	this->AllRemove();
}

CObject& CList::At(_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	return *m_ppObjects[uiIndex]; 
}

CObject& CList::operator [](_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	return *m_ppObjects[uiIndex];
}

void CList::AtPut(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	//Release the previously stored object, so it can be deleted
	BETAONLY(if (m_ppObjects[uiIndex]) { m_ppObjects[uiIndex]->Release(); })

	m_ppObjects[uiIndex] = (CObject*)&rObject;
	ADDREF(rObject);
}

CObject& CList::Last() _GET
{
	_ASSERTE(m_uiCount > 0);

	return *m_ppObjects[m_uiCount - 1]; 
}

void CList::LastAdd(_IN _REF CObject& rObject)
{
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppObjects) { m_ppObjects = (CObject**)HeapReAlloc(Memory_hHeap, 0, m_ppObjects, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else             { m_ppObjects = (CObject**)HeapAlloc  (Memory_hHeap, 0,              sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppObjects[m_uiCount] = (CObject*)&rObject;
	m_uiCount++;
	ADDREF(rObject);
}

void CList::LastRemove()
{
	_ASSERTE(m_uiCount > 0);

	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
}

void CList::Remove(_IN SCF::UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	RELEASE(*(m_ppObjects[uiIndex]));

	m_uiCount--;
	for (SCF::UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CList::Swap(_IN SCF::UINT uiIndex1, _IN SCF::UINT uiIndex2)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex1 < m_uiCount);
	_ASSERTE(uiIndex2 < m_uiCount);

	register CObject* pTmp = m_ppObjects[uiIndex1];

	m_ppObjects[uiIndex1] = m_ppObjects[uiIndex2];

	m_ppObjects[uiIndex2] = pTmp;
}

void CList::Delete(_IN SCF::UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	RELEASE(*(m_ppObjects[uiIndex]));
	delete m_ppObjects[uiIndex];

	m_uiCount--;
	for (SCF::UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CList::Remove(_IN CObject& rObject)
{
	_ASSERTE(m_uiCount > 0);

	SCF::UINT uiIndex = (SCF::UINT)-1;
	for (SCF::UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { uiIndex = i; break; } }

	if (uiIndex >= m_uiCount) { return; }

	RELEASE(*(m_ppObjects[uiIndex]));

	m_uiCount--;
	for (SCF::UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CList::Delete(_IN CObject& rObject)
{
	_ASSERTE(m_uiCount > 0);

	SCF::UINT uiIndex = (SCF::UINT)-1;
	for (SCF::UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { uiIndex = i; break; } }

	if (uiIndex >= m_uiCount) { return; }

	RELEASE(*(m_ppObjects[uiIndex]));
	delete m_ppObjects[uiIndex];

	m_uiCount--;
	for (SCF::UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CList::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiCount);
}

void CList::Deserialize(_INOUT IStreamRead& rStream)
{
	m_uiCount = rStream.GetInt();

	if (m_ppObjects) { m_ppObjects = (CObject**)HeapReAlloc(Memory_hHeap, 0, m_ppObjects, sizeof(CObject*) * ((m_uiCount / ALLOC_GRANULARITY_PTRS) + 1) * ALLOC_GRANULARITY_PTRS); }
	else             { m_ppObjects = (CObject**)HeapAlloc  (Memory_hHeap, 0,              sizeof(CObject*) * ((m_uiCount / ALLOC_GRANULARITY_PTRS) + 1) * ALLOC_GRANULARITY_PTRS); }
}

void CList::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		rStream.Next((CObjectSerializable&)*m_ppObjects[i]);
	}
}

void CList::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		rStream.Next();
		m_ppObjects[i] = rStream.Current();
		ADDREF(*(m_ppObjects[i]));	
	}
}

void CList::AllRemove()
{
	BETAONLY(for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

	m_uiCount = 0;
	if (m_ppObjects) { HeapFree(Memory_hHeap, 0, m_ppObjects); m_ppObjects = NULL; }
}

void CList::AllDelete()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { RELEASE(*(m_ppObjects[i])); delete m_ppObjects[i]; }

	m_uiCount = 0;
	if (m_ppObjects) { HeapFree(Memory_hHeap, 0, m_ppObjects); m_ppObjects = NULL; }
}

void CList::AllDispose()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Dispose(); }
}

int CList::Find(_IN CObject& rObject) _GET 
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { return i; } }

	return -1;
}

int CList::FindEqual(_IN CObject& rObject) _GET  
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i]->IsEqualTo(rObject) || rObject.IsEqualTo(*m_ppObjects[i])) { return i; } }

	return -1;
}

bool CList::SortBubble(_IN CComparer& rComparer)
{
	register CObject* pTmp = NULL;

	for (SCF::UINT i = 0; i < m_uiCount; i++)
	{
		bool bSwapped = FALSE;

		for (SCF::UINT j = m_uiCount - 1; j > i; j--)
		{
			if (!rComparer.LessOrEqual(*m_ppObjects[j - 1], *m_ppObjects[j]))
			{
				pTmp = m_ppObjects[j - 1];
				m_ppObjects[j - 1] = m_ppObjects[j];
				m_ppObjects[j] = pTmp;
				
				bSwapped = TRUE;
			}
		}

		if (!bSwapped) { return TRUE; }
	}

	return TRUE;
}
