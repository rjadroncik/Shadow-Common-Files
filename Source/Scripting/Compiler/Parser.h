#pragma once

#include "Parser/CompilationUnit.h"
#include "Scanner/Token.h"

namespace SCFCompiler
{
	enum ParseResults
	{
		ResultError = 0,
		ResultOkAndContinue = 1,
		ResultOk = 2,
	};

    class SCRIPTING_API CCompiler;

	class SCRIPTING_API CParser : public CObject
	{
	public:
		ENUM ClassKey() _GET { return ClassParser; }
		CString   ToString() _GET { return STRING("Parser"); }

	public:
		CParser(CCompiler& rCompiler);
		virtual ~CParser();

	public:
		bool Parse(_IN CList<CToken>& rTokens, _INOUT CCompilationUnit& rCompilationUnit);

	protected:
		bool ParsePackage(_INOUT CEnumeratorList& rTokens);
		bool ParseImports(_INOUT CEnumeratorList& rTokens);

	protected:
		bool ParseUDT      (_INOUT CEnumeratorList& rTokens);
		bool ParseEnum     (_INOUT CEnumeratorList& rTokens, ENUM eVisibility);
		bool ParseInterface(_INOUT CEnumeratorList& rTokens, ENUM eVisibility);
		bool ParseClass    (_INOUT CEnumeratorList& rTokens, ENUM eVisibility, bool bAbstractClass);

	protected:
		bool ParseCE         (_INOUT CEnumeratorList& rTokens);
		bool ParseConstructor(_INOUT CEnumeratorList& rTokens);
		bool ParseDestructor (_INOUT CEnumeratorList& rTokens);
		bool ParseMethod     (_INOUT CEnumeratorList& rTokens);
		bool ParseField      (_INOUT CEnumeratorList& rTokens);
		bool ParseProperty   (_INOUT CEnumeratorList& rTokens);

	protected:
		ParseResults ParseImport(_INOUT CEnumeratorList& rTokens);

	protected:
		CCompilationUnit* m_pCompilationUnit;
        CCompiler* m_pCompiler;
	};
};
