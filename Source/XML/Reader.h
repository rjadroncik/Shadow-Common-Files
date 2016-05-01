#pragma once

#include "Node.h"
#include "Attribute.h"

namespace SCFXML
{
	class CXMLReader;

	typedef bool (__thiscall CXMLReader::*XMLPARSE_FUNC) ();

	class XML_API CXMLReader : public SCFBase::CObject
	{
	public:
		CXMLReader();
		~CXMLReader();

	public:
		//Calls read directly, skipping initialization of member variables thus is a tiny bit faster :P
		CXMLReader(_IN CString& rText, _INOUT CXMLNode& rRoot, _INOUT CDictionaryString<CChar>* pEntities = NULL);

	public:
		//Parses a given textual form of XML into an XML DOM tree, using an optional entity dictionary
		//If no dictionary is supplied, the default dictionary used containing the default XML entities: {&, <, >, ', "}
		bool Read(_IN CString& rText, _INOUT CXMLNode& rRoot, _INOUT CDictionaryString<CChar>* pEntities = NULL);

	private:
		//{_}<{/,?,!}name_{ATRIBUTES}{_}{/}>{TEXT}
		bool ParseTagStart();
		bool ParseTagType();
		bool ParseTagName();
		bool ParseTagData();
		bool ParseTagEnd();

		//{_}name{_}={_}{",'}value{",'}{_}
		bool ParseAttributeNameStart();
		bool ParseAttributeName();
		bool ParseAttributeValueStart();
		bool ParseAttributeValue();
		bool ParseAttributeValueQuoted();
		bool ParseAttributeValueQuotedDouble();

		bool ParseTagComment();
		bool ParseTagNotation();
		bool ParseTagCDATA();
		bool ParseTagDeclaration();

	private:
		CString* ParseValueString();

	private:
		//For optimization, data values are cached from the input string object
		const TCHAR* m_szText;
		UINT m_uiTextLength;

	private:
		//Index of the current character
		UINT m_uiChar;
		//Pointer to the next function to evaluate (this is a form of state representation of a typical state machine)
		XMLPARSE_FUNC m_fpNext;

		//A stack containing the current hierarchical relationship for the processed node
		CStack m_Nodes;

	private:
		//Currently constructed/parsed elements
		CXMLNode*      m_pNode;
		CXMLAttribute* m_pAttribute;
		CStringRange*  m_pString;

	private:
		//The last top level node extracted
		CXMLNode* m_pRoot;

	private:
		CDictionaryString<CChar>* m_pEntities;
	};
};