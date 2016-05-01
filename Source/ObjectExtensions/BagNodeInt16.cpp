#include "BagNodeInt16.h"
#include "Memory.h"

using namespace SCFPrivate;
using namespace SCFBase;

CFSBHeap BagNodeInt16_Heap(sizeof(CBagNodeInt16));

CBagNodeInt16* CBagNodeInt16::Create()
{
	CBagNodeInt16* pNew = (CBagNodeInt16*)BagNodeInt16_Heap.Allocate();

	CMemory::Erase(pNew->m_paSubNodes, sizeof(pNew->m_paSubNodes));

	return pNew;
}

void CBagNodeInt16::Delete(_IN CBagNodeInt16* pNode, _IN UINT uiLevel)
{
	if (uiLevel < MAX_DEPTH_BAG_INT16)
	{
		for (UINT i = 0; i < 16; i++)
		{
			if (pNode->m_paSubNodes[i]) { Delete(pNode->m_paSubNodes[i], uiLevel + 1); }
		}
	}

	BagNodeInt16_Heap.Free(pNode);
}
