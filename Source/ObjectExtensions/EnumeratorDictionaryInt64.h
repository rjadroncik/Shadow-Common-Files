#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryInt64.h"
#include "EnumeratorDictionaryInt64Raw.h"

namespace SCFBase
{
	template<class TValue>
	class CEnumeratorDictionaryInt64 : public CEnumeratorDictionaryInt64Raw, public IEnumerator<TValue>
	{
		template<class TValue>
		friend class CDictionaryInt64;

	public:
		CEnumeratorDictionaryInt64(_IN CDictionaryInt64<TValue>& rDictionary) : CEnumeratorDictionaryInt64Raw(rDictionary) {}
		virtual ~CEnumeratorDictionaryInt64() {}

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }

	public:
		virtual TValue* Current() _GET { return (TValue*)CEnumeratorRaw::ProtectedCurrent(); }
	};
};