#pragma once

#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorRaw : public CObject
	{
	public:
		typedef bool(__thiscall CEnumeratorRaw::*ENUMERATOR_NEXT) ();

	public:
		inline CObject& Source()  _GET { return *m_pSource; }
		
	protected:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		inline bool ProtectedNext() { return CALL_MEMBER(*this, m_pfNext)(); }

	protected:
		//Returns true while there still is a next element to be enumerated
		inline bool ProtectedHasNext() _GET { return m_bHasNext; }

	protected:
		inline CObject* ProtectedCurrent() _GET { _ASSERTE(m_pCurrent); return m_pCurrent; }

	protected:
		ENUMERATOR_NEXT m_pfNext;

	protected:
		CObject* m_pCurrent;
		CObject* m_pSource;

	protected:
		bool m_bHasNext;

	protected:
		CEnumeratorRaw(_IN _REF CObject& rSource);
		virtual ~CEnumeratorRaw();
	};
};
