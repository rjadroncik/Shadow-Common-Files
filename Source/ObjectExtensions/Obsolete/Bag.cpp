#include "StdAfx.h"

#include "Bag.h"
//#include "EnumeratorBag.h"

using namespace SCFObjectExtensions;
using namespace SCFPrivate;

CBag::CBag()
{
	m_pNodeRoot = NULL;

	m_Heap.BlockSize(sizeof(CBagNode));
}

CBag::~CBag()
{
	if (m_pNodeRoot) { CBagNode::Delete(m_Heap, m_pNodeRoot); }
}

void* CBag::AsType(_IN ENUM eType) _GET
{
	switch (eType)
	{
	case ClassBag: { return (CBag*)this; }
	}

	return NULL;
}

bool CBag::Contains(_IN CObject& rObject) _GET
{
	register CBagNode* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (&rObject == pNodeCurrent->Object()) { return TRUE; }

		if (&rObject > pNodeCurrent->Object()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                   { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}

	return FALSE;
}

void CBag::Remove(_IN CObject& rObject)
{
	register CBagNode* pNodeCurrent = m_pNodeRoot;

	while (pNodeCurrent)
	{
		if (&rObject == pNodeCurrent->Object()) 
		{
			if (pNodeCurrent == m_pNodeRoot) { m_pNodeRoot = pNodeCurrent->RemoveRoot(); }
			else                             { pNodeCurrent->Remove(); }

			pNodeCurrent->ChildLeft (NULL);
			pNodeCurrent->ChildRight(NULL);

			CBagNode::Delete(m_Heap, pNodeCurrent);
			return; 
		}

		if (&rObject > pNodeCurrent->Object()) { pNodeCurrent = pNodeCurrent->ChildRight(); }
		else                                   { pNodeCurrent = pNodeCurrent->ChildLeft(); }
	}
}

void CBag::Add(_IN _REF CObject& rObject)
{
	//If there are no nodes yet, create the first node
	if (!m_pNodeRoot)
	{
		m_pNodeRoot = CBagNode::Create(m_Heap, rObject);
		return;
	}

	register CBagNode* pNodeCurrent = m_pNodeRoot;

	for (;;)
	{
		//If the bag already contains this object
		if (&rObject == pNodeCurrent->Object()) { return; }

		if (&rObject > pNodeCurrent->Object()) 
		{ 
			if (!pNodeCurrent->ChildRight())
			{
				register CBagNode* pNodeNew = CBagNode::Create(m_Heap, rObject);
				
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
				register CBagNode* pNodeNew = CBagNode::Create(m_Heap, rObject);

				pNodeCurrent->ChildLeft(pNodeNew);
				pNodeNew->Parent(pNodeCurrent);

				pNodeNew->Rebalance();
				break; 
			} 
			else { pNodeCurrent = pNodeCurrent->ChildLeft(); }
		}
	}
}

void CBag::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CBagNode::Delete(m_Heap, m_pNodeRoot);
		m_pNodeRoot = NULL; 
	}
}

void CBag::AllDelete()
{
	if (!m_pNodeRoot) { return; }

	//CEnumeratorDictionaryInt64 Enumerator(*this);
	//while (Enumerator.Next()) { Enumerator.CurrentDelete(); }

	this->AllRemove();
}

void CBag::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	//CEnumeratorDictionaryInt64 Enumerator(*this);
	//while (Enumerator.Next()) { Enumerator.Current()->Dispose(); }
}