#include "DictionaryString.h"
#include "DictionaryNodeString.h"
#include "String.h"
#include "EnumeratorDictionaryString.h"

#include <malloc.h>

using namespace SCFBase;
using namespace SCFPrivate;

CDictionaryStringRaw::CDictionaryStringRaw()
{
	m_pNodeFirst = nullptr;
	m_uiNodes = 0;
	m_uiCount = 0;
}

CDictionaryStringRaw::~CDictionaryStringRaw()
{
	if (m_pNodeFirst) 
	{
		CDictionaryNodeString::Delete(m_pNodeFirst); 
	}
}

CObject* CDictionaryStringRaw::AtPut(_IN CString& rName, _IN _REF CObject& rObject) _SET
{
	register CDictionaryNodeString* pNodeCurrent = m_pNodeFirst;
	register UINT uiIndex = 0;

	//If there are no nodes yet..
	if (!pNodeCurrent)
	{
		//Create the first node
		m_pNodeFirst = CDictionaryNodeString::Create(rName[uiIndex]);
		m_uiNodes++;

		//If we already are at the last letter of the name, store the object pointer
		if (uiIndex == (rName.Length() - 1)) { m_pNodeFirst->Object(&rObject); m_pNodeFirst->UsageAdd(); m_uiCount++; return nullptr; }

		//Update current node
		pNodeCurrent = m_pNodeFirst;
	}

	//While we still have letters to process..
	while (uiIndex < rName.Length())
	{
		//If we have a match of letter at the current node
		if (pNodeCurrent->Letter() == rName[uiIndex])
		{
			//The node containing the last letter of a name always stores/references the object
			//If there is already an object with the given name, replace it
			if (uiIndex == (rName.Length() - 1)) 
			{ 
				CObject* pPrevious = pNodeCurrent->Object(); 
				pNodeCurrent->Object(&rObject); 

				//Add usage to unused leaf node
				if (!pPrevious) { pNodeCurrent->UsageAdd(); m_uiCount++; }

				return pPrevious;
			}

			//Add usage to each node that is part of the object name
			pNodeCurrent->UsageAdd();

			//Move on to the next character
			uiIndex++;

			//If the current node has children, move down the hierarchy
			if (pNodeCurrent->ChildFirst()) { pNodeCurrent = pNodeCurrent->ChildFirst(); continue; }
			else
			{
				//Make a node
				CDictionaryNodeString* pNewNode = CDictionaryNodeString::Create(rName[uiIndex]);
				m_uiNodes++;

				//Make the node the first child of the current node
				pNodeCurrent->ChildFirst(pNewNode);

				//Store value, if this node represents the last letter of the name
				if (uiIndex == (rName.Length() - 1)) { pNewNode->Object(&rObject); pNewNode->UsageAdd(); m_uiCount++; return nullptr; }

				//Update current node & continue
				pNodeCurrent = pNewNode;
				continue;
			}
		}

		//If we can still find the letter we are looking for
		if (pNodeCurrent->Letter() < rName[uiIndex])
		{
			//If the current node has siblings, move on to the next sibling
			if (pNodeCurrent->Next()) { pNodeCurrent = pNodeCurrent->Next(); continue; }
			else
			{
				//Make a node
				CDictionaryNodeString* pNewNode = CDictionaryNodeString::Create(rName[uiIndex]);
				m_uiNodes++;

				//Make the node the next sibling of the current node
				pNodeCurrent->Next(pNewNode);

				//Store value, if this node represents the last letter of the name
				if (uiIndex == (rName.Length() - 1)) { pNewNode->Object(&rObject); pNewNode->UsageAdd(); m_uiCount++; return nullptr; }

				//Update current node & continue
				pNodeCurrent = pNewNode;
				continue;
			}
		}

		//If we are already past the letter we are looking for
		if (pNodeCurrent->Letter() > rName[uiIndex])
		{
			//Make a copy of the current node
			CDictionaryNodeString* pNewNode = CDictionaryNodeString::CreateCopy(pNodeCurrent);
			m_uiNodes++;

			//Modify current node, so that [pNewNode] can be inserted after,
			//BUT logically we insert a new node BEFORE the current one,
			//this is done to avoid the necessity of updating the parent node 
			//when the address of its first node changes, by doing it this way, 
			//the address does NOT change, instead we exchange the contents	of the node
			pNodeCurrent->Letter    (rName[uiIndex]);
			pNodeCurrent->ChildFirst(nullptr);
			pNodeCurrent->Next      (pNewNode);

			//FFS.. just zero-ing the usage count
			//while (pNodeCurrent->Usage()) { pNodeCurrent->UsageRemove(); }
			pNodeCurrent->UsageReset();
			
			//Store value, if this node represents the last letter of the name
			if (uiIndex == (rName.Length() - 1)) { pNodeCurrent->Object(&rObject); pNodeCurrent->UsageAdd(); m_uiCount++; return nullptr; }
			else                                 { pNodeCurrent->Object(nullptr); }

			//We leave the [pNodeCurrent] variable unchanged!!!, we just continue
			continue;
		}
	}

	return nullptr;
}

CDictionaryNodeString* CDictionaryStringRaw::NameToNode(_IN CString& rName) _GET 
{
	if (!m_pNodeFirst) { return nullptr; }

	register CDictionaryNodeString* pNode = m_pNodeFirst;
	register UINT uiIndex = 0;

	//This allows us to stop the search automatically because
	//if the name is not in the dictionary, at some point [pNodeCurrent]
	//is assigned then value [nullptr], through the [pNodeCurrent = pNodeCurrent->Next()] statement
	while (pNode)
	{
		//Check for matching characters..
		if (pNode->Letter() == rName[uiIndex])
		{
			uiIndex++;
			//..If the last character matches.. we found our object
			if (uiIndex == rName.Length()) { return pNode; }
			//..If any other character matches, move down the tree
			else { pNode = pNode->ChildFirst(); }
		}
		//..Character doesn't match, thus move on to the next sibling node
		else { pNode = pNode->Next(); }
	}

	return nullptr;
}

CObject* CDictionaryStringRaw::At(_IN CString& rName) _GET
{
	if (!m_pNodeFirst) { return nullptr; }

	register CDictionaryNodeString* pNode = m_pNodeFirst;
	register UINT uiIndex = 0;

	//This allows us to stop the search automatically because
	//if the name is not in the dictionary, at some point [pNodeCurrent]
	//is assigned then value [nullptr], through the [pNodeCurrent = pNodeCurrent->Next()] statement
	while (pNode)
	{
		//Check for matching characters..
		if (pNode->Letter() == rName[uiIndex])
		{
			uiIndex++;
			//..If the last character matches.. we found our object
			if (uiIndex == rName.Length()) { return pNode->Object(); }
			//..If any other character matches, move down the tree
			else { pNode = pNode->ChildFirst(); }
		}
		//..Character doesn't match, thus move on to the next sibling node
		else { pNode = pNode->Next(); }
	}

	return nullptr;
}

bool CDictionaryStringRaw::Contains(_IN CObject& rObject) const
{
	if (!m_pNodeFirst) { return false; }

	CEnumeratorDictionaryStringRaw Enumerator(*this);

	while (Enumerator.ProtectedNext())
	{
		if (Enumerator.ProtectedCurrent() == &rObject) { return true; }
	}

	return false;
}

bool CDictionaryStringRaw::ContainsName(_IN CString& rName) _GET
{
	if (!m_pNodeFirst) { return false; }

	register CDictionaryNodeString* pNode = m_pNodeFirst;
	register UINT uiIndex = 0;

	while (pNode)
	{
		if (pNode->Letter() == rName[uiIndex])
		{
			uiIndex++;

			if ((uiIndex == rName.Length()) && pNode->Object()) { return true; }
			else { pNode = pNode->ChildFirst(); }
		}
		else { pNode = pNode->Next(); }
	}

	return false;
}

const CString CDictionaryStringRaw::NameOf(_IN CObject& rObject) _GET
{
	//Return a zero length string to indicate that object was not found
	if (!m_pNodeFirst) { return CString(); }

	static CDictionaryNodeString** s_ppStackNodes = (CDictionaryNodeString**)malloc(sizeof(CDictionaryNodeString*) * 1024);
	register UINT uiStackDepth = 0;

	//Push first node
	s_ppStackNodes[uiStackDepth] = m_pNodeFirst;
	uiStackDepth++;

	while (uiStackDepth && s_ppStackNodes[uiStackDepth - 1])
	{
		if (s_ppStackNodes[uiStackDepth - 1]->Object() == &rObject)
		{
			//Create temporary string
			CString TmpString;
			TmpString.Resize(uiStackDepth);

			//Fill string in reverse order by popping the stack
			for (UINT i = 0; i < uiStackDepth; i++) { TmpString.AtPut(i, s_ppStackNodes[i]->Letter()); }

			return TmpString;
		}

		//Check, whether we can move down the hierarchy from the current node..
		if (s_ppStackNodes[uiStackDepth - 1]->ChildFirst())
		{
			//..If we can. then we do so, the return value is stored in the [m_pEnumCurrent] variable & returned in a statement below
			s_ppStackNodes[uiStackDepth] = s_ppStackNodes[uiStackDepth - 1]->ChildFirst();
			uiStackDepth++;
		}
		else
		{
			//..We cannot move down so we try moving right..
			if (s_ppStackNodes[uiStackDepth - 1]->Next())
			{
				//We replace the top node in the stack with the right neighbor of the current node
				s_ppStackNodes[uiStackDepth - 1] = s_ppStackNodes[uiStackDepth - 1]->Next();
			}
			else
			{
				//We are finished with a set of siblings, so we try moving up the tree
				//It may be that we will have to move up multiple levels, so we use a [while] statement 
				while (uiStackDepth && !s_ppStackNodes[uiStackDepth - 1]->Next()) { uiStackDepth--; }

				//If we aren't finished yet
				if (uiStackDepth)
				{
					//Now that we are on the right level, we again try to move right
					//We replace the top node in the stack with its right neighbor
					s_ppStackNodes[uiStackDepth - 1] = s_ppStackNodes[uiStackDepth - 1]->Next();
				}
			}
		}
	}

	//Return a zero length string to indicate that object was not found
	return CString();
}

CObject* CDictionaryStringRaw::RemoveKey(_IN CString& rName)
{
	register CDictionaryNodeString* pNode = m_pNodeFirst;
	UINT uiIndex = 0;

	CDictionaryNodeString* pPrevious = nullptr;
	CDictionaryNodeString* pParent   = nullptr;

	bool bFoundUnused = false;
	UINT uiNodesToDelete = 0;

	//This allows us to stop the search automatically because
	//if the name is not in the dictionary, at some point [pNode]
	//is assigned then value [nullptr], through the [pNode = pNode->Next()] statement
	while (pNode)
	{
		//Check for matching characters..
		if (pNode->Letter() == rName[uiIndex])
		{
			//Decrease usage of the current node
			pNode->UsageRemove();

			//This will stop the updating of the [pPrevious] & [pParent] variables
			//as we traverse the tree so the the identify the node to be deleted
			if (!bFoundUnused && (pNode->Usage() == 0)) { bFoundUnused = true; }

			if (bFoundUnused) { uiNodesToDelete++; }

			uiIndex++;
			//..If the last character matches.. we found our object
			if (uiIndex == rName.Length()) 
			{
				//The only difference against the [At(..)] function
				//is that we prune the tree after we find the object
				CObject* pObject = pNode->Object(); 

				if (bFoundUnused)
				{
					if (pParent)
					{
						if (pPrevious)
						{
							//Reconnect tree
							CDictionaryNodeString* pToDelete = pPrevious->Next();
							pPrevious->Next(pToDelete->Next());

							//Safely destroy only what we want
							pToDelete->Next(nullptr);
							CDictionaryNodeString::Delete(pToDelete); 
						}
						else
						{
							//Reconnect tree
							CDictionaryNodeString* pToDelete = pParent->ChildFirst();
							pParent->ChildFirst(pToDelete->Next());

							//Safely destroy only what we want
							pToDelete->Next(nullptr);
							CDictionaryNodeString::Delete(pToDelete); 
						}
					}
					else
					{
						if (pPrevious)
						{
							//Reconnect tree
							CDictionaryNodeString* pToDelete = pPrevious->Next();
							pPrevious->Next(pToDelete->Next());

							//Safely destroy only what we want
							pToDelete->Next(nullptr);
							CDictionaryNodeString::Delete(pToDelete); 
						}
						else
						{
							//Reconnect tree
							CDictionaryNodeString* pToDelete = m_pNodeFirst;
							m_pNodeFirst = pToDelete->Next();

							//Safely destroy only what we want
							pToDelete->Next(nullptr);
							CDictionaryNodeString::Delete(pToDelete); 
						}
					}
				}
				else 
				{ 
					pNode->Object(nullptr);
				}

				m_uiNodes -= uiNodesToDelete;

				m_uiCount--;
				return pObject; 
			}
			//..If any other character matches, move down the tree
			else 
			{ 
				if (!bFoundUnused)
				{
					pPrevious = nullptr;
					pParent = pNode;
				}
				pNode = pNode->ChildFirst();
			}
		}
		//..Character doesn't match, thus move on to the next sibling node
		else 
		{ 
			if (!bFoundUnused)
			{
				pPrevious = pNode;
			}
			pNode = pNode->Next();
		}
	}

	return nullptr;
}

bool CDictionaryStringRaw::RemoveValue(_IN CObject& rObject)
{
	if (!m_pNodeFirst) { return false; }

	CEnumeratorDictionaryStringRaw Enumerator(*this);

	while (Enumerator.ProtectedNext())
	{
		if (Enumerator.ProtectedCurrent() == &rObject)
		{
			RemoveKey(Enumerator.CurrentPath());
			return true;
		}
	}

	return false;
}

void CDictionaryStringRaw::AllRemove()
{
	if (m_pNodeFirst) 
	{ 
		CDictionaryNodeString::Delete(m_pNodeFirst); 
		m_pNodeFirst = nullptr; 
	}
	m_uiNodes = 0;
	m_uiCount = 0;

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryString_Heap.AllocatedBytes() == 0);
}

void CDictionaryStringRaw::AllDelete()
{
	if (m_pNodeFirst) 
	{ 
		CDictionaryNodeString::DeleteWithObject(m_pNodeFirst); 
		m_pNodeFirst = nullptr; 
	}
	m_uiNodes = 0;
	m_uiCount = 0;

	//Worked when heap was local for each dictionary
	//_ASSERTE(DictionaryString_Heap.AllocatedBytes() == 0);
}

void CDictionaryStringRaw::AllDispose()
{
	if (!m_pNodeFirst) { return; }

	CEnumeratorDictionaryStringRaw Enumerator(*this);
	while (Enumerator.ProtectedNext()) { Enumerator.ProtectedCurrent()->Dispose(); }
}

CString CDictionaryStringRaw::ToString() _GET { return STRING("{DictionaryString}"); }
