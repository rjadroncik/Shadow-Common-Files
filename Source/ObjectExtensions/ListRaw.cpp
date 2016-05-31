#include "List.h"
#include "EnumeratorList.h"

using namespace SCFBase;
using namespace SCFPrivate;

CListRaw::CListRaw()
{
	m_pNodeFirst = NULL;
	m_pNodeLast = NULL;

	m_uiCount = 0;
}

CListRaw::~CListRaw()
{
	if (m_pNodeFirst) { CListNode::Delete(m_pNodeFirst); }
}

CObject& CListRaw::At(_IN UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((BYTE)(uiIndex - uiCurIndex));
}

CObject& CListRaw::operator [](_IN UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((BYTE)(uiIndex - uiCurIndex));
}

void CListRaw::AtPut(_IN UINT uiIndex, _IN _REF CObject& rObject) _SET
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((BYTE)(uiIndex - uiCurIndex), rObject);
}

void CListRaw::LastAdd(_IN _REF CObject& rObject)
{
	if (m_pNodeLast)
	{
		if (m_pNodeLast->IsFull())
		{
			CListNode* pNode = CListNode::Create(rObject, m_pNodeLast, NULL);
			m_pNodeLast->Next(pNode);
			m_pNodeLast = pNode;
		}
		else { m_pNodeLast->ObjectLastAdd(rObject); }
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}
void CListRaw::LastRemove()
{
	_ASSERTE(m_pNodeLast);
	_ASSERTE(!m_pNodeLast->IsEmpty());

	m_pNodeLast->ObjectLastRemove();

	if (m_pNodeLast->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeLast->Previous();
		CListNode::Delete(m_pNodeLast);

		if (pNode) { m_pNodeLast = pNode; pNode->Next(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}
void CListRaw::LastDelete()
{
	_ASSERTE(m_pNodeLast);
	_ASSERTE(!m_pNodeLast->IsEmpty());

	m_pNodeLast->ObjectLastDelete();

	if (m_pNodeLast->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeLast->Previous();
		CListNode::Delete(m_pNodeLast);

		if (pNode) { m_pNodeLast = pNode; pNode->Next(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}

void CListRaw::FirstAdd(_IN _REF CObject& rObject)
{
	if (m_pNodeFirst)
	{
		if (m_pNodeFirst->IsFull())
		{
			CListNode* pNode = CListNode::Create(rObject, NULL, m_pNodeFirst);
			m_pNodeFirst->Previous(pNode);
			m_pNodeFirst = pNode;
		}
		else { m_pNodeFirst->ObjectFirstAdd(rObject); }
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}
void CListRaw::FirstRemove()
{
	_ASSERTE(m_pNodeFirst);
	_ASSERTE(!m_pNodeFirst->IsEmpty());

	m_pNodeFirst->ObjectFirstRemove();

	if (m_pNodeFirst->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeFirst->Next();
		m_pNodeFirst->Next(NULL);
		CListNode::Delete(m_pNodeFirst);

		if (pNode) { m_pNodeFirst = pNode; pNode->Previous(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}
void CListRaw::FirstDelete()
{
	_ASSERTE(m_pNodeFirst);
	_ASSERTE(!m_pNodeFirst->IsEmpty());

	m_pNodeFirst->ObjectFirstDelete();

	if (m_pNodeFirst->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeFirst->Next();
		m_pNodeFirst->Next(NULL);
		CListNode::Delete(m_pNodeFirst);

		if (pNode) { m_pNodeFirst = pNode; pNode->Previous(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}

bool CListRaw::Contains(_IN CObject& rObject) _GET
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (BYTE i = 0; i < pNode->Count(); i++)
		{ 
			if (&(pNode->Object(i)) == &rObject) { return TRUE; }
		}

		pNode = pNode->Next();
	}

	return FALSE;
}

void CListRaw::Remove(_IN CObject& rObject)
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (BYTE i = 0; i < pNode->Count(); i++)
		{ 
			if (&(pNode->Object(i)) == &rObject) 
			{ 
				pNode->ObjectRemove(i);

				if (pNode->IsEmpty())
				{
					if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
					if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

					CListNode::Delete(pNode);
				}

				m_uiCount--;
			}
		}

		pNode = pNode->Next();
	}
}

void CListRaw::Insert(_IN UINT uiIndex, _IN _REF CObject& rObject)
{
	_ASSERTE(uiIndex <= m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	if (pNode)
	{
		if (pNode->IsFull())
		{
			if (!pNode->Next() || pNode->Next()->IsFull())
			{
				//Make a new node, insert it behind the node which contains the index at which we want to insert the object & initialize it with the last object from that node to make room for the inserted node
				CListNode::Create(pNode->Object(ALLOC_GRANULARITY_PTRS - 1), pNode, pNode->Next());
			}
			else
			{
				//Move the last object of the full node to the next node to make room for the inserted node
				pNode->Next()->ObjectFirstAdd(pNode->Object(ALLOC_GRANULARITY_PTRS - 1));
			}

			pNode->ObjectLastRemove();
		}

		pNode->ObjectInsert((BYTE)(uiIndex - uiCurIndex), rObject);
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}

void CListRaw::Remove(_IN UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	pNode->ObjectRemove((BYTE)(uiIndex - uiCurIndex));

	if (pNode->IsEmpty())
	{
		if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
		if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

		CListNode::Delete(pNode);
	}

	m_uiCount--;
}

void CListRaw::Delete(_IN UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	pNode->ObjectDelete((BYTE)(uiIndex - uiCurIndex));

	if (pNode->IsEmpty())
	{
		if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
		if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

		CListNode::Delete(pNode);
	}

	m_uiCount--;
}

void CListRaw::AllAdd(_INOUT IEnumerator<CObject>& rEnumerator)
{
	//Trivial implementation for now (hiding implementation offers chance for seamless optimization)
	while (rEnumerator.Next())
	{
		this->LastAdd(*rEnumerator.Current()); 
	}
}

void CListRaw::AllRemove() 
{ 
	if (m_pNodeFirst) 
	{
		CListNode::Delete(m_pNodeFirst);

		m_pNodeFirst = NULL; 
		m_pNodeLast = NULL;

		m_uiCount = 0;
	}

//	_ASSERTE(List_Heap.AllocatedBytes() == 0);
}

void CListRaw::AllDelete()
{
	if (m_pNodeFirst) 
	{
		CListNode::DeleteWithObjects(m_pNodeFirst);

		m_pNodeFirst = NULL; 
		m_pNodeLast = NULL;

		m_uiCount = 0;
	}

//	_ASSERTE(List_Heap.AllocatedBytes() == 0);
}

void CListRaw::AllDispose()
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (BYTE i = 0; i < pNode->Count(); i++) { pNode->Object(i).Dispose(); }

		pNode = pNode->Next();
	}
}