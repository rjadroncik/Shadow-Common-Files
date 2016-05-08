#pragma once

#include "Enumerator.h"
#include "EnumeratorRaw.h"
#include "DictionaryString.h"
#include "DictionaryStringRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryString : public CEnumeratorRaw, public IEnumerator<CObject>
	{
		template<class TValue>
		friend class CDictionaryString;

		friend class OBJECT_EXTENSIONS_API CDictionaryStringRaw;

	public:
		CString ToString() _GET { return STRING("{EnumeratorDictionaryString}"); }

	public:
		CEnumeratorDictionaryString(_IN CDictionaryStringRaw& rDictionary);
		CEnumeratorDictionaryString(_IN CDictionaryStringRaw& rDictionary, _IN CString& rRootPath);
		virtual ~CEnumeratorDictionaryString();

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

	public:
		CString CurrentPath() _GET;

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
		//This function is called until a node which contains an object is found or enumeration ends
		//(not all nodes contain objects)
		bool NextNode();

		//This is used by [CDictionaryString] when deleting content
		inline SCFPrivate::CDictionaryNodeString* CurrentNode() { return m_Stack.ppNodes[m_Stack.uiDepth - 1]; }


	protected:
		bool NextNodeForSerialization();

	protected:
		struct SStack
		{
			SCFPrivate::CDictionaryNodeString** ppNodes;
			UINT                           uiDepth;

		} m_Stack;

	protected:
		SCFPrivate::CDictionaryNodeString* m_pRoot;
		CString*                           m_pRootPath;
	};
};