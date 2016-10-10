#pragma once

#include "PathStepPredicate.h"
#include "BoundValue.h"
#include "PathOperatorComparison.h"

namespace SCFXMLPrivate
{
	class CXMLPathPredicateParser;

	typedef bool (__thiscall CXMLPathPredicateParser::*XMLPREDICATEPARSE_FUNC) ();

	class CXMLPathPredicateParser
	{
	public:
		CXMLPathPredicateParser();
		~CXMLPathPredicateParser();

	public:
		CXMLPathPredicate* Parse(_IN CString& rText, _IN UINT uiStartChar, _OUT UINT& rOutCharLast);

	private:
		//Decides what goes next - a bracket: '[', ']' a bound value 
		bool ParseLeftValueType();

		bool ParseLeftValueAttribute();     //@attributeName
		bool ParseLeftValueChildNode();     //nodeName
		bool ParseLeftValueLiteralNumber(); //number
		bool ParseLeftValueLiteralText();   //'some text'

		bool ParseRightValueType();

		bool ParseRightValueAttribute();     //@attributeName
		bool ParseRightValueChildNode();     //nodeName
		bool ParseRightValueLiteralNumber(); //number
		bool ParseRightValueLiteralText();   //'some text'

		bool ParseOperatorCompare(); //<, >, =, !=, <=, >= 
		bool ParseOperatorLogical(); //And, Or

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
		XMLPREDICATEPARSE_FUNC m_fpNext;

	private:
		//Currently the last created bound value on the left side of a comparison operator
		CXMLBoundValue* m_pValueLeft;
		CXMLPathOperatorComparison* m_pOperatorComparison;

	
		//Predicate stack from root to the last added node
		SCFBase::CStack<CXMLPathPredicate> m_Predicates;

		//Indicates whether the next parsed predicate will be pushed ono the stack
		bool m_bPushNext;
	};
};