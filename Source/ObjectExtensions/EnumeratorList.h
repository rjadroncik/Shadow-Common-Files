#pragma once

#include "Enumerator.h"
#include "EnumeratorListRaw.h"
#include "ListRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorList : public CEnumeratorListRaw, public IEnumerator<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CListRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorList}"); }

	public:
		CEnumeratorList(_IN _REF CListRaw& rList) : CEnumeratorListRaw(rList) { }
		virtual ~CEnumeratorList() {}

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorListRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorListRaw::ProtectedHasNext(); }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		virtual bool Finished() _GET { return CEnumeratorListRaw::ProtectedFinished(); }

	public:
		virtual CObject* Current() _GET { return CEnumeratorListRaw::ProtectedCurrent(); }
	};
};