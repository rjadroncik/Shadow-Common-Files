#pragma once

#include "Container.h"
#include "DictionaryInt64Raw.h"
#include "EnumeratorDictionaryInt64.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	template<class TValue>
	class CDictionaryInt64 : public CDictionaryInt64Raw, public IContainer<TValue>
	{
		template<class TValue>
		friend class CEnumeratorDictionaryInt64;

	public:
		CDictionaryInt64() : CDictionaryInt64Raw() {}
		virtual ~CDictionaryInt64() {}

	public:
		UINT Size()    _GET { return CDictionaryInt64Raw::Size(); }
		bool IsEmpty() _GET { return CDictionaryInt64Raw::IsEmpty(); }

		inline IEnumerator<TValue>& NewEnumerator() _GET { return *(new CEnumeratorDictionaryInt64<TValue>(*this)); }

	public:
		//Establishes a relation between the key & the object
		inline TValue* AtPut(_IN UINT64 ui64Key, _IN _REF TValue& rObject) _SET { return (TValue*)CDictionaryInt64Raw::AtPut(ui64Key, rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		inline TValue* Remove(_IN UINT64 ui64Key) { return (TValue*)CDictionaryInt64Raw::Remove(ui64Key); }

	public:
		inline bool Contains(_IN TValue& rObject) _GET { return CDictionaryInt64Raw::Contains(rObject); }

	public:
		//Object & name translation functions
		inline UINT64 KeyOf(_IN TValue& rObject) _GET { return CDictionaryInt64Raw::KeyOf(rObject); }
		inline TValue* At(_IN UINT64 ui64Key) _GET { return (TValue*)CDictionaryInt64Raw::At(ui64Key); }
	};
};