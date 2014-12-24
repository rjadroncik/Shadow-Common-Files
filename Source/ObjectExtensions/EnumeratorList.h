#pragma once

#include "Enumerator.h"
#include "List.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorList : public CEnumerator
	{
		friend class OBJECT_EXTENSIONS_API CListRaw;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorList; }
		CString   ToString() _GET { return STRING("{EnumeratorList}"); }

	public:
		CEnumeratorList(_IN CListRaw& rList);
		virtual ~CEnumeratorList();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CListNode* m_pNode;
		SCF::BYTE m_ucIndex;
	};
};