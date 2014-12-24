#pragma once

#include "Enumerator.h"
#include "BagInt64.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagInt64 : public CEnumerator
	{
		friend class OBJECT_EXTENSIONS_API CBagInt64;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorBagInt64; }
		CString   ToString() _GET { return STRING("{EnumeratorBagInt64}"); }

	public:
		CEnumeratorBagInt64(_IN CBagInt64& rBag);
		virtual ~CEnumeratorBagInt64();

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