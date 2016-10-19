#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "SetObject.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorSetObject : public CEnumeratorRaw, public IEnumerator<CObject>
	{
		friend class OBJECT_EXTENSIONS_API CSetObject;

	public:
		CString ToString() _GET { return STRING("{EnumeratorSetObject}"); }

	public:
		CEnumeratorSetObject(_IN CSetObject& rBag);
		virtual ~CEnumeratorSetObject();

	public:
		//Every enumeration goes trough 3 stages (start, continue, end), the next function calls the appropriate stage fucntion
		virtual bool Next() { return CEnumeratorRaw::ProtectedNext(); }

	public:
		//Returns true while there still is a next element to be enumerated
		virtual bool HasNext() _GET { return CEnumeratorRaw::ProtectedHasNext(); }

	public:
		virtual CObject* Current() _GET { return CEnumeratorRaw::ProtectedCurrent(); }

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_pCurrent = NULL; return FALSE; }

	protected:
		SCFPrivate::CSetNodeObject* m_pNode;
	};
};