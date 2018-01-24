#include "Preprocessor.h"

//using namespace SCF;
using namespace SCFLibrarian;

bool CharIsAlpha(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) || (cChar == '_')) { return true; }

	return false;
}

bool CharIsNum(_IN TCHAR cChar)
{
	if ((cChar >= '0') && (cChar <= '9')) { return true; }

	return false;
}

bool CharIsAlphaNum(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) ||
	    ((cChar >= '0') && (cChar <= '9')) || (cChar == '_')) { return true; }

	return false;
}

bool CharIsSpecial(_IN TCHAR cChar)
{
	//This simplifies things as the namespace/package delimiter is part of a type name,
	//so we treat strings containing it as single entities instead of recognizing it
	//as an operator & handling it in the parser level
	//if (cChar == ':') { return false; } (cant be handled here .. nt so simple XD)
	if (cChar == '_')
	{ 
		return false; 
	}

	//Currently includes 35 = '#', 36 = '$', 64 = '@', 126 = '~' which are not used, but may be used in the future
	if (((cChar > 32)  && (cChar < 48)) ||
	    ((cChar > 57)  && (cChar < 65)) ||
	    ((cChar > 90)  && (cChar < 95)) ||
	    ((cChar > 122) && (cChar < 127))) { return true; }

	return false;
}

bool CharIsWhiteSpace(_IN TCHAR cChar)
{
	switch (cChar)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		{ return true; }
	default:
		{ return false; }
	}
}

bool CharIsNBWhiteSpace(_IN TCHAR cChar)
{
	switch (cChar)
	{
	case ' ':
	case '\t':
		{ return true; }
	default:
		{ return false; }
	}
}

bool CharIsLineEnd(_IN TCHAR cChar)
{
	switch (cChar)
	{
	case '\r':
	case '\n':
		{ return true; }
	default:
		{ return false; }
	}
}

bool CharIsWordSeparator(_IN TCHAR cChar)
{
	if (CharIsAlpha     (cChar)) { return false; }
	if (CharIsNum       (cChar)) { return false; }
	if (CharIsSpecial   (cChar)) { return true; }
	if (CharIsWhiteSpace(cChar)) { return true; }

	return false;
}

bool CharsFormOperator(_IN TCHAR cChar1st, _IN TCHAR cChar2nd)
{
	if (cChar2nd == '=')
	{
		if ((cChar1st == '+') ||
			(cChar1st == '-') ||
			(cChar1st == '*') ||
			(cChar1st == '/') ||
			(cChar1st == '>') ||
			(cChar1st == '<') ||
			(cChar1st == '=') ||
			(cChar1st == '%') ||
			(cChar1st == '!')) { return true; }
	}

	if (cChar2nd == cChar1st)
	{
		if ((cChar1st == '+') ||
			(cChar1st == '-') ||
			(cChar1st == '|') ||
			(cChar1st == '&')) { return true; }
	}

	return false;
}

CPreprocessor::CPreprocessor()
{
}

CPreprocessor::~CPreprocessor()
{
}

bool ParseDefineName(_INOUT CStringRange& rText)
{
	UINT uiStart = 0;

	while ((uiStart < rText.Length()) && CharIsNBWhiteSpace(rText[uiStart])) { uiStart++; }

	UINT uiEnd = uiStart;
	while ((uiEnd < rText.Length()) && !CharIsNBWhiteSpace(rText[uiEnd])) { uiEnd++; }

	rText.ChangeStart(rText.Start() + uiStart);
	rText.ChangeLength(uiEnd - uiStart);

	return true;
}

bool ParseDefineValue(_INOUT CStringRange& rText)
{
	UINT uiStart = 0;

	while ((uiStart < rText.Length()) && CharIsNBWhiteSpace(rText[uiStart])) { uiStart++; }

	UINT uiEnd = uiStart;
	while ((uiEnd < rText.Length()) && !CharIsLineEnd(rText[uiEnd])) { uiEnd++; }

	while ((uiEnd > uiStart) && CharIsNBWhiteSpace(rText[uiEnd])) { uiEnd--; }

	rText.ChangeStart(rText.Start() + uiStart);
	rText.ChangeLength(uiEnd - uiStart);

	return true;
}

bool CPreprocessor::Preprocess(_IN CString& rText)
{
	static CString Define(SCFTEXT("#define"));

	m_Text = rText;
	CStringRange DefinePotential = CStringRange(m_Text, 0, 7);
	CStringRange DefineName      = CStringRange(m_Text, 0);
	CStringRange DefineValue     = CStringRange(m_Text, 0);

	for (UINT i = 0; i < m_Text.Length(); i++)
	{
		DefinePotential.ChangeStart(i);

		if (DefinePotential == Define)
		{
			DefineName.ChangeStart(DefinePotential.Start() + DefinePotential.Length() + 1);
			DefineName.ChangeLength(m_Text.Length() - DefineName.Start());

			if (ParseDefineName(DefineName))
			{
				DefineValue.ChangeStart(DefineName.Start() + DefineName.Length() + 1);
				DefineValue.ChangeLength(m_Text.Length() - DefineValue.Start());

				if (ParseDefineValue(DefineValue))
				{
					m_Defines.AtPut(DefineName, DefineValue);
				}
			}
		}
	}

	return true;
}
