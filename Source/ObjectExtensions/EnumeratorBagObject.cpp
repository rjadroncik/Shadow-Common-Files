#include "EnumeratorBagObject.h"
#include "BagNodeObject.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorBagObject::CEnumeratorBagObject(_IN CBagObject& rBag) : CEnumeratorRaw(rBag)
{
	m_pNode = NULL;
	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextStart;

	m_bHasNext = rBag.Size() > 0;
}

CEnumeratorBagObject::~CEnumeratorBagObject()
{
}

bool CEnumeratorBagObject::NextStart()
{
	if (((CBagObject*)m_pSource)->m_pNodeRoot)
	{
		m_pNode = ((CBagObject*)m_pSource)->m_pNodeRoot->LeafSmallest();
		m_pCurrent = &(m_pNode->Object()); 

		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextEnd; m_bHasNext = FALSE; 
		return FALSE;
	}
}

bool CEnumeratorBagObject::NextContinue()
{
	if (m_pNode->ChildRight())
	{
		m_pNode = m_pNode->ChildRight()->LeafSmallest();

		if (m_pNode) { m_pCurrent = &(m_pNode->Object()); return TRUE; }
		else         { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextEnd; m_bHasNext = FALSE; return FALSE; }
	}
	else
	{
		if (!m_pNode->Parent()) { (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextEnd; m_bHasNext = FALSE; return FALSE; }

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

			if (!m_pNode || !m_pNode->Parent()) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagObject::NextEnd; m_bHasNext = FALSE; return FALSE; }
			else                                { m_pNode = m_pNode->Parent(); }
		}
	}

	m_pCurrent = &(m_pNode->Object()); 
	return TRUE;
}


//CObject* CEnumeratorBagObject::Current() _GET 
//{ 
//	if (m_pNode) { return &m_pNode->Object(); }
//	else         { return NULL; }
//}

//bool CEnumeratorBagObject::Next()
//{
//	if (!m_pBag->m_pNodeRoot) { return FALSE; }
//
//	//Handle starting 
//	if (!m_pNode)
//	{
//		m_pNode = m_pBag->m_pNodeRoot->LeafSmallest();
//
//		if (m_pNode) { return TRUE; }
//		else         { return FALSE; }
//	}
//
//	if (m_pNode->ChildRight())
//	{
//		m_pNode = m_pNode->ChildRight()->LeafSmallest();
//
//		if (m_pNode) { return TRUE; }
//		else         { return FALSE; }
//	}
//	else
//	{
//		if (!m_pNode->Parent()) { return FALSE; }
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
//			if (!m_pNode || !m_pNode->Parent()) { return FALSE; }
//			else                                { m_pNode = m_pNode->Parent(); }
//		}
//	}
//
//	return TRUE;
//}