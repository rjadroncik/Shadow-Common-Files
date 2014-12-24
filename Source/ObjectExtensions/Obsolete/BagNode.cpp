#include "StdAfx.h"
#include "BagNode.h"

using namespace SCFPrivate;

CBagNode* CBagNode::Create(_INOUT CFSBHeap& rHeap, _IN CObject& rObject)
{
	CBagNode* pNew = (CBagNode*)rHeap.Allocate();

	pNew->m_pObject     = (CObject*)&rObject;
	pNew->m_pParent     = NULL;
	pNew->m_pChildLeft  = NULL;
	pNew->m_pChildRight = NULL;
	pNew->m_uiLevel     = 1;

	if (pNew->m_pObject) { pNew->m_pObject->AddRef(); }

	return pNew;
}

void CBagNode::Delete(_INOUT CFSBHeap& rHeap, _IN CBagNode* pNode)
{
	if (pNode->m_pObject) { pNode->m_pObject->Release(); }

	if (pNode->ChildLeft())  { Delete(rHeap, pNode->ChildLeft()); }
	if (pNode->ChildRight()) { Delete(rHeap, pNode->ChildRight()); }

	rHeap.Free(pNode);
}

void CBagNode::Skew()
{
	if (this->IsRoot()) { return; }

	CBagNode *pParentNew = this->ChildLeft();

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

bool CBagNode::Split()
{
	if (this->IsRoot()) { return FALSE; }

	CBagNode *pParentNew = this->ChildRight();

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
		return TRUE;
	}

	return FALSE;
}

void CBagNode::Rebalance()
{
	if (this->IsRoot()) { return; }

	CBagNode* pNode = this;
 
	/* The node already is initialized via the constructor
	pNode->Level(1);
	pNode->ChildLeft (NULL);
	pNode->ChildRight(NULL);*/

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

CBagNode* CBagNode::LeafSmallest()
{
	if (!this->ChildLeft()) { return this; }
	else                    { return this->ChildLeft()->LeafSmallest(); }
}

CBagNode* CBagNode::LeafBiggest()
{
	if (!this->ChildRight()) { return this; }
	else                     { return this->ChildRight()->LeafBiggest(); }
}

CBagNode* CBagNode::RemoveRoot()
{
	if (m_pChildLeft && m_pChildRight)
	{
		CBagNode* pNewRoot = NULL;

		if (m_pChildLeft->Level() >= m_pChildRight->Level())  { pNewRoot = m_pChildLeft->LeafBiggest(); }
		else                                                  { pNewRoot = m_pChildRight->LeafSmallest(); }

		pNewRoot->Remove();
		pNewRoot->Parent(NULL);

		if (m_pChildLeft)  { m_pChildLeft->Parent (pNewRoot); }
		if (m_pChildRight) { m_pChildRight->Parent(pNewRoot); }

		pNewRoot->ChildLeft (m_pChildLeft);
		pNewRoot->ChildRight(m_pChildRight);

		return pNewRoot;
	}
	else
	{
		if (m_pChildLeft)  { m_pChildLeft->Parent(NULL);  return m_pChildLeft; }
		if (m_pChildRight) { m_pChildRight->Parent(NULL); return m_pChildRight; }

		return NULL;
	}
}

void CBagNode::Remove()
{ 
	CBagNode* pLeaf = this;
	CBagNode* pNode = this;
	
	if (this->ChildLeft())
	{
		for (pLeaf = this->ChildLeft(); pLeaf->ChildRight(); pLeaf = pLeaf->ChildRight()) {}
	}
	else { if (this->ChildRight()) { pLeaf = this->ChildRight(); } }

	pNode = (pLeaf->Parent() == this) ? (pLeaf) : (pLeaf->Parent());

	if (pLeaf->Parent()->ChildLeft() == pLeaf) { pLeaf->Parent()->ChildLeft (NULL); }
	else                                       { pLeaf->Parent()->ChildRight(NULL); }

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