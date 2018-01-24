#include "EnumeratorSetObject.h"
#include "SetNodeObject.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorSetObject::CEnumeratorSetObject(_IN CSetObject& rBag) : CEnumeratorRaw(rBag)
{
	m_pNode = nullptr;
	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextStart;

	m_bHasNext = rBag.Size() > 0;
}

CEnumeratorSetObject::~CEnumeratorSetObject()
{
}

bool CEnumeratorSetObject::NextStart()
{
	if (((CSetObject*)m_pSource)->m_pNodeRoot)
	{
		m_pNode = ((CSetObject*)m_pSource)->m_pNodeRoot->LeafSmallest();
		m_pCurrent = &(m_pNode->Object()); 

		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextContinue;
		return true;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextEnd; m_bHasNext = false; 
		return false;
	}
}

bool CEnumeratorSetObject::NextContinue()
{
	if (m_pNode->ChildRight())
	{
		m_pNode = m_pNode->ChildRight()->LeafSmallest();

		if (m_pNode) { m_pCurrent = &(m_pNode->Object()); return true; }
		else         { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextEnd; m_bHasNext = false; return false; }
	}
	else
	{
		if (!m_pNode->Parent()) { (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextEnd; m_bHasNext = false; return false; }

		//If the current node is the left child of the parent
		if (m_pNode->Parent()->ChildLeft() == m_pNode)
		{
			m_pNode = m_pNode->Parent();
		}
		//Current node is the right child
		else
		{
			do { m_pNode = m_pNode->Parent(); }
			while (m_pNode->Parent() && (m_pNode->Parent()->ChildRight() == m_pNode));

			if (!m_pNode || !m_pNode->Parent()) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetObject::NextEnd; m_bHasNext = false; return false; }
			else                                { m_pNode = m_pNode->Parent(); }
		}
	}

	m_pCurrent = &(m_pNode->Object()); 
	return true;
}


//CObject* CEnumeratorSetObject::Current() _GET 
//{ 
//	if (m_pNode) { return &m_pNode->Object(); }
//	else         { return nullptr; }
//}

//bool CEnumeratorSetObject::Next()
//{
//	if (!m_pBag->m_pNodeRoot) { return false; }
//
//	//Handle starting 
//	if (!m_pNode)
//	{
//		m_pNode = m_pBag->m_pNodeRoot->LeafSmallest();
//
//		if (m_pNode) { return true; }
//		else         { return false; }
//	}
//
//	if (m_pNode->ChildRight())
//	{
//		m_pNode = m_pNode->ChildRight()->LeafSmallest();
//
//		if (m_pNode) { return true; }
//		else         { return false; }
//	}
//	else
//	{
//		if (!m_pNode->Parent()) { return false; }
//
//		//If the current node is the left child of the parent
//		if (m_pNode->Parent()->ChildLeft() == m_pNode)
//		{
//			m_pNode = m_pNode->Parent();
//		}
//		//Current node is the right child
//		else
//		{
//			do
//			{
//				m_pNode = m_pNode->Parent();
//			}
//			while (m_pNode->Parent() && (m_pNode->Parent()->ChildRight() == m_pNode));
//
//			if (!m_pNode || !m_pNode->Parent()) { return false; }
//			else                                { m_pNode = m_pNode->Parent(); }
//		}
//	}
//
//	return true;
//}