#include "Enumerator.h"
#include "Document.h"

using namespace SCFXML;

CXMLEnumerator::CXMLEnumerator(_IN _REF CXMLDocument& rDocument) : CEnumeratorRaw(rDocument)
{
	m_Stack.Push(*(CXMLNode*)m_pSource);
	m_pfNext = (ENUMERATOR_NEXT)&CXMLEnumerator::NextStart;
}

CXMLEnumerator::CXMLEnumerator(_IN _REF CXMLNode& rNode) : CEnumeratorRaw(rNode)
{
	m_Stack.Push(*(CXMLNode*)m_pSource);
	m_pfNext = (ENUMERATOR_NEXT)&CXMLEnumerator::NextStart;
}

CXMLEnumerator::~CXMLEnumerator()
{
}

bool CXMLEnumerator::NextStart()
{
	m_pCurrent = (CXMLNode*)m_pSource;

	m_pfNext = (ENUMERATOR_NEXT)&CXMLEnumerator::NextContinue;
	return TRUE;
}

bool CXMLEnumerator::NextContinue()
{
	//Check, whether we can move down the hierarchy from the current node..
	if (m_Stack.Top()->ChildFirst())
	{
		//..If we can. then we do so, the return value is stored in the [m_pCurrent] variable & returned in a statement below
		m_Stack.Push(*m_Stack.Top()->ChildFirst());
	}
	else
	{
		//..We cannot move down so we try moving right..
		if (m_Stack.Top()->Next())
		{
			CXMLNode* pTmpNode = m_Stack.Top();

			//We replace the top node in the stack with the right neighbor of the current node
			m_Stack.Pop();
			m_Stack.Push(*pTmpNode->Next());
		}
		else
		{
			//We are finished with a set of siblings, so we try moving up the tree
			//It may be that we will have to move up multiple levels, so we use a [while] statement 
			while (!m_Stack.IsEmpty() && !m_Stack.Top()->Next()) { m_Stack.Pop(); }

			//If we aren't finished yet
			if (m_Stack.Size() > 1)
			{
				//Now that we are on the right level, we again try to move right
				//We replace the top node in the stack with its right neighbor
				CXMLNode* pTmpNode = (CXMLNode*)m_Stack.Top();
				m_Stack.Pop();
				m_Stack.Push(*pTmpNode->Next());
			}
			else
			{
				//Pop the enumeration root node
				if (!m_Stack.IsEmpty()) { m_Stack.Pop(); }
			
				m_pfNext = (ENUMERATOR_NEXT)&CXMLEnumerator::NextEnd;
				return FALSE;
			}
		}
	}

	//Return always the current node (or more precisely the one that just became current)
	m_pCurrent = (CXMLNode*)m_Stack.Top();
	return TRUE;
}
