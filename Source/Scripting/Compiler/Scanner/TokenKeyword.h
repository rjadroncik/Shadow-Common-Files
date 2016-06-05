#pragma once

#include "Token.h"

namespace SCFCompiler
{
	enum Keywords
	{
		KEYWORD_MIN = 100,

		//Definable types
		KeywordPackage = KEYWORD_MIN,
		KeywordClass,
		KeywordInterface,
		KeywordEnum,

		//Reserved values
		KeywordNull = 200,
		KeywordTrue,
		KeywordFalse,

		//Flow control
		KeywordSwitch = 300,
		KeywordCase,
		KeywordDefault,
		KeywordIf,
		KeywordElse,

		KeywordBreak,
		KeywordContinue,
		KeywordReturn,

		//Loops
		KeywordDo,
		KeywordWhile,
		KeywordFor,
		KeywordForeach,
		KeywordIn,

		//Argument usage specifiers
		KeywordOut = 400,

		//Access level specifiers
		KeywordPublic = 500,
		KeywordProtected,
		KeywordPrivate,

		//Method & variable modifiers
		KeywordStatic,
		KeywordConst,

		//Method modifiers
		KeywordVirtual,
		KeywordAbstract,
		KeywordOverride,

		//Class declaration
		KeywordExtends = 600,
		KeywordImplements,

		//Object/type management operators
		KeywordNew = 700,
		KeywordDelete,
		KeywordImport,
		KeywordTypeof,

		KEYWORD_MAX = KeywordTypeof
	};

	class SCRIPTING_API CTokenKeyword : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenKeyword(_INOUT _REF CStringRange& rText, ENUM eKeyword, UINT uiLine, UINT uiColumn);

		virtual ~CTokenKeyword();

	public:
		ENUM Keyword() _GET { return m_eKeyword; }

	protected:
		CTokenKeyword() {}

	protected:
		ENUM m_eKeyword;
	};
};
