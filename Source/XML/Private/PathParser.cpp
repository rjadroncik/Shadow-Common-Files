#include "PathParser.h"

#include "PathStepParent.h"
#include "PathStepChildrenAll.h"
#include "PathStepChildrenNamed.h"
#include "PathStepDescendantsAll.h"
#include "PathStepDescendantsNamed.h"

#include "PathStepAttributesAll.h"
#include "PathStepAttributesNamed.h"
#include "PathStepDescendantsAttributesAll.h"
#include "PathStepDescendantsAttributesNamed.h"

#include "PathStepPredicate.h"
#include "PathStepIndex.h"

using namespace SCFXML;
using namespace SCFXMLPrivate;

extern bool CharIsWhiteSpace(_IN TCHAR cChar);
extern bool CharIsNum(_IN TCHAR cChar);

bool CharIsOperator(_IN TCHAR cChar)
{
	//Currently includes 35 = '#', 36 = '$', 64 = '@', 126 = '~' which are not used, but may be used in the future
	if (cChar > '>') { return false; }
	if ((cChar < '<') && (cChar != '!')) { return false; }

	return true;
}

#define NEXT(state) m_fpNext = &CXMLPathParser::state

CXMLPathParser::CXMLPathParser()
{
	m_szText = nullptr;
	m_uiTextLength = 0;

	m_uiChar = 0;

	m_fpNext = nullptr;
	m_pPathStepFirst = nullptr; 
	m_pPathStepLast = nullptr;

	m_pString = nullptr;
}

CXMLPathParser::~CXMLPathParser()
{
}

CXMLPathStep* CXMLPathParser::Parse(_IN CString& rText)
{
	NEXT(ParseStepFirst);

	m_uiChar = 0;
	m_pPathStepFirst = nullptr; 
	m_pPathStepLast = nullptr;

	m_szText       = rText.Value();
	m_uiTextLength = rText.Length();

	if (m_pString) { delete m_pString; }
	m_pString = new CStringRange(rText, 0, 0);

	//Perform parsing
	while ((m_uiChar < m_uiTextLength) && (this->*(m_fpNext))()) {}

	delete m_pString;

	return m_pPathStepFirst;
}

bool CXMLPathParser::ParseStepFirst()
{
	//Matches: /
	if (m_szText[m_uiChar] == '/')
	{
		//Matches: //
		if (((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar + 1] == '/'))
		{
			if ((m_uiChar + 3) < m_uiTextLength)
			{
				//Matches: //@*
				if ((m_szText[m_uiChar + 2] == '@') && (m_szText[m_uiChar + 3] == '*'))
				{
					NEXT(ParseStepType);

					m_pPathStepFirst = new CXMLPathStepDescendantsAttributesAll();
					m_pPathStepLast = m_pPathStepFirst;

					m_uiChar += 4;
					return true;
				}
			}

			if ((m_uiChar + 2) < m_uiTextLength)
			{
				//Matches: //*
				if (m_szText[m_uiChar + 2] == '*')
				{
					NEXT(ParseStepType);

					m_pPathStepFirst = new CXMLPathStepDescendantsAll();
					m_pPathStepLast = m_pPathStepFirst;

					m_uiChar += 3;
					return true;
				}

				//Matches: //@value
				if (m_szText[m_uiChar + 2] == '@')
				{
					NEXT(ParseStepName);

					m_pPathStepFirst = new CXMLPathStepDescendantsAttributesNamed();
					m_pPathStepLast = m_pPathStepFirst;

					m_uiChar += 3;

					m_pString->ChangeStart(m_uiChar);
					m_pString->ChangeLength(0);
					return true;
				}
			}

			//Matches: //value
			NEXT(ParseStepName);

			m_pPathStepFirst = new CXMLPathStepDescendantsNamed();
			m_pPathStepLast = m_pPathStepFirst;

			m_uiChar += 2;

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);
			return true;
		}

		if ((m_uiChar + 2) < m_uiTextLength)
		{
			//Matches: /@*
			if ((m_szText[m_uiChar + 1] == '@') && (m_szText[m_uiChar + 2] == '*'))
			{
				NEXT(ParseStepType);

				m_pPathStepFirst = new CXMLPathStepAttributesAll();
				m_pPathStepLast = m_pPathStepFirst;

				m_uiChar += 3;
				return true;
			}

			//Matches: /..
			if ((m_szText[m_uiChar + 1] == '.') && (m_szText[m_uiChar + 2] == '.'))
			{
				NEXT(ParseStepType);

				m_pPathStepFirst = new CXMLPathStepParent();
				m_pPathStepLast = m_pPathStepFirst;

				m_uiChar += 3;
				return true;
			}
		}

		if ((m_uiChar + 1)  < m_uiTextLength)
		{
			//Matches: /*
			if (m_szText[m_uiChar + 1] == '*')
			{
				NEXT(ParseStepType);

				m_pPathStepFirst = new CXMLPathStepChildrenAll();
				m_pPathStepLast = m_pPathStepFirst;

				m_uiChar += 2;
				return true;
			}

			//Matches: /@value
			if (m_szText[m_uiChar + 1] == '@')
			{
				NEXT(ParseStepName);

				m_pPathStepFirst = new CXMLPathStepAttributesNamed();
				m_pPathStepLast = m_pPathStepFirst;

				m_uiChar += 2;

				m_pString->ChangeStart(m_uiChar);
				m_pString->ChangeLength(0);
				return true;
			}
		}

		//Matches: /value
		NEXT(ParseStepName);

		m_pPathStepFirst = new CXMLPathStepChildrenNamed();
		m_pPathStepLast = m_pPathStepFirst;

		m_uiChar += 1;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);
		return true;
	}

	//Matches: ..
	if (((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar] == '.') && (m_szText[m_uiChar + 1] == '.'))
	{
		NEXT(ParseStepType);

		m_pPathStepLast = new CXMLPathStepParent();
		m_pPathStepFirst = m_pPathStepLast; 

		m_uiChar += 2;
		return true;
	}

	//Matches: value
	NEXT(ParseStepName);

	m_pPathStepLast = new CXMLPathStepChildrenNamed();
	m_pPathStepFirst = m_pPathStepLast; 

	return true;
}

bool CXMLPathParser::ParseStepType()
{
	//Matches: /
	if (m_szText[m_uiChar] == '/')
	{
		//Matches: //
		if (((m_uiChar + 1) < m_uiTextLength) && (m_szText[m_uiChar + 1] == '/'))
		{
			if ((m_uiChar + 3) < m_uiTextLength)
			{
				//Matches: //@*
				if ((m_szText[m_uiChar + 2] == '@') && (m_szText[m_uiChar + 3] == '*'))
				{
					m_pPathStepLast->Next(new CXMLPathStepDescendantsAttributesAll());
					m_pPathStepLast = m_pPathStepLast->Next();

					m_uiChar += 4;
					return true;
				}
			}

			if ((m_uiChar + 2) < m_uiTextLength)
			{
				//Matches: //*
				if (m_szText[m_uiChar + 2] == '*')
				{
					m_pPathStepLast->Next(new CXMLPathStepDescendantsAll());
					m_pPathStepLast = m_pPathStepLast->Next();

					m_uiChar += 3;
					return true;
				}

				//Matches: //@value
				if (m_szText[m_uiChar + 2] == '@')
				{
					NEXT(ParseStepName);

					m_pPathStepLast->Next(new CXMLPathStepDescendantsAttributesNamed());
					m_pPathStepLast = m_pPathStepLast->Next();

					m_uiChar += 3;

					m_pString->ChangeStart(m_uiChar);
					m_pString->ChangeLength(0);
					return true;
				}
			}

			//Matches: //value
			NEXT(ParseStepName);

			m_pPathStepLast->Next(new CXMLPathStepDescendantsNamed());
			m_pPathStepLast = m_pPathStepLast->Next();

			m_uiChar += 2;

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);
			return true;
		}

		if ((m_uiChar + 2) < m_uiTextLength)
		{
			//Matches: /@*
			if ((m_szText[m_uiChar + 1] == '@') && (m_szText[m_uiChar + 2] == '*'))
			{
				m_pPathStepLast->Next(new CXMLPathStepAttributesAll());
				m_pPathStepLast = m_pPathStepLast->Next();

				m_uiChar += 3;
				return true;
			}

			//Matches: /..
			if ((m_szText[m_uiChar + 1] == '.') && (m_szText[m_uiChar + 2] == '.'))
			{
				m_pPathStepLast->Next(new CXMLPathStepParent());
				m_pPathStepLast = m_pPathStepLast->Next();

				m_uiChar += 3;
				return true;
			}
		}

		if ((m_uiChar + 1)  < m_uiTextLength)
		{
			//Matches: /*
			if (m_szText[m_uiChar + 1] == '*')
			{
				m_pPathStepLast->Next(new CXMLPathStepChildrenAll());
				m_pPathStepLast = m_pPathStepLast->Next();

				m_uiChar += 2;
				return true;
			}

			//Matches: /@value
			if (m_szText[m_uiChar + 1] == '@')
			{
				NEXT(ParseStepName);

				m_pPathStepLast->Next(new CXMLPathStepAttributesNamed());
				m_pPathStepLast = m_pPathStepLast->Next();

				m_uiChar += 2;

				m_pString->ChangeStart(m_uiChar);
				m_pString->ChangeLength(0);
				return true;
			}
		}

		//Matches: /value
		NEXT(ParseStepName);
	
		m_pPathStepLast->Next(new CXMLPathStepChildrenNamed());
		m_pPathStepLast = m_pPathStepLast->Next();

		m_uiChar += 1;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);
		return true;
	}

	//Matches: [predicate]
	if (m_szText[m_uiChar] == '[')
	{
		NEXT(ParsePredicateStart);
		m_uiChar++;
		return true;
	}

	return false;
}

bool CXMLPathParser::ParsePredicateStart()
{
	//Matches: [integer]
	if (CharIsNum(m_szText[m_uiChar]))
	{
		NEXT(ParseStepIndex);
		
		m_pPathStepLast->Next(new CXMLPathStepIndex());
		m_pPathStepLast = m_pPathStepLast->Next();

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	NEXT(ParseStepType);

	CXMLPathPredicate* pPredicate = m_PredicateParser.Parse(m_pString->Parent(), m_uiChar, m_uiChar);
	if (pPredicate)
	{
		m_pPathStepLast->Next(new CXMLPathStepPredicate(pPredicate));
		m_pPathStepLast = m_pPathStepLast->Next();

		return true;
	}

	return false;
}

bool CXMLPathParser::ParseStepIndex() //Matches: integer]
{
	if (m_szText[m_uiChar] == ']')
	{
		NEXT(ParseStepType);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLPathStepIndex*)m_pPathStepLast)->Index(CInt::Parse(*m_pString, nullptr));
	}

	m_uiChar++;
	return true;
}

bool CXMLPathParser::ParseStepName()
{
	if (m_szText[m_uiChar] == '[')
	{
		NEXT(ParsePredicateStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLPathStepNamed*)m_pPathStepLast)->QName(*m_pString);

		m_uiChar++;
		return true;
	}

	if (m_szText[m_uiChar] == '/')
	{
		NEXT(ParseStepType);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLPathStepNamed*)m_pPathStepLast)->QName(*m_pString);

		return true;	
	}

	if ((m_uiChar + 1) == m_uiTextLength)
	{
		m_uiChar++;

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		((CXMLPathStepNamed*)m_pPathStepLast)->QName(*m_pString);
		return true;
	}

	m_uiChar++;
	return true;
}
