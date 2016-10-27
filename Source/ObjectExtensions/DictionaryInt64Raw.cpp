#include "DictionaryInt64Raw.h"
#include "EnumeratorDictionaryInt64.h"

using namespace SCFBase;
using namespace SCFPrivate;

CDictionaryInt64Raw::CDictionaryInt64Raw()
{
	m_pNodeRoot = NULL;
	m_uiCount = 0;
}

CDictionaryInt64Raw::~CDictionaryInt64Raw()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeInt64::Delete(m_pNodeRoot);
	}
}

bool CDictionaryInt64Raw::Contains(_IN CObject& rObject) _GET
{
	if (!m_pNodeRoot) { return FALSE; }

	CEnumeratorDictionaryInt64Raw Enumerator(*this);
	while (Enumerator.ProtectedNext()) 
	{
		if (Enumerator.ProtectedCurrent() == &rObject) { return TRUE; }
	}

	return FALSE;
}

UINT64 CDictionaryInt64Raw::KeyOf(_IN CObject& rObject) _GET
{
	if (!m_pNodeRoot) { return (UINT64)-1; }

	CEnumeratorDictionaryInt64Raw Enumerator(*this);
	while (Enumerator.ProtectedNext())
	{
		if (Enumerator.ProtectedCurrent() == &rObject) { return Enumerator.CurrentKey(); }
	}

	return (UINT64)-1;
}

CObject* CDictionaryInt64Raw::At(_IN UINT64 ui64Key) _GET
{
	register CDictionaryNodeInt64* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (ui64Key == pNodeCurrent->Key()) { return pNodeCurrent->Object(); }

		if (ui64Key > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                               { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return NULL;
}

CObject* CDictionaryInt64Raw::Remove(_IN UINT64 ui64Key)
{
	register CDictionaryNodeInt64* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (ui64Key == pNodeCurrent->Key()) 
		{
			CObject* pObject = pNodeCurrent->Object();

			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (NULL);
			pNodeCurrent->ChildRight(NULL);

			CDictionaryNodeInt64::Delete(pNodeCurrent);
			m_uiCount--;

			return pObject; 
		}

		if (ui64Key > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                               { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return NULL;
}

CObject* CDictionaryInt64Raw::AtPut(_IN UINT64 ui64Key, _IN _REF CObject& rObject) _SET
{
	//If there are no nodes yet..
	if (!m_pNodeRoot)
	{
		//Create the first node
		m_pNodeRoot = CDictionaryNodeInt64::Create(ui64Key, rObject);
		m_uiCount++;

		return NULL;
	}

	register CDictionaryNodeInt64* pNodeCurrent = m_pNodeRoot;

	for (;;)
	{
		if (ui64Key == pNodeCurrent->Key()) { CObject* pPrevious = pNodeCurrent->Object(); pNodeCurrent->Object(&rObject); if (!pPrevious) { m_uiCount++; } return pPrevious; }

		if (ui64Key > pNodeCurrent->Key()) 
		{ 
			if (!pNodeCurrent->ChildRight())
			{
				register CDictionaryNodeInt64* pNodeNew = CDictionaryNodeInt64::Create(ui64Key, rObject);
				m_uiCount++;

				pNodeCurrent->ChildRight(pNodeNew);
				pNodeNew->Parent(pNodeCurrent);

				pNodeNew->Rebalance();
				break; 
			} 
			else { pNodeCurrent = pNodeCurrent->ChildRight(); }
		}
		else                              
		{ 
			if (!pNodeCurrent->ChildLeft())
			{ 
				register CDictionaryNodeInt64* pNodeNew = CDictionaryNodeInt64::Create(ui64Key, rObject);
				m_uiCount++;
				 
				pNodeCurrent->ChildLeft(pNodeNew);
				pNodeNew->Parent(pNodeCurrent);

				pNodeNew->Rebalance();
				break; 
			} 
			else { pNodeCurrent = pNodeCurrent->ChildLeft(); }
		}
	}

	return NULL;
}

void CDictionaryInt64Raw::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CDictionaryNodeInt64::Delete(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryInt64_Heap.AllocatedBytes() == 0);
}

void CDictionaryInt64Raw::AllDelete()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeInt64::DeleteWithObject(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryInt64_Heap.AllocatedBytes() == 0);
}

void CDictionaryInt64Raw::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorDictionaryInt64Raw Enumerator(*this);
	while (Enumerator.ProtectedNext()) { Enumerator.ProtectedCurrent()->Dispose(); }
}

CString CDictionaryInt64Raw::ToString() _GET { return STRING("{DictionaryInt64}"); }
