#include "Stack.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

extern HANDLE Memory_hHeap;

CStack::CStack()
{
	m_uiCount   = 0;
	m_ppObjects = NULL;
}

CStack::~CStack()
{
	BETAONLY(for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

	if (m_ppObjects) { HeapFree(Memory_hHeap, 0, m_ppObjects); }
}

void CStack::Push(_IN _REF CObject& rObject)
{
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppObjects) { m_ppObjects = (CObject**)HeapReAlloc(Memory_hHeap, 0, m_ppObjects, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else             { m_ppObjects = (CObject**)HeapAlloc  (Memory_hHeap, 0,              sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppObjects[m_uiCount] = (CObject*)&rObject;
	ADDREF(*(m_ppObjects[m_uiCount]));
	m_uiCount++;
}

void CStack::Pop()
{
	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
}

void CStack::PopAndDelete()
{
	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
	delete m_ppObjects[m_uiCount];
}

void CStack::AllRemove()
{
	BETAONLY(for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

		m_uiCount = 0;
	if (m_ppObjects) { HeapFree(Memory_hHeap, 0, m_ppObjects); m_ppObjects = NULL; }
}

void CStack::AllDelete()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { RELEASE(*(m_ppObjects[i])); delete m_ppObjects[i]; }

	m_uiCount = 0;
	if (m_ppObjects) { HeapFree(Memory_hHeap, 0, m_ppObjects); m_ppObjects = NULL; }
}

void CStack::AllDispose()
{
	for (SCF::UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Dispose(); }
}