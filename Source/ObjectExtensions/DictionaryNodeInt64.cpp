#include "DictionaryNodeInt64.h"

using namespace SCFPrivate;

CFSBHeap DictionaryNodeInt64_Heap(sizeof(CDictionaryNodeInt64));

CDictionaryNodeInt64* CDictionaryNodeInt64::Create(_IN UINT64 ui64Key, _IN CObject& rObject)
{
	CDictionaryNodeInt64* pNew = (CDictionaryNodeInt64*)DictionaryNodeInt64_Heap.Allocate();

	pNew->m_ui64Key     = ui64Key;
	pNew->m_pObject     = (CObject*)&rObject;
	pNew->m_pParent     = nullptr;
	pNew->m_pChildLeft  = nullptr;
	pNew->m_pChildRight = nullptr;
	pNew->m_uiLevel     = 1;

	BETAONLY(if (pNew->m_pObject) { pNew->m_pObject->AddRef(); })

	return pNew;
}

void CDictionaryNodeInt64::Delete(_IN CDictionaryNodeInt64* pNode)
{
	BETAONLY(if (pNode->m_pObject) { pNode->m_pObject->Release(); })

	if (pNode->ChildLeft())  { Delete(pNode->ChildLeft()); }
	if (pNode->ChildRight()) { Delete(pNode->ChildRight()); }

	DictionaryNodeInt64_Heap.Free(pNode);
}

void CDictionaryNodeInt64::DeleteWithObject(_IN CDictionaryNodeInt64* pNode)
{
	if (pNode->m_pObject) { RELEASE(*(pNode->m_pObject)); delete pNode->m_pObject; }

	if (pNode->ChildLeft())  { DeleteWithObject(pNode->ChildLeft()); }
	if (pNode->ChildRight()) { DeleteWithObject(pNode->ChildRight()); }

	DictionaryNodeInt64_Heap.Free(pNode);
}

void CDictionaryNodeInt64::Skew()
{
	if (this->IsRoot()) { return; }

	CDictionaryNodeInt64 *pParentNew = this->ChildLeft();

	if (this->Parent()->ChildLeft() == this) { this->Parent()->ChildLeft (pParentNew); }
	else                                     { this->Parent()->ChildRight(pParentNew); }
	
	//Update parent pointers
	pParentNew->Parent(this->Parent());
	this->Parent(pParentNew);

	this->ChildLeft(pParentNew->ChildRight());

	if (this->ChildLeft()) { this->ChildLeft()->Parent(this); }

	pParentNew->ChildRight(this);
	this->Level((this->ChildLeft()) ? (this->ChildLeft()->Level() + 1) : (1));
}

bool CDictionaryNodeInt64::Split()
{
	if (this->IsRoot()) { return false; }

	CDictionaryNodeInt64 *pParentNew = this->ChildRight();

	if (pParentNew && pParentNew->ChildRight() && (pParentNew->ChildRight()->Level() == this->Level())) 
	{ 
		if (this->Parent()->ChildLeft() == this) { this->Parent()->ChildLeft (pParentNew); }
		else                                     { this->Parent()->ChildRight(pParentNew); }

		//Update parent pointers
		pParentNew->Parent(this->Parent());
		this->Parent(pParentNew);
		
		this->ChildRight(pParentNew->ChildLeft());

		if (this->ChildRight()) { this->ChildRight()->Parent(this); }

		pParentNew->ChildLeft(this);
		pParentNew->Level(this->Level() + 1);
		return true;
	}

	return false;
}

void CDictionaryNodeInt64::Rebalance()
{
	if (this->IsRoot()) { return; }

	CDictionaryNodeInt64* pNode = this;
 
	/* The node already is initialized via the constructor
	pNode->Level(1);
	pNode->ChildLeft (nullptr);
	pNode->ChildRight(nullptr);*/

	for (pNode = pNode->Parent(); !pNode->IsRoot(); pNode = pNode->Parent()) 
	{
		if (pNode->Level() != ((pNode->ChildLeft()) ? (pNode->ChildLeft()->Level() + 1) : (1)))
		{
			pNode->Skew();

			if (!pNode->ChildRight() || (pNode->Level() != pNode->ChildRight()->Level())) { pNode = pNode->Parent(); }
		}

		if (!pNode->Parent()->Split()) { break; }
	}
}

CDictionaryNodeInt64* CDictionaryNodeInt64::LeafSmallest()
{
	if (!this->ChildLeft()) { return this; }
	else                    { return this->ChildLeft()->LeafSmallest(); }
}

CDictionaryNodeInt64* CDictionaryNodeInt64::LeafBiggest()
{
	if (!this->ChildRight()) { return this; }
	else                     { return this->ChildRight()->LeafBiggest(); }
}

CDictionaryNodeInt64* CDictionaryNodeInt64::RemoveRoot()
{
	if (m_pChildLeft && m_pChildRight)
	{
		CDictionaryNodeInt64* pNewRoot = nullptr;

		if (m_pChildLeft->Level() >= m_pChildRight->Level())  { pNewRoot = m_pChildLeft->LeafBiggest(); }
		else                                                  { pNewRoot = m_pChildRight->LeafSmallest(); }

		pNewRoot->Remove();
		pNewRoot->Parent(nullptr);

		if (m_pChildLeft)  { m_pChildLeft->Parent (pNewRoot); }
		if (m_pChildRight) { m_pChildRight->Parent(pNewRoot); }

		pNewRoot->ChildLeft (m_pChildLeft);
		pNewRoot->ChildRight(m_pChildRight);

		return pNewRoot;
	}
	else
	{
		if (m_pChildLeft)  { m_pChildLeft->Parent(nullptr);  return m_pChildLeft; }
		if (m_pChildRight) { m_pChildRight->Parent(nullptr); return m_pChildRight; }

		return nullptr;
	}
}

void CDictionaryNodeInt64::Remove()
{ 
	CDictionaryNodeInt64* pLeaf = this;
	CDictionaryNodeInt64* pNode = this;
	
	if (this->ChildLeft())
	{
		for (pLeaf = this->ChildLeft(); pLeaf->ChildRight(); pLeaf = pLeaf->ChildRight()) {}
	}
	else { if (this->ChildRight()) { pLeaf = this->ChildRight(); } }

	pNode = (pLeaf->Parent() == this) ? (pLeaf) : (pLeaf->Parent());

	if (pLeaf->Parent()->ChildLeft() == pLeaf) { pLeaf->Parent()->ChildLeft (nullptr); }
	else                                       { pLeaf->Parent()->ChildRight(nullptr); }

	if (this != pLeaf)
	{
		if (this->Parent()->ChildLeft() == this) { this->Parent()->ChildLeft (pLeaf); }
		else                                     { this->Parent()->ChildRight(pLeaf); }
		
		pLeaf->Parent(this->Parent());

		if (this->ChildLeft()) { this->ChildLeft()->Parent(pLeaf); }
		pLeaf->ChildLeft(this->ChildLeft());

		if (this->ChildRight()) { this->ChildRight()->Parent(pLeaf); }
		pLeaf->ChildRight(this->ChildRight());

		pLeaf->Level(this->Level());
	}

	for (; !pNode->IsRoot(); pNode = pNode->Parent())
	{
		if (pNode->Level() > (pNode->ChildLeft() ? pNode->ChildLeft()->Level() + 1 : 1))
		{
			pNode->Level(pNode->Level() - 1);

			if (pNode->Split()) 
			{
				if (pNode->Split()) { pNode->Parent()->Parent()->Skew(); }
				break;
			}
		}
		else 
		{
			if (pNode->Level() <= ((pNode->ChildRight()) ? (pNode->ChildRight()->Level() + 1) : (1))) { break; } 
			else
			{
				pNode->Skew();

				if (pNode->Level() > pNode->Parent()->Level()) 
				{
					pNode->Skew();
					pNode->Parent()->Parent()->Split();
					break;
				}
				pNode = pNode->Parent();
			}
		}
	}
}