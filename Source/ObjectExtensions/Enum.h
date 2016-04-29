#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CEnum : public CValue
	{
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_eValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static SCF::ENUM Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString   Print(_IN SCF::ENUM eValue);

	public:
		CString ToString() _GET;

	public:
		CEnum(_IN CString&  rString);
		CEnum(_IN SCF::ENUM eValue);
		CEnum(_IN CEnum&    rValue);

		virtual ~CEnum();

	public:
		//Assignment operators
		void operator =(_IN CEnum&    rEnum)  { m_eValue = rEnum.Value(); }
		void operator =(_IN SCF::ENUM anEnum) { m_eValue = anEnum; }

		bool operator ==(_IN CEnum& rEnum) { return (m_eValue == rEnum.Value()); }
		bool operator !=(_IN CEnum& rEnum) { return (m_eValue != rEnum.Value()); }

	public:
		inline SCF::ENUM Value() _GET { return m_eValue; }

	protected:
		SCF::ENUM m_eValue;
	};
};

