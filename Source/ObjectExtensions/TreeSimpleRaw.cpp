#include "TreeSimple.h"

using namespace SCFBase;

CTreeSimpleRaw::CTreeSimpleRaw()
{
	m_pValue = nullptr;

	m_pParent   = nullptr;
	m_pPrevious = nullptr;
	m_pNext     = nullptr;

	m_pChildFirst = nullptr;
	m_pChildLast  = nullptr;
}

CTreeSimpleRaw::CTreeSimpleRaw(_IN _REF CObject& rValue)
{
	m_pValue = (CObject*)&rValue;
	ADDREF(*(m_pValue));

	m_pParent   = nullptr;
	m_pPrevious = nullptr;
	m_pNext     = nullptr;

	m_pChildFirst = nullptr;
	m_pChildLast  = nullptr;
}

CTreeSimpleRaw::~CTreeSimpleRaw()
{
	CTreeSimpleRaw* pCurChild  = m_pChildFirst;
	CTreeSimpleRaw* pNextChild = nullptr;

	while (pCurChild)
	{
		pNextChild = pCurChild->Next();
		delete pCurChild;
		pCurChild = pNextChild;
	}

	BETAONLY(if (m_pValue) { m_pValue->Release(); })
}

void CTreeSimpleRaw::DeleteWithValues()
{
	CTreeSimpleRaw* pCurChild  = m_pChildFirst;
	CTreeSimpleRaw* pNextChild = nullptr;

	while (pCurChild)
	{
		pNextChild = pCurChild->Next();
		pCurChild->DeleteWithValues();
		pCurChild = pNextChild;
	}

	if (m_pValue) {  RELEASE(*(m_pValue)); delete m_pValue; m_pValue = nullptr; }

	m_pChildFirst = nullptr;
	delete this;
}

void CTreeSimpleRaw::ChildAdd(_INOUT _REF CTreeSimpleRaw& rChild)
{
	rChild.Previous(m_pChildLast);
	rChild.Next(nullptr);
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

void CTreeSimpleRaw::ChildAddFirst(_INOUT _REF CTreeSimpleRaw& rChild)
{
	rChild.Next(m_pChildFirst);
	rChild.Previous(nullptr);
	rChild.Parent(this);

	if (!m_pChildFirst)
	{
		m_pChildFirst = &rChild;
		m_pChildLast  = m_pChildFirst;
		return;
	}

	m_pChildFirst->Previous(&rChild);
	m_pChildFirst = &rChild;
}

void CTreeSimpleRaw::ChildRemove(_INOUT CTreeSimpleRaw& rChild)
{
	_ASSERTE(m_pChildFirst);

	//if (!m_pChildFirst) { return; }

	//If we are removing the last & only child
	if (m_pChildFirst == m_pChildLast)
	{
		//Set child pointers to [nullptr]
		m_pChildFirst = nullptr;
		m_pChildLast = nullptr;

		//Isolate removed node
		rChild.Parent(nullptr);
		rChild.Previous(nullptr);
		rChild.Next(nullptr);
		return;
	}

	//Try removing the node by modifying its previous sibling
	if (rChild.Previous()) 
	{
		//Omit the node in the linked list
		rChild.Previous()->Next(rChild.Next());

		//Isolate removed node
		rChild.Parent(nullptr);
		rChild.Previous(nullptr);
		rChild.Next(nullptr);
		return;
	} 

	//Try removing the node by modifying its next sibling
	if (rChild.Next()) 
	{
		//Omit the node in the linked list
		rChild.Next()->Previous(rChild.Previous());

		//Isolate removed node
		rChild.Parent(nullptr);
		rChild.Previous(nullptr);
		rChild.Next(nullptr);
		return;
	} 
}

void CTreeSimpleRaw::ChildReplace(_INOUT CTreeSimpleRaw& rChildOld, _INOUT CTreeSimpleRaw& rChildNew)
{
	_ASSERTE(m_pChildFirst);

	//Inform parent (this node)
	if (&rChildOld == m_pChildFirst) { m_pChildFirst = &rChildNew; }
	if (&rChildOld == m_pChildLast)  { m_pChildLast  = &rChildNew; }

	//Rebind new node connections
	rChildNew.Next    (rChildOld.Next());
	rChildNew.Previous(rChildOld.Previous());
	rChildNew.Parent(this);

	//Isolate removed node
	rChildOld.Parent(nullptr);
	rChildOld.Previous(nullptr);
	rChildOld.Next(nullptr);

	//Inform siblings
	if (rChildNew.Next())     { rChildNew.Next()->Previous(&rChildNew); }
	if (rChildNew.Previous()) { rChildNew.Previous()->Next(&rChildNew); }
}

void CTreeSimpleRaw::DeleteSiblings()
{
	CTreeSimpleRaw* pCurrent = this->Next();

	while (pCurrent)
	{
		CTreeSimpleRaw* pNext = pCurrent->Next();
		delete pCurrent;
		pCurrent = pNext;
	}

	pCurrent = this->Previous();

	while (pCurrent)
	{
		CTreeSimpleRaw* pPrevious = pCurrent->Previous();
		delete pCurrent;
		pCurrent = pPrevious;
	}

	//Remove references to siblings
	this->Previous(nullptr);
	this->Next(nullptr);
}

void CTreeSimpleRaw::ReplaceWith(_INOUT CTreeSimpleRaw& rNode)
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
	this->Parent(nullptr);
	this->Previous(nullptr);
	this->Next(nullptr);
}

UINT CTreeSimpleRaw::Level(_IN UINT uiLevelsTraversed) _GET
{
	if (m_pParent)
	{
		return m_pParent->Level(uiLevelsTraversed + 1);
	}

	return uiLevelsTraversed;
}