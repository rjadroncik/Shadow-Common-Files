#include "DictionaryObjectRaw.h"
#include "EnumeratorDictionaryObjectRaw.h"
#include "ComparerIdentity.h"

using namespace SCFBase;
using namespace SCFPrivate;

const CComparer* DictionaryObject_pComparer = nullptr;

CDictionaryObjectRaw::CDictionaryObjectRaw()
{
	if (!DictionaryObject_pComparer) { DictionaryObject_pComparer = new CComparerIdentity(); }

	m_pComparer = DictionaryObject_pComparer;
	m_bComparerOwned = false;

	ADDREF(*m_pComparer);

	m_pNodeRoot = nullptr;
	m_uiCount = 0;
}

CDictionaryObjectRaw::CDictionaryObjectRaw(_IN CComparer& rComparer, _IN bool bTakeOwnage)
{
	if (!DictionaryObject_pComparer) { DictionaryObject_pComparer = new CComparerIdentity(); }

	m_pComparer = &rComparer;

	ADDREF(*m_pComparer);
	m_bComparerOwned = bTakeOwnage;

	m_pNodeRoot = nullptr;
	m_uiCount = 0;
}

CDictionaryObjectRaw::~CDictionaryObjectRaw()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeObject::Delete(m_pNodeRoot);
	}

	RELEASE(*m_pComparer);
	if (m_bComparerOwned) { delete m_pComparer; }
}

bool CDictionaryObjectRaw::Contains(_IN CObject& rObject) _GET
{
	if (!m_pNodeRoot) { return false; }

	CEnumeratorDictionaryObjectRaw Enumerator(*this);
	while (Enumerator.ProtectedNext()) 
	{
		if (Enumerator.ProtectedCurrent() == &rObject) { return true; }
	}

	return false;
}

CObject* CDictionaryObjectRaw::KeyOf(_IN CObject& rObject) _GET
{
	if (!m_pNodeRoot) { return (CObject*)-1; }

	CEnumeratorDictionaryObjectRaw Enumerator(*this);
	while (Enumerator.ProtectedNext())
	{
		if (Enumerator.ProtectedCurrent() == &rObject) { return Enumerator.CurrentKey(); }
	}

	return (CObject*)-1;
}

CObject* CDictionaryObjectRaw::At(_IN CObject& rKey) _GET
{
	register CDictionaryNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (m_pComparer->Equal(rKey, pNodeCurrent->Key())) { return pNodeCurrent->Object(); }

		if (!m_pComparer->LessOrEqual(rKey, pNodeCurrent->Key())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                      { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (rKey == pNodeCurrent->Key()) { return pNodeCurrent->Object(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return nullptr;
}

CObject* CDictionaryObjectRaw::Remove(_IN CObject& rKey)
{
	register CDictionaryNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		//if (rKey == pNodeCurrent->Key()) 
		if (m_pComparer->Equal(rKey, pNodeCurrent->Key()))
		{
			CObject* pObject = pNodeCurrent->Object();

			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (nullptr);
			pNodeCurrent->ChildRight(nullptr);

			CDictionaryNodeObject::Delete(pNodeCurrent);
			m_uiCount--;

			return pObject; 
		}

		if (!m_pComparer->LessOrEqual(rKey, pNodeCurrent->Key())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                      { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return nullptr;
}

CObject* CDictionaryObjectRaw::RemoveAndDeleteKey(_IN CObject& rKey)
{
	register CDictionaryNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		//if (rKey == pNodeCurrent->Key()) 
		if (m_pComparer->Equal(rKey, pNodeCurrent->Key()))
		{
			CObject* pObject = pNodeCurrent->Object();

			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (nullptr);
			pNodeCurrent->ChildRight(nullptr);

			CDictionaryNodeObject::DeleteWithKey(pNodeCurrent);
			m_uiCount--;

			return pObject; 
		}

		if (!m_pComparer->LessOrEqual(rKey, pNodeCurrent->Key())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                      { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return nullptr;
}


CObject* CDictionaryObjectRaw::AtPut(_IN _REF CObject& rKey, _IN _REF CObject& rObject) _SET
{
	//If there are no nodes yet..
	if (!m_pNodeRoot)
	{
		//Create the first node
		m_pNodeRoot = CDictionaryNodeObject::Create(rKey, rObject);
		m_uiCount++;
		return nullptr;
	}

	register CDictionaryNodeObject* pNodeCurrent = m_pNodeRoot;

	for (;;)
	{
		//if (pKey == pNodeCurrent->Key()) { CObject* pPrevious = &pNodeCurrent->Object(); pNodeCurrent->Object(rObject); return pPrevious; }
		if (m_pComparer->Equal(rKey, pNodeCurrent->Key())) { CObject* pPrevious = pNodeCurrent->Object(); pNodeCurrent->Object(&rObject); if (!pPrevious) { m_uiCount++; } return pPrevious; }

		//if (pKey > pNodeCurrent->Key()) 
		if (!m_pComparer->LessOrEqual(rKey, pNodeCurrent->Key())) 
		{ 
			if (!pNodeCurrent->ChildRight())
			{
				register CDictionaryNodeObject* pNodeNew = CDictionaryNodeObject::Create(rKey, rObject);
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
				register CDictionaryNodeObject* pNodeNew = CDictionaryNodeObject::Create(rKey, rObject);
				m_uiCount++;

				pNodeCurrent->ChildLeft(pNodeNew);
				pNodeNew->Parent(pNodeCurrent);

				pNodeNew->Rebalance();
				break; 
			} 
			else { pNodeCurrent = pNodeCurrent->ChildLeft(); }
		}
	}

	return nullptr;
}

void CDictionaryObjectRaw::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CDictionaryNodeObject::Delete(m_pNodeRoot);

		m_pNodeRoot = nullptr; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryObject_Heap.AllocatedBytes() == 0);
}

void CDictionaryObjectRaw::AllDelete()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeObject::DeleteWithObject(m_pNodeRoot);

		m_pNodeRoot = nullptr; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryObject_Heap.AllocatedBytes() == 0);
}

void CDictionaryObjectRaw::AllDeleteWithKeys()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeObject::DeleteWithObjectAndKey(m_pNodeRoot);

		m_pNodeRoot = nullptr; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryObject_Heap.AllocatedBytes() == 0);
}

void CDictionaryObjectRaw::AllDeleteOnlyKeys()
{
	if (m_pNodeRoot) 
	{
		CDictionaryNodeObject::DeleteWithKey(m_pNodeRoot);

		m_pNodeRoot = nullptr; 
		m_uiCount = 0;
	}

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryObject_Heap.AllocatedBytes() == 0);
}

void CDictionaryObjectRaw::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorDictionaryObjectRaw Enumerator(*this);
	while (Enumerator.ProtectedNext()) { Enumerator.ProtectedCurrent()->Dispose(); }
}

void CDictionaryObjectRaw::AllDisposeWithKeys()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorDictionaryObjectRaw Enumerator(*this);
	while (Enumerator.ProtectedNext()) { Enumerator.ProtectedCurrent()->Dispose(); Enumerator.CurrentKey()->Dispose(); }
}

void CDictionaryObjectRaw::AllDisposeOnlyKeys()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorDictionaryObjectRaw Enumerator(*this);
	while (Enumerator.ProtectedNext()) { Enumerator.CurrentKey()->Dispose(); }
}


CString CDictionaryObjectRaw::ToString() _GET { return STRING("{DictionaryObject}"); }
