#include "EnumeratorSetInt64.h"
#include "SetNodeInt64.h"
#include "Memory.h"
#include "SetInt64.h"

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorSetInt64::CEnumeratorSetInt64(_IN CSetInt64& rBag) : CEnumeratorRaw(rBag)
{
	CMemory::Erase(m_paNodes,   sizeof(m_paNodes));
	CMemory::Erase(m_caIndexes, sizeof(m_caIndexes));

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetInt64::NextStart;

	m_bHasNext = rBag.Size() > 0;
}

CEnumeratorSetInt64::~CEnumeratorSetInt64()
{
} 

void CEnumeratorSetInt64::CurrentRemove()
{
	if (m_paNodes[MAX_DEPTH_BAG_INT64])
	{
		m_paNodes[MAX_DEPTH_BAG_INT64 - 1]->SubNode(m_caIndexes[MAX_DEPTH_BAG_INT64 - 1], nullptr);
		((CSetInt64*)m_pSource)->m_uiCount--;
	}
}

void CEnumeratorSetInt64::CurrentDelete()
{
	if (m_paNodes[MAX_DEPTH_BAG_INT64])
	{
		delete (CObject*)m_paNodes[8];

		m_paNodes[MAX_DEPTH_BAG_INT64 - 1]->SubNode(m_caIndexes[MAX_DEPTH_BAG_INT64 - 1], nullptr);
		((CSetInt64*)m_pSource)->m_uiCount--;
	}
}

bool CEnumeratorSetInt64::NextStart()
{
	m_paNodes[0] = ((CSetInt64*)m_pSource)->m_pNodeRoot;
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
				if (ucLevel == 0) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetInt64::NextEnd; m_bHasNext = false; return false; }
				else              { m_caIndexes[ucLevel] = -1; ucLevel--; break; }
			}

			//Check whether there is a sub-node for the current index
			if (m_paNodes[ucLevel]->SubNode(j))
			{
				//If there is, we move down
				m_caIndexes[ucLevel] = j;
				m_paNodes[ucLevel + 1] = m_paNodes[ucLevel]->SubNode(j);
				ucLevel++;

				//If we reached a leave, we return true
				if (ucLevel == MAX_DEPTH_BAG_INT64) 
				{
					m_pCurrent = (CObject*)m_paNodes[MAX_DEPTH_BAG_INT64];

					m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetInt64::NextContinue;
					return true; 
				}
				else { m_caIndexes[ucLevel] = -1; break; }
			}
		}
	}
}

bool CEnumeratorSetInt64::NextContinue()
{
	BYTE ucLevel = MAX_DEPTH_BAG_INT64 - 1;

	for (;;)
	{
		//Start on the next index
		for (BYTE j = m_caIndexes[ucLevel] + 1; j < 17; j++)
		{
			//If we are finished with the current node
			if (j == 16) 
			{
				//We either move up or we are finished if we can't
				if (ucLevel == 0) { m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorSetInt64::NextEnd;  m_bHasNext = false; return false; }
				else              { m_caIndexes[ucLevel] = -1; ucLevel--; break; }
			}

			//Check whether there is a sub-node for the current index
			if (m_paNodes[ucLevel]->SubNode(j))
			{
				//If there is, we move down
				m_caIndexes[ucLevel] = j;
				m_paNodes[ucLevel + 1] = m_paNodes[ucLevel]->SubNode(j);
				ucLevel++;

				//If we reached a leave, we return true
				if (ucLevel == MAX_DEPTH_BAG_INT64) { m_pCurrent = (CObject*)m_paNodes[MAX_DEPTH_BAG_INT64]; return true; }
				else                                { m_caIndexes[ucLevel] = -1; break; }
			}
		}
	}
}

//bool CEnumeratorSetInt64::Next()
//{
//	BYTE ucLevel = MAX_DEPTH_BAG_INT64 - 1;
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
//				if (ucLevel == 0) { return false; }
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
//				//If we reached a leave, we return true
//				if (ucLevel == MAX_DEPTH_BAG_INT64) { return true; }
//				else                                { m_caIndexes[ucLevel] = -1; break; }
//			}
//		}
//	}
//}
