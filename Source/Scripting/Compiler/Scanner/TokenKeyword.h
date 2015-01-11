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
		SCF::ENUM ClassKey() _GET { return ClassTokenKeyword; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenKeyword(_INOUT _REF CStringRange& rText, SCF::ENUM eKeyword, SCF::UINT uiLine, SCF::UINT uiColumn);

		virtual ~CTokenKeyword();

	public:
		SCF::ENUM Keyword() _GET { return m_eKeyword; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const { rStream.PutInt(m_eKeyword); }
		void Deserialize(_INOUT IStreamRead&  rStream)       { m_eKeyword = rStream.GetInt(); }

	protected:
		CTokenKeyword() {}

	protected:
		SCF::ENUM m_eKeyword;
	};
};
