#include "StdAfx.h"
#include "Dictionary.h"

using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

using namespace SCFPrivate;

void* CDFDictionary::AsType(_IN ENUM eType) const
{
	switch (eType)
	{
	case ClassDFDictionary: { return (CDFDictionary*)this; }
	}

	return NULL;
}

CDFDictionary::CDFDictionary()
{

}

CDFDictionary::~CDFDictionary()
{
}

CDictionaryNodeString* CDFDictionary::NameToNode(_IN CString& rName) _GET
{
	if (!m_pNodeFirst) { return NULL; }

	register CDictionaryNodeString* pNode = m_pNodeFirst;
	register UINT uiIndex = 0;

	//This allows us to stop the search automatically because
	//if the name is not in the dictionary, at some point [pNodeCurrent]
	//is assigned then value [NULL], through the [pNodeCurrent = pNodeCurrent->Next()] statement
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

	return NULL;
}

bool CDFDictionary::DirectoryRenameOrMove(_IN CString& rPathOld, _IN CString& rPathNew)
{
	register CDictionaryNodeString* pNodeOld = NameToNode(rPathOld);

	{
		CObject* pRecordOld = this->AtPut(rPathNew, pNodeOld->Object());
		if (pRecordOld) { delete pRecordOld; }
	}

	register CDictionaryNodeString* pNodeNew = NameToNode(rPathNew);




	return FALSE;
}

/*bool CDFDictionary::DirectoryDelete(_IN CString& rPath)
{
	register CDictionaryNodeString* pNode = m_pNodeFirst;
	UINT uiIndex = 0;

	CDictionaryNodeString* pPrevious = NULL;
	CDictionaryNodeString* pParent   = NULL;

	CDictionaryNodeString* pFoundPrevious = NULL;
	CDictionaryNodeString* pFoundParent   = NULL;

	UINT uiNodesToDelete = 0;

	//This allows us to stop the search automatically because
	//if the name is not in the dictionary, at some point [pNode]
	//is assigned then value [NULL], through the [pNode = pNode->Next()] statement
	while (pNode)
	{
		//Check for matching characters..
		if (pNode->Letter() == rPath[uiIndex])
		{
			//Decrease usage of the current node
			pNode->UsageRemove();

			//If this node is the holder of the immediate parent of the directory
			//we want to delete, store it's relative so we know how to perform 
			//the deletion
			if (((pNode->Letter() == '/') || (pNode->Letter() == '\\')) && (uiIndex < (rPath.Length() - 1)))
			{
				uiNodesToDelete = 0;
		
				pFoundPrevious = pPrevious;
				pFoundParent   = pParent;
			}

			uiNodesToDelete++;

			uiIndex++;
			//..If the last character matches.. we found our object
			if (uiIndex == rPath.Length()) 
			{
				delete pNode->Object(); 

				if (pFoundParent)
				{
					if (pFoundPrevious)
					{
						//Reconnect tree
						CDictionaryNodeString* pToDelete = pFoundPrevious->Next();
						pFoundPrevious->Next(pToDelete->Next());

						//Safely delete only what we want
						pToDelete->Next(NULL);
						this->NodeDelete(pToDelete);
					}
					else
					{
						//Reconnect tree
						CDictionaryNodeString* pToDelete = pFoundParent->ChildFirst();
						pFoundParent->ChildFirst(pToDelete->Next());

						//Safely delete only what we want
						pToDelete->Next(NULL);
						this->NodeDelete(pToDelete);
					}
				}
				else
				{
					if (pFoundPrevious)
					{
						//Reconnect tree
						CDictionaryNodeString* pToDelete = pFoundPrevious->Next();
						pFoundPrevious->Next(pToDelete->Next());

						//Safely delete only what we want
						pToDelete->Next(NULL);
						this->NodeDelete(pToDelete);
					}
					else
					{
						//Reconnect tree
						CDictionaryNodeString* pToDelete = m_pNodeFirst;
						m_pNodeFirst = pToDelete->Next();

						//Safely delete only what we want
						pToDelete->Next(NULL);
						this->NodeDelete(pToDelete);
					}
				}

				m_uiNodes -= uiNodesToDelete;
				return TRUE; 
			}
			//..If any other character matches, move down the tree
			else 
			{ 
				//Always keep track of relatives 
				pPrevious = NULL;
				pParent = pNode;

				pNode = pNode->ChildFirst();
			}
		}
		//..Character doesn't match, thus move on to the next sibling node
		else 
		{ 
			//Always keep track of relatives 
			pPrevious = pNode;

			pNode = pNode->Next();
		}
	}

	return FALSE;
}*/
