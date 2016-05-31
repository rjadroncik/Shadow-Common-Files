#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryObject.h"
#include "EnumeratorDictionaryObjectRaw.h"

namespace SCFBase
{
	template<class TKey, class TValue>
	class CEnumeratorDictionaryObject : public CEnumeratorDictionaryObjectRaw, public IEnumerator<CObject>
	{
		template<class TKey, class TValue>
		friend class CDictionaryObject;

	public:
		CString ToString() _GET { return STRING("{EnumeratorDictionaryObject}"); }

	public:
		CEnumeratorDictionaryObject(_IN CDictionaryObject<TKey, TValue>& rDictionary) : CEnumeratorDictionaryObjectRaw(rDictionary) {}
		virtual ~CEnumeratorDictionaryObject() {}

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		virtual bool Finished() _GET { return CEnumeratorRaw::ProtectedFinished(); }

	public:
		virtual CObject* Current() _GET { return CEnumeratorRaw::ProtectedCurrent(); }

	public:
		inline TKey* CurrentKey() _GET { return (TKey*)CEnumeratorDictionaryObjectRaw::CurrentKey(); }
	};
};