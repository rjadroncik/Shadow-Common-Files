#include "TokenWord.h"

using namespace SCFCompiler;

CString CTokenWord::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	if (m_eKeyword)
	{
		for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'k'); }
	}
	else
	{
		for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'w'); }
	}

	return RetVal;
}

CTokenWord::CTokenWord(_INOUT _REF CStringRange& rText) : CToken(rText)
{
	m_eKeyword = 0;
}

CTokenWord::~CTokenWord()
{
}

bool CTokenWord::Update()
{
	static CDictionaryString s_Keywords;
	
	if (!s_Keywords.Size())
	{
		//Primitive types
		s_Keywords.AtPut(STRING("int"),      *(new CEnum(KeywordInt)));
		s_Keywords.AtPut(STRING("float"),    *(new CEnum(KeywordFloat)));
		s_Keywords.AtPut(STRING("bool"),     *(new CEnum(KeywordBool)));
		s_Keywords.AtPut(STRING("char"),     *(new CEnum(KeywordChar)));
		s_Keywords.AtPut(STRING("string"),   *(new CEnum(KeywordString)));
		s_Keywords.AtPut(STRING("void"),     *(new CEnum(KeywordVoid)));

		//Definable types
		s_Keywords.AtPut(STRING("package"), *(new CEnum(KeywordPackage)));
		s_Keywords.AtPut(STRING("class"),   *(new CEnum(KeywordClass)));
		s_Keywords.AtPut(STRING("enum"),    *(new CEnum(KeywordEnum)));
		s_Keywords.AtPut(STRING("import"),  *(new CEnum(KeywordImport)));

		//Reserved values
		s_Keywords.AtPut(STRING("null"),  *(new CEnum(KeywordNull)));
		s_Keywords.AtPut(STRING("true"),  *(new CEnum(KeywordTrue)));
		s_Keywords.AtPut(STRING("false"), *(new CEnum(KeywordFalse)));
	
		//Flow control
		s_Keywords.AtPut(STRING("switch"),  *(new CEnum(KeywordSwitch)));
		s_Keywords.AtPut(STRING("case"),    *(new CEnum(KeywordCase)));
		s_Keywords.AtPut(STRING("default"), *(new CEnum(KeywordDefault)));
		s_Keywords.AtPut(STRING("if"),      *(new CEnum(KeywordIf)));
		s_Keywords.AtPut(STRING("else"),    *(new CEnum(KeywordElse)));

		s_Keywords.AtPut(STRING("break"),    *(new CEnum(KeywordBreak)));
		s_Keywords.AtPut(STRING("continue"), *(new CEnum(KeywordContinue)));
		s_Keywords.AtPut(STRING("return"),   *(new CEnum(KeywordReturn)));

		//Loops
		s_Keywords.AtPut(STRING("do"),      *(new CEnum(KeywordDo)));
		s_Keywords.AtPut(STRING("while"),   *(new CEnum(KeywordWhile)));
		s_Keywords.AtPut(STRING("for"),     *(new CEnum(KeywordFor)));
		s_Keywords.AtPut(STRING("foreach"), *(new CEnum(KeywordForeach)));


		//Argument usage specifiers
		s_Keywords.AtPut(STRING("in"),    *(new CEnum(KeywordIn)));
		s_Keywords.AtPut(STRING("inout"), *(new CEnum(KeywordInout)));
		s_Keywords.AtPut(STRING("inref"), *(new CEnum(KeywordInref)));
		s_Keywords.AtPut(STRING("out"),   *(new CEnum(KeywordOut)));

		//Access level specifiers
		s_Keywords.AtPut(STRING("public"),    *(new CEnum(KeywordPublic)));
		s_Keywords.AtPut(STRING("protected"), *(new CEnum(KeywordProtected)));
		s_Keywords.AtPut(STRING("private"),   *(new CEnum(KeywordPrivate)));

		//Method & variable modifiers
		s_Keywords.AtPut(STRING("static"),  *(new CEnum(KeywordStatic)));
		s_Keywords.AtPut(STRING("const"),   *(new CEnum(KeywordConst)));

		//Method modifiers
		s_Keywords.AtPut(STRING("virtual"),  *(new CEnum(KeywordVirtual)));
		s_Keywords.AtPut(STRING("abstract"), *(new CEnum(KeywordAbstract)));
		s_Keywords.AtPut(STRING("override"), *(new CEnum(KeywordOverride)));

		//Class declaration
		s_Keywords.AtPut(STRING("extends"),    *(new CEnum(KeywordExtends)));
		s_Keywords.AtPut(STRING("implements"), *(new CEnum(KeywordImplements)));


		//Object/type management operators
		s_Keywords.AtPut(STRING("new"),    *(new CEnum(KeywordNew)));
		s_Keywords.AtPut(STRING("delete"), *(new CEnum(KeywordDelete)));
		s_Keywords.AtPut(STRING("import"), *(new CEnum(KeywordImport)));
		s_Keywords.AtPut(STRING("typeof"), *(new CEnum(KeywordTypeof)));

		//Class sections
// 		s_Keywords.AtPut(STRING("constructors:"), *(new CEnum(KeywordConstructors)));
// 		s_Keywords.AtPut(STRING("destructor:"),   *(new CEnum(KeywordDestructor)));
// 		s_Keywords.AtPut(STRING("attributes:"),   *(new CEnum(KeywordAttributes)));
// 		s_Keywords.AtPut(STRING("methods:"),      *(new CEnum(KeywordMethods)));
// 		s_Keywords.AtPut(STRING("properties:"),   *(new CEnum(KeywordProperties)));
	}

	CEnum* pEnum = (CEnum*)s_Keywords.At(*m_pText);
	if (pEnum) 
	{
		m_eKeyword = pEnum->Value(); 
	}

	return TRUE;
}

