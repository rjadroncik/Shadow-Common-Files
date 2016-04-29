#pragma once
#include "Value.h"
#include "FormatFloat.h"

namespace SCFBase
{
	enum DecimalSeparators
	{
		DecimalSeparatorDot    = '.',
		DecimalSeparatorComma  = ',',
	};

	class OBJECT_EXTENSIONS_API CFloat : public CValue
	{
		friend CFloat operator +(_IN float fValue, _IN CFloat& rFloat) { return CFloat(fValue + rFloat.Value()); }
		friend CFloat operator -(_IN float fValue, _IN CFloat& rFloat) { return CFloat(fValue - rFloat.Value()); }
		friend CFloat operator /(_IN float fValue, _IN CFloat& rFloat) { return CFloat(fValue / rFloat.Value()); }
		friend CFloat operator *(_IN float fValue, _IN CFloat& rFloat) { return CFloat(fValue * rFloat.Value()); }

	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_fValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static float   Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString Print(_IN float fValue);
		static void    Print(_IN float fValue, _OUT CString& rResult);

		static void      DecimalSeparator(_IN SCF::ENUM eChar);
		static SCF::ENUM DecimalSeparator();

	public:
		static void FractionDigits(_IN SCF::UINT uiMin, _IN SCF::UINT uiMax);
		static SCF::UINT FractionDigitsMin();
		static SCF::UINT FractionDigitsMax();

	public:
		CString ToString() _GET;
		CString ToString(_IN CFormatFloat& rFormat) _GET;

	public:
		CFloat(_IN CString& rString);
		CFloat(_IN float    fValue);
		CFloat(_IN CFloat&  rValue);

		virtual ~CFloat();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		//Arithmetic operators
		CFloat operator +(_IN CFloat& rFloat) { return CFloat(m_fValue + rFloat.Value()); }
		CFloat operator -(_IN CFloat& rFloat) { return CFloat(m_fValue - rFloat.Value()); }
		CFloat operator /(_IN CFloat& rFloat) { return CFloat(m_fValue / rFloat.Value()); }
		CFloat operator *(_IN CFloat& rFloat) { return CFloat(m_fValue * rFloat.Value()); }
	
		CFloat operator +(_IN float fValue) { return CFloat(m_fValue + fValue); }
		CFloat operator -(_IN float fValue) { return CFloat(m_fValue - fValue); }
		CFloat operator /(_IN float fValue) { return CFloat(m_fValue / fValue); }
		CFloat operator *(_IN float fValue) { return CFloat(m_fValue * fValue); }

		//Arithmetic assignment operators
		void operator +=(_IN CFloat& rFloat) { m_fValue += rFloat.Value(); }
		void operator -=(_IN CFloat& rFloat) { m_fValue -= rFloat.Value(); }
		void operator /=(_IN CFloat& rFloat) { m_fValue /= rFloat.Value(); }
		void operator *=(_IN CFloat& rFloat) { m_fValue *= rFloat.Value(); }

		//Assignment operators
		void operator =(_IN CFloat& rFloat) { m_fValue = rFloat.Value(); }
		void operator =(_IN float   aFloat) { m_fValue = aFloat; }

		//Comparison operators
		bool operator < (_IN CFloat& rFloat) { return (m_fValue <  rFloat.Value()); }
		bool operator <=(_IN CFloat& rFloat) { return (m_fValue <= rFloat.Value()); }
		bool operator > (_IN CFloat& rFloat) { return (m_fValue >  rFloat.Value()); }
		bool operator >=(_IN CFloat& rFloat) { return (m_fValue >= rFloat.Value()); }

		bool operator ==(_IN CFloat& rFloat) { return (m_fValue == rFloat.Value()); }
		bool operator !=(_IN CFloat& rFloat) { return (m_fValue != rFloat.Value()); }

	public:
		inline float Value() _GET { return m_fValue; }

	protected:
		float m_fValue;

	private:
		CFloat() { m_fValue = 0; }
	};
};

