#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "BagObject.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorBagObject : public CEnumeratorRaw, public IEnumerator<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CBagObject;

	public:
		CString ToString() _GET { return STRING("{EnumeratorBagObject}"); }

	public:
		CEnumeratorBagObject(_IN CBagObject& rBag);
		virtual ~CEnumeratorBagObject();

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
		SCFPrivate::CBagNodeObject* m_pNode;
	};
};