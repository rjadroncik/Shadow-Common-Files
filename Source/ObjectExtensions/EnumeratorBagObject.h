#pragma once

#include "Enumerator.h"
#include "BagObject.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagObject : public CEnumerator
	{
		friend class OBJECT_EXTENSIONS_API CBagObject;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorBagObject; }
		CString   ToString() _GET { return STRING("{EnumeratorBagObject}"); }

	public:
		CEnumeratorBagObject(_IN CBagObject& rBag);
		virtual ~CEnumeratorBagObject();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CBagNodeObject* m_pNode;
	};
};