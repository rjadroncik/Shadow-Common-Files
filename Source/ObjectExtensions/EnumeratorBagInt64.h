#pragma once

#include "EnumeratorRaw.h"
#include "BagInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagInt64 : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CBagInt64;

	public:
		CString ToString() _GET { return STRING("{EnumeratorBagInt64}"); }

	public:
		CEnumeratorBagInt64(_IN CBagInt64& rBag);
		virtual ~CEnumeratorBagInt64();

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		bool Finished() _GET { return CEnumeratorRaw::ProtectedFinished(); }

 	public:
 		//This is correct :)
 		SCF::UINT64 Current() _GET { return (SCF::UINT64)m_paNodes[MAX_DEPTH_BAG_INT64]; }

	public:
		void CurrentRemove();
		void CurrentDelete();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CBagNodeInt64* m_paNodes  [MAX_DEPTH_BAG_INT64 + 1];
		char                       m_caIndexes[MAX_DEPTH_BAG_INT64];
	};
};