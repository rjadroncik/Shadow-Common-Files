#include "BagInt16.h"
#include "EnumeratorBagInt16.h"

using namespace SCFBase;
using namespace SCFPrivate;

CBagInt16::CBagInt16()
{
	m_pNodeRoot = CBagNodeInt16::Create();
	m_uiCount = 0;
}

CBagInt16::~CBagInt16()
{
	if (m_pNodeRoot) { CBagNodeInt16::Delete(m_pNodeRoot, 0); }
}

#define INDEX (USHORT)(uiAddress & 0xf000) >> 12

bool CBagInt16::Contains(_IN USHORT usValue) _GET
{
	USHORT uiAddress = usValue;
	CBagNodeInt16* pNode = m_pNodeRoot;

	for (UINT i = 0; i < (MAX_DEPTH_BAG_INT16 - 1); i++)
	{
		//Check nodes as we traverse the address, return FALSE if address not stored
		if (!pNode->SubNode(INDEX)) { return FALSE; }

		pNode = pNode->SubNode(INDEX);
		uiAddress <<= 4;
	}

	//If last node is found, address is stored, return TRUE
	if (pNode->SubNode(INDEX)) { return TRUE; }

	return FALSE;
}

void CBagInt16::Remove(_IN USHORT usValue)
{
	USHORT uiAddress = usValue;
	CBagNodeInt16* pNode = m_pNodeRoot;

	for (UINT i = 0; i < (MAX_DEPTH_BAG_INT16 - 1); i++)
	{
		//Check nodes as we traverse the address, abort if address not stored
		if (!pNode->SubNode(INDEX)) { return; }

		pNode = pNode->SubNode(INDEX);
		uiAddress <<= 4;
	}

	//Remove last node - thus removing ONLY the specified address
	if (pNode->SubNode(INDEX))
	{
		pNode->SubNode(INDEX, NULL);
		m_uiCount--;
	}
}

void CBagInt16::Add(_IN USHORT usValue)
{
	USHORT uiAddress = usValue;
	CBagNodeInt16* pNode = m_pNodeRoot;

	for (UINT i = 0; i < (MAX_DEPTH_BAG_INT16 - 1); i++)
	{
		//Create nodes as we traverse the address
		if (!pNode->SubNode(INDEX)) 
		{
			pNode->SubNode(INDEX, CBagNodeInt16::Create()); 
		}

		pNode = pNode->SubNode(INDEX);
		uiAddress <<= 4;
	}

	if (!pNode->SubNode(INDEX))
	{
		//Instead of sub node place address of object at the leaf of the tree
		pNode->SubNode(INDEX, (CBagNodeInt16*)&usValue);
		m_uiCount++;
	}
}

void CBagInt16::AllRemove() 
{ 
	if (m_pNodeRoot) 
	{
		CBagNodeInt16::Delete(m_pNodeRoot, 0);
		m_pNodeRoot = NULL; 
	}
}