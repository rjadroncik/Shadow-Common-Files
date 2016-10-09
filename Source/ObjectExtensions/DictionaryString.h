#pragma once

#include "String.h"
#include "DictionaryStringRaw.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	template<class TValue> 
	class CDictionaryString : public CDictionaryStringRaw, public IContainer<TValue>
	{
		template<class TValue>
		friend class CEnumeratorDictionaryString;

	public:
		inline CDictionaryString() : CDictionaryStringRaw() {}
		inline virtual ~CDictionaryString() {}

	public:
		inline UINT Size()    _GET { return m_uiCount; }
		inline bool IsEmpty() _GET { return (m_uiCount == 0); }

		inline IEnumerator<TValue>& NewEnumerator() _GET { return *(new CEnumeratorDictionaryString<TValue>(*this)); }

	public:
		//Establishes a relation between the key & the object
		inline TValue* AtPut(_IN CString& rName, _IN _REF TValue& rObject) _SET { return (TValue*)CDictionaryStringRaw::AtPut(rName, rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, returns TRUE, if object was removed, FALSE otherwise
		inline bool RemoveValue(_IN TValue& rObject) { return CDictionaryStringRaw::RemoveValue(rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		inline TValue* RemoveKey(_IN CString& rName) { return (TValue*)CDictionaryStringRaw::RemoveKey(rName); }

	public:
		inline bool Contains(_IN TValue& rObject) _GET { return CDictionaryStringRaw::Contains(rObject); }

		//Object & name translation functions
		inline const CString  NameOf(_IN TValue& rObject) _GET { return CDictionaryStringRaw::NameOf(rObject); } 
		inline       TValue*  At    (_IN CString& rName)  _GET { return (TValue*)CDictionaryStringRaw::At(rName); } 
	};
}