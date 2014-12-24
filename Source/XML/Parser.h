#pragma once

#include "Node.h"

namespace SCFXML
{
	class CXMLParser;

	typedef bool (__thiscall CXMLParser::*XMLPARSE_FUNC) ();

	class XML_API CXMLParser
	{
	public:
	CXMLParser();
	~CXMLParser();

	public:
		CXMLNode* Parse(_IN CString& rText);

	public:
		//DEPRECATED: Legacy implementation for testing & comparison only
		static CXMLNode* ParseOld(_IN CString& rText);

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

	private:
		CString* ParseValueString();

	private:
		void UpdateNodeTopLevel();

	private:
		//Input text
		CString m_Text;

	private:
		//Index of the current character
		SCF::UINT m_uiChar;
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
		CXMLNode* m_pNodeTopLevel;

	private:
		CDictionaryString m_Entities;
	};
};