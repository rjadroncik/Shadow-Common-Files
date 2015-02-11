#pragma once

#include "FSBHeap.h"

using namespace SCFBase;

namespace SCFBase
{
	template<class T>
	class CDictionaryString;
	class OBJECT_EXTENSIONS_API CDictionaryStringRaw;
};

namespace SCFPrivate
{
	//Each node in the dictionary tree/forest represents one letter of a word
	//To find a word, you traverse the tree, matching characters in the nodes against characters at appropriate indexes in a string
	class CDictionaryNodeString
	{
		template<class T>
		friend class SCFBase::CDictionaryString;
		friend class OBJECT_EXTENSIONS_API SCFBase::CDictionaryStringRaw;

	private:
		CDictionaryNodeString();
		~CDictionaryNodeString();

	public:
		//Used instead of constructors
		static CDictionaryNodeString* Create          (_IN SCF::TCHAR cLetter);
		static CDictionaryNodeString* CreateCopy      (_IN CDictionaryNodeString* pNode);

		static void                   Delete          (_IN CDictionaryNodeString* pNode);
		static void                   DeleteWithObject(_IN CDictionaryNodeString* pNode);

	public:
		inline SCF::TCHAR Letter()                  _GET { return m_cLetter; }
		inline void  Letter(_IN SCF::TCHAR cLetter) _SET { m_cLetter = cLetter; }

	public:
		inline SCF::UINT Usage() _GET { return m_uiUsage; }

		inline void UsageAdd()    _SET { m_uiUsage++; }
		inline void UsageRemove() _SET { m_uiUsage--; }

		inline void UsageReset() _SET { m_uiUsage = 0; }

	public:
		inline CObject* Object()                          _GET { return m_pObject; }
		inline void     Object(_IN _REF CObject* pObject) _SET { BETAONLY(if (m_pObject) { m_pObject->Release(); }) m_pObject = (CObject*)pObject; BETAONLY(if (m_pObject) { m_pObject->AddRef(); }) }

	public:
		inline CDictionaryNodeString* ChildFirst()                                            _GET { return m_pChildFirst; }
		inline void                   ChildFirst(_IN _REF CDictionaryNodeString* pChildFirst) _SET { m_pChildFirst = (CDictionaryNodeString*)pChildFirst; }

		inline CDictionaryNodeString* Next()                                      _GET { return m_pNext; }
		inline void                   Next(_IN _REF CDictionaryNodeString* pNext) _SET { m_pNext = (CDictionaryNodeString*)pNext; }

	private:
		//If this node represents the last letter of a word, this member is set to a valid pointer, [NULL] otherwise
		CObject*      m_pObject;
		SCF::UINT     m_uiUsage;

		SCF::TCHAR    m_cLetter;
	private:
		//We only have 2 traversal pointers, as we always traverse down the tree (non recursively for name to object conversion)
		CDictionaryNodeString* m_pChildFirst;
		CDictionaryNodeString* m_pNext;
	};
};
