#include "ListRaw.h"
#include "ListNode.h"
#include "EnumeratorListRaw.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorListRaw::CEnumeratorListRaw(_IN _REF CListRaw& rList) : CEnumeratorRaw(rList)
{
	m_pNode = NULL;
	m_ucIndex = 0;

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorListRaw::NextStart;

	m_bHasNext = rList.m_uiCount > 0;
}

CEnumeratorListRaw::~CEnumeratorListRaw()
{
} 

bool CEnumeratorListRaw::NextStart()
{
	m_pNode = ((CListRaw*)m_pSource)->m_pNodeFirst;

	if (m_pNode)
	{
		m_pCurrent = &(m_pNode->Object(0));
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorListRaw::NextContinue;
		return TRUE;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorListRaw::NextEnd; m_bHasNext = FALSE;
		return FALSE;
	}
}

bool CEnumeratorListRaw::NextContinue()
{
	m_ucIndex++;

	if (m_ucIndex == m_pNode->Count())
	{
		m_pNode = m_pNode->Next();
		
		if (m_pNode) { m_ucIndex = 0; }
		else 
		{
			m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorListRaw::NextEnd; m_bHasNext = FALSE;
			return FALSE;
		} 
	}

	m_pCurrent = &(m_pNode->Object(m_ucIndex));
	return TRUE;
}
