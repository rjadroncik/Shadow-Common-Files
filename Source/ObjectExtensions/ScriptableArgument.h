#pragma once
#include "String.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CScriptableArgument: public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScriptableArgument; }

	public:
		//[eClassKey] - Specifies the value type, this can be any object.
		//
		//[bReference] - if set to true then the object keeps a direct reference
		//               to the object used as the value of the property
		CScriptableArgument(_IN SCF::ENUM eClassKey, _IN bool bReference = FALSE);
		virtual ~CScriptableArgument();

	public:
		inline bool IsReference() { return m_bReference; }

	public:
		inline SCF::ENUM ValueClassKey() { return m_eClassKey; }

	public:
		void           Enumeration(_IN _REF CString* pEnumeration) _SET { m_pEnumeration = pEnumeration; }
		const CString* Enumeration()                               _GET { return m_pEnumeration; }

	public:
		CString AsString() _GET { return STRING("{ScriptableArgument}"); }

	private:
		SCF::ENUM m_eClassKey;
		const CString* m_pEnumeration;

		bool m_bReference;
	};
};
