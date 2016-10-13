#pragma once
#include "Value.h"

namespace SCFBase
{
	#define WHITESPACE_CHARACTERS "\a\b\f\n\r\t\v "

	#define STRING_CREATE_ARGS(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(TCHAR)) - 1, (bool)FALSE
	#define STRING_CREATE_ARGS_ALLOC(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(TCHAR)) - 1, (bool)TRUE

	#define STRING_ASSIGN_ARGS(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(TCHAR)) - 1

	#define STRING(quote) CString((const LPTSTR) SCFTEXT(quote), (const UINT)((sizeof(SCFTEXT(quote)) / sizeof(TCHAR)) - 1), (bool)FALSE)
	#define STRINGREF(string) CString(string, (bool)FALSE)

	#define STRING_RETURN(string) CString(string, (bool)FALSE, (bool)TRUE)

	#define IS_STRING(object) (((object).ClassKey() == ClassString) || ((object).ClassKey() == ClassStringRange))

	class OBJECT_EXTENSIONS_API CString : public CValue
	{
		friend class OBJECT_EXTENSIONS_API CStringRange;
		friend class OBJECT_EXTENSIONS_API CStreamString;
		friend class OBJECT_EXTENSIONS_API CStreamStringRead;
		friend class OBJECT_EXTENSIONS_API CStreamStringWrite;

		friend CString operator  +(_IN LPTSTR szText, _IN CString& rString) { return CString(szText, rString); }
		friend bool    operator ==(_IN LPTSTR szText, _IN CString& rString) { return (rString == szText); }
		friend bool    operator ==(_IN char*  szText, _IN CString& rString) { return (rString == szText); }

	public:
		UINT Parse(_IN CString& rString) { if (this != &rString ) { *this = rString; } return rString.Length(); }

	public:
		ValueKind Kind() _GET { return ValueString; }

	public:
		virtual bool IsRange() _GET { return FALSE; }

	public:
		CString ToString() _GET { return STRINGREF(*this); }
		CString& ToNew() { return *(new CString(*this, false, true)); }

	public:
		//Copy constructors
		CString(_IN CString& rString);
		CString(_IN CString& rString, _IN bool bAllocateMemory);

		//Special constructor which DOES NOT allocate new memory, but instead takes away control of the memory form the source string & assigns it to the created string
		//Should be used ONLY in this form [CString(Object.ToString())] to preserve the value returned by [ToString()] WITHOUT the overhead of copying the actual memory where the data is stored
		//The code is short, fast & similar to one of the forms of the [Assign(..)] method
		//CString(_INOUT CString vString);

		//Special constructor which can be called only with [bAllocateMemory = FALSE] && [bTakeOwnageOfMemory = TRUE]
		//This constructor DOES NOT allocate new memory, but instead takes away control of the memory form the source string & assigns it to the created string
		//The code is short, fast & similar to one of the forms of the [Assign(..)] method
		CString(_INOUT CString& rString, _IN bool bAllocateMemory, _IN bool bTakeOwnageOfMemory);

		//Helper constructors
		CString();
		CString(_IN LPTSTR szText, _IN bool bAllocateMemory = TRUE);
		CString(_IN LPTSTR sText, _IN UINT uiLength, _IN bool bAllocateMemory = TRUE);

		CString(_IN char* szText);

		//Concatenation constructors
		CString(_IN LPTSTR szText1,    _IN CString& rString2);
		CString(_IN CString& rString1, _IN LPTSTR   szText2);
		CString(_IN CString& rString1, _IN CString& rString2);
		CString(_IN CString& rString,  _IN TCHAR    cChar);

		virtual ~CString();

		//Concatenation operators
		CString operator +(_IN CString& rString) const { return CString(*this, rString); }
		CString operator +(_IN LPTSTR   szText)  const { return CString(*this, szText); }
		CString operator +(_IN TCHAR    cChar)   const { return CString(*this, cChar); }

	public:
		TCHAR operator [](_IN UINT uiIndex) _GET { return m_szValue[uiIndex]; }

		TCHAR At   (_IN UINT uiIndex) _GET              { return m_szValue[uiIndex]; }
		void  AtPut(_IN UINT uiIndex, _IN TCHAR cValue) { m_szValue[uiIndex] = cValue; }

	public:
		int IndexOf(_IN TCHAR cChar);

	public:
		//Assignment operators
		void operator = (_IN CString& rString);
		void operator = (_IN LPTSTR   szText);
		void operator = (_IN TCHAR   cChar);

		void operator = (_IN char* szText);

		void operator +=(_IN CString& rString);
		void operator +=(_IN LPTSTR   szText);
		void operator +=(_IN TCHAR    cChar);

		//Comparison operator
		bool operator ==(_IN CString& rString) const;
		bool operator ==(_IN LPTSTR   szText)  const;
		bool operator ==(_IN char*    szText)  const;
		bool operator ==(_IN TCHAR    cChar)   const;

		bool operator <=(_IN CString& rString) const;
		bool operator <=(_IN LPTSTR   szText)  const;

		//Equality with regard to case (Case Insensitive)
		bool IsEqualCI(_IN CString& rString) const;
		bool IsEqualCI(_IN LPTSTR   szText)  const;

		//Allows to test for object equality
		bool IsEqualTo(_IN CObject& rObject) const;

	public:
		//Faster than using the operator [=], since we pass the string length & sometimes we even NEED this
		void Assign(_IN LPTSTR sText, _IN UINT uiLength);

		//Assigns the memory of the given string object (source string) to the string on which the method is invoked (target string)
		//This method DOES NOT allocate new memory, but instead takes away control of the memory form the source string & assigns it to the target string
		//In case the target string has already some memory allocated, this memory is FREED!!!
		//Should be used ONLY in this form [String.Assign(Object.ToString())] to preserve the value returned by [ToString()] WITHOUT the overhead of copying the actual memory where the data is stored
		void Assign(_INOUT CString vString);

	public:
		void Resize(_IN UINT uiLength);
		void Clear();

		UINT Length()  _GET { return  m_uiLength; }
		bool IsEmpty() _GET { return (m_uiLength == 0); }

	public:
		//Low level routines for manipulating the storage directly
		UINT BytesReserved()                _GET;
		void BytesReserve(_IN UINT uiCount) _SET;

		UINT CharsReserved()                _GET;
		void CharsReserve(_IN UINT uiCount) _SET;

	public:
		//This must be called if u stored a string directly using the pointer
		//returned by [Value()]
		void LengthScan();

	public:
		inline LPTSTR Value() _GET { return m_szValue; }

	public:
		void ToASCII(_OUT char* szText) const;

	public:
#ifdef _BETA
		bool Locked() { return (m_uiLocks > 0); }

		void LockAdd()     { m_uiLocks++; }
		void LockRelease() { m_uiLocks--; }
#endif

	public:
		//Size - allocates a new memory chunk, Resize - reallocates existing chunk
		void InternalSize  (_IN UINT uiLength);
		void InternalResize(_IN UINT uiLength);

	protected:
		LPTSTR StringAlloc  (_IN UINT uiLength);
		void   StringFree   (_IN LPTSTR szString);
		LPTSTR StringRealloc(_IN LPTSTR szString, _IN UINT uiLength, _IN UINT uiLengthNew);

	protected:
		LPTSTR m_szValue;
		UINT   m_uiLength;

	protected:
		BETAONLY(UINT m_uiLocks;)

	protected:
		bool m_bMemoryAllocated;
	};
};
