#pragma once

#include "EnumeratorRaw.h"
#include "DictionaryObjectRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryObjectRaw : public CEnumeratorRaw
	{
		friend class OBJECT_EXTENSIONS_API CDictionaryObjectRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorDictionaryObject}"); }

	public:
		CEnumeratorDictionaryObjectRaw(_IN CDictionaryObjectRaw& rDictionary);
		virtual ~CEnumeratorDictionaryObjectRaw();

	public:
		CObject* CurrentKey() _GET;

	public:
		//After you use these, "garbage" is left in the dictionary & will be
		//removed only when the dictionary itself is destroyed
		void CurrentShallowRemove();
		void CurrentShallowDelete();

	protected:
		bool NextStart();
		bool NextContinue();
		bool NextEnd() { m_pCurrent = nullptr; return false; }

	protected:
		SCFPrivate::CDictionaryNodeObject* m_pNode;
	};
};