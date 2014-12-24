#pragma once

#include "Enumerator.h"
#include "DictionaryObject.h"
#include "DictionaryObjectRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryObject : public CEnumerator
	{
		template<class TKey, class TValue>
		friend class CDictionaryObject;
		friend class OBJECT_EXTENSIONS_API CDictionaryObjectRaw;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorDictionaryObject; }
		CString   ToString() _GET { return STRING("{EnumeratorDictionaryObject}"); }

	public:
		CEnumeratorDictionaryObject(_IN CDictionaryObjectRaw& rDictionary);
		virtual ~CEnumeratorDictionaryObject();

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
		bool NextEnd() { m_bFinished = TRUE; return FALSE; }

	protected:
		SCFPrivate::CDictionaryNodeObject* m_pNode;
	};
};