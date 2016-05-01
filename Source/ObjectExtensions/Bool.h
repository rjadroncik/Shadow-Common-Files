#pragma once
#include "Value.h"

namespace SCFBase
{
 	class OBJECT_EXTENSIONS_API CBool : public CValue
	{
		friend CBool operator <=(_IN bool bValue, _IN CBool& rBool) { return CBool(!(bValue && !rBool.m_bValue)); }
		friend CBool operator >=(_IN bool bValue, _IN CBool& rBool) { return CBool(!(!bValue && rBool.m_bValue)); }
		friend CBool operator < (_IN bool bValue, _IN CBool& rBool) { return CBool(!bValue && rBool.m_bValue); }
		friend CBool operator > (_IN bool bValue, _IN CBool& rBool) { return CBool(bValue && !rBool.m_bValue); }

		friend CBool operator ==(_IN bool bValue, _IN CBool& rBool) { return CBool(bValue == rBool.m_bValue); }
		friend CBool operator !=(_IN bool bValue, _IN CBool& rBool) { return CBool(bValue != rBool.m_bValue); }
		friend CBool operator &&(_IN bool bValue, _IN CBool& rBool) { return CBool(bValue && rBool.m_bValue); }
		friend CBool operator ||(_IN bool bValue, _IN CBool& rBool) { return CBool(bValue || rBool.m_bValue); }

	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_bValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static bool    Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString Print(_IN bool bValue);

	public:
		CString ToString() _GET;

	public:
		CBool(_IN CString& rString);
		CBool(_IN bool     bValue);
		CBool(_IN CBool&   rValue);

		virtual ~CBool();

	public:
		//Assignment operator
		void operator =(_IN CBool& rBool) { m_bValue = rBool.m_bValue; }
		void operator =(_IN bool aBool)   { m_bValue = aBool; }
	
		//Comparison operators
		bool operator ==(_IN CBool& rBool) { return (m_bValue == rBool.m_bValue); }
		bool operator !=(_IN CBool& rBool) { return (m_bValue != rBool.m_bValue); }
		bool operator ==(_IN bool bValue)  { return (m_bValue == bValue); }
		bool operator !=(_IN bool bValue)  { return (m_bValue != bValue); }

		bool operator <=(_IN CBool& rBool) { return !(m_bValue && !rBool.m_bValue); }
		bool operator >=(_IN CBool& rBool) { return !(!m_bValue && rBool.m_bValue); }
		bool operator < (_IN CBool& rBool) { return (!m_bValue && rBool.m_bValue); }
		bool operator > (_IN CBool& rBool) { return (m_bValue && !rBool.m_bValue); }

		//Logical operators
		bool operator &&(_IN CBool& rBool) { return (m_bValue && rBool.m_bValue); }
		bool operator ||(_IN CBool& rBool) { return (m_bValue || rBool.m_bValue); }
		bool operator &&(_IN bool bValue)  { return (m_bValue && bValue); }
		bool operator ||(_IN bool bValue)  { return (m_bValue || bValue); }

		bool operator !() const { return !m_bValue; }
		
	public:
		bool Value() _GET { return m_bValue; }

	protected:
		bool m_bValue;

	private:
		CBool() { m_bValue = FALSE; }
	};
};
