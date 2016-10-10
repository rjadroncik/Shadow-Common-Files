#include "SetNodeInt64.h"
#include "Memory.h"

using namespace SCFPrivate;
using namespace SCFBase;

CFSBHeap SetNodeInt64_Heap(sizeof(CSetNodeInt64));

CSetNodeInt64* CSetNodeInt64::Create()
{
	CSetNodeInt64* pNew = (CSetNodeInt64*)SetNodeInt64_Heap.Allocate();

	CMemory::Erase(pNew->m_paSubNodes, sizeof(pNew->m_paSubNodes));

	return pNew;
}

void CSetNodeInt64::Delete(_IN CSetNodeInt64* pNode, _IN UINT uiLevel)
{
	if (uiLevel < MAX_DEPTH_BAG_INT64)
	{
		for (UINT i = 0; i < 16; i++)
		{
			if (pNode->m_paSubNodes[i]) { Delete(pNode->m_paSubNodes[i], uiLevel + 1); }
		}
	}

	SetNodeInt64_Heap.Free(pNode);
}

void CSetNodeInt64::DeleteWithObject(_IN CSetNodeInt64* pNode, _IN UINT uiLevel)
{
	if (uiLevel < MAX_DEPTH_BAG_INT64)
	{
		for (UINT i = 0; i < 16; i++)
		{
			if (pNode->m_paSubNodes[i]) { Delete(pNode->m_paSubNodes[i], uiLevel + 1); }
		}
	}

	SetNodeInt64_Heap.Free(pNode);
}
