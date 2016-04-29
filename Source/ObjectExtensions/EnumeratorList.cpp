#include "EnumeratorList.h"
#include "ListNode.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorList::CEnumeratorList(_IN CListRaw& rList) : CEnumeratorRaw(rList)
{
	m_pNode = NULL;
	m_ucIndex = 0;

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorList::NextStart;

	m_bHasNext = rList.Size() > 0;
}

CEnumeratorList::~CEnumeratorList()
{
} 

bool CEnumeratorList::NextStart()
{
	m_pNode = ((CListRaw*)m_pSource)->m_pNodeFirst;

	if (m_pNode)
	{
		m_pCurrent = &(m_pNode->Object(0));
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorList::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorList::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}
}

bool CEnumeratorList::NextContinue()
{
	m_ucIndex++;

	if (m_ucIndex == m_pNode->Count())
	{
		m_pNode = m_pNode->Next();
		
		if (m_pNode) { m_ucIndex = 0; }
		else 
		{
			m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorList::NextEnd; m_bHasNext = FALSE;
			return FALSE;
		} 
	}

	m_pCurrent = &(m_pNode->Object(m_ucIndex));
	return TRUE;
}
