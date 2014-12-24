#include "BagNodeInt64.h"
#include "Memory.h"

using namespace SCFPrivate;
using namespace SCFBase;

CFSBHeap BagNodeInt64_Heap(sizeof(CBagNodeInt64));

CBagNodeInt64* CBagNodeInt64::Create()
{
	CBagNodeInt64* pNew = (CBagNodeInt64*)BagNodeInt64_Heap.Allocate();

	CMemory::Erase(pNew->m_paSubNodes, sizeof(pNew->m_paSubNodes));

	return pNew;
}

void CBagNodeInt64::Delete(_IN CBagNodeInt64* pNode, _IN SCF::UINT uiLevel)
{
	if (uiLevel < MAX_DEPTH_BAG_INT64)
	{
		for (SCF::UINT i = 0; i < 16; i++)
		{
			if (pNode->m_paSubNodes[i]) { Delete(pNode->m_paSubNodes[i], uiLevel + 1); }
		}
	}

	BagNodeInt64_Heap.Free(pNode);
}

void CBagNodeInt64::DeleteWithObject(_IN CBagNodeInt64* pNode, _IN SCF::UINT uiLevel)
{
	if (uiLevel < MAX_DEPTH_BAG_INT64)
	{
		for (SCF::UINT i = 0; i < 16; i++)
		{
			if (pNode->m_paSubNodes[i]) { Delete(pNode->m_paSubNodes[i], uiLevel + 1); }
		}
	}

	BagNodeInt64_Heap.Free(pNode);
}
