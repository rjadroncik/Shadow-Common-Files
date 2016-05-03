#pragma once

#include "EnumeratorRaw.h"

namespace SCFBase
{
	template<class TValue>
	class IEnumerator
	{
	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() = 0;

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET = 0;
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		virtual bool Finished() _GET = 0;

	public:
		virtual TValue* Current() _GET = 0;

	protected:
		virtual ~IEnumerator() {}
	};
};