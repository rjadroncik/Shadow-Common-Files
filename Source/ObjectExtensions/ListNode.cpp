#include "ListNode.h"

using namespace SCFPrivate;

CFSBHeap ListNode_Heap(sizeof(CListNode));

CListNode* CListNode::Create(_IN CObject& rObject, _IN CListNode* pPrevious, _IN CListNode* pNext)
{
	CListNode* pNew = (CListNode*)ListNode_Heap.Allocate();

	pNew->m_pPrevious = (CListNode*)pPrevious;
	pNew->m_pNext     = (CListNode*)pNext;

	pNew->m_ucCount = 1;
	pNew->m_paObjects[0] = &(CObject&)rObject;

	ADDREF(*(pNew->m_paObjects[0]));

	return pNew;
}

void CListNode::Delete(_IN CListNode* pNode)
{
	for (BYTE i = 0; i < pNode->m_ucCount; i++) { RELEASE(*(pNode->m_paObjects[i])); }

	if (pNode->m_pNext) { Delete(pNode->m_pNext); }

	ListNode_Heap.Free(pNode);
}

void CListNode::DeleteWithObjects(_IN CListNode* pNode)
{
	for (BYTE i = 0; i < pNode->m_ucCount; i++) { RELEASE(*(pNode->m_paObjects[i])); delete pNode->m_paObjects[i]; }

	if (pNode->m_pNext) { DeleteWithObjects(pNode->m_pNext); }

	ListNode_Heap.Free(pNode);
}

void CListNode::ObjectLastAdd(_IN _REF CObject& rObject)
{
	_ASSERTE(m_ucCount < ALLOC_GRANULARITY_PTRS);

	m_paObjects[m_ucCount] = (CObject*)&rObject;
	m_ucCount++;
	ADDREF(rObject);
}
void CListNode::ObjectLastRemove()
{
	_ASSERTE(m_ucCount > 0);

	m_ucCount--;
	RELEASE(*(m_paObjects[m_ucCount]));
}
void CListNode::ObjectLastDelete()
{
	_ASSERTE(m_ucCount > 0);

	m_ucCount--;
	RELEASE(*(m_paObjects[m_ucCount])); delete m_paObjects[m_ucCount];
}

void CListNode::ObjectFirstAdd(_IN _REF CObject& rObject)
{
	_ASSERTE(m_ucCount < ALLOC_GRANULARITY_PTRS);

	for (UINT i = m_ucCount; i > 0; i--) { m_paObjects[i] = m_paObjects[i - 1]; }

	m_paObjects[0] = (CObject*)&rObject;
	m_ucCount++;
	ADDREF(rObject);
}
void CListNode::ObjectFirstRemove()
{
	_ASSERTE(m_ucCount > 0);

	RELEASE(*(m_paObjects[0]));
	m_ucCount--;

	for (UINT i = 0; i < m_ucCount; i++) { m_paObjects[i] = m_paObjects[i + 1]; }
}
void CListNode::ObjectFirstDelete()
{
	_ASSERTE(m_ucCount > 0);

	RELEASE(*(m_paObjects[0])); delete m_paObjects[0];
	m_ucCount--;

	for (UINT i = 0; i < m_ucCount; i++) { m_paObjects[i] = m_paObjects[i + 1]; }
}

void CListNode::ObjectInsert(_IN UINT uiIndex, _IN _REF CObject& rObject)
{
	_ASSERTE(m_ucCount < ALLOC_GRANULARITY_PTRS);

	for (UINT i = m_ucCount; i > uiIndex; i--) { m_paObjects[i] = m_paObjects[i - 1]; }

	m_paObjects[uiIndex] = (CObject*)&rObject;
	m_ucCount++;
	ADDREF(rObject);
}

void CListNode::ObjectRemove(_IN UINT uiIndex)
{
	_ASSERTE(m_ucCount > 0);
	_ASSERTE(uiIndex < m_ucCount);

	RELEASE(*(m_paObjects[uiIndex]));

	m_ucCount--;
	for (UINT i = uiIndex; i < m_ucCount; i++) { m_paObjects[i] = m_paObjects[i + 1]; }
}
void CListNode::ObjectDelete(_IN UINT uiIndex)
{
	_ASSERTE(m_ucCount > 0);
	_ASSERTE(uiIndex < m_ucCount);

	RELEASE(*(m_paObjects[uiIndex])); delete m_paObjects[uiIndex];

	m_ucCount--;
	for (UINT i = uiIndex; i < m_ucCount; i++) { m_paObjects[i] = m_paObjects[i + 1]; }
}

CListNode* CListNode::Deserialization_Create(_IN CListNode* pPrevious)
{
	CListNode* pNew = (CListNode*)ListNode_Heap.Allocate();

	pNew->m_pPrevious = (CListNode*)pPrevious;

	return pNew;
}