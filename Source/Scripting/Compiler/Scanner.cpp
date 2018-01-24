#include "Scanner.h"

#include "Scanner/TokenKeyword.h"
#include "Scanner/TokenIdentifier.h"
#include "Scanner/TokenOperator.h"
#include "Scanner/TokenChar.h"
#include "Scanner/TokenString.h"
#include "Scanner/TokenNumber.h"
#include "Scanner/TokenType.h"

//using namespace SCF;
using namespace SCFCompiler;

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
	if (cChar == '_') { return false; }

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

CDictionaryString<SCFBase::CEnum> Scanner_Keywords;

void Scanner_Initialize()
{
	if (!Scanner_Keywords.Size())
	{
		//Primitive types
		Scanner_Keywords.AtPut(STRING("int"),      *(new CEnum(TypeInt)));
		Scanner_Keywords.AtPut(STRING("long"),     *(new CEnum(TypeLong)));
		Scanner_Keywords.AtPut(STRING("float"),    *(new CEnum(TypeFloat)));
		Scanner_Keywords.AtPut(STRING("double"),   *(new CEnum(TypeDouble)));
		Scanner_Keywords.AtPut(STRING("bool"),     *(new CEnum(TypeBool)));
		Scanner_Keywords.AtPut(STRING("char"),     *(new CEnum(TypeChar)));
		Scanner_Keywords.AtPut(STRING("string"),   *(new CEnum(TypeString)));
		Scanner_Keywords.AtPut(STRING("void"),     *(new CEnum(TypeVoid)));
		Scanner_Keywords.AtPut(STRING("object"),   *(new CEnum(TypeObject)));
		Scanner_Keywords.AtPut(STRING("var"),      *(new CEnum(TypeVar)));

		//Definable types
		Scanner_Keywords.AtPut(STRING("package"),   *(new CEnum(KeywordPackage)));
		Scanner_Keywords.AtPut(STRING("class"),     *(new CEnum(KeywordClass)));
		Scanner_Keywords.AtPut(STRING("interface"), *(new CEnum(KeywordInterface)));
		Scanner_Keywords.AtPut(STRING("enum"),      *(new CEnum(KeywordEnum)));
		Scanner_Keywords.AtPut(STRING("import"),    *(new CEnum(KeywordImport)));

		//Reserved values
		Scanner_Keywords.AtPut(STRING("null"),  *(new CEnum(KeywordNull)));
		Scanner_Keywords.AtPut(STRING("true"),  *(new CEnum(KeywordTrue)));
		Scanner_Keywords.AtPut(STRING("false"), *(new CEnum(KeywordFalse)));

		//Flow control
		Scanner_Keywords.AtPut(STRING("switch"),  *(new CEnum(KeywordSwitch)));
		Scanner_Keywords.AtPut(STRING("case"),    *(new CEnum(KeywordCase)));
		Scanner_Keywords.AtPut(STRING("default"), *(new CEnum(KeywordDefault)));
		Scanner_Keywords.AtPut(STRING("if"),      *(new CEnum(KeywordIf)));
		Scanner_Keywords.AtPut(STRING("else"),    *(new CEnum(KeywordElse)));

		Scanner_Keywords.AtPut(STRING("break"),    *(new CEnum(KeywordBreak)));
		Scanner_Keywords.AtPut(STRING("continue"), *(new CEnum(KeywordContinue)));
		Scanner_Keywords.AtPut(STRING("return"),   *(new CEnum(KeywordReturn)));

		//Loops
		Scanner_Keywords.AtPut(STRING("do"),      *(new CEnum(KeywordDo)));
		Scanner_Keywords.AtPut(STRING("while"),   *(new CEnum(KeywordWhile)));
		Scanner_Keywords.AtPut(STRING("for"),     *(new CEnum(KeywordFor)));
		Scanner_Keywords.AtPut(STRING("foreach"), *(new CEnum(KeywordForeach)));
		Scanner_Keywords.AtPut(STRING("in"),      *(new CEnum(KeywordIn)));

		//Argument usage specifiers
		Scanner_Keywords.AtPut(STRING("out"),   *(new CEnum(KeywordOut)));

		//Access level specifiers
		Scanner_Keywords.AtPut(STRING("public"),    *(new CEnum(KeywordPublic)));
		Scanner_Keywords.AtPut(STRING("protected"), *(new CEnum(KeywordProtected)));
		Scanner_Keywords.AtPut(STRING("private"),   *(new CEnum(KeywordPrivate)));

		//Method & variable modifiers
		Scanner_Keywords.AtPut(STRING("static"),  *(new CEnum(KeywordStatic)));
		Scanner_Keywords.AtPut(STRING("const"),   *(new CEnum(KeywordConst)));

		//Method modifiers
		Scanner_Keywords.AtPut(STRING("virtual"),  *(new CEnum(KeywordVirtual)));
		Scanner_Keywords.AtPut(STRING("abstract"), *(new CEnum(KeywordAbstract)));
		Scanner_Keywords.AtPut(STRING("override"), *(new CEnum(KeywordOverride)));

		//Class declaration
		Scanner_Keywords.AtPut(STRING("extends"),    *(new CEnum(KeywordExtends)));
		Scanner_Keywords.AtPut(STRING("implements"), *(new CEnum(KeywordImplements)));

		//Object/type management operators
		Scanner_Keywords.AtPut(STRING("new"),    *(new CEnum(KeywordNew)));
		Scanner_Keywords.AtPut(STRING("delete"), *(new CEnum(KeywordDelete)));
		Scanner_Keywords.AtPut(STRING("import"), *(new CEnum(KeywordImport)));
		Scanner_Keywords.AtPut(STRING("typeof"), *(new CEnum(KeywordTypeof)));
	}
}

#define NEXT(state) m_fpNext = &CScanner::state

CScanner::CScanner()
{
	Scanner_Initialize();
}

CScanner::~CScanner()
{
	m_Tokens.AllDelete();
}

void CScanner::TextReconstruct(_OUT CString& rOutText)
{
	rOutText.Resize(0);

	UINT uiLine = 1;
	UINT uiColumn = 1;

	CEnumeratorList<CToken> TokenEnumerator(m_Tokens);
	while (TokenEnumerator.Next())
	{
		CToken* pToken = ((CToken*)TokenEnumerator.Current());

		while (uiLine < pToken->Line())
		{
			rOutText += '\n';
			uiLine++;
			uiColumn = 1;
		}

		while (uiColumn < pToken->Column())
		{
			rOutText += ' ';
			uiColumn++;
		}

		rOutText += pToken->Text();
		uiColumn += pToken->Text().Length();
	}
}

void CScanner::PrintWords(_OUT CString& rOutText)
{
	rOutText.Resize(0);

	CEnumeratorList<CToken> TokenEnumerator(m_Tokens);
	while (TokenEnumerator.Next())
	{
		rOutText += ((CToken*)TokenEnumerator.Current())->ToString();

		if (TokenEnumerator.Current() != &(m_Tokens.Last())) { rOutText += ' '; }
	}
}

void CScanner::PrintWordTypes(_OUT CString& rOutText)
{
	rOutText.Resize(0);

	CEnumeratorList<CToken> TokenEnumerator(m_Tokens);
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
	m_uiStart = 0;
	m_uiChar = 0;
	m_uiLine = 1;
	m_uiColumn = 0;
	NEXT(ScanWordStart);

	//Perform parsing
	while ((m_uiChar < m_Text.Length()) && (this->*(m_fpNext))()) {}

	return true;
}

bool CScanner::ScanWordStart()
{
	const TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlpha(cChar))
	{
		NEXT(ScanTokenWordContinue);
		m_uiStart = m_uiChar;

		IncrementChar(1);
		return true;
	}

	if (CharIsNum(cChar))
	{
		NEXT(ScanTokenNumberContinue);
		m_uiStart = m_uiChar;

		IncrementChar(1);
		return true;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	if (CharIsSpecial(cChar))
	{
		//Handle comments
		if ((cChar == '/') && (m_Text.Length() > (m_uiChar + 1)))
		{
			if (m_Text[m_uiChar + 1] == '/') 
			{
				NEXT(ScanCommentLine);

				IncrementChar(2);
				return true; 
			}

			if (m_Text[m_uiChar + 1] == '*')
			{
				NEXT(ScanCommentBlock);  

				IncrementChar(2);
				return true; 
			}
		}

		if ((cChar == '\"') || (cChar == '\''))
		{
			if (cChar == '\"') 
			{
				NEXT(ScanTokenStringContinue);
				m_uiStart = m_uiChar;
			}
			else
			{ 
				NEXT(ScanTokenStringContinueChar);
				m_uiStart = m_uiChar;
			}
		}
		else
		{
			NEXT(ScanTokenOperatorContinue);
			m_uiStart = m_uiChar;
		}

		IncrementChar(1);
		return true; 
	}

	return false;
}

bool CScanner::ScanTokenWordContinue()
{
	const TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlphaNum(cChar) || (cChar == ':') || (cChar == '.'))
	{
		IncrementChar(1);
		return true;
	}

	CStringRange* pString = new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart);

	CEnum* pEnum = (CEnum*)Scanner_Keywords.At(*pString);
	if (pEnum) 
	{
		if ((pEnum->Value() >= KEYWORD_MIN) && (pEnum->Value() <= KEYWORD_MAX))
		{
			m_Tokens.LastAdd(*(new CTokenKeyword(*pString, pEnum->Value(), m_uiLine, m_uiColumn)));
		}
		else
		{
			m_Tokens.LastAdd(*(new CTokenType(*pString, pEnum->Value(), m_uiLine, m_uiColumn)));
		}
	}
	else { m_Tokens.LastAdd(*(new CTokenIdentifier(*pString, m_uiLine, m_uiColumn))); }

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return true;
	}

	return false;
}

bool CScanner::ScanTokenNumberContinue()
{
	const TCHAR cChar = m_Text[m_uiChar];

	if ((cChar == '.') || (cChar == 'x'))
	{
		NEXT(ScanTokenNumberContinue2nd);

		IncrementChar(1);
		return true;
	}

	if (CharIsAlpha(cChar)) { return false; }

	if (CharIsNum(cChar))
	{
		IncrementChar(1);
		return true;
	}

	m_Tokens.LastAdd(*(new CTokenNumber(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return true;
	}

	return false;
}

bool CScanner::ScanTokenNumberContinue2nd()
{
	const TCHAR cChar = m_Text[m_uiChar];
	
	if (CharIsAlpha(cChar)) { return false; }

	if (CharIsNum(cChar))
	{
		IncrementChar(1);
		return true;
	}

	m_Tokens.LastAdd(*(new CTokenNumber(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	if (CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return true;
	}

	return false;
}

bool CScanner::ScanTokenOperatorContinue()
{
	const TCHAR cChar = m_Text[m_uiChar];

	if (CharIsSpecial(cChar))
	{
		//If the combination of the previous char & current char form an operator
		if (CharsFormOperator(m_Text[m_uiChar - 1], m_Text[m_uiChar]))
		{
			IncrementChar(1);
			return true;
		}

		NEXT(ScanWordStart);

		m_Tokens.LastAdd(*(new CTokenOperator(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));
		return true;
	}

	m_Tokens.LastAdd(*(new CTokenOperator(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));

	if (CharIsAlphaNum(cChar))
	{
		NEXT(ScanWordStart);
		return true;
	}

	if (CharIsWhiteSpace(cChar))
	{
		NEXT(ScanWhitespaceContinue);

		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	return false;
}

bool CScanner::ScanTokenStringContinue()
{
	//Skip escaped characters (supports only 2 char sequences this way)
	if (m_Text[m_uiChar] == '\\')
	{
		IncrementChar(2);
		return true;
	}

	if (m_Text[m_uiChar] == '\"') 
	{
		NEXT(ScanWordStart); 

		IncrementChar(1);
		m_Tokens.LastAdd(*(new CTokenString(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));
		return true;
	}

	IncrementChar(1);
	return true;
}

bool CScanner::ScanTokenStringContinueChar()
{
	//Skip escaped characters (supports only 2 char sequences this way)
	if (m_Text[m_uiChar] == '\\')
	{
		IncrementChar(2);
		return true;
	}

	if (m_Text[m_uiChar] == '\'')
	{
		NEXT(ScanWordStart); 
	
		IncrementChar(1);
		m_Tokens.LastAdd(*(new CTokenChar(*(new CStringRange(m_Text, m_uiStart, m_uiChar - m_uiStart)), m_uiLine, m_uiColumn)));
		return true;
	}

	IncrementChar(1);
	return true;
}

bool CScanner::ScanWhitespaceContinue()
{
	const TCHAR cChar = m_Text[m_uiChar];

	if (CharIsAlphaNum(cChar) || CharIsSpecial(cChar))
	{
		NEXT(ScanWordStart);
		return true;
	}

	if (CharIsWhiteSpace(cChar))
	{
		CheckForNextLine(cChar);
		IncrementChar(1);
		return true;
	}

	return false;
}

bool CScanner::ScanCommentLine()
{
	if (m_Text[m_uiChar] == '\n') { NEXT(ScanWordStart); }

	CheckForNextLine(m_Text[m_uiChar]);
	IncrementChar(1);
	return true;
}

bool CScanner::ScanCommentBlock()
{
	if ((m_Text[m_uiChar - 1] == '*') && (m_Text[m_uiChar] == '/')) { NEXT(ScanWordStart); }

	CheckForNextLine(m_Text[m_uiChar]);
	IncrementChar(1);
	return true;
}

void CScanner::CheckForNextLine(TCHAR cChar)
{
	if (cChar == '\n')
	{
		m_uiLine++;
		m_uiColumn = 0;
	}
}