#include "Parser.h"

using namespace SCF;
using namespace SCFXML;

#define NEXT(state) m_fpNext = &CXMLParser::state

bool CharIsAlpha(_IN TCHAR cChar)
{
	if (((cChar >= 'a') && (cChar <= 'z')) ||
		((cChar >= 'A') && (cChar <= 'Z')) ||
		 (cChar == '_')) { return TRUE; }

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
		((cChar >= '0') && (cChar <= '9'))) { return TRUE; }

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

CXMLParser::CXMLParser()
{
	m_uiChar = 0;

	m_pNode         = NULL;
	m_pAttribute    = NULL;
	m_pString       = NULL;
	m_pNodeTopLevel = NULL;

	m_fpNext = NULL;

	m_Entities.AtPut(STRING("amp"),  *(new CString("&")));
	m_Entities.AtPut(STRING("lt"),   *(new CString("<")));
	m_Entities.AtPut(STRING("gt"),   *(new CString(">")));
	m_Entities.AtPut(STRING("apos"), *(new CString("'")));
	m_Entities.AtPut(STRING("quot"), *(new CString("\"")));
}

CXMLParser::~CXMLParser()
{
	m_Nodes.AllRemove();
	if (m_pString) { delete m_pString; }
}

CXMLNode* CXMLParser::Parse(_IN CString& rText)
{
	NEXT(ParseTagStart);

	m_uiChar = 0;

	m_pNode         = NULL;
	m_pAttribute    = NULL;
	m_pNodeTopLevel = NULL;

	m_Nodes.AllDelete();
	if (m_pString) { delete m_pString; }

	m_Text = rText;
	if (m_Text[m_Text.Length() - 1] != '\n') { m_Text += '\n'; }

	m_pString = new CStringRange(m_Text, 0, 0);

	//Perform parsing
	while ((m_uiChar < m_Text.Length()) && (this->*(m_fpNext))()) {}

	//Select root node - the first regular top level node
	CXMLNode* pRoot = m_pNodeTopLevel;

	while (m_pNodeTopLevel && (m_pNodeTopLevel->Previous()))
	{
		m_pNodeTopLevel = (CXMLNode*)m_pNodeTopLevel->Previous();

		if (m_pNodeTopLevel->Type() == NodeRegular) { pRoot = m_pNodeTopLevel;}
	}

	return pRoot;
}

bool CXMLParser::ParseTagStart()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (cChar == '<')
	{
		NEXT(ParseTagType);
	}

	m_uiChar++;
	return TRUE;
}

void CXMLParser::UpdateNodeTopLevel()
{
	if (m_Nodes.Size() == 1)
	{
		CXMLNode* pTopLevelNodeNext = (CXMLNode*)m_Nodes.Top();

		if (m_pNodeTopLevel) 
		{
			m_pNodeTopLevel->Next(pTopLevelNodeNext);
			pTopLevelNodeNext->Previous(m_pNodeTopLevel);
		}		

		m_pNodeTopLevel = pTopLevelNodeNext;
	}
}

bool CXMLParser::ParseTagType()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (cChar == '/')
	{
		NEXT(ParseTagEnd);
		UpdateNodeTopLevel();

		m_Nodes.Pop();
		m_uiChar++;

		return TRUE;
	}

	m_pNode = new CXMLNode();

	if (!m_Nodes.IsEmpty()) 
	{ 
		CXMLNode* pParent = (CXMLNode*)m_Nodes.Top();

		pParent->ChildAdd(*m_pNode);
	}

	m_Nodes.Push(*m_pNode);

	if (cChar == '!')
	{
		if ((m_Text.Length() > (m_uiChar + 2)) && (m_Text[m_uiChar + 1] == '-') && (m_Text[m_uiChar + 2] == '-'))
		{
			NEXT(ParseTagComment);

			m_pNode->Type(NodeComment);
			m_uiChar += 3;		

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);

			return TRUE;
		}

		if ((m_Text.Length() > (m_uiChar + 7)) && (m_Text[m_uiChar + 1] == '[') && (m_Text[m_uiChar + 7] == '['))
		{
			NEXT(ParseTagCDATA);

			m_pNode->Type(NodeCData);
			m_uiChar += 8;		

			m_pString->ChangeStart(m_uiChar);
			m_pString->ChangeLength(0);

			return TRUE;
		}

		NEXT(ParseTagNotation);

		m_pNode->Type(NodeNotation);
		m_uiChar++;

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return TRUE;
 	}

	NEXT(ParseTagName);

	if (cChar == '?') { m_uiChar++; m_pNode->Type(NodeDeclaration); }
	
	m_pString->ChangeStart(m_uiChar);
	m_pString->ChangeLength(0);

	return TRUE;
}

bool CXMLParser::ParseTagEnd()
{
	if (m_Text[m_uiChar] == '>')
	{
		NEXT(ParseTagStart);
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseTagName()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (cChar == '>')
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Name(*m_pString);

		m_uiChar++;
		return TRUE;
	}

	if (cChar == '/')
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Name(*m_pString);

		UpdateNodeTopLevel();

		m_Nodes.Pop();
		m_uiChar += 2;

		return TRUE;
	}

 	if (CharIsWhiteSpace(cChar))
 	{
		NEXT(ParseAttributeNameStart);
 
		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Name(*m_pString);
 	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseTagData()
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (cChar == '<')
	{
		if ((m_Text.Length() > (m_uiChar + 1)) && (m_Text[m_uiChar + 1] == '/'))
		{
			NEXT(ParseTagEnd); 

			UpdateNodeTopLevel();

			m_pString->ChangeLength(m_uiChar - m_pString->Start());
			m_pNode->Value(ParseValueString(*m_pString));		

			m_Nodes.Pop();

			m_uiChar += 2;
			return TRUE;
		}

		NEXT(ParseTagType); 
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseAttributeNameStart() 
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (cChar == '>')
	{
		NEXT(ParseTagData); 

		m_uiChar++;
		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return TRUE;
	}

	if (cChar == '/')
	{
		NEXT(ParseTagStart);

		UpdateNodeTopLevel();

		m_Nodes.Pop();
		m_uiChar += 2;

		return TRUE;
	}

	if (cChar == '?')
	{
		NEXT(ParseTagStart); 
	
		UpdateNodeTopLevel();

		m_Nodes.Pop();
		m_uiChar += 2;

		return TRUE;
	}

	if (!CharIsWhiteSpace(cChar)) 
	{
		NEXT(ParseAttributeName); 
		
		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return TRUE;	
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseAttributeName() 
{
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if ((cChar == '=') || CharIsWhiteSpace(cChar)) 
	{ 
		NEXT(ParseAttributeValueStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());

		m_pAttribute = new CXMLAttribute();
		m_pAttribute->Name(*m_pString);
	}

	m_uiChar++;
	return TRUE; 
}

bool CXMLParser::ParseAttributeValueStart() 
{ 
	const SCF::TCHAR cChar = m_Text[m_uiChar];

	if (!CharIsWhiteSpace(cChar)) 
	{
		NEXT(ParseAttributeValue);

		if (cChar == '\'') { m_uiChar++; NEXT(ParseAttributeValueQuoted); }
		if (cChar == '\"') { m_uiChar++; NEXT(ParseAttributeValueQuotedDouble); }

		m_pString->ChangeStart(m_uiChar);
		m_pString->ChangeLength(0);

		return TRUE;	
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseAttributeValue()
{
	if (CharIsWhiteSpace(m_Text[m_uiChar])) 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString(*m_pString));

		m_pNode->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return TRUE; 
}

bool CXMLParser::ParseAttributeValueQuoted()
{
	if (m_Text[m_uiChar] == '\'') 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString(*m_pString));

		m_pNode->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return TRUE; 
}

bool CXMLParser::ParseAttributeValueQuotedDouble()
{
	if (m_Text[m_uiChar] == '\"') 
	{ 
		NEXT(ParseAttributeNameStart); 

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pAttribute->Value(ParseValueString(*m_pString));

		m_pNode->AttributeAdd(*m_pAttribute);
	}

	m_uiChar++;
	return TRUE; 
}

bool CXMLParser::ParseTagComment()
{
	if ((m_Text[m_uiChar] == '-') && (m_Text.Length() > (m_uiChar + 2)) && (m_Text[m_uiChar + 1] == '-') && (m_Text[m_uiChar + 2] == '>'))
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Value(new CString(*m_pString));

		m_Nodes.Pop();
		m_uiChar += 3;		

		return TRUE;
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseTagNotation()
{
	if (m_Text[m_uiChar] == '>')
	{
		NEXT(ParseTagStart);

		m_Nodes.Pop();
	}

	m_uiChar++;
	return TRUE;
}

bool CXMLParser::ParseTagCDATA()
{
	if ((m_Text[m_uiChar] == ']') && (m_Text.Length() > (m_uiChar + 2)) && (m_Text[m_uiChar + 1] == ']') && (m_Text[m_uiChar + 2] == '>'))
	{
		NEXT(ParseTagStart);

		m_pString->ChangeLength(m_uiChar - m_pString->Start());
		m_pNode->Value(new CString(*m_pString));

		m_Nodes.Pop();
		m_uiChar += 3;		

		return TRUE;
	}

	m_uiChar++;
	return TRUE;
}

CString* CXMLParser::ParseValueString()
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
			while ((sCurSrc < sEnd) && )
			{

			}
		}
		else
		{
			*sCurDst = *sCurSrc;
		
			sCurSrc++;
			sCurDst++;
		}
	}	
}


///////////////////////////////////////////////////////////////////////////
///                  DEPRECATED: LEGACY IMPLEMENTATION                  ///
///////////////////////////////////////////////////////////////////////////

typedef enum TagTypes
{
	TAG_START = 1,
	TAG_END,
	TAG_SINGLE,

} TagTypes;

struct STagInfo 
{
	//Type of the tag & the node
	TagTypes eType;
	SCFXML::NodeTypes eNodeType;

	//The range of characters occupied by the whole tag
	SCF::UINT uiStart;
	SCF::UINT uiLength;

	//The range of characters occupied by the tag name
	SCF::UINT uiNameStart;
	SCF::UINT uiNameLength;

	//The range of characters occupied by the whole tag
	SCF::UINT uiAttributesStart;
	SCF::UINT uiAttributesLength;
};

//Collects information about the next tag (type, character ranges)
bool TagExamineNext(_IN CString& sString, _OUT STagInfo* pOutTag);

//Extracts all nodes from a string & returns the last top-level node (NOT NECESERALY THE ROOT!)
CXMLNode* NodesExtractAll(_IN CString& sString);

//Extracts all attributes from a tag definition & stores them in a given node
bool AttributesExtractAll(_IN CString& sString, _INOUT CXMLNode* pNode);

//Extract the first attribute in the string & return a class representing it
CXMLAttribute* AttributeExtract(_IN CString& sString, _OUT SCF::UINT* uipOutCharsProcessed);

CXMLNode* NodesExtractAll(_IN CString& sString)
{
	CStack NodeStack;

	UINT      uiCurChar = 0;
	STagInfo  Tag = { TAG_END, NodeComment, 0, 0, 0, 0, 0, 0 };

	CXMLNode* pLastTopLevelNode	= NULL;
	CXMLNode* pNode				= NULL;

	UINT uiLength = (UINT)sString.Length();

	while ((uiCurChar < uiLength) && TagExamineNext(CStringRange(sString, uiCurChar, uiLength - uiCurChar), &Tag))
	{
		switch (Tag.eType)
		{
		case TAG_SINGLE:
			{
				pNode = new CXMLNode();
				pNode->Type(Tag.eNodeType);

				if (!NodeStack.IsEmpty()) { pNode->Parent(((CXMLNode*)NodeStack.Top())); }


				if (Tag.eNodeType != NodeCData)
				{
					pNode->Name         (CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiNameStart,       Tag.uiNameLength));
					AttributesExtractAll(CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiAttributesStart, Tag.uiAttributesLength), pNode);
				}
				else
				{
					pNode->Value(new CString(CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiNameStart, Tag.uiNameLength)));
				}

				if (!NodeStack.IsEmpty()) { ((CXMLNode*)NodeStack.Top())->ChildAdd(*pNode); }
				else
				{
					if (pLastTopLevelNode)
					{
						pNode->Previous(pLastTopLevelNode);
						pLastTopLevelNode->Next(pNode);
					}
					pLastTopLevelNode = pNode;
				}
				break;
			}
		case TAG_START:
			{
				pNode = new CXMLNode();
				pNode->Type(Tag.eNodeType);

				if (!NodeStack.IsEmpty()) { pNode->Parent(((CXMLNode*)NodeStack.Top())); }

				pNode->Name         (CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiNameStart,       Tag.uiNameLength));
				AttributesExtractAll(CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiAttributesStart, Tag.uiAttributesLength), pNode);

				if (!NodeStack.IsEmpty()) { ((CXMLNode*)NodeStack.Top())->ChildAdd(*pNode); }
				else
				{
					if (pLastTopLevelNode)
					{
						pNode->Previous(pLastTopLevelNode);
						pLastTopLevelNode->Next(pNode);
					}
					pLastTopLevelNode = pNode;
				}

				NodeStack.Push(*pNode);
				break;
			}

		case TAG_END:
			{
				if (!NodeStack.IsEmpty())
				{
					if (((CXMLNode*)NodeStack.Top())->ChildFirst() == NULL)
					{
						((CXMLNode*)NodeStack.Top())->Value(new CString(CStringRange(sString, uiCurChar, Tag.uiStart)));
					}
				}

				while (!NodeStack.IsEmpty())
				{
					if (((CXMLNode*)NodeStack.Top())->Name() == CStringRange(sString, uiCurChar + Tag.uiStart + Tag.uiNameStart, Tag.uiNameLength))
					{
						NodeStack.Pop();
						break;
					}
					NodeStack.Pop();
				}
				break;
			}
		}
		uiCurChar += Tag.uiStart + Tag.uiLength;
	}

	return pLastTopLevelNode;
}

bool AttributesExtractAll(_IN CString& sString, _OUT CXMLNode* pNode)
{
	if (!pNode)	{ return FALSE; }

	UINT uiLength = (UINT)sString.Length();

	CXMLAttribute* pAttribute = NULL;
	UINT uiCurChar        = 0;
	UINT uiAtributeLength = 0;

	while ((pAttribute = AttributeExtract(CStringRange(sString, uiCurChar, uiLength - uiCurChar), &uiAtributeLength)) != 0)
	{
		pNode->AttributeAdd(*pAttribute);
		uiCurChar += uiAtributeLength;
	}

	return TRUE;
}

CXMLAttribute* AttributeExtract(_IN CString& sString, _OUT UINT* uipOutCharsProcessed)
{	
	UINT uiLength = (UINT)sString.Length();

	//Find the first non-whitespace character
	int iNameStart  = CStringSearch(sString).FindCharsInv(STRING(WHITESPACE_CHARS));
	if (iNameStart < 0) { return NULL; }

	//Find the first "=" character
	int iNameLength = CStringSearch(CStringRange(sString, iNameStart, uiLength - iNameStart)).FindChars(STRING("="));
	if (iNameLength < 0) { return NULL; }

	//Find the first non-whitespace character before the "=" character
	iNameLength     = CStringSearch(CStringRange(sString, iNameStart, iNameLength)).FindCharsInvRev(STRING(WHITESPACE_CHARS));
	if (iNameLength < 0) { return NULL; }
	iNameLength++;

	//Find the first "=" character
	int iValueStart = CStringSearch(CStringRange(sString, iNameStart + iNameLength, uiLength - (iNameStart + iNameLength))).FindChars(STRING("="));
	if (iValueStart < 0) { return NULL; }

	//Skip the "=" character
	iValueStart++;

	//Find the first non-whitespace character after the "="
	int iTemp       = CStringSearch(CStringRange(sString, iNameStart + iNameLength + iValueStart, uiLength - (iNameStart + iNameLength + iValueStart))).FindCharsInv(STRING(WHITESPACE_CHARS));
	if (iTemp != -1) { iValueStart += iTemp; }

	int  iValueLength = 0;
	bool bQuoted      = FALSE;

	if (sString[iNameStart + iNameLength + iValueStart] == '\"')
	{
		bQuoted = TRUE;
		iValueStart++;
		iValueLength = CStringSearch(CStringRange(sString, iNameStart + iNameLength + iValueStart, uiLength - (iNameStart + iNameLength + iValueStart))).FindChars(STRING("\""));
	}

	if (sString[iNameStart + iNameLength + iValueStart] == '\'')
	{
		bQuoted = TRUE;
		iValueStart++;
		iValueLength = CStringSearch(CStringRange(sString, iNameStart + iNameLength + iValueStart, uiLength - (iNameStart + iNameLength + iValueStart))).FindChars(STRING("\'"));
	}

	if (!bQuoted)
	{
		//Find the first whitespace character after the start of the value
		iValueLength = CStringSearch(CStringRange(sString, iNameStart + iNameLength + iValueStart, uiLength - (iNameStart + iNameLength + iValueStart))).FindChars(STRING(WHITESPACE_CHARS));
		if (iValueLength < 0)
		{
			iValueLength = uiLength - (iNameStart + iNameLength + iValueStart);
		}
	}

	if (uipOutCharsProcessed)
	{
		*uipOutCharsProcessed = iNameStart + iNameLength + iValueStart + iValueLength; 
		if (bQuoted) { *uipOutCharsProcessed += 1; }
	}

	//Create a new attribute
	CXMLAttribute* pAttribute = new CXMLAttribute();
	pAttribute->Name(CStringRange(sString, iNameStart, iNameLength));

	//Create & store it's value
	CString* pValue = new CString(CStringRange(sString, iNameStart + iNameLength + iValueStart, iValueLength));
	pAttribute->Value(pValue);

	return pAttribute;
}

bool TagExamineNext(_IN CString& sString, _OUT STagInfo* pOutTag)
{
	//Check arguments
	if (!pOutTag) { return FALSE; }

	//Compute input string length
	UINT uiLength = (UINT)sString.Length();

	//Find the start of the tag
	int iTagStart = CStringSearch(sString).FindChars(STRING("<"));
	if (iTagStart < 0) { return FALSE; }

	//Prepare temporary variables
	bool bTypeFound      = FALSE;
	UINT uiTagLength     = 0;
	UINT uiTagCloseChars = 0;

	UINT uiStringStart = iTagStart;
	UINT uiStringEnd   = uiLength;

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 9) == STRING("<![CDATA["))) 
	{
		pOutTag->eType     = TAG_SINGLE;
		pOutTag->eNodeType = NodeCData;
		bTypeFound         = TRUE;
		uiStringStart     += 9;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING("]]>"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars = 3;
		uiTagLength = iFoundString + uiTagCloseChars + 9;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;

		pOutTag->uiStart  = iTagStart;
		pOutTag->uiLength = uiTagLength;

		pOutTag->uiNameStart  = uiStringStart - iTagStart;
		pOutTag->uiNameLength = uiStringEnd - uiStringStart;

		pOutTag->uiAttributesStart	= 0;
		pOutTag->uiAttributesLength	= 0;

		return TRUE;
	}

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 4) == STRING("<!--"))) 
	{
		pOutTag->eType     = TAG_SINGLE;
		pOutTag->eNodeType = NodeComment;
		bTypeFound         = TRUE;
		uiStringStart     += 4;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING("-->"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars = 3;
		uiTagLength = iFoundString + uiTagCloseChars + 4;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;

		pOutTag->uiStart  = iTagStart;
		pOutTag->uiLength = uiTagLength;

		pOutTag->uiNameStart  = uiStringStart - iTagStart;
		pOutTag->uiNameLength = uiStringEnd - uiStringStart;

		pOutTag->uiAttributesStart	= 0;
		pOutTag->uiAttributesLength	= 0;

		return TRUE;
	}

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 2) == STRING("<?"))) 
	{
		pOutTag->eType     = TAG_SINGLE;
		pOutTag->eNodeType = NodeDeclaration;
		bTypeFound         = TRUE;
		uiStringStart     += 2;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING("?>"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars	= 2;
		uiTagLength = iFoundString + uiTagCloseChars + 2;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;
	}

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 2) == STRING("<!"))) 
	{
		pOutTag->eType     = TAG_SINGLE;
		pOutTag->eNodeType = NodeNotation;
		bTypeFound         = TRUE;
		uiStringStart     += 2;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING(">"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars = 1;
		uiTagLength = iFoundString + uiTagCloseChars + 2;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;
	}

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 2) == STRING("</"))) 
	{
		pOutTag->eType     = TAG_END;
		pOutTag->eNodeType = NodeRegular;
		bTypeFound         = TRUE;
		uiStringStart     += 2;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING(">"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars	= 1;
		uiTagLength = iFoundString + uiTagCloseChars + 2;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;
	}

	if (!bTypeFound && (CStringRange(sString, uiStringStart, 1) == STRING("<"))) 
	{
		pOutTag->eType     = TAG_START;
		pOutTag->eNodeType = NodeRegular;
		bTypeFound         = TRUE;
		uiStringStart     += 1;

		int iFoundString = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindString(STRING(">"));
		if (iFoundString == -1) { return FALSE; }

		uiTagCloseChars = 1;
		uiTagLength = iFoundString + uiTagCloseChars + 1;
		uiStringEnd = iTagStart + uiTagLength - uiTagCloseChars;

		if (sString[iTagStart + uiTagLength - 2] == '/')
		{
			pOutTag->eType     = TAG_SINGLE;
			pOutTag->eNodeType = NodeRegular;
			uiTagCloseChars    = 2;
			uiStringEnd--;
		}
	}

	//If tag type is not recognized, report error
	if (!bTypeFound) { return FALSE; }

	//Find the first non-whitespace character after the tag definition
	int iNameStart  = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindCharsInv(STRING(WHITESPACE_CHARS));
	if (iNameStart < 0)	{ return FALSE; }
	uiStringStart+= iNameStart;
	iNameStart = uiStringStart - iTagStart;

	//Find the first whitespace character after the tag name, if there are any
	int iNameLength = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindChars(STRING(WHITESPACE_CHARS));
	if (iNameLength < 0)
	{
		//There is no whitespace after the tag name, which gives the name the length of the text remaining to parse
		iNameLength = uiStringEnd - uiStringStart;
	}
	uiStringStart+= iNameLength;

	//Find the first non-whitespace character after the tag name
	int iTemp = CStringSearch(CStringRange(sString, uiStringStart, uiStringEnd - uiStringStart)).FindCharsInv(STRING(WHITESPACE_CHARS));
	if (iTemp < 0)
	{
		//No attributes
		pOutTag->uiAttributesStart	= 0;
		pOutTag->uiAttributesLength	= 0;
	}
	else
	{
		//Store information about the range of characters that form the attribute list
		uiStringStart += iTemp;

		pOutTag->uiAttributesStart	= uiStringStart	- iTagStart;
		pOutTag->uiAttributesLength	= uiStringEnd   - uiStringStart;
	}

	//Copy collected data into output structure
	pOutTag->uiStart  = iTagStart;
	pOutTag->uiLength = uiTagLength;

	pOutTag->uiNameStart  = iNameStart;
	pOutTag->uiNameLength = iNameLength;

	return TRUE;
}

CXMLNode* CXMLParser::ParseOld(_IN CString& rText)
{
	//Extract all nodes :)
	CXMLNode* pNodeTopLevel = NodesExtractAll(rText);

	//Select root node - the first regular top level node
	CXMLNode* pRoot = pNodeTopLevel;

	while (pNodeTopLevel && (pNodeTopLevel->Previous()))
	{
		pNodeTopLevel = (CXMLNode*)pNodeTopLevel->Previous();

		if (pNodeTopLevel->Type() == NodeRegular) { pRoot = pNodeTopLevel;}
	}

	return pRoot;
}