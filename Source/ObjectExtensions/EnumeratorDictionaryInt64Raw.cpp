#include "EnumeratorDictionaryInt64Raw.h"
#include "DictionaryInt64Raw.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorDictionaryInt64Raw::CEnumeratorDictionaryInt64Raw(_IN CDictionaryInt64Raw& rDictionary) : CEnumeratorRaw(rDictionary)
{
	m_pNode = NULL;
	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextStart;

	m_bHasNext = rDictionary.Size() > 0;
}

CEnumeratorDictionaryInt64Raw::~CEnumeratorDictionaryInt64Raw()
{
} 

void CEnumeratorDictionaryInt64Raw::CurrentShallowRemove()
{
	if (m_pNode) { m_pNode->Object(NULL); ((CDictionaryInt64Raw*)m_pSource)->m_uiCount--; }
}

void CEnumeratorDictionaryInt64Raw::CurrentShallowDelete()
{
	if (m_pNode) 
	{ 
		CObject* pObject = m_pNode->Object();

		m_pNode->Object(NULL);
		((CDictionaryInt64Raw*)m_pSource)->m_uiCount--;

		delete pObject;
	}
}

//CObject* CEnumeratorDictionaryInt64Raw::Current() _GET 
//{ 
//	if (m_pNode) { return m_pNode->Object(); }
//	else         { return NULL; }
//}

UINT64 CEnumeratorDictionaryInt64Raw::CurrentKey() _GET
{
	if (m_pNode) { return m_pNode->Key(); }
	else         { return (UINT64)-1; }
}

bool CEnumeratorDictionaryInt64Raw::NextStart()
{
	if (((CDictionaryInt64Raw*)m_pSource)->m_pNodeRoot)
	{
		m_pNode = ((CDictionaryInt64Raw*)m_pSource)->m_pNodeRoot->LeafSmallest();
		m_pCurrent = m_pNode->Object(); 

		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}
}

bool CEnumeratorDictionaryInt64Raw::NextContinue()
{
	if (m_pNode->ChildRight())
	{
		m_pNode = m_pNode->ChildRight()->LeafSmallest();

		if (m_pNode) { m_pCurrent = m_pNode->Object(); return TRUE; }
		else         { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextEnd; m_bHasNext = FALSE; return FALSE; }
	}
	else
	{
		if (!m_pNode->Parent()) { (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextEnd; m_bHasNext = FALSE; return FALSE; }

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

			if (!m_pNode || !m_pNode->Parent()) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryInt64Raw::NextEnd; m_bHasNext = FALSE; return FALSE; }
			else                                { m_pNode = m_pNode->Parent(); }
		}
	}

	m_pCurrent = m_pNode->Object();
	return TRUE;
}

//bool CEnumeratorDictionaryInt64Raw::Next()
//{
//	if (!m_pDictionary->m_pNodeRoot) { return FALSE; }
//
//	//Handle starting 
//	if (!m_pNode)
//	{
//		m_pNode = m_pDictionary->m_pNodeRoot->LeafSmallest();
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