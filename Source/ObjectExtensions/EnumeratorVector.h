#pragma once

#include "Enumerator.h"
#include "EnumeratorVectorRaw.h"

namespace SCFBase
{
	template<class TValue>
	class CEnumeratorVector : public CEnumeratorVectorRaw, public IEnumerator<TValue>
	{
		template<class TValue>
		friend class CVector;

	public:
		CEnumeratorVector(_IN _REF CVector<TValue> & rVector) : CEnumeratorVectorRaw(rVector) { }
		virtual ~CEnumeratorVector() {}

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