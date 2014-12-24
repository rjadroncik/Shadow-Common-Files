#include "TreeSimple.h"

using namespace SCFBase;

CTreeSimple::CTreeSimple()
{
	m_pValue = NULL;

	m_pParent   = NULL;
	m_pPrevious = NULL;
	m_pNext     = NULL;

	m_pChildFirst = NULL;
	m_pChildLast  = NULL;
}

CTreeSimple::CTreeSimple(_IN _REF CObject& rObject)
{
	m_pValue = (CObject*)&rObject;
	ADDREF(*(m_pValue));

	m_pParent   = NULL;
	m_pPrevious = NULL;
	m_pNext     = NULL;

	m_pChildFirst = NULL;
	m_pChildLast  = NULL;
}

CTreeSimple::~CTreeSimple()
{
	CTreeSimple* pCurChild  = m_pChildFirst;
	CTreeSimple* pNextChild = NULL;

	while (pCurChild)
	{
		pNextChild = pCurChild->Next();
		delete pCurChild;
		pCurChild = pNextChild;
	}

	BETAONLY(if (m_pValue) { m_pValue->Release(); })
}

void CTreeSimple::DeleteWithValues()
{
	CTreeSimple* pCurChild  = m_pChildFirst;
	CTreeSimple* pNextChild = NULL;

	while (pCurChild)
	{
		pNextChild = pCurChild->Next();
		pCurChild->DeleteWithValues();
		pCurChild = pNextChild;
	}

	if (m_pValue) {  RELEASE(*(m_pValue)); delete m_pValue; m_pValue = NULL; }

	m_pChildFirst = NULL;
	delete this;
}

void CTreeSimple::ChildAdd(_INOUT _REF CTreeSimple& rChild)
{
	rChild.Previous(m_pChildLast);
	rChild.Next(NULL);
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

void CTreeSimple::ChildAddFirst(_INOUT _REF CTreeSimple& rChild)
{
	rChild.Next(m_pChildFirst);
	rChild.Previous(NULL);
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

void CTreeSimple::ChildRemove(_INOUT CTreeSimple& rChild)
{
	_ASSERTE(m_pChildFirst);

	//if (!m_pChildFirst) { return; }

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

void CTreeSimple::ChildReplace(_INOUT CTreeSimple& rChildOld, _INOUT CTreeSimple& rChildNew)
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
	rChildOld.Parent(NULL);
	rChildOld.Previous(NULL);
	rChildOld.Next(NULL);

	//Inform siblings
	if (rChildNew.Next())     { rChildNew.Next()->Previous(&rChildNew); }
	if (rChildNew.Previous()) { rChildNew.Previous()->Next(&rChildNew); }
}

void CTreeSimple::DeleteSiblings()
{
	CTreeSimple* pCurrent = this->Next();

	while (pCurrent)
	{
		CTreeSimple* pNext = pCurrent->Next();
		delete pCurrent;
		pCurrent = pNext;
	}

	pCurrent = this->Previous();

	while (pCurrent)
	{
		CTreeSimple* pPrevious = pCurrent->Previous();
		delete pCurrent;
		pCurrent = pPrevious;
	}

	//Remove references to siblings
	this->Previous(NULL);
	this->Next(NULL);
}

void CTreeSimple::ReplaceWith(_INOUT CTreeSimple& rNode)
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

SCF::UINT CTreeSimple::Level(_IN SCF::UINT uiLevelsTraversed) _GET
{
	if (m_pParent)
	{
		return m_pParent->Level(uiLevelsTraversed + 1);
	}

	return uiLevelsTraversed;
}