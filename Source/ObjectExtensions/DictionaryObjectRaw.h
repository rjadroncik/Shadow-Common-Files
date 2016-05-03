#pragma once

#include "Container.h"
#include "DictionaryNodeObject.h"
#include "Comparer.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	class OBJECT_EXTENSIONS_API CDictionaryObjectRaw : public CObject, public IContainer
	{
		friend class OBJECT_EXTENSIONS_API CEnumeratorDictionaryObjectRaw;

	public:
		CString ToString() _GET;

	protected:
		CDictionaryObjectRaw(_IN CComparer& rComparer, _IN bool bTakeOwnage = TRUE);
		virtual ~CDictionaryObjectRaw();

	protected:
		//Establishes a relation between the key & the object
		CObject* AtPut(_IN _REF CObject& rKey, _IN _REF CObject& rObject) _SET;

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		CObject* Remove(_IN CObject& rKey);

	protected:
		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		//Also deletes the key for the object, this means the key supplied in a previous call to [AtPut(..)]
		//Does NOT delete the argument [rKey] (unless it is the same object supplied in a previous call to [AtPut(..)])
		CObject* RemoveAndDeleteKey(_IN CObject& rKey);

	protected:
		bool Contains   (_IN CObject& rObject) _GET;
		bool ContainsKey(_IN CObject& rKey)    _GET { return (At(rKey) != NULL); }

	protected:
		//Object & name translation functions
		CObject* KeyOf(_IN CObject& rObject) _GET;
		CObject* At   (_IN CObject& rKey)    _GET; 

	public:
		//Removes all objects & keys without deleting them
		void AllRemove();

		//Removes & deletes all contained objects, keys are just removed
		void AllDelete();
	
		//Calls [Dispose()] on each object to prepare them for deletion, keys are just removed
		void AllDispose();

	public:
		//Removes & deletes all contained objects & keys
		void AllDeleteWithKeys();

		//Calls [Dispose()] on each object & key to prepare them for deletion
		void AllDisposeWithKeys();

	public:
		//Removes & deletes all keys, contained objects are just removed
		void AllDeleteOnlyKeys();

		//Calls [Dispose()] on each key to prepare them for deletion, contained objects are just removed
		void AllDisposeOnlyKeys();

	public:
		UINT Size()    _GET { return m_uiCount; }
		bool IsEmpty() _GET { return (m_uiCount == 0); }

	protected:
		//The root node of the AA-tree used to store the data & perform operations in O(log(n)), where n - number of stored key-value/object pairs 
		SCFPrivate::CDictionaryNodeObject* m_pNodeRoot;
		UINT m_uiCount;
	
	protected:
		//A fixed-size block heap used to store the dictionary nodes
		//SCFPrivate::CFSBHeap m_Heap;

	protected:
		const CComparer* m_pComparer;
		bool m_bComparerOwned;

	protected:
		CDictionaryObjectRaw();
	};
};