#pragma once

#include "Scanner.h"

namespace SCFCompiler
{
	class CParserMethod;

	typedef bool (__thiscall CParserMethod::*PARSE_FUNC) ();

	class SCRIPTING_API CParserMethod
	{
	public:
		CParserMethod();
		~CParserMethod();

	public:
		bool Parse(_IN CList& rTokens);

	public:
		//Debugging feature - reconstructs the text from input word stream
		void TextReconstruct(_OUT CString& rOutText);

	protected:
		//Input list of words
		CEnumeratorList* m_pTokens;

	private:
		//Pointer to the next function to evaluate (this is a form of state representation of a typical state machine)
		PARSE_FUNC m_fpNext;
	};
};
