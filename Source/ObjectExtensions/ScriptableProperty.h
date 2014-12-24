#pragma once
#include "String.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CScriptableProperty : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassScriptableProperty; }

	public:
		//[eClassKey] - Specifies the value type, this can be any object.
		//
		//[bReference] - if set to true then the object keeps a direct reference
		//               to the object used as the value of the property
		CScriptableProperty(_IN CString& rEnumeration, _IN bool bReference = FALSE, _IN bool bSettable = TRUE, _IN bool bImmediate = TRUE, _IN bool bHidden = FALSE);
		CScriptableProperty(_IN SCF::ENUM eClassKey,        _IN bool bReference = FALSE, _IN bool bSettable = TRUE, _IN bool bImmediate = TRUE, _IN bool bHidden = FALSE);
		virtual ~CScriptableProperty();

	public:
		inline bool IsSettable()  { return m_bSettable; }
		inline bool IsReference() { return m_bReference; }
		inline bool IsImmediate() { return m_bImmediate; }
		inline bool IsHidden()    { return m_bHidden; }

	public:
		inline SCF::ENUM ValueClassKey() { return m_eClassKey; }

	public:
		const CString* Enumeration() _GET { return m_pEnumeration; }

	public:
		CString AsString() _GET { return STRING("{ScriptableProperty}"); }

	private:
		SCF::ENUM m_eClassKey;
		CString* m_pEnumeration;

		bool m_bReference;
		bool m_bSettable;
		bool m_bImmediate;
		bool m_bHidden;
	};
};
