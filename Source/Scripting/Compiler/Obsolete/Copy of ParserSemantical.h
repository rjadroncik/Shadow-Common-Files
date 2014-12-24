#pragma once

#include "LexicalAnalyzer.h"

namespace SCFCompiler
{
	class SCRIPTING_API CParserSemantical
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassParserSemantical; }
		CString   AsString() const { return STRING("ParserSemantical"); }

	public:
		CParserSemantical();
		virtual ~CParserSemantical();

	public:
		bool Parse(_IN CVector& rTokens);

	public:
		//Debugging feature - reconstructs the text from input word stream
		void TextReconstruct(_OUT CString& rOutText);

	protected:
		bool ParseClass();

	protected:
		//Input list of words
		CVector* m_pTokens;
		//Constructed set of classes (parsed, but not yet assembled)
		CVector m_Classes;

	private:
		//Index of the current word
		SCF::UINT m_uiWord;
		//Pointer to the next function to evaluate (this is a form of state representation of a typical state machine)
		PARSE_FUNC m_fpNext;

	private:
		CObject* m_pObject;
	};
};
