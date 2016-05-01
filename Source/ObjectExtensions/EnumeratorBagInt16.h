#pragma once

#include "EnumeratorRaw.h"
#include "BagInt16.h"

#ifdef WIN32
#else
#include <inttypes.h>
#endif

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagInt16 : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CBagInt16;

	public:
		CString ToString() _GET { return STRING("{EnumeratorBagInt16}"); }

	public:
		CEnumeratorBagInt16(_IN CBagInt16& rBag);
		virtual ~CEnumeratorBagInt16();

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
 		USHORT Current() _GET { return (USHORT)(intptr_t)(void*)m_paNodes[MAX_DEPTH_BAG_INT16]; }

	public:
		void CurrentRemove();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CBagNodeInt16* m_paNodes  [MAX_DEPTH_BAG_INT16 + 1];
		char                       m_caIndexes[MAX_DEPTH_BAG_INT16];
	};
};
