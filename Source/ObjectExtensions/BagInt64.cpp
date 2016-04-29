#include "BagInt64.h"
#include "EnumeratorBagInt64.h"

using namespace SCFBase;
using namespace SCFPrivate;

#define INDEX (SCF::UINT64)(ui64Tmp & 0xf000000000000000) >> 60

CBagInt64::CBagInt64()
{
	m_pNodeRoot = CBagNodeInt64::Create();

	m_uiCount = 0;
}

CBagInt64::~CBagInt64()
{
	if (m_pNodeRoot) { CBagNodeInt64::Delete(m_pNodeRoot, 0); }
}

bool CBagInt64::Contains(_IN SCF::UINT64 ui64Value) _GET
{
	CBagNodeInt64* pNode = m_pNodeRoot;
	SCF::UINT64 ui64Tmp = ui64Value;

	for (SCF::UINT i = 0; i < (MAX_DEPTH_BAG_INT64 - 1); i++)
	{
		//Check nodes as we traverse the address, return FALSE if address not stored
		if (!pNode->SubNode(INDEX)) { return FALSE; }

		pNode = pNode->SubNode(INDEX);
		ui64Tmp <<= 4;
	}

	//If last node is found, address is stored, return TRUE
	if (pNode->SubNode(INDEX)) { return TRUE; }

	return FALSE;
}

void CBagInt64::Remove(_IN SCF::UINT64 ui64Value)
{
	CBagNodeInt64* pNode = m_pNodeRoot;
	SCF::UINT64 ui64Tmp = ui64Value;

	for (SCF::UINT i = 0; i < (MAX_DEPTH_BAG_INT64 - 1); i++)
	{
		//Check nodes as we traverse the address, abort if address not stored
		if (!pNode->SubNode(INDEX)) { return; }

		pNode = pNode->SubNode(INDEX);
		ui64Tmp <<= 4;
	}

	//Remove last node - thus removing ONLY the specified address
	if (pNode->SubNode(INDEX))
	{
		pNode->SubNode(INDEX, NULL);
		m_uiCount--;
	}
}

void CBagInt64::Add(_IN SCF::UINT64 ui64Value)
{
	CBagNodeInt64* pNode = m_pNodeRoot;
	SCF::UINT64 ui64Tmp = ui64Value;

	for (SCF::UINT i = 0; i < (MAX_DEPTH_BAG_INT64 - 1); i++)
	{
		//Create nodes as we traverse the address
		if (!pNode->SubNode(INDEX)) 
		{
			pNode->SubNode(INDEX, CBagNodeInt64::Create()); 
		}

		pNode = pNode->SubNode(INDEX);
		ui64Tmp <<= 4;
	}

	if (!pNode->SubNode(INDEX))
	{
		//Instead of sub node place address of object at the leaf of the tree
		pNode->SubNode(INDEX, (CBagNodeInt64*)ui64Value);
		m_uiCount++;
	}
}

void CBagInt64::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CBagNodeInt64::Delete(m_pNodeRoot, 0);
		m_pNodeRoot = NULL; 
	}
}

void CBagInt64::AllDelete()
{
	if (m_pNodeRoot) 
	{
		CBagNodeInt64::Delete(m_pNodeRoot, 0);
		m_pNodeRoot = NULL; 
	}
}

void CBagInt64::AllDispose()
{
	if (!m_pNodeRoot) { return; }

	CEnumeratorBagInt64 Enumerator(*this);
	while (Enumerator.Next()) { ((CObject*)Enumerator.Current())->Dispose(); }
}