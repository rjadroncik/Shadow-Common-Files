#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryString.h"
#include "EnumeratorDictionaryStringRaw.h"

namespace SCFBase
{
	template<class TValue>
	class CEnumeratorDictionaryString : public CEnumeratorDictionaryStringRaw, public IEnumerator<TValue>
	{
		template<class TValue>
		friend class CDictionaryString;

	public:
		CEnumeratorDictionaryString(_IN CDictionaryString<TValue>& rDictionary) : CEnumeratorDictionaryStringRaw(rDictionary) {}
		CEnumeratorDictionaryString(_IN CDictionaryString<TValue>& rDictionary, _IN CString& rRootPath) : CEnumeratorDictionaryStringRaw(rDictionary, rRootPath) {}
		virtual ~CEnumeratorDictionaryString() {}

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		virtual bool Finished() _GET { return CEnumeratorRaw::ProtectedFinished(); }

	public:
		virtual TValue* Current() _GET { return (TValue*)CEnumeratorRaw::ProtectedCurrent(); }

	public:
		inline CString CurrentPath() _GET { return CEnumeratorDictionaryStringRaw::CurrentPath();; }
	};
};