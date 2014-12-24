#include "EnumeratorDirectory.h"

#include <SCFObjectExtensionsPrivate.h>

using namespace SCF;
using namespace SCFBase;
using namespace SCFPrivate;
using namespace SCFDatafileIOPrivate;

CEnumeratorDirectory::CEnumeratorDirectory(_IN CDictionaryString<CRecord>& rDictionary, _IN CString& rDirectoryPath) : CEnumeratorDictionaryString(rDictionary, rDirectoryPath)
{
	_ASSERT((m_pRoot->Letter() == '\\') || (m_pRoot->Letter() == '/'));

	if ((m_pRoot->Letter() == '\\') || (m_pRoot->Letter() == '/'))
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDirectory::NextStart;
	}
	else
	{
		m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDirectory::NextEnd;
	}
}

CEnumeratorDirectory::~CEnumeratorDirectory()
{
} 

bool CEnumeratorDirectory::NextNode()
{
	//Check, whether we can move down the hierarchy from the current node..
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst() && (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Letter() != '\\'))
	{
		//..If we can. then we do so, the return value is stored in the [m_Stack.ppNodes[m_Stack.uiDepth - 1]] variable & returned in a statement below
		m_Stack.ppNodes[m_Stack.uiDepth] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst();
		m_Stack.uiDepth++;
	}
	else
	{
		//..We cannot move down so we try moving right..
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next())
		{
			//We replace the top node _IN the stack with the right neighbor of the current node
			m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
		}
		else
		{
			//We are finished with a set of siblings, so we try moving up the tree
			//It may be that we will have to move up multiple levels, so we use a [while] statement 
			while (m_Stack.uiDepth && !m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next()) { m_Stack.uiDepth--; }

			//If we aren't finished yet
			if (m_Stack.uiDepth)
			{
				//Now that we are on the right level, we again try to move right
				//We replace the top node in the stack with its right neighbor
				m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
			}
			//At the end of the traverse, the stack is empty
			else { return FALSE; }
		}
	}

	return TRUE;
}

bool CEnumeratorDirectory::NextStart()
{
	if (m_pRoot)
	{
		m_Stack.ppNodes[0] = m_pRoot; 

		//Check, whether we can move down the hierarchy from the (relative) ROOT node..
		if (m_pRoot->ChildFirst())
		{
			//..If we can. then we do so, the return value is stored in the [m_Stack.ppNodes[m_Stack.uiDepth - 1]] variable & returned in a statement below
			m_Stack.ppNodes[1] = m_pRoot->ChildFirst();
			m_Stack.uiDepth++;

			//If root doesn't contain an object, try to find the next one the classic way
			while (this->NextNode())
			{
				if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) 
				{
					m_pCurrent = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object();

					m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDirectory::NextContinue; 
					return TRUE;
				}
			}
		}
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDirectory::NextEnd;
	return FALSE; 
}

bool CEnumeratorDirectory::NextContinue()
{
	while (this->NextNode())
	{
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) 
		{
			m_pCurrent = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object(); return TRUE;
		}
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDirectory::NextEnd;
	return FALSE;
}

/*
bool CEnumeratorDirectory::NextNode()
{
	//At the end of the traverse, the stack is empty
	if (!m_Stack.uiDepth) { return FALSE; }

	//This condition makes sure we properly start the enumeration
	//(so that the enumeration starts with the root)
	if (m_Stack.ppNodes[0] == NULL) 
	{
		m_Stack.ppNodes[0] = m_pRoot; 
	
		//Check, whether we can move down the hierarchy from the ROOT node..
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst())
		{
			//..If we can. then we do so, the return value is stored in the [m_Stack.ppNodes[m_Stack.uiDepth - 1]] variable & returned in a statement below
			m_Stack.ppNodes[m_Stack.uiDepth] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst();
			m_Stack.uiDepth++;
			return TRUE; 
		}

		return FALSE; 
	}

	//Check, whether we can move down the hierarchy from the current node..
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst() && (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Letter() != '\\'))
	{
		//..If we can. then we do so, the return value is stored in the [m_Stack.ppNodes[m_Stack.uiDepth - 1]] variable & returned in a statement below
		m_Stack.ppNodes[m_Stack.uiDepth] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst();
		m_Stack.uiDepth++;
	}
	else
	{
		//..We cannot move down so we try moving right..
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next())
		{
			//We replace the top node _IN the stack with the right neighbor of the current node
			m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
		}
		else
		{
			//We are finished with a set of siblings, so we try moving up the tree
			//It may be that we will have to move up multiple levels, so we use a [while] statement 
			while (m_Stack.uiDepth && !m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next()) { m_Stack.uiDepth--; }
	
			//If we aren't finished yet
			if (m_Stack.uiDepth)
			{
				//Now that we are on the right level, we again try to move right
				//We replace the top node in the stack with its right neighbor
				m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
			}
			//At the end of the traverse, the stack is empty
			else { return FALSE; }
		}
	}

	return TRUE;
}

bool CEnumeratorDirectory::Next()
{
	while (this->NextNode())
	{
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) 
		{
			//Check whether this is a valid subdirectory
			if ((m_Stack.ppNodes[0]->Letter() != '\\') &&
				(m_Stack.ppNodes[0]->Letter() != '/'))
			{
				return FALSE;
			}
			else
			{
				return TRUE;
			}
		}
	}

	return FALSE;
}
*/