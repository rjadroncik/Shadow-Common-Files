#include "Scanner.h"

#include "Scanner/TokenWord.h"
#include "Scanner/TokenOperator.h"
#include "Scanner/TokenChar.h"
#include "Scanner/TokenString.h"
#include "Scanner/TokenNumber.h"


using namespace SCF;
using namespace SCFCompiler;

bool CharIsAlpha(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) || (cChar == '_')) { return TRUE; }

	return FALSE;
}

bool CharIsNum(_IN TCHAR cChar)
{
	if ((cChar >= '0') && (cChar <= '9')) { return TRUE; }

	return FALSE;
}

bool CharIsAlphaNum(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) ||
	    ((cChar >= '0') && (cChar <= '9')) || (cChar == '_')) { return TRUE; }

	return FALSE;
}

bool CharIsSpecial(_IN TCHAR cChar)
{
	//This simplifies things as the namespace/package delimiter is part of a type name,
	//so we treat strings containing it as single entities instead of recognizing it
	//as an operator & handling it in the parser level
	//if (cChar == ':') { return FALSE; } (cant be handled here .. nt so simple XD)
	if (cChar == '_')
	{ 
		return FALSE; 
	}

	//Currently includes 35 = '#', 36 = '$', 64 = '@', 126 = '~' which are not used, but may be used in the future
	if (((cChar > 32)  && (cChar < 48)) ||
	    ((cChar > 57)  && (cChar < 65)) ||
	    ((cChar > 90)  && (cChar < 95)) ||
	    ((cChar > 122) && (cChar < 127))) { return TRUE; }

	return FALSE;
}

bool CharIsWhiteSpace(_IN TCHAR cChar)
{
	switch (cChar)
	{
	case ' ':
	case '\t':
	case '\r':
	case '\n':
		{ return TRUE; }
	default:
		{ return FALSE; }
	}
}

bool CharIsWordSeparator(_IN TCHAR cChar)
{
	if (CharIsAlpha     (cChar)) { return FALSE; }
	if (CharIsNum       (cChar)) { return FALSE; }
	if (CharIsSpecial   (cChar)) { return TRUE; }
	if (CharIsWhiteSpace(cChar)) { return TRUE; }

	return FALSE;
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
			(cChar1st == '!')) { return TRUE; }
	}

	if (cChar2nd == cChar1st)
	{
		if ((cChar1st == '+') ||
			(cChar1st == '-') ||
			(cChar1st == '|') ||
			(cChar1st == '&')) { return TRUE; }
	}

	return FALSE;
}

#define NEXT(state) m_fpNext = &CScanner::state

CScanner::CScanner()
{
	m_pToken = NULL;
}

CScanner::~CScanner()
{
	m_Tokens.AllDelete();
}

void CScanner::TextReconstruct(_OUT CString& rOutText)
{
	rOutText.Resize(0);

	CEnumeratorList TokenEnumerator(m_Tokens);
	while (TokenEnumerator.Next())
	{
		rOutText += ((CToken*)TokenEnumerator.Current())->AsString();

		if (TokenEnumerator.Current() != &(m_Tokens.Last())) { rOutText += ' '; }
	}
}

void CScanner::PrintWordTypes(_OUT CString& rOutText)
{
	rOutText.Resize(0);

	CEnumeratorList TokenEnumerator(m_Tokens);
	while (TokenEnumerator.Next())
	{
		rOutText += ((CToken*)TokenEnumerator.Current())->TypeString();

		if (TokenEnumerator.Current() != &(m_Tokens.Last())) { rOutText += ' '; }
	}
}

bool CScanner::Scan(_IN CString& rText)
{
	//Clean up after last parsing
	m_Tokens.AllDelete();

	//Copy text & make sure the text is properly terminated with a newline character
	m_Text = rText;
	if (m_Text[m_Text.Length() - 1] != '\n') { m_Text += '\n'; }

	//Reset state
	m_pToken = NULL;
	m_uiChar = 0;
	NEXT(ScanWordStart);

	//Perform parsing
	while ((m_uiChar < m_Text.Length()) && (this->*(m_fpNext))()) {}

	//Update tokens (perform additional processing to gather more information required in subsequent compiler stages)
	CEnumeratorList TokenEnumerator(m_Tokens);
	while (TokenEnumerator.Next()) { ((CToken*)TokenEnumerator.Current())->Update(); }

	return TRUE;
}

bool CScanner::ScanWordStart()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlpha(cChar) || (cChar == ':'))
	{
		NEXT(ScanTokenWordContinue);

		m_pToken = new CTokenWord(*(new CStringRange(m_Text, m_uiChar, 1)));
		m_Tokens.LastAdd(*m_pToken);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsNum(cChar))
	{
		NEXT(ScanTokenNumberContinue);

		m_pToken = new CTokenNumber(*(new CStringRange(m_Text, m_uiChar, 1)));
		m_Tokens.LastAdd(*m_pToken);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsSpecial(cChar))
	{
		//Handle comments
		if ((cChar == '/') && (m_Text.Length() > (m_uiChar + 1)))
		{
			if (m_Text[m_uiChar + 1] == '/') 
			{
				NEXT(ScanCommentLine);

				m_uiChar += 2;
				return TRUE; 
			}

			if (m_Text[m_uiChar + 1] == '*')
			{
				NEXT(ScanCommentBlock);  

				m_uiChar += 2;
				return TRUE; 
			}
		}

		if ((cChar == '\"') || (cChar == '\''))
		{
			if (cChar == '\"') 
			{
				NEXT(ScanTokenStringContinue);

				m_pToken = new CTokenString(*(new CStringRange(m_Text, m_uiChar, 1)));
				m_Tokens.LastAdd(*m_pToken);
			}
			else
			{ 
				NEXT(ScanTokenStringContinueChar);

				m_pToken = new CTokenChar(*(new CStringRange(m_Text, m_uiChar, 1)));
				m_Tokens.LastAdd(*m_pToken);
			}
		}
		else
		{
			NEXT(ScanTokenOperatorContinue);

			m_pToken = new CTokenOperator(*(new CStringRange(m_Text, m_uiChar, 1)));
			m_Tokens.LastAdd(*m_pToken);
		}

		m_uiChar++;
		return TRUE; 
	}

	return FALSE;
}

bool CScanner::ScanTokenWordContinue()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlphaNum(cChar) || (cChar == ':'))
	{
		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);
		m_uiChar++;
		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return TRUE;
	}

	return FALSE;
}

bool CScanner::ScanTokenNumberContinue()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if ((cChar == '.') || (cChar == 'x'))
	{
		NEXT(ScanTokenNumberContinue2nd);

		if (cChar == '.') { ((CTokenNumber*)m_pToken)->ValueClassKey(ClassFloat); }
		else              { ((CTokenNumber*)m_pToken)->ValueClassKey(ClassInt); }

		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);
		m_uiChar++;

		return TRUE;
	}

	if (CharIsAlpha(cChar)) { return FALSE; }

	if (CharIsNum(cChar))
	{
		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);
		m_uiChar++;

		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return TRUE;
	}

	return FALSE;
}

bool CScanner::ScanTokenNumberContinue2nd()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];
	
	if (CharIsAlpha(cChar)) { return FALSE; }

	if (CharIsNum(cChar))
	{
		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);
		m_uiChar++;

		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		m_uiChar++;
		return TRUE;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return TRUE;
	}

	return FALSE;
}

bool CScanner::ScanTokenOperatorContinue()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlphaNum(cChar))
	{
		NEXT(ScanWordStart);
		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);
		m_uiChar++;
		return TRUE;
	}

	if (CharIsSpecial(cChar))
	{
		//If the combination of the previous char & current char form an operator
		if (CharsFormOperator(m_Text[m_uiChar - 1], m_Text[m_uiChar]))
		{
			m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);
			m_uiChar++;

			return TRUE;
		}

		NEXT(ScanWordStart);
		return TRUE;
	}

	return FALSE;
}

bool CScanner::ScanTokenStringContinue()
{
	//Skip escaped characters (supports only 2 char sequences this way)
	if (m_Text[m_uiChar] == '\\')
	{
		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 2);
		m_uiChar += 2;
		return TRUE;
	}

	m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);

	if (m_Text[m_uiChar] == '\"') 
	{
		NEXT(ScanWordStart); 
	}

	m_uiChar++;
	return TRUE;
}

bool CScanner::ScanTokenStringContinueChar()
{
	//Skip escaped characters (supports only 2 char sequences this way)
	if (m_Text[m_uiChar] == '\\')
	{
		m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 2);
		m_uiChar += 2;
		return TRUE;
	}

	m_pToken->Text().ChangeLength(m_pToken->Text().Length() + 1);

	//Skip escaped characters (supports only 2 char sequences this way)
	if (m_Text[m_uiChar] == '\\')
	{
		m_uiChar += 2;
		return TRUE;
	}

	if (m_Text[m_uiChar] == '\'')
	{
		NEXT(ScanWordStart); 
	}

	m_uiChar++;
	return TRUE;
}

bool CScanner::ScanWhitespaceContinue()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlphaNum(cChar) || CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return TRUE;
	}

	if (CharIsWhiteSpace(cChar))
	{
		m_uiChar++;
		return TRUE;
	}

	return FALSE;
}

bool CScanner::ScanCommentLine()
{
	if (m_Text[m_uiChar] == '\n') { NEXT(ScanWordStart); }

	m_uiChar++;
	return TRUE;
}

bool CScanner::ScanCommentBlock()
{
	if ((m_Text[m_uiChar - 1] == '*') && (m_Text[m_uiChar] == '/')) { NEXT(ScanWordStart); }

	m_uiChar++;
	return TRUE;
}
