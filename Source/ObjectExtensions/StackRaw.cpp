#include "StackRaw.h"

#include <malloc.h>

using namespace SCFBase;

CStackRaw::CStackRaw()
{
	m_uiCount   = 0;
	m_ppObjects = NULL;
}

CStackRaw::~CStackRaw()
{
	BETAONLY(for (UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

	if (m_ppObjects) { free(m_ppObjects); }
}

void CStackRaw::Push(_IN _REF CObject& rObject)
{
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppObjects) { m_ppObjects = (CObject**)realloc(m_ppObjects, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else             { m_ppObjects = (CObject**)malloc (             sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppObjects[m_uiCount] = (CObject*)&rObject;
	ADDREF(*(m_ppObjects[m_uiCount]));
	m_uiCount++;
}

void CStackRaw::Pop()
{
	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
}

void CStackRaw::PopAndDelete()
{
	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
	delete m_ppObjects[m_uiCount];
}

void CStackRaw::AllRemove()
{
	BETAONLY(for (UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

		m_uiCount = 0;
	if (m_ppObjects) { free(m_ppObjects); m_ppObjects = NULL; }
}

void CStackRaw::AllDelete()
{
	for (UINT i = 0; i < m_uiCount; i++) { RELEASE(*(m_ppObjects[i])); delete m_ppObjects[i]; }

	m_uiCount = 0;
	if (m_ppObjects) { free(m_ppObjects); m_ppObjects = NULL; }
}

void CStackRaw::AllDispose()
{
	for (UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Dispose(); }
}