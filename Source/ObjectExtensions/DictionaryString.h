#pragma once

#include "String.h"
#include "DictionaryStringRaw.h"

namespace SCFBase
{
	//Represents a dictionary which implements a relation of the type key <-> value/object,
	//where the translation key -> value/object is very fast, while the reverse one is slow
	template<class T> 
	class CDictionaryString : public CDictionaryStringRaw
	{
	//public:
	//	CEnumerator* EnumeratorNew() _GET;

	public:
		inline CDictionaryString() {}
		inline virtual ~CDictionaryString() {}

	public:
		//Establishes a relation between the key & the object
		inline T* AtPut(_IN CString& rName, _IN _REF T& rObject) _SET { return (T*)CDictionaryStringRaw::AtPut(rName, rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, returns TRUE, if object was removed, FALSE otherwise
		inline bool RemoveValue(_IN T& rObject) { return CDictionaryStringRaw::RemoveValue(rObject); }

		//Removes an object from the dictionary, DOES NOT delete it!, the return value is the removed object
		inline T* RemoveKey(_IN CString& rName) { return (T*)CDictionaryStringRaw::RemoveKey(rName); }

	public:
		inline bool Contains(_IN T& rObject) _GET { return CDictionaryStringRaw::Contains(rObject); }

		//Object & name translation functions
		inline const CString  NameOf(_IN T& rObject) _GET { return     CDictionaryStringRaw::NameOf(rObject); } 
		inline       T*       At(_IN CString& rName) _GET { return (T*)CDictionaryStringRaw::At(rName); } 
	};
}