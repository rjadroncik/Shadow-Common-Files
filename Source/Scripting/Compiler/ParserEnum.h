#pragma once

#include "Scanner.h"

namespace SCFCompiler
{
	class SCRIPTING_API CParserClass
	{
	public:
		CParserClass();
		~CParserClass();

	public:
		bool Parse(_IN CEnumeratorList& rTokens);

	public:
		//Debugging feature - reconstructs the text from parsed content
		void TextReconstruct(_OUT CString& rOutText);

	protected:
		//Input list of words
		CEnumeratorList* m_pTokens;
	
	protected:
		//Constructed class (parsed, but not yet assembled)
		CClass* m_pClass;
	};
};
