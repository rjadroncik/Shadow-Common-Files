#pragma once
#include "DictionaryString.h"
#include "Enum.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CScriptableEnumeration : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScriptableEnumeration; }

	public:
		CScriptableEnumeration();
		virtual ~CScriptableEnumeration();

	public:
		void ValueDefine(_IN CString& rName, _IN SCF::ENUM eValue) _SET;

	public:
		CString AsString() _GET { return STRING("{ScriptableEnumeration}"); }

	public:
		//Never delete the return value
		CEnum*   Value    (_IN CString& rName) _GET;
		CString* ValueName(_IN SCF::ENUM eValue)    _GET;

	private:
		CDictionaryString m_Values;
		CDictionaryInt64  m_Names;
	};
};
