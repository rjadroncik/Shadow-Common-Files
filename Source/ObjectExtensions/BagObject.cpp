#include "BagObject.h"
#include "EnumeratorBagObject.h"
#include "ComparerIdentity.h"

using namespace SCFBase;
using namespace SCFPrivate;

const CComparer* BagObject_pComparer = NULL;

CBagObject::CBagObject()
{
	if (!BagObject_pComparer) { BagObject_pComparer = new CComparerIdentity(); }

	m_pComparer = BagObject_pComparer;
	m_bComparerOwned = FALSE;

	ADDREF(*m_pComparer);

	m_pNodeRoot = NULL;
	m_uiCount = 0;
}

CBagObject::CBagObject(_IN CComparer& rComparer, _IN bool bTakeOwnage)
{
	if (!BagObject_pComparer) { BagObject_pComparer = new CComparerIdentity(); }

	m_pComparer = &rComparer;

	ADDREF(*m_pComparer);
	m_bComparerOwned = bTakeOwnage;

	m_pNodeRoot = NULL;
	m_uiCount = 0;
}

CBagObject::~CBagObject()
{
	if (m_pNodeRoot) 
	{
		CBagNodeObject::Delete(m_pNodeRoot);
	}

	RELEASE(*m_pComparer);
	if (m_bComparerOwned) { delete m_pComparer; }
}

bool CBagObject::Contains(_IN CObject& rObject) _GET
{
	register CBagNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (m_pComparer->Equal(rObject, pNodeCurrent->Object())) { return TRUE; }

		if (!m_pComparer->LessOrEqual(rObject, pNodeCurrent->Object())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (rKey == pNodeCurrent->Key()) { return pNodeCurrent->Object(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return FALSE;
}

void CBagObject::Remove(_IN CObject& rObject)
{
	register CBagNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		//if (rKey == pNodeCurrent->Key()) 
		if (m_pComparer->Equal(rObject, pNodeCurrent->Object()))
		{
			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (NULL);
			pNodeCurrent->ChildRight(NULL);

			CBagNodeObject::Delete(pNodeCurrent);
			m_uiCount--;
	
			return; 
		}

		if (!m_pComparer->LessOrEqual(rObject, pNodeCurrent->Object())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}
}

void CBagObject::Add(_IN _REF CObject& rObject) _SET
{
	//If there are no nodes yet..
	if (!m_pNodeRoot)
	{
		//Create the first node
		m_pNodeRoot = CBagNodeObject::Create(rObject);
		m_uiCount++;
		return;
	}

	register CBagNodeObject* pNodeCurrent = m_pNodeRoot;

	for (;;)
	{
		//if (pKey == pNodeCurrent->Key()) { CObject* pPrevious = &pNodeCurrent->Object(); pNodeCurrent->Object(rObject); return pPrevious; }
		if (m_pComparer->Equal(rObject, pNodeCurrent->Object())) { return; }

		//if (pKey > pNodeCurrent->Key()) 
		if (!m_pComparer->LessOrEqual(rObject, pNodeCurrent->Object())) 
		{ 
			if (!pNodeCurrent->ChildRight())
			{
				register CBagNodeObject* pNodeNew = CBagNodeObject::Create(rObject);
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
				register CBagNodeObject* pNodeNew = CBagNodeObject::Create(rObject);
				m_uiCount++;

				pNodeCurrent->ChildLeft(pNodeNew);
				pNodeNew->Parent(pNodeCurrent);

				pNodeNew->Rebalance();
				break; 
			} 
			else { pNodeCurrent = pNodeCurrent->ChildLeft(); }
		}
	}
}

void CBagObject::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CBagNodeObject::Delete(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}
}

void CBagObject::AllDelete()
{
	if (m_pNodeRoot) 
	{
		CBagNodeObject::DeleteWithObject(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}
}

void CBagObject::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorBagObject Enumerator(*this);
	while (Enumerator.Next()) { Enumerator.Current()->Dispose(); }
}

//CString CBagObject::ToString() _GET { return STRING("{BagObject}"); }
