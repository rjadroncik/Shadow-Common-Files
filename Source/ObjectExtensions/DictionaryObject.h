#pragma once

#include "DictionaryObjectRaw.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	template<class TKey, class TValue> 
	class CDictionaryObject : public CDictionaryObjectRaw, public IContainer<TValue>
	{
		template<class TKey, class TValue>
		friend class CEnumeratorDictionaryObject;

	public:
		inline CDictionaryObject(_IN CComparer& rComparer, _IN bool bTakeOwnage = TRUE) : CDictionaryObjectRaw(rComparer, bTakeOwnage) {}
		inline virtual ~CDictionaryObject() {}

	public:
		//Establishes a relation between the key & the object
		inline TValue* AtPut(_IN _REF TKey& rKey, _IN _REF TValue& rObject) _SET { return (TValue*)CDictionaryObjectRaw::AtPut(rKey, rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		inline TValue* Remove(_IN TKey& rKey) { return (TValue*)CDictionaryObjectRaw::Remove(rKey); }

	public:
		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		//Also deletes the key for the object, this means the key supplied in a previous call to [AtPut(..)]
		//Does NOT delete the argument [rKey] (unless it is the same object supplied in a previous call to [AtPut(..)])
		inline TValue* RemoveAndDeleteKey(_IN TKey& rKey) { return (TValue*)CDictionaryObjectRaw::RemoveAndDeleteKey(rKey); }

	public:
		inline bool Contains   (_IN TValue& rObject) _GET { return CDictionaryObjectRaw::Contains(rObject); }
		inline bool ContainsKey(_IN TKey& rKey)      _GET { return CDictionaryObjectRaw::ContainsKey(rKey); }

	public:
		//Object & name translation functions
		inline TKey*   KeyOf(_IN TValue& rObject) _GET { return (TKey*)  CDictionaryObjectRaw::KeyOf(rObject); }
		inline TValue* At   (_IN TKey& rKey)      _GET { return (TValue*)CDictionaryObjectRaw::At(rKey); }

	public:
		UINT Size()    _GET { return CDictionaryObjectRaw::Size(); }
		bool IsEmpty() _GET { return CDictionaryObjectRaw::IsEmpty(); }

	protected:
		inline CDictionaryObject() {}
	};
}