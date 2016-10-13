#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CChar : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_cValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		ValueKind Kind() _GET { return ValueChar; }

	public:
		static TCHAR Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString    Print(_IN TCHAR cValue);

	public:
		CString ToString() _GET;

	public:
		CChar(_IN CString& rString);
		CChar(_IN CChar& rValue);
		CChar(_IN TCHAR  cValue);

		virtual ~CChar();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		TCHAR Value() _GET { return m_cValue; }

	protected:
		TCHAR m_cValue;

	private:
		CChar() { m_cValue = 0; }
	};
};
