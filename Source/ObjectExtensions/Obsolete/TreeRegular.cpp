#include "StdAfx.h"
#include "TreeRegular.h"

using namespace SCFObjectExtensions;

CTreeRegular::CTreeRegular()
{
	m_pValue = NULL;

	m_pParent   = NULL;
	m_pPrevious = NULL;
	m_pNext     = NULL;

	m_pChildFirst = NULL;
	m_pChildLast  = NULL;
}

CTreeRegular::CTreeRegular(_IN _REF CObject& rObject)
{
	m_pValue = (CObject*)&rObject;
	m_pValue->AddRef();

	m_pParent   = NULL;
	m_pPrevious = NULL;
	m_pNext     = NULL;

	m_pChildFirst = NULL;
	m_pChildLast  = NULL;
}

CTreeRegular::~CTreeRegular()
{
	CTreeRegular* pCurChild  = m_pChildFirst;
	CTreeRegular* pNextChild = NULL;

	while (pCurChild)
	{
		pNextChild = pCurChild->Next();
		delete pCurChild;
		pCurChild = pNextChild;
	}

	if (m_pValue) { m_pValue->Release(); }
}

void CTreeRegular::ChildAdd(_INOUT _REF CTreeRegular& rChild)
{
	rChild.Previous(m_pChildLast);
	rChild.Parent(this);

	if (!m_pChildLast)
	{
		m_pChildFirst = &rChild;
		m_pChildLast  = m_pChildFirst;
		return;
	}

	m_pChildLast->Next(&rChild);
	m_pChildLast = &rChild;
}

void CTreeRegular::ChildRemove(_INOUT CTreeRegular& rChild)
{
	if (!m_pChildFirst) { return; }

	//If we are removing the last & only child
	if (m_pChildFirst == m_pChildLast)
	{
		//Set child pointers to [NULL]
		m_pChildFirst = NULL;
		m_pChildLast = NULL;

		//Isolate removed node
		rChild.Parent(NULL);
		rChild.Previous(NULL);
		rChild.Next(NULL);
		return;
	}

	//Try removing the node by modifying its previous sibling
	if (rChild.Previous()) 
	{
		//Omit the node in the linked list
		rChild.Previous()->Next(rChild.Next());

		//Isolate removed node
		rChild.Parent(NULL);
		rChild.Previous(NULL);
		rChild.Next(NULL);
		return;
	} 

	//Try removing the node by modifying its next sibling
	if (rChild.Next()) 
	{
		//Omit the node in the linked list
		rChild.Next()->Previous(rChild.Previous());

		//Isolate removed node
		rChild.Parent(NULL);
		rChild.Previous(NULL);
		rChild.Next(NULL);
		return;
	} 
}

void CTreeRegular::DeleteSiblings()
{
	CTreeRegular* pCurrent = this->Next();

	while (pCurrent)
	{
		CTreeRegular* pNext = pCurrent->Next();
		delete pCurrent;
		pCurrent = pNext;
	}

	pCurrent = this->Previous();

	while (pCurrent)
	{
		CTreeRegular* pPrevious = pCurrent->Previous();
		delete pCurrent;
		pCurrent = pPrevious;
	}

	//Remove references to siblings
	this->Previous(NULL);
	this->Next(NULL);
}

void CTreeRegular::ReplaceWith(_INOUT CTreeRegular& rNode)
{
	//Replace the old node with the new one in the linked list
	if (this->Previous()) { this->Previous()->Next(&rNode); } 
	if (this->Next())     { this->Next()->Previous(&rNode); }

	//Update the new nodes relatives
	rNode.Parent  (this->Parent());
	rNode.Previous(this->Previous());
	rNode.Next    (this->Next());

	if (this->Parent())
	{
		if (this->Parent()->ChildFirst() == this) { this->Parent()->ChildFirst(&rNode); }
		if (this->Parent()->ChildLast() == this)  { this->Parent()->ChildLast(&rNode); }
	}

	//Isolate removed node
	this->Parent(NULL);
	this->Previous(NULL);
	this->Next(NULL);
}

SCF::UINT CTreeRegular::Level(_IN SCF::UINT uiLevelsTraversed) _GET
{
	if (m_pParent)
	{
		return m_pParent->Level(uiLevelsTraversed + 1);
	}

	return uiLevelsTraversed;
}