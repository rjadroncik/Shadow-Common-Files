#pragma once

#include "Enumerator.h"
#include "DictionaryInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryInt64 : public CEnumerator
	{
		friend class OBJECT_EXTENSIONS_API CDictionaryInt64;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorDictionaryInt64; }
		CString   ToString() _GET { return STRING("{EnumeratorDictionaryInt64}"); }

	public:
		CEnumeratorDictionaryInt64(_IN CDictionaryInt64& rDictionary);
		virtual ~CEnumeratorDictionaryInt64();

	public:
		SCF::UINT64 CurrentKey() _GET;

	public:
		//After you use these, "garbage" is left in the dictionary & will be
		//removed only when the dictionary itself is destroyed
		void CurrentShallowRemove();
		void CurrentShallowDelete();
	
	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CDictionaryNodeInt64* m_pNode;
	};
};