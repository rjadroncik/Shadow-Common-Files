#include "List.h"
#include "EnumeratorList.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumerator* CListRaw::EnumeratorNew() _GET { return new CEnumeratorList(*this); }

CListRaw::CListRaw()
{
	m_pNodeFirst = NULL;
	m_pNodeLast = NULL;

	m_uiCount = 0;
}

CListRaw::~CListRaw()
{
	if (m_pNodeFirst) { CListNode::Delete(m_pNodeFirst); }
}

CObject& CListRaw::At(_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((SCF::BYTE)(uiIndex - uiCurIndex));
}

CObject& CListRaw::operator [](_IN SCF::UINT uiIndex) _GET 
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((SCF::BYTE)(uiIndex - uiCurIndex));
}

void CListRaw::AtPut(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject) _SET
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	return pNode->Object((SCF::BYTE)(uiIndex - uiCurIndex), rObject);
}

void CListRaw::LastAdd(_IN _REF CObject& rObject)
{
	if (m_pNodeLast)
	{
		if (m_pNodeLast->IsFull())
		{
			CListNode* pNode = CListNode::Create(rObject, m_pNodeLast, NULL);
			m_pNodeLast->Next(pNode);
			m_pNodeLast = pNode;
		}
		else { m_pNodeLast->ObjectLastAdd(rObject); }
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}
void CListRaw::LastRemove()
{
	_ASSERTE(m_pNodeLast);
	_ASSERTE(!m_pNodeLast->IsEmpty());

	m_pNodeLast->ObjectLastRemove();

	if (m_pNodeLast->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeLast->Previous();
		CListNode::Delete(m_pNodeLast);

		if (pNode) { m_pNodeLast = pNode; pNode->Next(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}
void CListRaw::LastDelete()
{
	_ASSERTE(m_pNodeLast);
	_ASSERTE(!m_pNodeLast->IsEmpty());

	m_pNodeLast->ObjectLastDelete();

	if (m_pNodeLast->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeLast->Previous();
		CListNode::Delete(m_pNodeLast);

		if (pNode) { m_pNodeLast = pNode; pNode->Next(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}

void CListRaw::FirstAdd(_IN _REF CObject& rObject)
{
	if (m_pNodeFirst)
	{
		if (m_pNodeFirst->IsFull())
		{
			CListNode* pNode = CListNode::Create(rObject, NULL, m_pNodeFirst);
			m_pNodeFirst->Previous(pNode);
			m_pNodeFirst = pNode;
		}
		else { m_pNodeFirst->ObjectFirstAdd(rObject); }
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}
void CListRaw::FirstRemove()
{
	_ASSERTE(m_pNodeFirst);
	_ASSERTE(!m_pNodeFirst->IsEmpty());

	m_pNodeFirst->ObjectFirstRemove();

	if (m_pNodeFirst->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeFirst->Next();
		m_pNodeFirst->Next(NULL);
		CListNode::Delete(m_pNodeFirst);

		if (pNode) { m_pNodeFirst = pNode; pNode->Previous(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}
void CListRaw::FirstDelete()
{
	_ASSERTE(m_pNodeFirst);
	_ASSERTE(!m_pNodeFirst->IsEmpty());

	m_pNodeFirst->ObjectFirstDelete();

	if (m_pNodeFirst->IsEmpty()) 
	{
		CListNode* pNode = m_pNodeFirst->Next();
		m_pNodeFirst->Next(NULL);
		CListNode::Delete(m_pNodeFirst);

		if (pNode) { m_pNodeFirst = pNode; pNode->Previous(NULL); }
		else 
		{ 
			m_pNodeFirst = NULL;
			m_pNodeLast  = NULL;
		}
	}

	m_uiCount--;
}

bool CListRaw::Contains(_IN CObject& rObject) _GET
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (SCF::BYTE i = 0; i < pNode->Count(); i++)
		{ 
			if (&(pNode->Object(i)) == &rObject) { return TRUE; }
		}

		pNode = pNode->Next();
	}

	return FALSE;
}

void CListRaw::Remove(_IN CObject& rObject)
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (SCF::BYTE i = 0; i < pNode->Count(); i++)
		{ 
			if (&(pNode->Object(i)) == &rObject) 
			{ 
				pNode->ObjectRemove(i);

				if (pNode->IsEmpty())
				{
					if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
					if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

					CListNode::Delete(pNode);
				}

				m_uiCount--;
			}
		}

		pNode = pNode->Next();
	}
}

void CListRaw::Insert(_IN SCF::UINT uiIndex, _IN _REF CObject& rObject)
{
	_ASSERTE(uiIndex <= m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	if (pNode)
	{
		if (pNode->IsFull())
		{
			if (!pNode->Next() || pNode->Next()->IsFull())
			{
				//Make a new node, insert it behind the node which contains the index at which we want to insert the object & initialize it with the last object from that node to make room for the inserted node
				CListNode::Create(pNode->Object(ALLOC_GRANULARITY_PTRS - 1), pNode, pNode->Next());
			}
			else
			{
				//Move the last object of the full node to the next node to make room for the inserted node
				pNode->Next()->ObjectFirstAdd(pNode->Object(ALLOC_GRANULARITY_PTRS - 1));
			}

			pNode->ObjectLastRemove();
		}

		pNode->ObjectInsert((SCF::BYTE)(uiIndex - uiCurIndex), rObject);
	}
	else
	{
		m_pNodeFirst = CListNode::Create(rObject);
		m_pNodeLast  = m_pNodeFirst;
	}

	m_uiCount++;
}

void CListRaw::Remove(_IN SCF::UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	pNode->ObjectRemove((SCF::BYTE)(uiIndex - uiCurIndex));

	if (pNode->IsEmpty())
	{
		if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
		if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

		CListNode::Delete(pNode);
	}

	m_uiCount--;
}

void CListRaw::Delete(_IN SCF::UINT uiIndex)
{
	_ASSERTE(m_uiCount > 0);
	_ASSERTE(uiIndex < m_uiCount);

	CListNode* pNode = m_pNodeFirst;
	SCF::UINT uiCurIndex = 0;

	while (pNode && ((uiCurIndex + pNode->Count()) <= uiIndex))
	{
		uiCurIndex += pNode->Count();
		pNode = pNode->Next();
	}

	pNode->ObjectDelete((SCF::BYTE)(uiIndex - uiCurIndex));

	if (pNode->IsEmpty())
	{
		if (pNode->Next())     { pNode->Next()->Previous(pNode->Previous()); }
		if (pNode->Previous()) { pNode->Previous()->Next(pNode->Next()); }

		CListNode::Delete(pNode);
	}

	m_uiCount--;
}

void CListRaw::AllAdd(_INOUT CEnumerator& rEnumerator)
{
	//Trivial implementation for now (hiding implementation offers chance for seamless optimization)
	while (rEnumerator.Next())
	{
		this->LastAdd(*rEnumerator.Current()); 
	}
}

void CListRaw::AllRemove() 
{ 
	if (m_pNodeFirst) 
	{
		CListNode::Delete(m_pNodeFirst);

		m_pNodeFirst = NULL; 
		m_pNodeLast = NULL;

		m_uiCount = 0;
	}

//	_ASSERTE(List_Heap.AllocatedBytes() == 0);
}

void CListRaw::AllDelete()
{
	if (m_pNodeFirst) 
	{
		CListNode::DeleteWithObjects(m_pNodeFirst);

		m_pNodeFirst = NULL; 
		m_pNodeLast = NULL;

		m_uiCount = 0;
	}

//	_ASSERTE(List_Heap.AllocatedBytes() == 0);
}

void CListRaw::AllDispose()
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (SCF::BYTE i = 0; i < pNode->Count(); i++) { pNode->Object(i).Dispose(); }

		pNode = pNode->Next();
	}
}


void CListRaw::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt(m_uiCount);

	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		rStream.PutByte(pNode->Count());
		pNode = pNode->Next();
	}
}

void CListRaw::Deserialize(_INOUT IStreamRead& rStream)
{
	m_uiCount = rStream.GetInt();

	if (m_uiCount) { m_pNodeFirst = CListNode::Deserialization_Create(); m_pNodeFirst->Count(rStream.GetByte()); }

	register CListNode* pNodePrevious = m_pNodeFirst;
	register CListNode* pNode         = m_pNodeFirst;

	for (SCF::UINT i = m_pNodeFirst->Count(); i < m_uiCount; i += pNode->Count()) 
	{ 
		pNode = CListNode::Deserialization_Create(pNodePrevious);
		pNode->Count(rStream.GetByte());

		pNodePrevious->Next(pNode);
		pNodePrevious = pNode;
	}

	//Has to be done manually as we use [Deserialization_Create(..)] to speed things up
	pNode->Next(NULL);

	m_pNodeLast = pNode;
}

void CListRaw::DependentsSerialize(_INOUT IStreamWriteObject& rStream) const
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (SCF::BYTE i = 0; i < pNode->Count(); i++) { rStream.Next((CObjectSerializable&)(pNode->Object(i))); }

		pNode = pNode->Next();
	}
}

void CListRaw::DependentsDeserialize(_INOUT IStreamReadObject& rStream)
{
	register CListNode* pNode = m_pNodeFirst;

	while (pNode)
	{
		for (SCF::BYTE i = 0; i < pNode->Count(); i++) { rStream.Next(); pNode->Deserialization_Object(i, *rStream.Current()); }

		pNode = pNode->Next();
	}
}