#include "SetObject.h"
#include "EnumeratorSetObject.h"
#include "ComparerIdentity.h"

using namespace SCFBase;
using namespace SCFPrivate;

const CComparer* BagObject_pComparer = NULL;

CSetObject::CSetObject()
{
	if (!BagObject_pComparer) { BagObject_pComparer = new CComparerIdentity(); }

	m_pComparer = BagObject_pComparer;
	m_bComparerOwned = FALSE;

	ADDREF(*m_pComparer);

	m_pNodeRoot = NULL;
	m_uiCount = 0;
}

CSetObject::CSetObject(_IN CComparer& rComparer, _IN bool bTakeOwnage)
{
	if (!BagObject_pComparer) { BagObject_pComparer = new CComparerIdentity(); }

	m_pComparer = &rComparer;

	ADDREF(*m_pComparer);
	m_bComparerOwned = bTakeOwnage;

	m_pNodeRoot = NULL;
	m_uiCount = 0;
}

CSetObject::~CSetObject()
{
	if (m_pNodeRoot) 
	{
		CSetNodeObject::Delete(m_pNodeRoot);
	}

	RELEASE(*m_pComparer);
	if (m_bComparerOwned) { delete m_pComparer; }
}

bool CSetObject::Contains(_IN CObject& rObject) _GET
{
	register CSetNodeObject* pNodeCurrent = m_pNodeRoot;

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

void CSetObject::Remove(_IN CObject& rObject)
{
	register CSetNodeObject* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		//if (rKey == pNodeCurrent->Key()) 
		if (m_pComparer->Equal(rObject, pNodeCurrent->Object()))
		{
			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (NULL);
			pNodeCurrent->ChildRight(NULL);

			CSetNodeObject::Delete(pNodeCurrent);
			m_uiCount--;
	
			return; 
		}

		if (!m_pComparer->LessOrEqual(rObject, pNodeCurrent->Object())) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }

		//if (pKey > pNodeCurrent->Key()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		//else                            { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}
}

void CSetObject::Add(_IN _REF CObject& rObject) _SET
{
	//If there are no nodes yet..
	if (!m_pNodeRoot)
	{
		//Create the first node
		m_pNodeRoot = CSetNodeObject::Create(rObject);
		m_uiCount++;
		return;
	}

	register CSetNodeObject* pNodeCurrent = m_pNodeRoot;

	for (;;)
	{
		//if (pKey == pNodeCurrent->Key()) { CObject* pPrevious = &pNodeCurrent->Object(); pNodeCurrent->Object(rObject); return pPrevious; }
		if (m_pComparer->Equal(rObject, pNodeCurrent->Object())) { return; }

		//if (pKey > pNodeCurrent->Key()) 
		if (!m_pComparer->LessOrEqual(rObject, pNodeCurrent->Object())) 
		{ 
			if (!pNodeCurrent->ChildRight())
			{
				register CSetNodeObject* pNodeNew = CSetNodeObject::Create(rObject);
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
				register CSetNodeObject* pNodeNew = CSetNodeObject::Create(rObject);
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

void CSetObject::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CSetNodeObject::Delete(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}
}

void CSetObject::AllDelete()
{
	if (m_pNodeRoot) 
	{
		CSetNodeObject::DeleteWithObject(m_pNodeRoot);

		m_pNodeRoot = NULL; 
		m_uiCount = 0;
	}
}

void CSetObject::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorSetObject Enumerator(*this);
	while (Enumerator.Next()) { Enumerator.Current()->Dispose(); }
}

//CString CSetObject::ToString() _GET { return STRING("{BagObject}"); }
