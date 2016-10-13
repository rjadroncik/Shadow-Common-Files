#pragma once
#include "Value.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CPointer : public CValue
	{
	public:
		UINT Parse(_IN CString& rString) { UINT uiCharsParsed = 0; m_vpValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		ValueKind Kind() _GET { return ValuePointer; }

	public:
		static void*   Parse(_IN CString& rString, _OUT _OPT UINT* uipOutCharsParsed);
		static CString Print(_IN void* vpValue);

	public:
		CString ToString() _GET;

	public:
		CPointer(_IN CString&  rValue);
		CPointer(_IN void*    vpValue);
		CPointer(_IN CPointer& rValue);

		virtual ~CPointer();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		//Assignment operators
		void operator =(_IN CPointer& rPointer)  { m_vpValue = (void*)rPointer.Value(); }
		void operator =(_IN void*     aPointer)  { m_vpValue = (void*)aPointer; }

		bool operator ==(_IN CPointer& rPointer) { return (m_vpValue == rPointer.Value()); }
		bool operator !=(_IN CPointer& rPointer) { return (m_vpValue != rPointer.Value()); }

	public:
		inline void* Value() _GET { return m_vpValue; }

	protected:
		void* m_vpValue;

	private:
		CPointer() { m_vpValue = NULL; }
	};
};