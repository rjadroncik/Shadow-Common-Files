#pragma once

#include "Enumerator.h"
#include "DictionaryString.h"
#include "DictionaryStringRaw.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnumeratorDictionaryString : public CEnumerator
	{
		template<class T>
		friend class CDictionaryString;

		friend class OBJECT_EXTENSIONS_API CDictionaryStringRaw;

	public:
		SCF::ENUM ClassKey() _GET { return ClassEnumeratorDictionaryString; }
		CString   ToString() _GET { return STRING("{EnumeratorDictionaryString}"); }

	public:
		CEnumeratorDictionaryString(_IN CDictionaryStringRaw& rDictionary);
		CEnumeratorDictionaryString(_IN CDictionaryStringRaw& rDictionary, _IN CString& rRootPath);
		virtual ~CEnumeratorDictionaryString();

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
			SCF::UINT                           uiDepth;

		} m_Stack;

	protected:
		SCFPrivate::CDictionaryNodeString* m_pRoot;
		CString*                           m_pRootPath;
	};
};