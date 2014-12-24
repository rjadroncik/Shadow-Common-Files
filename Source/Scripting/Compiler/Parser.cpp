#include "Parser.h"
#include "Compiler.h"

#include "Scanner/TokenNumber.h"
#include "Scanner/TokenKeyword.h"
#include "Scanner/TokenIdentifier.h"
#include "Scanner/TokenOperator.h"
#include "Scanner/TokenType.h"

using namespace SCF;
using namespace SCFCompiler;
using namespace SCFScripting;

#define NEXT(state) m_fpNext = &CParser::state

CParser::CParser(CCompiler& rCompiler)
{
    m_pCompiler = &rCompiler;
}

CParser::~CParser()
{
}

bool CParser::Parse(_IN CList<CToken>& rTokens, _INOUT CCompilationUnit& rCompilationUnit)
{
	CEnumeratorList Tokens(rTokens);
	if (!Tokens.Next()) { return FALSE; }

	m_pCompilationUnit = &rCompilationUnit;

	if (!ParsePackage(Tokens)) { return FALSE; }
	if (!ParseImports(Tokens)) { return FALSE; }
	if (!ParseUDT    (Tokens)) { return FALSE; }

	return TRUE;
}

inline CTokenType* ParseType(_INOUT CEnumeratorList& rTokens)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenType)) { return NULL; }

	CTokenType* pTokenType = (CTokenType*)rTokens.Current();
	if (pTokenType)
	{
		//On success iterate to next token
		rTokens.Next();
	}

	return pTokenType;
}

inline CTokenIdentifier* ParseIdentifier(_INOUT CEnumeratorList& rTokens)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenIdentifier)) { return NULL; }

	CTokenIdentifier* pTokenIdentifier = (CTokenIdentifier*)rTokens.Current();

	//On success iterate to next token
	rTokens.Next();
	return pTokenIdentifier;
}

inline CTokenNumber* ParseNumber(_INOUT CEnumeratorList& rTokens)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenNumber)) { return NULL; }

	CTokenNumber* pTokenNumber = (CTokenNumber*)rTokens.Current();

	//On success iterate to next token
	rTokens.Next();
	return pTokenNumber;
}

inline bool ParseOperator(_INOUT CEnumeratorList& rTokens, _IN SCF::ENUM eOperator)
{
	//Expecting an operator
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenOperator)) { return FALSE; }

	CTokenOperator* pTokenOperator = (CTokenOperator*)rTokens.Current();

	//Expecting the operator given in [eOperator]
	if (pTokenOperator->Operator() != eOperator) { return FALSE; }

	//On success iterate to next token
	rTokens.Next();
	return TRUE;
}

inline bool ParseKeyword(_INOUT CEnumeratorList& rTokens, _IN SCF::ENUM eKeyword)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenKeyword)) { return FALSE; }

	CTokenKeyword* pTokenKeyword = (CTokenKeyword*)rTokens.Current();

	if (pTokenKeyword->Keyword() != eKeyword) { return FALSE; }

	//On success iterate to next token
	rTokens.Next();
	return TRUE;
}

inline SCF::ENUM ParseKeyword(_INOUT CEnumeratorList& rTokens)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenKeyword)) { return 0; }

	CTokenKeyword* pTokenKeyword = (CTokenKeyword*)rTokens.Current();

	//On success iterate to next token
	rTokens.Next();
	return pTokenKeyword->Keyword();
}

inline SCF::ENUM ParseKeyword(_INOUT CEnumeratorList& rTokens, SCF::ENUM min, SCF::ENUM max)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenKeyword)) { return 0; }

	CTokenKeyword* pTokenKeyword = (CTokenKeyword*)rTokens.Current();

	if ((pTokenKeyword->Keyword() < min) || (pTokenKeyword->Keyword() > max))
	{
		return 0;
	}

	//On success iterate to next token
	rTokens.Next();
	return pTokenKeyword->Keyword();
}

inline SCF::ENUM ParseVisibility(_INOUT CEnumeratorList& rTokens)
{
	//Expecting a word
	if (rTokens.Finished() || (rTokens.Current()->ClassKey() != ClassTokenKeyword)) { return 0; }

	CTokenKeyword* pTokenKeyword = (CTokenKeyword*)rTokens.Current();

	//Test whether it is one of the visibility keywords: "public", "protected", "private"
	if ((pTokenKeyword->Keyword() < KeywordPublic) || (pTokenKeyword->Keyword() > KeywordPrivate)) { return 0; }

	//On success iterate to next token
	rTokens.Next();
	return pTokenKeyword->Keyword(); 
}

bool CParser::ParsePackage(_INOUT CEnumeratorList& rTokens)
{
	//KEYWORD: "package"
	if (!ParseKeyword(rTokens, KeywordPackage)) { return FALSE; }

	//IDENTIFER: packageName
	CTokenIdentifier* pTokenPackageName = ParseIdentifier(rTokens);
	if (!pTokenPackageName) { return FALSE; }

	//Select or create a package with the given name
    m_pCompilationUnit->Package((CPackage*)(m_pCompiler->Packages().At(pTokenPackageName->Text())));

 	if (m_pCompilationUnit->Package() == NULL)
 	{
        CPackage* pPackage = new CPackage();

        pPackage->Name(pTokenPackageName->Text());

 	    m_pCompiler->Packages().AtPut(pPackage->Name(), *pPackage);
 	}

	//OPERATOR: ";"
	return ParseOperator(rTokens, OperatorEnd);
}

ParseResults CParser::ParseImport(_INOUT CEnumeratorList& rTokens)
{
	//KEYWORD: "import"
	if (!ParseKeyword(rTokens, KeywordImport)) { return ResultOk; }

	//IDENTIFER: importedPackageName
	CTokenIdentifier* pTokenImportedPackageName = ParseIdentifier(rTokens);
	if (!pTokenImportedPackageName) { return ResultError; }

	//Create a package import directive for the package with the given name
 	if (m_pCompiler->Packages().ContainsName(pTokenImportedPackageName->Text()))
 	{
 		m_pCompilationUnit->Imports().LastAdd(*m_pCompiler->Packages().At(pTokenImportedPackageName->Text()));
 	}
 	else { return ResultError; }

	//OPERATOR: ";"
	if (ParseOperator(rTokens, OperatorEnd)) { return ResultOkAndContinue; }
	else                                     { return ResultError; }
}

bool CParser::ParseImports(_INOUT CEnumeratorList& rTokens)
{
	ParseResults eResult = ResultOk; 

	while ((eResult = ParseImport(rTokens)) == ResultOkAndContinue) { }

	if (eResult == ResultOk) { return TRUE; }
	else                     { return FALSE; }
}

bool CParser::ParseUDT(_INOUT CEnumeratorList& rTokens)
{
	//KEYWORD: "public" | "protected" | "private"
	SCF::ENUM eVisibility = ParseVisibility(rTokens);
	if (!eVisibility) { return FALSE; }

	SCF::ENUM eUDT = ParseKeyword(rTokens);

	bool bAbstractClass = FALSE;

	switch (eUDT)
	{
	case KeywordEnum: 
		{
			if (!ParseEnum(rTokens, eVisibility)) { return FALSE; }	

			return ParseUDT(rTokens);
		}
	case KeywordInterface: 
		{
			if (!ParseInterface(rTokens, eVisibility)) { return FALSE; }	

			return ParseUDT(rTokens);
		}
	case KeywordAbstract:
		{
			bAbstractClass = TRUE;

			eUDT = ParseKeyword(rTokens, eVisibility);
			if (eUDT != KeywordClass) { return FALSE; }
		}
	case KeywordClass: 
		{
			if (!ParseClass(rTokens, eVisibility, bAbstractClass)) { return FALSE; }	

			return ParseUDT(rTokens);
		}
	}

	return FALSE;
}

inline bool ParseEnumPair(_INOUT CEnumeratorList& rTokens, SCFScripting::CEnum& rEnum)
{
	//IDENTIFER: implementedInterfaceName
	CTokenIdentifier* pTokenEnumPairName = ParseIdentifier(rTokens);
	if (!pTokenEnumPairName) { return FALSE; }

	if (!ParseOperator(rTokens, OperatorAssign)) { return FALSE; }

	CTokenNumber* pTokenEnumPairValue = ParseNumber(rTokens);
	if (!pTokenEnumPairValue) { return FALSE; }

	rEnum.Literal(pTokenEnumPairName->Text(), CInt(pTokenEnumPairValue->Text()).Value());

	return TRUE;
}

bool CParser::ParseEnum(_INOUT CEnumeratorList& rTokens, SCF::ENUM eVisibility)
{
	//IDENTIFER: enumName
	CTokenIdentifier* pTokenEnumName = ParseIdentifier(rTokens);
	if (!pTokenEnumName) { return FALSE; }

	SCFScripting::CEnum* pEnum = new SCFScripting::CEnum();
	pEnum->Name(pTokenEnumName->Text());

	m_pCompilationUnit->Enums().LastAdd(*pEnum);
	
	if (ParseOperator(rTokens, OperatorBlockOpen)) 
	{
		while (ParseEnumPair(rTokens, *pEnum))
		{
			if (!ParseOperator(rTokens, OperatorSeparator)) { break; }
		}

		if (ParseOperator(rTokens, OperatorBlockClose)) 
		{
			return TRUE;
		}
	}

	return FALSE;
}

inline bool ParseFunctionArgument(_INOUT CEnumeratorList& rTokens)
{
	//KEYWORD: "out"
	bool isOut = ParseKeyword(rTokens, KeywordOut);

	//KEYWORD: "Bool", ..., "Void"
	CTokenType* pType = ParseType(rTokens);
	if (!pType)
	{
		//IDENTIFER: returnValueType
		CTokenIdentifier* pClass = ParseIdentifier(rTokens);
		if (!pClass) { return FALSE; }
	}

	//IDENTIFER: returnValueType
	CTokenIdentifier* pName = ParseIdentifier(rTokens);
	if (!pName) { return FALSE; }

	return true;
}

inline bool ParseFunctionDefinition(_INOUT CEnumeratorList& rTokens)
{
	//KEYWORD: "public" | "protected" | "private"
	SCF::ENUM eVisibility = ParseVisibility(rTokens);
	//if (!eVisibility) { return FALSE; }

	//KEYWORD: "static"
	bool isStatic = ParseKeyword(rTokens, KeywordStatic);

	//KEYWORD: "abstract"
	bool isAbstract = ParseKeyword(rTokens, KeywordAbstract);

	//KEYWORD: "virtual"
	bool isVirtual = ParseKeyword(rTokens, KeywordVirtual);

	//RETURN TYPE: predefined
	CTokenType* pReturnType = ParseType(rTokens);
	if (!pReturnType) 
	{
		//RETURN TYPE: predefined
		CTokenIdentifier* pReturnClass = ParseIdentifier(rTokens);
		if (!pReturnClass)
		{
			return FALSE; 
		}
	}

	//IDENTIFER: methodName
	CTokenIdentifier* pMethodName = ParseIdentifier(rTokens);
	if (!pMethodName) { return FALSE; }

	//OPERATOR: "("
	if (!ParseOperator(rTokens, OperatorBracketOpen)) { return FALSE; }

	//Function arguments
	while (ParseFunctionArgument(rTokens))
	{
		if (!ParseOperator(rTokens, OperatorSeparator)) { break; }
	}

	//OPERATOR: "("
	if (!ParseOperator(rTokens, OperatorBracketClose)) { return FALSE; }

	return TRUE;
}

inline bool ParseInterfaceContents(_INOUT CEnumeratorList& rTokens)
{
	if (ParseOperator(rTokens, OperatorBlockOpen)) 
	{
		while (ParseFunctionDefinition(rTokens))
		{
			if (!ParseOperator(rTokens, OperatorEnd)) { break; }
		}

		if (ParseOperator(rTokens, OperatorBlockClose)) 
		{

			return TRUE;
		}
	}   

	return FALSE;
}

inline bool ParseExtendedInterfaceName(CInterface* pInterface, _INOUT CEnumeratorList& rTokens)
{
	//IDENTIFER: implementedInterfaceName
	CTokenIdentifier* pTokenImplementedInterfaceName = ParseIdentifier(rTokens);
	if (!pTokenImplementedInterfaceName) { return FALSE; }


	//TODO: pInterface->BaseInterfaces().LastAdd(

	return TRUE;
}

bool CParser::ParseInterface(_INOUT CEnumeratorList& rTokens, SCF::ENUM eVisibility)
{
	//IDENTIFER: interfaceName
	CTokenIdentifier* pTokenInterfaceName = ParseIdentifier(rTokens);
	if (!pTokenInterfaceName) { return FALSE; }

	//Create an interface
    CInterface* pInterface = new CInterface();

    pInterface->Name(pTokenInterfaceName->Text());
    
    m_pCompilationUnit->Interfaces().LastAdd(*pInterface);

	//KEYWORD: "extends"
	if (!ParseKeyword(rTokens, KeywordExtends)) 
	{
		//if interface DOES NOT extend another interface, just parse its contents
		return ParseInterfaceContents(rTokens);
	}

	//IDENTIFER: extendedInterfaceName [ OPERATOR: "," IDENTIFER: extendedInterfaceName ]
	while (ParseExtendedInterfaceName(pInterface, rTokens)) 
	{
		if (!ParseOperator(rTokens, OperatorSeparator)) 
		{ 
			//On success iterate to next token
			//rTokens.Next();
			break; 
		}
	}

	//CREATE A EXTENDS DIRECTIVE FOR THE INTERFACE WITH THE GIVEN NAME!!!

	return ParseInterfaceContents(rTokens);
}

inline bool ParseImplementedInterfaceName(_INOUT CEnumeratorList& rTokens)
{
	//IDENTIFER: implementedInterfaceName
	CTokenIdentifier* pTokenImplementedInterfaceName = ParseIdentifier(rTokens);
	if (!pTokenImplementedInterfaceName) { return FALSE; }

	//CREATE A IMPLEMENTS DIRECTIVE FOR THE CLAS WITH THE GIVEN NAME!!!

	return TRUE;
}

bool CParser::ParseClass(_INOUT CEnumeratorList& rTokens, SCF::ENUM eVisibility, bool bAbstractClass)
{
	//IDENTIFER: className
	CTokenIdentifier* pTokenClassName = ParseIdentifier(rTokens);
	if (!pTokenClassName) { return FALSE; }

	//CREATE A IMPORT DIRECTIVE FOR THE PACKAGE WITH THE GIVEN NAME!!!

	//KEYWORD: "extends"
	if (!ParseKeyword(rTokens, KeywordExtends)) 
	{
		//On success iterate to next token
		rTokens.Next();
		return TRUE;
	}

	//IDENTIFER: extendedClassName
	CTokenIdentifier* pTokenKeywordExtendsClass = ParseIdentifier(rTokens);
	if (!pTokenKeywordExtendsClass) { return FALSE; }

	//CREATE A EXTENDS DIRECTIVE FOR THE CLASS WITH THE GIVEN NAME!!!

	//KEYWORD: "implements"
	if (!ParseKeyword(rTokens, KeywordImplements)) 
	{ 
		//On success iterate to next token
		rTokens.Next();
		return TRUE;
	}

	//IDENTIFER: implementedInterfaceName [ OPERATOR: "," IDENTIFER: implementedInterfaceName ]
	while (ParseImplementedInterfaceName(rTokens)) 
	{
		if (!ParseOperator(rTokens, OperatorSeparator)) 
		{ 
			//On success iterate to next token
			rTokens.Next();
			return TRUE; 
		}
	}

	return FALSE;
}