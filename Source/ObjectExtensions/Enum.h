#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnum : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_eValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		ValueKind Kind() _GET { return ValueEnum; }

	public:
		static ENUM Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString   Print(_IN ENUM eValue);

	public:
		CString ToString() _GET;

	public:
		CEnum(_IN CString&  rString);
		CEnum(_IN ENUM eValue);
		CEnum(_IN CEnum&    rValue);

		virtual ~CEnum();

	public:
		//Assignment operators
		void operator =(_IN CEnum&    rEnum)  { m_eValue = rEnum.Value(); }
		void operator =(_IN ENUM anEnum) { m_eValue = anEnum; }

		bool operator ==(_IN CEnum& rEnum) { return (m_eValue == rEnum.Value()); }
		bool operator !=(_IN CEnum& rEnum) { return (m_eValue != rEnum.Value()); }

	public:
		inline ENUM Value() _GET { return m_eValue; }

	protected:
		ENUM m_eValue;
	};
};

