#pragma once

#include "Container.h"
#include "DictionaryNodeString.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type string <-> object,
	//where the translation string -> object is very fast, while the reverse one is slow
	class OBJECT_EXTENSIONS_API CDictionaryStringRaw : public CContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorDictionaryString;

	public:
		CString ToString() _GET;

	protected:
		CDictionaryStringRaw();
		virtual ~CDictionaryStringRaw();

	public:
		//Establishes a relation between the name & the object
		CObject* AtPut(_IN CString& rName, _IN _REF CObject& rObject) _SET;

		//Removes an object from the dictionary, DOES NOT delete it!, returns TRUE, if object was removed, FALSE otherwise
		bool RemoveValue(_IN CObject& rObject);

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		CObject* RemoveKey(_IN CString& rName);

	public:
		bool Contains    (_IN CObject& rObject) _GET;
		bool ContainsName(_IN CString& rName)   _GET;

		//Object & name translation functions
		const CString  NameOf(_IN CObject& rObject) _GET;
		      CObject* At    (_IN CString& rName)   _GET; 

	public:
		//Removes all objects without deleting them
		void AllRemove();
	
		//Removes & deletes all contained objects
		void AllDelete();
	
		//Calls [Dispose()] on each object to prepare them for deletion
		void AllDispose();

	public:
		UINT Size()    _GET { return m_uiCount; }
		bool      IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		//Needed for dictionary content enumeration
		SCFPrivate::CDictionaryNodeString* NameToNode(_IN CString& rName) const;

	protected:
		//The first node, corresponding to the "smallest" first letter of all the words stored _IN the tree
		SCFPrivate::CDictionaryNodeString* m_pNodeFirst;
		UINT m_uiNodes;
		UINT m_uiCount;

	protected:
		//A fixed-size block heap used to store the dictionary nodes
		//SCFPrivate::CFSBHeap m_Heap;
	};
};
