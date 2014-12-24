#include "EnumeratorDictionaryObject.h"
#include "DictionaryNodeObject.h"
#include "DictionaryObjectRaw.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorDictionaryObject::CEnumeratorDictionaryObject(_IN CDictionaryObjectRaw& rDictionary) : CEnumerator(rDictionary)
{
	m_pNode = NULL;
	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextStart;

	m_bHasNext = rDictionary.Size() > 0;
}

CEnumeratorDictionaryObject::~CEnumeratorDictionaryObject()
{
} 

void CEnumeratorDictionaryObject::CurrentShallowRemove()
{
	if (m_pNode) { m_pNode->Object(NULL); ((CDictionaryObjectRaw*)((CDictionaryObjectRaw*)m_pSource))->m_uiCount--; }
}

void CEnumeratorDictionaryObject::CurrentShallowDelete()
{
	if (m_pNode) 
	{ 
		CObject* pObject = m_pNode->Object();

		m_pNode->Object(NULL);
		((CDictionaryObjectRaw*)m_pSource)->m_uiCount--;

		delete pObject;
	}
}

// CObject* CEnumeratorDictionaryObject::Current() _GET 
// { 
// 	if (m_pNode) { return m_pNode->Object(); }
// 	else         { return NULL; }
// }

CObject* CEnumeratorDictionaryObject::CurrentKey() _GET
{
	if (m_pNode) { return &m_pNode->Key(); }
	else         { return NULL; }
}

bool CEnumeratorDictionaryObject::NextStart()
{
	if (((CDictionaryObjectRaw*)m_pSource)->m_pNodeRoot)
	{
		m_pNode = ((CDictionaryObjectRaw*)m_pSource)->m_pNodeRoot->LeafSmallest();
		m_pCurrent = m_pNode->Object(); 

		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}
}

bool CEnumeratorDictionaryObject::NextContinue()
{
 	if (m_pNode->ChildRight())
 	{
 		m_pNode = m_pNode->ChildRight()->LeafSmallest();
 
 		if (m_pNode) { m_pCurrent = m_pNode->Object(); return TRUE; }
 		else         { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextEnd; m_bHasNext = FALSE; return FALSE; }
 	}
 	else
 	{
 		if (!m_pNode->Parent()) { (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextEnd; m_bHasNext = FALSE; return FALSE; }
 
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
 
 			if (!m_pNode || !m_pNode->Parent()) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryObject::NextEnd; m_bHasNext = FALSE; return FALSE; }
 			else                                { m_pNode = m_pNode->Parent(); }
 		}
 	}
 
 	m_pCurrent = m_pNode->Object();
	return TRUE;
}

// bool CEnumeratorDictionaryObject::Next()
// {
// 	if (!m_pDictionary->m_pNodeRoot) { return FALSE; }
// 
// 	//Handle starting 
// 	if (!m_pNode)
// 	{
// 		m_pNode = m_pDictionary->m_pNodeRoot->LeafSmallest();
// 
// 		if (m_pNode) { return TRUE; }
// 		else         { return FALSE; }
// 	}
// 
// 	if (m_pNode->ChildRight())
// 	{
// 		m_pNode = m_pNode->ChildRight()->LeafSmallest();
// 
// 		if (m_pNode) { return TRUE; }
// 		else         { return FALSE; }
// 	}
// 	else
// 	{
// 		if (!m_pNode->Parent()) { return FALSE; }
// 
// 		//If the current node is the left child of the parent
// 		if (m_pNode->Parent()->ChildLeft() == m_pNode)
// 		{
// 			m_pNode = m_pNode->Parent();
// 		}
// 		//Current node is the right child
// 		else
// 		{
// 			do
// 			{
// 				m_pNode = m_pNode->Parent();
// 			}
// 			while (m_pNode->Parent() && (m_pNode->Parent()->ChildRight() == m_pNode));
// 
// 			if (!m_pNode || !m_pNode->Parent()) { return FALSE; }
// 			else                                { m_pNode = m_pNode->Parent(); }
// 		}
// 	}
// 
// 	return TRUE;
// }