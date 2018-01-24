#include "Reader.h"

#include "Declaration.h"
#include "Notation.h"
#include "Comment.h"
#include "CData.h"

using namespace SCFXML;

#define NEXT(state) m_fpNext = &CXMLReader::state

CDictionaryString<CChar>* Parser_pEntitiesDefault = nullptr;

bool CharIsAlpha(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) ||
		 (cChar == '_')) { return true; }

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
		((cChar >= '0') && (cChar <= '9'))) { return true; }

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

CXMLReader::CXMLReader()
{
	m_szText = nullptr;
	m_uiTextLength = 0;

	m_uiChar = 0;

	m_pNode      = nullptr;
	m_pAttribute = nullptr;
	m_pString    = nullptr;
	m_pRoot      = nullptr;

	m_fpNext = nullptr;
}

CXMLReader::~CXMLReader()
{
	m_Nodes.AllRemove();
}

CXMLReader::CXMLReader(_IN CString& rText, _INOUT CXMLNode& rRoot, _INOUT CDictionaryString<CChar>* pEntities)
{
	//Valid since all member variables get (re)initialized in the [Read(..)] function
	Read(rText, rRoot, pEntities);
}

bool CXMLReader::Read(_IN CString& rText, _INOUT CXMLNode& rRoot, _INOUT CDictionaryString<CChar>* pEntities)
{
	NEXT(ParseTagStart);

	m_uiChar = 0;

	m_pNode      = nullptr;
	m_pAttribute = nullptr;
	m_pRoot      = &rRoot;

	if (pEntities) { m_pEntities = pEntities; }
	else
	{
		if (!Parser_pEntitiesDefault)
		{
			Parser_pEntitiesDefault = new CDictionaryString<CChar>();

			Parser_pEntitiesDefault->AtPut(STRING("amp"),  *(new CChar('&')));
			Parser_pEntitiesDefault->AtPut(STRING("lt"),   *(new CChar('<')));
			Parser_pEntitiesDefault->AtPut(STRING("gt"),   *(new CChar('>')));
			Parser_pEntitiesDefault->AtPut(STRING("apos"), *(new CChar('\'')));
			Parser_pEntitiesDefault->AtPut(STRING("quot"), *(new CChar('"')));
		}

		m_pEntities = Parser_pEntitiesDefault;
	}

	m_Nodes.AllDelete();

	m_szText       = rText.Value();
	m_uiTextLength = rText.Length();
	
	//if (m_szText[m_uiTextLength - 1] != '\n') { m_Text += '\n'; }

	m_pString = new CStringRange(rText, 0, 0);

	//Perform parsing
	while ((m_uiChar < m_uiTextLength) && (this->*(m_fpNext))()) {}

	delete m_pString;

	return true;
}

bool CXMLReader::ParseTagStart()
{
	const TCHAR cChar = m_szText[m_uiChar];

	if (cChar == '<')
	{
		NEXT(ParseTagType);
	}

	m_uiChar++;
	return true;
}

#define UPDATE_ROOT if (m_Nodes.Size() == 1) { m_pRoot->ChildAdd(*(CXMLNode*)m_Nodes.Top()); }

bool CXMLReader::ParseTagType()
{
	const TCHAR cChar = m_szText[m_uiChar];

	if (cChar == '/')
	{
		NEXT(ParseTagEnd);
		UPDATE_ROOT

		m_Nodes.Pop();
		m_uiChar++;

		return true;
	}

	if (cChar == '!')
	{
		if ((m_uiTextLength > (m_uiChar + 2)) && (m_szText[m_uiChar + 1] == '-') && (m_szText[m_uiChar + 2] == '-'))
		{
			NEXT(ParseTagComment);

			m_pNode = new CXMLComment();
			m_uiChar += 3;		

			if (!m_Nodes.IsEmpty()) { m_Nodes.Top()->ChildAdd(*m_pNode); }
			m_Nodes.Push(*m_pNode);

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);

			return true;
		}

		if ((m_uiTextLength > (m_uiChar + 7)) && (m_szText[m_uiChar + 1] == '[') && (m_szText[m_uiChar + 7] == '['))
		{
			NEXT(ParseTagCDATA);

			m_pNode = new CXMLCData();
			m_uiChar += 8;		

			if (!m_Nodes.IsEmpty()) { m_Nodes.Top()->ChildAdd(*m_pNode); }
			m_Nodes.Push(*m_pNode);

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);

			return true;
		}

		NEXT(ParseTagNotation);

		m_pNode = new CXMLNotation();
		m_uiChar++;

		if (!m_Nodes.IsEmpty()) { m_Nodes.Top()->ChildAdd(*m_pNode); }
		m_Nodes.Push(*m_pNode);

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
 	}

	if (cChar == '?') 
	{
		NEXT(ParseTagDeclaration);
		m_uiChar++;
	}
	else
	{
		NEXT(ParseTagName);

		m_pNode = new CXMLElement();

		if (!m_Nodes.IsEmpty()) { m_Nodes.Top()->ChildAdd(*m_pNode); }
		m_Nodes.Push(*m_pNode);
	}

	m_pString->ChangeStart(m_uiChar);
	m_pString->ChangeLength(0);

	return true;
}

bool CXMLReader::ParseTagEnd()
{
	if (m_szText[m_uiChar] == '>')
	{
		NEXT(ParseTagStart);
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseTagName()
{
	const TCHAR cChar = m_szText[m_uiChar];

	if (cChar == '>')
	{
		//NEXT(ParseTagStart);
		NEXT(ParseTagData);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->QName(*m_pString);

		m_uiChar++;
		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (cChar == '/')
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->QName(*m_pString);

		UPDATE_ROOT

		m_Nodes.Pop();
		m_uiChar += 2;

		return true;
	}

	if (cChar == ':')
	{
		m_pNode->PrefixLength(m_uiChar - m_pString->Start());

		m_uiChar++;
		return true;
	}

 	if (CharIsWhiteSpace(cChar))
 	{
		NEXT(ParseAttributeNameStart);
 
		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->QName(*m_pString);
 	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseTagData()
{
	const TCHAR cChar = m_szText[m_uiChar];

	if (cChar == '<')
	{
		if ((m_uiTextLength > (m_uiChar + 1)) && (m_szText[m_uiChar + 1] == '/'))
		{
			NEXT(ParseTagEnd); 

			UPDATE_ROOT

			m_pString->ChangeLength(m_uiChar - m_pString->Start());
			m_pNode->Value(ParseValueString());		

			m_Nodes.Pop();

			m_uiChar += 2;
			return true;
		}

		NEXT(ParseTagType); 
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseAttributeNameStart() 
{
	const TCHAR cChar = m_szText[m_uiChar];

	if (cChar == '>')
	{
		NEXT(ParseTagData); 

		m_uiChar++;
		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;
	}

	if (cChar == '/')
	{
		NEXT(ParseTagStart);

		UPDATE_ROOT

		m_Nodes.Pop();
		m_uiChar += 2;

		return true;
	}

	if (cChar == '?')
	{
		NEXT(ParseTagStart); 
	
		UPDATE_ROOT

		m_Nodes.Pop();
		m_uiChar += 2;

		return true;
	}

	if (!CharIsWhiteSpace(cChar)) 
	{
		NEXT(ParseAttributeName); 
		
		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;	
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseAttributeName() 
{
	const TCHAR cChar = m_szText[m_uiChar];

	if ((cChar == '=') || CharIsWhiteSpace(cChar)) 
	{ 
		NEXT(ParseAttributeValueStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());

		m_pAttribute = new CXMLAttribute();
		m_pAttribute->Name(*m_pString);
	}

	m_uiChar++;
	return true; 
}

bool CXMLReader::ParseAttributeValueStart() 
{ 
	const TCHAR cChar = m_szText[m_uiChar];

	if (!CharIsWhiteSpace(cChar)) 
	{
		NEXT(ParseAttributeValue);

		if (cChar == '\'') { m_uiChar++; NEXT(ParseAttributeValueQuoted); }
		if (cChar == '\"') { m_uiChar++; NEXT(ParseAttributeValueQuotedDouble); }

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return true;	
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseAttributeValue()
{
	if (CharIsWhiteSpace(m_szText[m_uiChar])) 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString());

		((CXMLElement*)m_pNode)->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return true; 
}

bool CXMLReader::ParseAttributeValueQuoted()
{
	if (m_szText[m_uiChar] == '\'') 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString());

		((CXMLElement*)m_pNode)->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return true; 
}

bool CXMLReader::ParseAttributeValueQuotedDouble()
{
	if (m_szText[m_uiChar] == '\"') 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString());

		((CXMLElement*)m_pNode)->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return true; 
}

bool CXMLReader::ParseTagComment()
{
	if ((m_szText[m_uiChar] == '-') && (m_uiTextLength > (m_uiChar + 2)) && (m_szText[m_uiChar + 1] == '-') && (m_szText[m_uiChar + 2] == '>'))
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Value(new CString(*m_pString));

		m_Nodes.Pop();
		m_uiChar += 3;		

		return true;
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseTagNotation()
{
	if (m_szText[m_uiChar] == '>')
	{
		NEXT(ParseTagStart);

		m_Nodes.Pop();
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseTagDeclaration()
{
	if (m_szText[m_uiChar] == '>')
	{
		NEXT(ParseTagStart);
	}

	m_uiChar++;
	return true;
}

bool CXMLReader::ParseTagCDATA()
{
	if ((m_szText[m_uiChar] == ']') && (m_uiTextLength > (m_uiChar + 2)) && (m_szText[m_uiChar + 1] == ']') && (m_szText[m_uiChar + 2] == '>'))
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Value(new CString(*m_pString));

		m_Nodes.Pop();
		m_uiChar += 3;		

		return true;
	}

	m_uiChar++;
	return true;
}

CString* CXMLReader::ParseValueString()
{
	CString* pResult = new CString();
	pResult->CharsReserve(m_pString->Length());

	TCHAR* sEnd = &(m_pString->Value()[m_pString->Length()]);
	
	TCHAR* sCurSrc = m_pString->Value();
	TCHAR* sCurDst = pResult->Value();

	while (sCurSrc < sEnd)
	{
		if (*sCurSrc == '&')
		{
			sCurSrc++;

			if (*sCurSrc == '#')
			{
				sCurSrc++;

				TCHAR* sEntityEnd = sCurSrc;

				while (sEntityEnd < sEnd)
				{
					if (*sEntityEnd == ';')
					{
						*sCurDst = (TCHAR)CInt::Parse(CStringRange(*m_pString, sCurSrc - m_pString->Value(), sEntityEnd - sCurSrc), nullptr);
						sCurDst++;

						break;
					}

					sEntityEnd++;
				}

				sCurSrc = sEntityEnd + 1;
			}
			else
			{
				TCHAR* sEntityEnd = sCurSrc;

				while (sEntityEnd < sEnd)
				{
					if (*sEntityEnd == ';')
					{
						CChar* pEntity = (CChar*)m_pEntities->At(CStringRange(*m_pString, sCurSrc - m_pString->Value(), sEntityEnd - sCurSrc));
						if (pEntity)
						{
							*sCurDst = pEntity->Value();
							sCurDst++;
						}

						break;
					}

					sEntityEnd++;
				}

				sCurSrc = sEntityEnd + 1;
			}
		}
		else
		{
			*sCurDst = *sCurSrc;
		
			sCurSrc++;
			sCurDst++;
		}
	}	

	pResult->Resize(sCurDst - pResult->Value());

	return pResult;
}
