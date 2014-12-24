#pragma once
#include "ScriptableArgument.h"
#include "DictionaryString.h"
#include "Vector.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CScriptableMethod : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScriptableMethod; }

	public:
		//The accessible argument descriptor [pReturnValue] will be deleted when 
		//an instance of this class is deleted
		CScriptableMethod(_IN CScriptableArgument* pReturnValue = NULL);
		virtual ~CScriptableMethod();

	public:
		inline const CScriptableArgument* ReturnValue() _GET { return m_pReturnValue; }
	
	public:
		//The accessible argument descriptor [rArgument] will be deleted when an
		//instance of this class is deleted 
		//Arguments must be specified in correct order!!!
		void ArgumentAdd(_IN CString& rName, _IN _REF CScriptableArgument& rArgument) _SET;

	public:
		const CScriptableArgument* ArgumentNamed(_IN CString& rName) _GET;
	
	public:
		inline CScriptableArgument& Argument(_IN SCF::UINT uiIndex) _GET { return (CScriptableArgument&)m_Arguments.At(uiIndex); }

	public:
		CString AsString() _GET { return STRING("{ScriptableMethod}"); }

	private:
		CDictionaryString m_ArgumentIndices;

		CVector m_Arguments;
	
		const CScriptableArgument* m_pReturnValue;
	};
};
