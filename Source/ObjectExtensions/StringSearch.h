#pragma once
#include "String.h"
#include "Int.h"
#include "Vector.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStringSearch : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStringSearch; }
		CString   ToString() _GET { return STRING("{StringSearch}"); }

	public:
		CStringSearch(_IN CString& rSearchedString);
		virtual ~CStringSearch();

	public:
		void     Source(_IN CString& rSearchedString);
		CString& Source() { return *m_pSource; }

	//All Find[..] functions return the starting index of the found string/char if a match was found, -1 otherwise

	public:
		int FindChars   (_IN CString& rCharacters);
		int FindCharsRev(_IN CString& rCharacters);
	
		int FindCharsInv   (_IN CString& rCharacters);
		int FindCharsInvRev(_IN CString& rCharacters);

	public:
		int FindString  (_IN CString& rString);
		int FindStringIC(_IN CString& rString);

		int FindStrings  (_IN CVector<CString>& rStrings, _OUT _OPT CInt* pOutStringIndex);
		int FindStringsIC(_IN CVector<CString>& rStrings, _OUT _OPT CInt* pOutStringIndex);

	protected:
		CString* m_pSource;

	private:
		CStringSearch();
	};
};
