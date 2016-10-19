#pragma once

#include "Enumerator.h"
#include "EnumeratorListRaw.h"
#include "ListRaw.h"

namespace SCFBase
{
	template<class TValue>
	class CEnumeratorList : public CEnumeratorListRaw, public IEnumerator<TValue>
	{
		template<class TValue>
		friend class CList;

	public:
		CEnumeratorList(_IN _REF CList<TValue>& rList) : CEnumeratorListRaw(rList) { }
		virtual ~CEnumeratorList() {}

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorListRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorListRaw::ProtectedHasNext(); }

	public:
		virtual TValue* Current() _GET { return (TValue*)CEnumeratorListRaw::ProtectedCurrent(); }
	};
};