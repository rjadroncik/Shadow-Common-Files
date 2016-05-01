#pragma once

#include "Container.h"
#include "DictionaryNodeInt64.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	class OBJECT_EXTENSIONS_API CDictionaryInt64 : public CContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorDictionaryInt64;

	public:
		CString ToString() _GET;

	public:
		CDictionaryInt64();
		virtual ~CDictionaryInt64();

	public:
		//Establishes a relation between the key & the object
		CObject* AtPut(_IN UINT64 ui64Key, _IN _REF CObject& rObject) _SET;

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		CObject* Remove(_IN UINT64 ui64Key);

	public:
		bool Contains   (_IN CObject&    rObject) _GET;
		bool ContainsKey(_IN UINT64 ui64Key) _GET { return (At(ui64Key) != NULL); }

	public:
		//Object & name translation functions
		UINT64 KeyOf(_IN CObject&    rObject) _GET;
		CObject*    At   (_IN UINT64 ui64Key) _GET; 

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
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CDictionaryNodeInt64* m_pNodeRoot;
		UINT m_uiCount;
	
	protected:
		//A fixed-size block heap used to store the dictionary nodes
		//SCFPrivate::CFSBHeap m_Heap;
	};
};