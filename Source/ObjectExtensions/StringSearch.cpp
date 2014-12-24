#include "StringSearch.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

CStringSearch::CStringSearch()
{
	m_pSource = NULL;
}

CStringSearch::CStringSearch(_IN CString& rSearchedString)
{
	m_pSource = (CString*)&rSearchedString; 
	ADDREF(*(m_pSource));
}

CStringSearch::~CStringSearch()
{
	RELEASE(*(m_pSource));
}

void CStringSearch::Source(_IN CString& rSearchedString)
{
	RELEASE(*(m_pSource));
	m_pSource = (CString*)&rSearchedString; 
	ADDREF(*(m_pSource));
}

int CStringSearch::FindChars(_IN CString& rCharacters)
{
	for (int i = 0; i < (int)m_pSource->Length(); i++)
	{
		for (int j = 0; j < (int)rCharacters.Length(); j++)
		{
			if ((*m_pSource)[i] == rCharacters[j]) { return i; }
		}
	}

	return -1;
}

int CStringSearch::FindCharsRev(_IN CString& rCharacters)
{
	for (int i = (int)m_pSource->Length() - 1; i >= 0; i--)
	{
		for (int j = 0; j < (int)rCharacters.Length(); j++)
		{
			if ((*m_pSource)[i] == rCharacters[j]) { return i; }
		}
	}

	return -1;
}

//Returns the index of the first occurrence of a character THAT IS NOT _IN a given character set
int CStringSearch::FindCharsInv(_IN CString& rCharacters)
{
	for (int i = 0; i < (int)m_pSource->Length(); i++)
	{
		register bool bNoMatch = true;

		for (int j = 0; j < (int)rCharacters.Length(); j++)
		{
			//If we found a match, mark result & stop lookup
			if ((*m_pSource)[i] == rCharacters[j]) { bNoMatch = FALSE; break; }
		}

		//If there was no match _IN the character set for the current character, quit
		if (bNoMatch) { return i; }
	}

	return -1;
}

int CStringSearch::FindCharsInvRev(_IN CString& rCharacters)
{
	for (int i = (int)m_pSource->Length() - 1; i >= 0; i--)
	{
		register bool bNoMatch = true;

		for (int j = 0; j < (int)rCharacters.Length(); j++)
		{
			//If we found a match, mark result & stop lookup
			if ((*m_pSource)[i] == rCharacters[j]) { bNoMatch = FALSE; break; }
		}

		//If there was no match _IN the character set for the current character, quit
		if (bNoMatch) { return i; }
	}

	return -1;
}

//Returns the index of the first occurrence of a sub-string
int CStringSearch::FindString(_IN CString& rString)
{
	for (int i = 0; i < (int)(m_pSource->Length() - rString.Length() + 1); i++)
	{
		register int j = 0;
		for (; j < (int)rString.Length(); j++)
		{ 
			if ((*m_pSource)[i + j] != rString[j]) { break; }
 		}

		if (j == (int)rString.Length()) { return i; }
	}

	return -1;
}

int CStringSearch::FindStringIC(_IN CString& rString)
{
	for (int i = 0; i < (int)(m_pSource->Length() - rString.Length() + 1); i++)
	{
		if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, &m_pSource->Value()[i], rString.Length(), rString.Value(), rString.Length()) == CSTR_EQUAL) { return i; }
		//if (!_wcsnicmp(&m_pSource->Value()[i], rString.Value(), rString.Length())) { return i; }
	}

	return -1;
}

//Returns the index of the first occurrence of a sub-string
int CStringSearch::FindStringsIC(_IN CVector<CString>& rStrings, _OUT _OPT CInt* pOutStringIndex)
{
	for (int i = 0; i < (int)m_pSource->Length(); i++)
	{
		for (int j = 0; j < (int)rStrings.Size(); j++)
		{
			//Skip empty sub-strings
			if (!((CString&)rStrings[j]).Length()) { continue; }

			//Skip this substring, if it is longer then the rest of the searched string
			if (((CString&)rStrings[j]).Length() > (m_pSource->Length() - i)) { continue; }

			//Compare sub-strings if they match return index in the searched string & store index of the found string
			if (CompareString(LOCALE_INVARIANT, NORM_IGNORECASE, &m_pSource->Value()[i], ((CString&)rStrings[j]).Length(), ((CString&)rStrings[j]).Value(), ((CString&)rStrings[j]).Length()) == CSTR_EQUAL)
			//if (!_wcsnicmp(&m_pSource->Value()[i], ((CString&)rStrings[j]).Value(), )) 
			{ 
				//..store result _IN output variable & quit
				if (pOutStringIndex) { *pOutStringIndex = (int)j; } 
				return i;
			}
		}
	}

	return -1;
}

//Returns the index of the first occurrence of a sub-string
int CStringSearch::FindStrings(_IN CVector<CString>& rStrings, _OUT _OPT CInt* pOutStringIndex)
{
	for (int i = 0; i < (int)m_pSource->Length(); i++)
	{
		for (int j = 0; j < (int)rStrings.Size(); j++)
		{
			//Skip empty sub-strings
			if (!((CString&)rStrings[j]).Length()) { continue; }

			//Skip this substring, if it is longer then the rest of the searched string
			if (((CString&)rStrings[j]).Length() > (m_pSource->Length() - i)) { continue; }

			//Compare characters & stop if we find a difference
			register int k = 0;
			for (; k < (int)((CString&)rStrings[j]).Length(); k++) { if ((*m_pSource)[i + k] != ((CString&)rStrings[j])[k]) { break; } }

			//If all character pairs were the same..
			if (k == (int)((CString&)rStrings[j]).Length()) 
			{
				//..store result _IN output variable & quit
				if (pOutStringIndex) { *pOutStringIndex = (int)j; } 
				return i;
			}
		}
	}

	return -1;
}