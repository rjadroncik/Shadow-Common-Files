#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryNodeInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryInt64Raw : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CDictionaryInt64Raw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorDictionaryInt64}"); }

	public:
		CEnumeratorDictionaryInt64Raw(_IN CDictionaryInt64Raw& rDictionary);
		virtual ~CEnumeratorDictionaryInt64Raw();

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