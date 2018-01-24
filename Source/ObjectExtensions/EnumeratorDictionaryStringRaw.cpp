#include "EnumeratorDictionaryStringRaw.h"
#include "DictionaryNodeString.h"

#include <malloc.h>

using namespace SCFBase;
using namespace SCFPrivate;

CEnumeratorDictionaryStringRaw::CEnumeratorDictionaryStringRaw(_IN CDictionaryStringRaw& rDictionary) : CEnumeratorRaw(rDictionary)
{
	m_pRoot     = rDictionary.m_pNodeFirst;
	m_pRootPath = nullptr;

	m_Stack.ppNodes = (CDictionaryNodeString**)malloc(sizeof(CDictionaryNodeString*) * 1024);
	m_Stack.uiDepth = 0;

	if (rDictionary.m_pNodeFirst)
	{
		//Push first node
		m_Stack.ppNodes[0] = nullptr;
		m_Stack.uiDepth++;
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextStart;

	m_bHasNext = rDictionary.m_uiCount > 0;
}

CEnumeratorDictionaryStringRaw::CEnumeratorDictionaryStringRaw(_IN CDictionaryStringRaw& rDictionary, _IN CString& rRootPath) : CEnumeratorRaw(rDictionary)
{
	m_pRoot = rDictionary.NameToNode(rRootPath);
	_ASSERTE(m_pRoot != nullptr);

	m_pRootPath = new CString(rRootPath);

	m_Stack.ppNodes = (CDictionaryNodeString**)malloc(sizeof(CDictionaryNodeString*) * 1024);
	m_Stack.uiDepth = 0;

	if (rDictionary.m_pNodeFirst)
	{
		//Push first node
		m_Stack.ppNodes[0] = nullptr;
		m_Stack.uiDepth++;
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextStart;
}

CEnumeratorDictionaryStringRaw::~CEnumeratorDictionaryStringRaw()
{
	free(m_Stack.ppNodes);

	if (m_pRootPath) { delete m_pRootPath; }
} 

void CEnumeratorDictionaryStringRaw::CurrentShallowRemove()
{
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1])
	{
		for (UINT i = 0; i < m_Stack.uiDepth; i++)
		{
			m_Stack.ppNodes[i]->UsageRemove();
		}

		//((CDictionaryStringRaw*)m_pDictionary)->Remove(this->CurrentPath());
		m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object(nullptr);
		((CDictionaryStringRaw*)m_pSource)->m_uiCount--;
	}
}

void CEnumeratorDictionaryStringRaw::CurrentShallowDelete()
{
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1])
	{
		for (UINT i = 0; i < m_Stack.uiDepth; i++) 
		{
			m_Stack.ppNodes[i]->UsageRemove();
		}

		CObject* pObject = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object();

		//((CDictionaryStringRaw*)m_pDictionary)->Remove(this->CurrentPath());
		m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object(nullptr);
		((CDictionaryStringRaw*)m_pSource)->m_uiCount--;

		delete pObject;
	}
}

bool CEnumeratorDictionaryStringRaw::NextStart()
{
	if (m_pRoot)
	{
		//Handle startup
		m_Stack.ppNodes[0] = m_pRoot; 

		if (m_pRoot->Object()) 
		{
			m_pCurrent = m_pRoot->Object();
			m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextContinue;

			return true;
		}

		//If root doesn't contain an object, try to find the next one the classic way
		while (this->NextNode())
		{
			if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) 
			{
				m_pCurrent = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object();
				m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextContinue; 

				return true;
			}
		}
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextEnd; m_bHasNext = false;
	return false;
}

bool CEnumeratorDictionaryStringRaw::NextContinue()
{
	while (this->NextNode())
	{
		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) { m_pCurrent = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object(); return true; }
	}

	m_pfNext = (ENUMERATOR_NEXT)&CEnumeratorDictionaryStringRaw::NextEnd; m_bHasNext = false;
	return false;
}

bool CEnumeratorDictionaryStringRaw::NextNode()
{
	//Check, whether we can move down the hierarchy from the current node..
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst())
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
			else { return false; }
		}
	}

	return true;
}

bool CEnumeratorDictionaryStringRaw::NextNodeForSerialization()
{
	//At the end of the traverse, the stack is empty
	if (!m_Stack.uiDepth) { return false; }

	//This condition makes sure we properly start the enumeration
	//(so that the enumeration starts with the root)
	if (m_Stack.ppNodes[0] == nullptr) { m_Stack.ppNodes[0] = m_pRoot; return true; }

	//Check, whether we can move down the hierarchy from the current node..
	if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst())
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
			else { return false; }
		}
	}

	return true;
}

//bool CEnumeratorDictionaryStringRaw::NextNode()
//{
//	//At the end of the traverse, the stack is empty
//	if (!m_Stack.uiDepth) { return false; }
//
//	//This condition makes sure we properly start the enumeration
//	//(so that the enumeration starts with the root)
//	if (m_Stack.ppNodes[0] == nullptr) { m_Stack.ppNodes[0] = m_pRoot; return true; }
//
//	//Check, whether we can move down the hierarchy from the current node..
//	if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst())
//	{
//		//..If we can. then we do so, the return value is stored in the [m_Stack.ppNodes[m_Stack.uiDepth - 1]] variable & returned in a statement below
//		m_Stack.ppNodes[m_Stack.uiDepth] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->ChildFirst();
//		m_Stack.uiDepth++;
//	}
//	else
//	{
//		//..We cannot move down so we try moving right..
//		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next())
//		{
//			//We replace the top node _IN the stack with the right neighbor of the current node
//			m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
//		}
//		else
//		{
//			//We are finished with a set of siblings, so we try moving up the tree
//			//It may be that we will have to move up multiple levels, so we use a [while] statement 
//			while (m_Stack.uiDepth && !m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next()) { m_Stack.uiDepth--; }
//	
//			//If we aren't finished yet
//			if (m_Stack.uiDepth)
//			{
//				//Now that we are on the right level, we again try to move right
//				//We replace the top node in the stack with its right neighbor
//				m_Stack.ppNodes[m_Stack.uiDepth - 1] = m_Stack.ppNodes[m_Stack.uiDepth - 1]->Next();
//			}
//			//At the end of the traverse, the stack is empty
//			else { return false; }
//		}
//	}
//
//	return true;
//}
//
//bool CEnumeratorDictionaryStringRaw::Next()
//{
//	while (this->NextNode())
//	{
//		if (m_Stack.ppNodes[m_Stack.uiDepth - 1]->Object()) { return true; }
//	}
//
//	return false;
//}

CString CEnumeratorDictionaryStringRaw::CurrentPath() _GET
{
	CString Path;

	if (m_pRootPath)
	{
		Path.Resize(m_pRootPath->Length() + m_Stack.uiDepth - 1);

		register const UINT uiLengthPath = m_pRootPath->Length() -1;
		register const UINT uiLengthFull = Path.Length();

		for (UINT i = 0; i < uiLengthPath; i++)            { Path.AtPut(i, m_pRootPath->At(i)); }
		for (UINT i = uiLengthPath; i < uiLengthFull; i++) { Path.AtPut(i, m_Stack.ppNodes[i - uiLengthPath]->Letter()); }

		return Path;
	}
	else
	{
		Path.Resize(m_Stack.uiDepth);

		register const UINT uiLengthFull = m_Stack.uiDepth;
	
		for (UINT i = 0; i < uiLengthFull; i++) { Path.AtPut(i, m_Stack.ppNodes[i]->Letter()); }

		return Path;
	}
}
