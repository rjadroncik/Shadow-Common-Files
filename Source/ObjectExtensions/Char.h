#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CChar : public CValue
	{
	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_cValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static SCF::TCHAR Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString    Print(_IN SCF::TCHAR cValue);

	public:
		CString ToString() _GET;

	public:
		CChar(_IN CString& rString);
		CChar(_IN CChar& rValue);
		CChar(_IN SCF::TCHAR  cValue);

		virtual ~CChar();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		SCF::TCHAR Value() _GET { return m_cValue; }

	protected:
		SCF::TCHAR m_cValue;

	private:
		CChar() { m_cValue = 0; }
	};
};
