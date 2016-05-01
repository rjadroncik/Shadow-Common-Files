#include "DictionaryNodeString.h"

using namespace SCFPrivate;

CFSBHeap DictionaryNodeString_Heap(sizeof(CDictionaryNodeString));

CDictionaryNodeString* CDictionaryNodeString::Create(_IN TCHAR cLetter)
{
	CDictionaryNodeString* pNew = (CDictionaryNodeString*)DictionaryNodeString_Heap.Allocate();

	pNew->m_pObject      = NULL;
	pNew->m_cLetter      = cLetter;
	pNew->m_uiUsage      = 0;
	pNew->m_pChildFirst  = NULL;
	pNew->m_pNext        = NULL;

	return pNew;
}

CDictionaryNodeString* CDictionaryNodeString::CreateCopy(_IN CDictionaryNodeString* pNode)
{
	CDictionaryNodeString* pNew = (CDictionaryNodeString*)DictionaryNodeString_Heap.Allocate();

	pNew->m_pObject      = pNode->m_pObject;
	pNew->m_cLetter      = pNode->m_cLetter;
	pNew->m_uiUsage      = pNode->m_uiUsage;
	pNew->m_pChildFirst  = pNode->m_pChildFirst;
	pNew->m_pNext        = pNode->m_pNext;
	
	BETAONLY(if (pNode->m_pObject) { pNode->m_pObject->AddRef(); })

	return pNew;
}

void CDictionaryNodeString::Delete(_IN CDictionaryNodeString* pNode)
{
	BETAONLY(if (pNode->m_pObject) { pNode->m_pObject->Release(); })

	if (pNode->ChildFirst()) { Delete(pNode->ChildFirst()); }
	if (pNode->Next())       { Delete(pNode->Next()); }

	DictionaryNodeString_Heap.Free(pNode);
}

void CDictionaryNodeString::DeleteWithObject(_IN CDictionaryNodeString* pNode)
{
	if (pNode->m_pObject) { RELEASE(*(pNode->m_pObject)); delete pNode->m_pObject; }

	if (pNode->ChildFirst()) { DeleteWithObject(pNode->ChildFirst()); }
	if (pNode->Next())       { DeleteWithObject(pNode->Next()); }

	//Delete(pNode);
	DictionaryNodeString_Heap.Free(pNode);
}