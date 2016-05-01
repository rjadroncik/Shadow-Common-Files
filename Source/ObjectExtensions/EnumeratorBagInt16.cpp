#include "EnumeratorBagInt16.h"
#include "BagNodeInt16.h"
#include "Memory.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorBagInt16::CEnumeratorBagInt16(_IN CBagInt16& rBag) : CEnumeratorRaw(rBag)
{
	CMemory::Erase(m_paNodes,   sizeof(m_paNodes));
	CMemory::Erase(m_caIndexes, sizeof(m_caIndexes));

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagInt16::NextStart;

	m_bHasNext = rBag.Size() > 0;
}

CEnumeratorBagInt16::~CEnumeratorBagInt16()
{
}

void CEnumeratorBagInt16::CurrentRemove()
{
	if (m_paNodes[MAX_DEPTH_BAG_INT16])
	{
		m_paNodes[MAX_DEPTH_BAG_INT16 - 1]->SubNode(m_caIndexes[MAX_DEPTH_BAG_INT16 - 1], NULL);
		((CBagInt16*)m_pSource)->m_uiCount--;
	}
}

bool CEnumeratorBagInt16::NextStart()
{
	m_paNodes[0] = ((CBagInt16*)m_pSource)->m_pNodeRoot;
	m_caIndexes[0] = -1;

	BYTE ucLevel = 0;

	for (;;)
	{
		//Start on the next index
		for (BYTE j = m_caIndexes[ucLevel] + 1; j < 17; j++)
		{
			//If we are finished with the current node
			if (j == 16) 
			{
				//We either move up or we are finished if we can't
				if (ucLevel == 0) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagInt16::NextEnd; m_bHasNext = FALSE; return FALSE; }
				else              { m_caIndexes[ucLevel] = -1; ucLevel--; break; }
			}

			//Check whether there is a sub-node for the current index
			if (m_paNodes[ucLevel]->SubNode(j))
			{
				//If there is, we move down
				m_caIndexes[ucLevel] = j;
				m_paNodes[ucLevel + 1] = m_paNodes[ucLevel]->SubNode(j);
				ucLevel++;

				//If we reached a leave, we return TRUE
				if (ucLevel == MAX_DEPTH_BAG_INT16) 
				{
					m_pCurrent = (CObject*)m_paNodes[MAX_DEPTH_BAG_INT16];

					m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagInt16::NextContinue;
					return TRUE; 
				}
				else { m_caIndexes[ucLevel] = -1; break; }
			}
		}
	}
}

bool CEnumeratorBagInt16::NextContinue()
{
	BYTE ucLevel = MAX_DEPTH_BAG_INT16 - 1;

	for (;;)
	{
		//Start on the next index
		for (BYTE j = m_caIndexes[ucLevel] + 1; j < 17; j++)
		{
			//If we are finished with the current node
			if (j == 16) 
			{
				//We either move up or we are finished if we can't
				if (ucLevel == 0) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorBagInt16::NextEnd; m_bHasNext = FALSE; return FALSE; }
				else              { m_caIndexes[ucLevel] = -1; ucLevel--; break; }
			}

			//Check whether there is a sub-node for the current index
			if (m_paNodes[ucLevel]->SubNode(j))
			{
				//If there is, we move down
				m_caIndexes[ucLevel] = j;
				m_paNodes[ucLevel + 1] = m_paNodes[ucLevel]->SubNode(j);
				ucLevel++;

				//If we reached a leave, we return TRUE
				if (ucLevel == MAX_DEPTH_BAG_INT16) { m_pCurrent = (CObject*)m_paNodes[MAX_DEPTH_BAG_INT16]; return TRUE; }
				else                                { m_caIndexes[ucLevel] = -1; break; }
			}
		}
	}
}

//bool CEnumeratorBagInt16::Next()
//{
//	BYTE ucLevel = MAX_DEPTH_BAG_INT16 - 1;
//
//	if (!m_paNodes[0])
//	{
//		m_paNodes[0] = m_pBag->m_pNodeRoot;
//		m_caIndexes[0] = -1;
//
//		ucLevel = 0;
//	}
//
//	for (;;)
//	{
//		//Start on the next index
//		for (BYTE j = m_caIndexes[ucLevel] + 1; j < 17; j++)
//		{
//			//If we are finished with the current node
//			if (j == 16) 
//			{
//				//We either move up or we are finished if we can't
//				if (ucLevel == 0) { return FALSE; }
//				else              { m_caIndexes[ucLevel] = -1; ucLevel--; break; }
//			}
//
//			//Check whether there is a sub-node for the current index
//			if (m_paNodes[ucLevel]->SubNode(j))
//			{
//				//If there is, we move down
//				m_caIndexes[ucLevel] = j;
//				m_paNodes[ucLevel + 1] = m_paNodes[ucLevel]->SubNode(j);
//				ucLevel++;
//
//				//If we reached a leave, we return TRUE
//				if (ucLevel == MAX_DEPTH_BAG_INT16) { return TRUE; }
//				else                                { m_caIndexes[ucLevel] = -1; break; }
//			}
//		}
//	}
//}
