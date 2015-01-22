#pragma once
#include "Value.h"

namespace SCFBase
{
	#define WHITESPACE_CHARACTERS "\a\b\f\n\r\t\v " 

	#define STRING_CREATE_ARGS(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(SCF::TCHAR)) - 1, (bool)FALSE
	#define STRING_CREATE_ARGS_ALLOC(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(SCF::TCHAR)) - 1, (bool)TRUE
	
	#define STRING_ASSIGN_ARGS(quote) SCFTEXT(quote), (sizeof(SCFTEXT(quote)) / sizeof(SCF::TCHAR)) - 1

	#define STRING(quote) CString((const SCF::LPTSTR) SCFTEXT(quote), (const SCF::UINT)((sizeof(SCFTEXT(quote)) / sizeof(SCF::TCHAR)) - 1), (bool)FALSE)
	#define STRINGREF(string) CString(string, (bool)FALSE)
	
	#define STRING_RETURN(string) CString(string, (bool)FALSE, (bool)TRUE)

	#define IS_STRING(object) (((object).ClassKey() == ClassString) || ((object).ClassKey() == ClassStringRange))

	class OBJECT_EXTENSIONS_API CString : public CValue
	{
		friend class OBJECT_EXTENSIONS_API CStringRange;
		friend class OBJECT_EXTENSIONS_API CStreamString;
		friend class OBJECT_EXTENSIONS_API CStreamStringRead;
		friend class OBJECT_EXTENSIONS_API CStreamStringWrite;

		friend CString operator  +(_IN SCF::LPTSTR szText, _IN CString& rString) { return CString(szText, rString); }
		friend bool    operator ==(_IN SCF::LPTSTR szText, _IN CString& rString) { return (rString == szText); }

	public:
		SCF::UINT Parse(_IN CString& rString) { if (this != &rString ) { *this = rString; } return rString.Length(); }

	public:
		virtual bool IsRange() _GET { return FALSE; }

	public:
		SCF::ENUM ClassKey() _GET { return ClassString; }
		CString   ToString() _GET { return STRINGREF(*this); }

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
		CString(_IN SCF::LPTSTR szText, _IN bool bAllocateMemory = TRUE);
		CString(_IN SCF::LPTSTR sText, _IN SCF::UINT uiLength, _IN bool bAllocateMemory = TRUE);

		CString(_IN char* szText);

		//Concatenation constructors
		CString(_IN SCF::LPTSTR szText1,  _IN CString&    rString2);
		CString(_IN CString&    rString1, _IN SCF::LPTSTR szText2);
		CString(_IN CString&    rString1, _IN CString&    rString2);
		CString(_IN CString&    rString,  _IN SCF::TCHAR  cChar);

		virtual ~CString();

		//Concatenation operators
		CString operator +(_IN CString&    rString) const { return CString(*this, rString); }
		CString operator +(_IN SCF::LPTSTR szText)  const { return CString(*this, szText ); }
		CString operator +(_IN SCF::TCHAR  cChar)   const { return CString(*this, cChar  ); }

	public:
		SCF::TCHAR operator [](_IN SCF::UINT uiIndex) _GET { return m_szValue[uiIndex]; }

		SCF::TCHAR At   (_IN SCF::UINT uiIndex) _GET                   { return m_szValue[uiIndex]; }
		void       AtPut(_IN SCF::UINT uiIndex, _IN SCF::TCHAR cValue) { m_szValue[uiIndex] = cValue; }

	public:
		int IndexOf(_IN SCF::TCHAR cChar);

	public:
		//Assignment operators
		void operator = (_IN CString&    rString);
		void operator = (_IN SCF::LPTSTR szText);
		void operator = (_IN SCF::TCHAR  cChar);

		void operator = (_IN char* szText);
	
		void operator +=(_IN CString&    rString);
		void operator +=(_IN SCF::LPTSTR szText);
		void operator +=(_IN SCF::TCHAR  cChar);

		//Comparison operator
		bool operator ==(_IN CString&    rString) const;
		bool operator ==(_IN SCF::LPTSTR szText)  const;
		bool operator ==(_IN SCF::TCHAR  cChar)   const;

		bool operator <=(_IN CString&    rString) const;
		bool operator <=(_IN SCF::LPTSTR szText)  const;

		//Equality with regard to case (Case Insensitive)
		bool IsEqualCI(_IN CString&    rString) const;
		bool IsEqualCI(_IN SCF::LPTSTR szText)  const;
	
		//Allows to test for object equality
		bool IsEqualTo(_IN CObject& rObject) const;
	
	public:
		//Faster than using the operator [=], since we pass the string length & sometimes we even NEED this
		void Assign(_IN SCF::LPTSTR sText, _IN SCF::UINT uiLength);
	
		//Assigns the memory of the given string object (source string) to the string on which the method is invoked (target string)
		//This method DOES NOT allocate new memory, but instead takes away control of the memory form the source string & assigns it to the target string
		//In case the target string has already some memory allocated, this memory is FREED!!!
		//Should be used ONLY in this form [String.Assign(Object.ToString())] to preserve the value returned by [ToString()] WITHOUT the overhead of copying the actual memory where the data is stored
		void Assign(_INOUT CString vString);

	public:
		void Resize(_IN SCF::UINT uiLength);
		void Clear();

		SCF::UINT Length()  _GET { return  m_uiLength; } 
		bool      IsEmpty() _GET { return (m_uiLength == 0); }
	
	public:
		//Low level routines for manipulating the storage directly
		SCF::UINT BytesReserved()                     _GET;
		void      BytesReserve(_IN SCF::UINT uiCount) _SET;
	
		SCF::UINT CharsReserved()                     _GET;
		void      CharsReserve(_IN SCF::UINT uiCount) _SET;

	public:
		//This must be called if u stored a string directly using the pointer 
		//returned by [Value()]
		void LengthScan();

	public:
		inline SCF::LPTSTR Value() _GET { return m_szValue; }

	public:
		void ToASCII(_OUT char* szText) const;

	public:
#ifdef _BETA
		bool Locked() { return (m_uiLocks > 0); }

		void LockAdd()     { m_uiLocks++; }
		void LockRelease() { m_uiLocks--; }
#endif

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const;
		void Deserialize(_INOUT IStreamRead&  rStream);
	
	public:
		//Size - allocates a new memory chunk, Resize - reallocates existing chunk
		void InternalSize  (_IN SCF::UINT uiLength);
		void InternalResize(_IN SCF::UINT uiLength);

	protected:
		SCF::LPTSTR StringAlloc  (_IN SCF::UINT uiLength);
		void        StringFree   (_IN SCF::LPTSTR szString);
		SCF::LPTSTR StringRealloc(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength, _IN SCF::UINT uiLengthNew);
       
	protected:
		SCF::LPTSTR m_szValue;
		SCF::UINT   m_uiLength;

	protected:
		BETAONLY(SCF::UINT m_uiLocks;)

	protected:
		bool m_bMemoryAllocated;
	};
};

#include "StreamReadObject.h"
#include "StreamWriteObject.h"

