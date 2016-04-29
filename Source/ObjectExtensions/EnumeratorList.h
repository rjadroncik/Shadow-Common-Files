#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "List.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorList : public CEnumeratorRaw, public CEnumerator<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CListRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorList}"); }

	public:
		CEnumeratorList(_IN CListRaw& rList);
		virtual ~CEnumeratorList();

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

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CListNode* m_pNode;
		SCF::BYTE m_ucIndex;
	};
};