#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryNodeInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryInt64 : public CEnumeratorRaw, public IEnumerator<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CDictionaryInt64;

	public:
		CString ToString() _GET { return STRING("{EnumeratorDictionaryInt64}"); }

	public:
		CEnumeratorDictionaryInt64(_IN CDictionaryInt64& rDictionary);
		virtual ~CEnumeratorDictionaryInt64();

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }

	public:
		virtual CObject* Current() _GET { return CEnumeratorRaw::ProtectedCurrent(); }

	public:
		UINT64 CurrentKey() _GET;

	public:
		//After you use these, "garbage" is left in the dictionary & will be
		//removed only when the dictionary itself is destroyed
		void CurrentShallowRemove();
		void CurrentShallowDelete();
	
	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_pCurrent = NULL; return FALSE; }

	protected:
		SCFPrivate::CDictionaryNodeInt64* m_pNode;
	};
};