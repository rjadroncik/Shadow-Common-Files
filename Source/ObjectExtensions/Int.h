#pragma once
#include "Value.h"
#include "FormatInt.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CInt : public CValue
	{
		//New 
		friend bool operator < (_IN int iValue, _IN CInt& rInt) { return (iValue <  rInt.Value()); }
		friend bool operator <=(_IN int iValue, _IN CInt& rInt) { return (iValue <= rInt.Value()); }
		friend bool operator > (_IN int iValue, _IN CInt& rInt) { return (iValue >  rInt.Value()); }
		friend bool operator >=(_IN int iValue, _IN CInt& rInt) { return (iValue >= rInt.Value()); }

		friend bool operator ==(_IN int iValue, _IN CInt& rInt) { return (iValue == rInt.Value()); }
		friend bool operator !=(_IN int iValue, _IN CInt& rInt) { return (iValue != rInt.Value()); }
	
		friend CInt operator +(_IN int iValue, _IN CInt& rInt) { return CInt(iValue + rInt.Value()); }
		friend CInt operator -(_IN int iValue, _IN CInt& rInt) { return CInt(iValue - rInt.Value()); }
		friend CInt operator /(_IN int iValue, _IN CInt& rInt) { return CInt(iValue / rInt.Value()); }
		friend CInt operator *(_IN int iValue, _IN CInt& rInt) { return CInt(iValue * rInt.Value()); }
		friend CInt operator %(_IN int iValue, _IN CInt& rInt) { return CInt(iValue % rInt.Value()); }

	public:
		static void      DigitsMin(_IN SCF::UINT uiMin);
		static SCF::UINT DigitsMin();
	
		static void      Base(_IN SCF::BYTE ucBase);
		static SCF::BYTE Base();

	public:
		SCF::UINT Parse(_IN CString& rString) { SCF::UINT uiCharsParsed = 0; m_iValue = Parse(rString, &uiCharsParsed); return uiCharsParsed; }

	public:
		static int     Parse(_IN CString& rString, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static CString Print(_IN int iValue);
		static void    Print(_IN int iValue, _OUT CString& rResult);

	private:
		static int ParseBase10(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed);
		static int ParseBase16(_IN CString& rString, _IN SCF::UINT uiValueStart, _OUT _OPT SCF::UINT* uipOutCharsParsed);

		static void PrintBase10(_IN int iValue, _OUT CString& rResult);
		static void PrintBase16(_IN int iValue, _OUT CString& rResult);

	public:
		SCF::ENUM ClassKey() _GET { return ClassInt; }
		CString   ToString() _GET;
		CString   ToString(_IN CFormatInt& rFormat) _GET;

	public:
		CInt(_IN CString& rString);
		CInt(_IN int      iValue);
		CInt(_IN CInt&    rValue);

		virtual ~CInt();

	public:
		bool IsSmallerThen(_IN CObject& rObject) const;
		bool IsEqualTo    (_IN CObject& rObject) const;

	public:
		//Arithmetic operators
		CInt operator +(_IN CInt& rInt) { return CInt(m_iValue + rInt.Value()); }
		CInt operator -(_IN CInt& rInt) { return CInt(m_iValue - rInt.Value()); }
		CInt operator /(_IN CInt& rInt) { return CInt(m_iValue / rInt.Value()); }
		CInt operator *(_IN CInt& rInt) { return CInt(m_iValue * rInt.Value()); }
		CInt operator %(_IN CInt& rInt) { return CInt(m_iValue % rInt.Value()); }
	
		CInt operator +(_IN int iValue) { return CInt(m_iValue + iValue); }
		CInt operator -(_IN int iValue) { return CInt(m_iValue - iValue); }
		CInt operator /(_IN int iValue) { return CInt(m_iValue / iValue); }
		CInt operator *(_IN int iValue) { return CInt(m_iValue * iValue); }
		CInt operator %(_IN int iValue) { return CInt(m_iValue % iValue); }

		//Arithmetic assignment operators
		void operator +=(_IN CInt& rInt) { m_iValue += rInt.Value(); }
		void operator -=(_IN CInt& rInt) { m_iValue -= rInt.Value(); }
		void operator /=(_IN CInt& rInt) { m_iValue /= rInt.Value(); }
		void operator *=(_IN CInt& rInt) { m_iValue *= rInt.Value(); }
		void operator %=(_IN CInt& rInt) { m_iValue %= rInt.Value(); }
	
		void operator +=(_IN int iValue) { m_iValue += iValue; }
		void operator -=(_IN int iValue) { m_iValue -= iValue; }
		void operator /=(_IN int iValue) { m_iValue /= iValue; }
		void operator *=(_IN int iValue) { m_iValue *= iValue; }
		void operator %=(_IN int iValue) { m_iValue %= iValue; }

		//Assignment operators
		void operator =(_IN CInt& rInt)  { m_iValue = rInt.Value(); }
		void operator =(_IN int anInt)   { m_iValue = anInt; }

		//Comparison operators
		bool operator < (_IN CInt& rInt) { return (m_iValue <  rInt.Value()); }
		bool operator <=(_IN CInt& rInt) { return (m_iValue <= rInt.Value()); }
		bool operator > (_IN CInt& rInt) { return (m_iValue >  rInt.Value()); }
		bool operator >=(_IN CInt& rInt) { return (m_iValue >= rInt.Value()); }

		bool operator ==(_IN CInt& rInt) { return (m_iValue == rInt.Value()); }
		bool operator !=(_IN CInt& rInt) { return (m_iValue != rInt.Value()); }
	
		//New 
		bool operator < (_IN int iValue) { return (m_iValue <  iValue); }
		bool operator <=(_IN int iValue) { return (m_iValue <= iValue); }
		bool operator > (_IN int iValue) { return (m_iValue >  iValue); }
		bool operator >=(_IN int iValue) { return (m_iValue >= iValue); }

		bool operator ==(_IN int iValue) { return (m_iValue == iValue); }
		bool operator !=(_IN int iValue) { return (m_iValue != iValue); }

	public:
		int Value() _GET { return m_iValue; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);

	protected:
		int m_iValue;

	private:
		CInt() { m_iValue = 0; }	
	};
};

