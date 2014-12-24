#pragma once

#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumerator : public CObject
	{
	public:
		typedef bool (__thiscall CEnumerator::*ENUMERATOR_NEXT) ();

	public:
		CEnumerator(_IN _REF CObject& rSource);
		virtual ~CEnumerator();

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		inline bool Next()     { return CALL_MEMBER(*this, m_pfNext)(); }

	public:
		//Returns true while there still is a next element to be enumerated
		inline bool HasNext()  { return m_bHasNext; }
		//Returns true if we already queried past the end of the enumeration, that is Next() already returned FALSE 
		inline bool Finished() { return m_bFinished; }

	public:
		inline CObject* Current() _GET { _ASSERTE(!m_bFinished && m_pCurrent); return m_pCurrent; }
		inline CObject& Source()  _GET { return *m_pSource; }

	protected:
		ENUMERATOR_NEXT m_pfNext;

	protected:
		CObject* m_pCurrent;
		CObject* m_pSource;

	protected:
		bool m_bHasNext;
		bool m_bFinished;
	};
};