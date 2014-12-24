#pragma once

#include "Token.h"

namespace SCFCompiler
{
	enum Keywords
	{
		//Primitive types
		KeywordBool = 1,
		KeywordInt,
		KeywordFloat,
		KeywordChar,
		KeywordString,
		KeywordVoid,

		//Definable types
		KeywordPackage,
		KeywordClass,
		KeywordInterface,
		KeywordEnum,

		//Reserved values
		KeywordNull,
		KeywordTrue,
		KeywordFalse,

		//Flow control
		KeywordSwitch,
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


		//Argument usage specifiers
		KeywordIn,
		KeywordInout,
		KeywordInref,
		KeywordOut,

		//Access level specifiers
		KeywordPublic,
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
		KeywordExtends,
		KeywordImplements,


		//Object/type management operators
		KeywordNew,
		KeywordDelete,
		KeywordImport,
		KeywordTypeof,

		//Class sections
// 		KeywordConstructors,
// 		KeywordDestructor,
// 		KeywordAttributes,
// 		KeywordMethods,
// 		KeywordProperties,
	};

	class SCRIPTING_API CTokenWord : public CToken
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTokenWord; }

	public:
		CString TypeString() _GET;

	public:
		CTokenWord(_INOUT _REF CStringRange& rText);
		virtual ~CTokenWord();

	public:
		bool Update();

	public:
		bool IsKeyword() _GET { return (m_eKeyword != 0); }

	public:
		SCF::ENUM Keyword() _GET { return m_eKeyword; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const { rStream.PutInt(m_eKeyword); }
		void Deserialize(_INOUT IStreamRead&  rStream)       { m_eKeyword = rStream.GetInt(); }

	protected:
		CTokenWord() {}

	protected:
		SCF::ENUM m_eKeyword;
	};
};
