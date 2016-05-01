#pragma once

#include "PathStep.h"
#include "PathPredicateParser.h"

namespace SCFXMLPrivate
{
	class CXMLPathParser;

	typedef bool (__thiscall CXMLPathParser::*XMLPATHPARSE_FUNC) ();

	class CXMLPathParser
	{
	public:
		CXMLPathParser();
		~CXMLPathParser();

	public:
		CXMLPathStep* Parse(_IN CString& rText);

	private:
		//Parses: {.., name, /*, /.., /name, //*, //name}[{/*, /.., /name, //*, //name}][...][{/@name, /*, /.., /name, //@name, //*, //name}] etc.
		bool ParseStepFirst();
		bool ParseStepType();
		bool ParseStepName();
	
		//Parses: '['@name {>, >=, =, != <=, <} literal {[and, or] ...}']' i.e.: //texture[@source='material.diffuseColor' and @id='Smoke_Diffuse']
		bool ParsePredicateStart();

		//Parses: '['+{0-9}']' i.e.: //usages/usage[1]
		bool ParseStepIndex();

	private:
		//For optimization, data values are cached from the string object
		const TCHAR* m_szText;
		UINT m_uiTextLength;

	private:
		CStringRange* m_pString;

	private:
		//Index of the current character
		UINT m_uiChar;
		//Pointer to the next function to evaluate (this is a form of state representation of a typical state machine)
		XMLPATHPARSE_FUNC m_fpNext;

	private:
		SCFXMLPrivate::CXMLPathStep* m_pPathStepFirst;
		//Currently the last created node
		SCFXMLPrivate::CXMLPathStep* m_pPathStepLast;

	private:
		CXMLPathPredicateParser m_PredicateParser;
	};
};