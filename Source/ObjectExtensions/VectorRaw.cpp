#include "VectorRaw.h"

#include <malloc.h>

using namespace SCFBase;

CVectorRaw::CVectorRaw()
{
	m_ppObjects = NULL;
	m_uiCount = 0;
}

CVectorRaw::~CVectorRaw()
{
	this->AllRemove();
}

void CVectorRaw::AtPut(_IN UINT uiIndex, _IN _REF CObject& rObject) _SET
{
	_ASSERTE(uiIndex < m_uiCount);

	//Release the previously stored object, so it can be deleted
	BETAONLY(if (m_ppObjects[uiIndex]) { m_ppObjects[uiIndex]->Release(); })

	m_ppObjects[uiIndex] = (CObject*)&rObject;
	ADDREF(rObject);
}

void CVectorRaw::LastAdd(_IN _REF CObject& rObject)
{
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppObjects) { m_ppObjects = (CObject**)realloc(m_ppObjects, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else             { m_ppObjects = (CObject**)malloc (             sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	m_ppObjects[m_uiCount] = (CObject*)&rObject;
	m_uiCount++;
	ADDREF(rObject);
}

void CVectorRaw::LastRemove()
{
	_ASSERTE(m_uiCount > 0);

	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
}

void CVectorRaw::LastDelete()
{
	_ASSERTE(m_uiCount > 0);

	m_uiCount--;
	RELEASE(*(m_ppObjects[m_uiCount]));
	delete m_ppObjects[m_uiCount];
}

void CVectorRaw::RemoveAt(_IN UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	RELEASE(*(m_ppObjects[uiIndex]));

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CVectorRaw::Swap(_IN UINT uiIndex1, _IN UINT uiIndex2)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex1 < m_uiCount);
	_ASSERTE(uiIndex2 < m_uiCount);

	register CObject* pTmp = m_ppObjects[uiIndex1];
	m_ppObjects[uiIndex1]  = m_ppObjects[uiIndex2];
	m_ppObjects[uiIndex2]  = pTmp;
}

void CVectorRaw::Insert(_IN UINT uiIndex, _IN CObject& rObject)
{
	_ASSERTE(uiIndex <= m_uiCount);

	//Reserve more memory if necessary
	if ((m_uiCount % ALLOC_GRANULARITY_PTRS) == 0)
	{
		if (m_ppObjects) { m_ppObjects = (CObject**)realloc(m_ppObjects, sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
		else             { m_ppObjects = (CObject**)malloc (             sizeof(CObject*) * (m_uiCount + ALLOC_GRANULARITY_PTRS)); }
	}

	//Move all objects after the inserted up by one (WAS WRONG in 1st version XD)
	for (UINT i = m_uiCount; i > uiIndex; i--) { m_ppObjects[i] = m_ppObjects[i - 1]; }

	//Now store the object on the specified location
	m_ppObjects[uiIndex] = (CObject*)&rObject;
	m_uiCount++;
	ADDREF(rObject);
}

void CVectorRaw::DeleteAt(_IN UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	RELEASE(*(m_ppObjects[uiIndex]));
	delete m_ppObjects[uiIndex];

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CVectorRaw::Remove(_IN CObject& rObject)
{
	_ASSERTE(m_uiCount > 0);

	UINT uiIndex = (UINT)-1;
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { uiIndex = i; break; } }

	if (uiIndex >= m_uiCount) { return; }

	RELEASE(*(m_ppObjects[uiIndex]));

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CVectorRaw::Delete(_IN CObject& rObject)
{
	_ASSERTE(m_uiCount > 0);

	UINT uiIndex = (UINT)-1;
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { uiIndex = i; break; } }

	if (uiIndex >= m_uiCount) { return; }

	RELEASE(*(m_ppObjects[uiIndex]));
	delete m_ppObjects[uiIndex];

	m_uiCount--;
	for (UINT i = uiIndex; i < m_uiCount; i++) { m_ppObjects[i] = m_ppObjects[i + 1]; }
}

void CVectorRaw::AllRemove()
{
	BETAONLY(for (UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Release(); })

	m_uiCount = 0;
	if (m_ppObjects) { free(m_ppObjects); m_ppObjects = NULL; }
}

void CVectorRaw::AllDelete()
{
	for (UINT i = 0; i < m_uiCount; i++) { RELEASE(*(m_ppObjects[i])); delete m_ppObjects[i]; }

	m_uiCount = 0;
	if (m_ppObjects) { free(m_ppObjects); m_ppObjects = NULL; }
}

void CVectorRaw::AllDispose()
{
	for (UINT i = 0; i < m_uiCount; i++) { m_ppObjects[i]->Dispose(); }
}

int CVectorRaw::Find(_IN CObject& rObject) _GET 
{
	for (UINT i = 0; i < m_uiCount; i++) { if (m_ppObjects[i] == &rObject) { return i; } }

	return -1;
}

int CVectorRaw::FindEqual(_IN CObject& rObject, _IN CComparer& rComparer) _GET  
{
	for (UINT i = 0; i < m_uiCount; i++) { if (rComparer.Equal(*(m_ppObjects[i]), rObject)) { return i; } }

	return -1;
}

bool CVectorRaw::SortBubble(_IN CComparer& rComparer)
{
	register CObject* pTmp = NULL;

	for (UINT i = 0; i < m_uiCount; i++)
	{
		bool bSwapped = FALSE;

		for (UINT j = m_uiCount - 1; j > i; j--)
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
