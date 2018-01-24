#include "PathPredicateParser.h"

#include "PathStepPredicate.h"
#include "PathOperatorComparison.h"
#include "PathOperatorLogical.h"

#include "BoundValueAttribute.h"
#include "BoundValueChildNode.h"
#include "BoundValueNumber.h"
#include "BoundValueString.h"
#include "BoundValueNode.h"

#include "ComparerBoundValue.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);
extern bool CharIsAlpha(_IN TCHAR cChar);
extern bool CharIsNum(_IN TCHAR cChar);
extern bool CharIsOperator(_IN TCHAR cChar);

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

#define NEXT(state) m_fpNext = &CXMLPathPredicateParser::state

CXMLPathPredicateParser::CXMLPathPredicateParser()
{
	m_szText = nullptr;
	m_uiTextLength = 0;

	m_uiChar = 0;

	m_fpNext = nullptr;

	m_pValueLeft = nullptr;
	m_pOperatorComparison = nullptr;
	
	m_pString = nullptr;

	m_bPushNext = true;
}

CXMLPathPredicateParser::~CXMLPathPredicateParser()
{
	m_Predicates.AllRemove();
}

CXMLPathPredicate* CXMLPathPredicateParser::Parse(_IN CString& rText, _IN UINT uiStartChar, _OUT UINT& rOutCharLast)
{
	NEXT(ParseLeftValueType);

	m_Predicates.AllRemove();

	m_uiChar = uiStartChar;

	m_pValueLeft = nullptr;
	m_pOperatorComparison = nullptr;

	m_szText       = rText.Value();
	m_uiTextLength = rText.Length();

	if (m_pString) { delete m_pString; }
	m_pString = new CStringRange(rText, 0, 0);

	m_bPushNext = true;

	//Perform parsing
	while ((m_uiChar < m_uiTextLength) && (this->*(m_fpNext))()) {}

	delete m_pString;

	rOutCharLast = m_uiChar;

	if (m_Predicates.Size() > 0) { return m_Predicates.Bottom(); }

	return nullptr;
}

bool CXMLPathPredicateParser::ParseLeftValueType()
{
	if (m_szText[m_uiChar] == '(')
	{
		m_bPushNext = true;

		m_uiChar++;
		return true;
	}

	if (CharIsNum(m_szText[m_uiChar]))
	{
		NEXT(ParseLeftValueLiteralNumber);

		m_pValueLeft = new CXMLBoundValueNumber();

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (m_szText[m_uiChar] == '\'')
	{
		NEXT(ParseLeftValueLiteralText);

		m_pValueLeft = new CXMLBoundValueString();

		m_uiChar++;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (m_szText[m_uiChar] == '.')
	{
		NEXT(ParseOperatorCompare);

		m_pValueLeft = new CXMLBoundValueNode();

		m_uiChar++;
		return true;
	}

	if (m_szText[m_uiChar] == '@')
	{
		NEXT(ParseLeftValueAttribute);

		m_pValueLeft = new CXMLBoundValueAttribute();

		m_uiChar++;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (CharIsAlpha(m_szText[m_uiChar]))
	{
		NEXT(ParseLeftValueChildNode);

		m_pValueLeft = new CXMLBoundValueChildNode();

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);
		return true;
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseLeftValueAttribute() //@attributeName
{
	if (CharIsOperator(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorCompare);
		
		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueAttribute*)m_pValueLeft)->Name(*m_pString);

		return true;
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseLeftValueChildNode() //nodeName
{
	if (CharIsOperator(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorCompare);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueChildNode*)m_pValueLeft)->Name(*m_pString);
	}

	m_uiChar++;
	return true;
}
bool CXMLPathPredicateParser::ParseLeftValueLiteralNumber() //number
{
	if (CharIsOperator(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorCompare);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueNumber*)m_pValueLeft)->Value(*m_pString);
	}

	m_uiChar++;
	return true;
}
bool CXMLPathPredicateParser::ParseLeftValueLiteralText() //'some text'
{
	if (m_szText[m_uiChar] == '\'')
	{
		NEXT(ParseOperatorCompare);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueString*)m_pValueLeft)->Value(*m_pString);
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseRightValueType()
{
	if (CharIsNum(m_szText[m_uiChar]))
	{
		NEXT(ParseRightValueLiteralNumber);

		m_pOperatorComparison->ValueLeft(*m_pValueLeft);
		m_pOperatorComparison->ValueRight(*(new CXMLBoundValueNumber()));

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (m_szText[m_uiChar] == '\'')
	{
		NEXT(ParseRightValueLiteralText);

		m_pOperatorComparison->ValueLeft(*m_pValueLeft);
		m_pOperatorComparison->ValueRight(*(new CXMLBoundValueString()));

		m_uiChar++;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (m_szText[m_uiChar] == '.')
	{
		NEXT(ParseOperatorLogical);

		m_pOperatorComparison->ValueLeft(*m_pValueLeft);
		m_pOperatorComparison->ValueRight(*(new CXMLBoundValueNode()));

		m_uiChar++;
		return true;
	}

	if (m_szText[m_uiChar] == '@')
	{
		NEXT(ParseRightValueAttribute);

		m_pOperatorComparison->ValueLeft(*m_pValueLeft);
		m_pOperatorComparison->ValueRight(*(new CXMLBoundValueAttribute()));

		m_uiChar++;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (CharIsAlpha(m_szText[m_uiChar]))
	{
		NEXT(ParseRightValueChildNode);

		m_pOperatorComparison->ValueLeft(*m_pValueLeft);
		m_pOperatorComparison->ValueRight(*(new CXMLBoundValueChildNode()));

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseRightValueAttribute() //@attributeName
{
	if (CharIsSpecial(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorLogical);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueAttribute*)m_pOperatorComparison->ValueRight())->Name(*m_pString);
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseRightValueChildNode() //nodeName
{
	if (CharIsSpecial(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorLogical);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueChildNode*)m_pOperatorComparison->ValueRight())->Name(*m_pString);
	}

	m_uiChar++;
	return true;
}
bool CXMLPathPredicateParser::ParseRightValueLiteralNumber() //number
{
	if (CharIsSpecial(m_szText[m_uiChar]) || CharIsWhiteSpace(m_szText[m_uiChar]))
	{
		NEXT(ParseOperatorLogical);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueNumber*)m_pOperatorComparison->ValueRight())->Value(*m_pString);
	}

	m_uiChar++;
	return true;
}
bool CXMLPathPredicateParser::ParseRightValueLiteralText() //'some text'
{
	if (m_szText[m_uiChar] == '\'')
	{
		NEXT(ParseOperatorLogical);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLBoundValueString*)m_pOperatorComparison->ValueRight())->Value(*m_pString);
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseOperatorCompare() //<, >, =, !=, <=, >= 
{
	if ((m_szText[m_uiChar] == '<') && ((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar + 1] == '='))
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::LessOrEqual, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}

		m_uiChar += 2;
		return true;
	}

	if ((m_szText[m_uiChar] == '>') && ((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar + 1] == '='))
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::GreaterOrEqual, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}
		m_uiChar += 2;
		return true;
	}

	if ((m_szText[m_uiChar] == '!') && ((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar + 1] == '='))
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::NotEqual, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}
		m_uiChar += 2;
		return true;
	}

	if (m_szText[m_uiChar] == '<')
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::Less, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}
		m_uiChar++;
		return true;
	}

	if (m_szText[m_uiChar] == '>')
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::Greater, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}
		m_uiChar++;
		return true;
	}

	if (m_szText[m_uiChar] == '=')
	{
		NEXT(ParseRightValueType);

		m_pOperatorComparison = new CXMLPathOperatorComparison(&CXMLPathOperatorComparison::Equal, *(new CXMLComparerBoundValue()));

		if (m_bPushNext) 
		{
			m_Predicates.Push(*m_pOperatorComparison);
			m_bPushNext = false;
		}
		else
		{
			CXMLPathOperatorLogical* pOperator = (CXMLPathOperatorLogical*)m_Predicates.Top();
			pOperator->PredicateRight(*m_pOperatorComparison);
		}

		m_uiChar++;
		return true;
	}

	m_uiChar++;
	return true;
}

bool CXMLPathPredicateParser::ParseOperatorLogical() //And, Or
{
	if (m_szText[m_uiChar] == ')')
	{
		if (m_Predicates.Size() > 1)
		{
			CXMLPathPredicate* pPredicate = m_Predicates.Top();
			m_Predicates.Pop();

			CXMLPathOperatorLogical* pOperatorLogical = (CXMLPathOperatorLogical*)m_Predicates.Top();

			pOperatorLogical->PredicateRight(*pPredicate);
		}

		m_uiChar++;
		return true;
	}

	if (((m_szText[m_uiChar]     == 'A') || (m_szText[m_uiChar]     == 'a')) && ((m_uiChar + 2) < m_uiTextLength) && 
		((m_szText[m_uiChar + 1] == 'N') || (m_szText[m_uiChar + 1] == 'n')) &&
		((m_szText[m_uiChar + 2] == 'D') || (m_szText[m_uiChar + 2] == 'd')))
	{
		if (m_Predicates.Size() == 0) { return false; }

		NEXT(ParseLeftValueType);

		CXMLPathOperatorLogical* pOperatorLogical = new CXMLPathOperatorLogical(&CXMLPathOperatorLogical::And);

		pOperatorLogical->PredicateLeft(*m_Predicates.Top());

		m_Predicates.Pop();
		m_Predicates.Push(*pOperatorLogical);

		m_uiChar += 3;
		return true;
	}

	if (((m_szText[m_uiChar]     == 'O') || (m_szText[m_uiChar]     == 'o')) && ((m_uiChar + 1) < m_uiTextLength) &&
		((m_szText[m_uiChar + 1] == 'R') || (m_szText[m_uiChar + 1] == 'r')))
	{
		if (m_Predicates.Size() == 0) { return false; }

		NEXT(ParseLeftValueType);

		CXMLPathOperatorLogical* pOperatorLogical = new CXMLPathOperatorLogical(&CXMLPathOperatorLogical::Or);

		pOperatorLogical->PredicateLeft(*m_Predicates.Top());

		m_Predicates.Pop();
		m_Predicates.Push(*pOperatorLogical);

		m_uiChar += 2;
		return true;
	}

	m_uiChar++;
	return true;
}

