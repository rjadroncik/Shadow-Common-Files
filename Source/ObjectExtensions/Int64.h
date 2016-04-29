#pragma once
#include "Value.h"
#include "FormatInt.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CInt64 : public CValue
	{
		friend CInt64 operator +(_IN SCF::INT64 i64Value, _IN CInt64& rInt) { return CInt64(i64Value + rInt.Value()); }
		friend CInt64 operator -(_IN SCF::INT64 i64Value, _IN CInt64& rInt) { return CInt64(i64Value - rInt.Value()); }
		friend CInt64 operator /(_IN SCF::INT64 i64Value, _IN CInt64& rInt) { return CInt64(i64Value / rInt.Value()); }
		friend CInt64 operator *(_IN SCF::INT64 i64Value, _IN CInt64& rInt) { return CInt64(i64Value * rInt.Value()); }
		friend CInt64 operator %(_IN SCF::INT64 i64Value, _IN CInt64& rInt) { return CInt64(i64Value % rInt.Value()); }

	public:
		static void      DigitsMin(_IN SCF::UINT uiMin);
		static SCF::UINT DigitsMin();

		static void      Base(_IN SCF::BYTE ucBase);
		static SCF::BYTE Base();

	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_i64Value = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static SCF::INT64 Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString    Print(_IN SCF::INT64 i64Value);
		static void       Print(_IN SCF::INT64 i64Value, _OUT CString& rResult);
	
	private:
		static SCF::INT64 ParseBase10(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static SCF::INT64 ParseBase16(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed);

		static void PrintBase10(_IN SCF::INT64 iValue, _OUT CString& rResult);
		static void PrintBase16(_IN SCF::INT64 iValue, _OUT CString& rResult);

	public:
		CString ToString() _GET;
		CString ToString(_IN CFormatInt& rFormat) _GET;

	public:
		CInt64(_IN CString& rString);
		CInt64(_IN SCF::INT64    i64Value);
		CInt64(_IN CInt64&  rValue);

		virtual ~CInt64();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		//Arithmetic operators
		CInt64 operator +(_IN CInt64& rInt) { return CInt64(m_i64Value + rInt.Value()); }
		CInt64 operator -(_IN CInt64& rInt) { return CInt64(m_i64Value - rInt.Value()); }
		CInt64 operator /(_IN CInt64& rInt) { return CInt64(m_i64Value / rInt.Value()); }
		CInt64 operator *(_IN CInt64& rInt) { return CInt64(m_i64Value * rInt.Value()); }
		CInt64 operator %(_IN CInt64& rInt) { return CInt64(m_i64Value % rInt.Value()); }
	
		CInt64 operator +(_IN SCF::INT64 i64Value) { return CInt64(m_i64Value + i64Value); }
		CInt64 operator -(_IN SCF::INT64 i64Value) { return CInt64(m_i64Value - i64Value); }
		CInt64 operator /(_IN SCF::INT64 i64Value) { return CInt64(m_i64Value / i64Value); }
		CInt64 operator *(_IN SCF::INT64 i64Value) { return CInt64(m_i64Value * i64Value); }
		CInt64 operator %(_IN SCF::INT64 i64Value) { return CInt64(m_i64Value % i64Value); }

		//Arithmetic assignment operators
		void operator +=(_IN CInt64& rInt) { m_i64Value += rInt.Value(); }
		void operator -=(_IN CInt64& rInt) { m_i64Value -= rInt.Value(); }
		void operator /=(_IN CInt64& rInt) { m_i64Value /= rInt.Value(); }
		void operator *=(_IN CInt64& rInt) { m_i64Value *= rInt.Value(); }
		void operator %=(_IN CInt64& rInt) { m_i64Value %= rInt.Value(); }

		//Assignment operators
		void operator =(_IN CInt64& rInt)  { m_i64Value = rInt.Value(); }
		void operator =(_IN SCF::INT64 anInt)   { m_i64Value = anInt; }

		//Comparison operators
		bool operator < (_IN CInt64& rInt) { return (m_i64Value <  rInt.Value()); }
		bool operator <=(_IN CInt64& rInt) { return (m_i64Value <= rInt.Value()); }
		bool operator > (_IN CInt64& rInt) { return (m_i64Value >  rInt.Value()); }
		bool operator >=(_IN CInt64& rInt) { return (m_i64Value >= rInt.Value()); }

		bool operator ==(_IN CInt64& rInt) { return (m_i64Value == rInt.Value()); }
		bool operator !=(_IN CInt64& rInt) { return (m_i64Value != rInt.Value()); }

	public:
		inline SCF::INT64 Value() _GET { return m_i64Value; }

	protected:
		SCF::INT64 m_i64Value;

	private:
		CInt64() { m_i64Value = 0; }	
	};
};

