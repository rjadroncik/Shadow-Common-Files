#include "../Description/ClassReference.h"
#include "../Description/InterfaceReference.h"
#include "../Description/Variable.h"

#include "Parser.h"
#include "Compiler.h"

#include "Scanner/TokenNumber.h"
#include "Scanner/TokenKeyword.h"
#include "Scanner/TokenIdentifier.h"
#include "Scanner/TokenOperator.h"
#include "Scanner/TokenType.h"

using namespace SCFCompiler;
using namespace SCFScripting;

#define NEXT(state) m_fpNext = &CParser::state

namespace
{
   inline CTokenType* ParseType(_INOUT CEnumeratorList<CToken>& rTokens)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenKind::TokenType)) { return NULL; }

      CTokenType* pTokenType = static_cast<CTokenType*>(rTokens.Current());
      if (pTokenType)
      {
         //On success iterate to next token
         rTokens.Next();
      }

      return pTokenType;
   }

   inline CTokenIdentifier* ParseIdentifier(_INOUT CEnumeratorList<CToken>& rTokens)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenIdentifier)) { return NULL; }

      CTokenIdentifier* pTokenIdentifier = static_cast<CTokenIdentifier*>(rTokens.Current());

      //On success iterate to next token
      rTokens.Next();
      return pTokenIdentifier;
   }

   inline CTokenNumber* ParseNumber(_INOUT CEnumeratorList<CToken>& rTokens)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenNumber)) { return NULL; }

      CTokenNumber* pTokenNumber = static_cast<CTokenNumber*>(rTokens.Current());

      //On success iterate to next token
      rTokens.Next();
      return pTokenNumber;
   }

   inline bool ParseOperator(_INOUT CEnumeratorList<CToken>& rTokens, _IN ENUM eOperator)
   {
      //Expecting an operator
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenOperator)) { return FALSE; }

      CTokenOperator* pTokenOperator = static_cast<CTokenOperator*>(rTokens.Current());

      //Expecting the operator given in [eOperator]
      if (pTokenOperator->Operator() != eOperator) { return FALSE; }

      //On success iterate to next token
      rTokens.Next();
      return TRUE;
   }

   inline bool ParseKeyword(_INOUT CEnumeratorList<CToken>& rTokens, _IN ENUM eKeyword)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenKeyword)) { return FALSE; }

      CTokenKeyword* pTokenKeyword = static_cast<CTokenKeyword*>(rTokens.Current());

      if (pTokenKeyword->Keyword() != eKeyword) { return FALSE; }

      //On success iterate to next token
      rTokens.Next();
      return TRUE;
   }

   inline ENUM ParseKeyword(_INOUT CEnumeratorList<CToken>& rTokens)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenKeyword)) { return 0; }

      CTokenKeyword* pTokenKeyword = static_cast<CTokenKeyword*>(rTokens.Current());

      //On success iterate to next token
      rTokens.Next();
      return pTokenKeyword->Keyword();
   }

   inline ENUM ParseKeyword(_INOUT CEnumeratorList<CToken>& rTokens, ENUM min, ENUM max)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenKeyword)) { return 0; }

      CTokenKeyword* pTokenKeyword = static_cast<CTokenKeyword*>(rTokens.Current());

      if ((pTokenKeyword->Keyword() < min) || (pTokenKeyword->Keyword() > max))
      {
         return 0;
      }

      //On success iterate to next token
      rTokens.Next();
      return pTokenKeyword->Keyword();
   }

   inline ENUM ParseVisibility(_INOUT CEnumeratorList<CToken>& rTokens)
   {
      //Expecting a word
      if (!rTokens.Current() || (rTokens.Current()->Kind() != TokenKeyword)) { return 0; }

      CTokenKeyword* pTokenKeyword = static_cast<CTokenKeyword*>(rTokens.Current());

      //Test whether it is one of the visibility keywords: "public", "protected", "private"
      if ((pTokenKeyword->Keyword() < KeywordPublic) || (pTokenKeyword->Keyword() > KeywordPrivate)) { return 0; }

      //On success iterate to next token
      rTokens.Next();
      return pTokenKeyword->Keyword();
   }

   inline bool ParseEnumPair(_INOUT CEnumeratorList<CToken>& rTokens, SCFScripting::CEnum& rEnum)
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

   inline bool ParseFunctionArgument(_INOUT CMethod& rMethod, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //KEYWORD: "out"
      bool isOut = ParseKeyword(rTokens, KeywordOut);

      //KEYWORD: "Bool", ...
      CTokenType* pType = ParseType(rTokens);
      if (!pType)
      {
         //IDENTIFER: value type
         CTokenIdentifier* pClass = ParseIdentifier(rTokens);
         if (!pClass) { return FALSE; }
      }

      //IDENTIFER: argument name
      CTokenIdentifier* pName = ParseIdentifier(rTokens);
      if (!pName) { return FALSE; }

      CArgument* pArgument = new CArgument();
      pArgument->IsOut(isOut);
      //pArgument->
      pArgument->Name(pName->Text());

      rMethod.ArgumentAdd(*pArgument);
      return true;
   }

   bool ParseFunctionArguments(_INOUT CMethod& rMethod, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //OPERATOR: "("
      if (!ParseOperator(rTokens, OperatorBracketOpen)) { return FALSE; }

      //Function arguments
      while (ParseFunctionArgument(rMethod, rTokens))
      {
         if (!ParseOperator(rTokens, OperatorSeparator)) { break; }
      }

      //OPERATOR: ")"
      if (!ParseOperator(rTokens, OperatorBracketClose)) { return FALSE; }

      return TRUE;
   }

   inline bool ParseInterfaceFunctionDefinition(_OUT CList<CMethod> rMethods, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //KEYWORD: "public" | "protected" | "private"
      ENUM eVisibility = ParseVisibility(rTokens);
      if (!eVisibility) { eVisibility = VisibilityPrivate; }

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

      CMethod* pMethod = new CMethod();

      pMethod->Name(pMethodName->Text());
      pMethod->Visibility(static_cast<Visibilities>(eVisibility));

      pMethod->IsStatic(isStatic);
      pMethod->IsAbstract(isAbstract);
      pMethod->IsVirtual(isVirtual);

      rMethods.LastAdd(*pMethod);

      return ParseFunctionArguments(*pMethod, rTokens);
   }

   inline bool ParseInterfaceContents(_INOUT CInterface& rInterface, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      if (ParseOperator(rTokens, OperatorBlockOpen))
      {
         while (ParseInterfaceFunctionDefinition(rInterface.Methods(), rTokens))
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

   inline bool ParseExtendedInterfaceName(_INOUT CInterface& rInterface, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //IDENTIFER: implementedInterfaceName
      CTokenIdentifier* pTokenImplementedInterfaceName = ParseIdentifier(rTokens);
      if (!pTokenImplementedInterfaceName) { return FALSE; }

      CInterfaceReference* pImplementedInterface = new CInterfaceReference();
      pImplementedInterface->Name(pTokenImplementedInterfaceName->Text());

      rInterface.BaseInterfaceAdd(*pImplementedInterface);
      return TRUE;
   }

   inline bool ParseImplementedInterfaceName(_INOUT CClass& rClass, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //IDENTIFER: implementedInterfaceName
      CTokenIdentifier* pTokenImplementedInterfaceName = ParseIdentifier(rTokens);
      if (!pTokenImplementedInterfaceName) { return FALSE; }

      CInterfaceReference* pImplementedInterface = new CInterfaceReference();
      pImplementedInterface->Name(pTokenImplementedInterfaceName->Text());

      rClass.InterfaceAdd(*pImplementedInterface);

      return TRUE;
   }

   bool ParseStatement()
   {
      return FALSE;
   }

   bool ParseFunctionBody(_INOUT CMethod& rMethod, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      if (!ParseOperator(rTokens, OperatorBlockOpen))
      {
         return FALSE;
      }

      //if (!ParseCode(*rMethod, rTokens)) 
      //{
      //	return FALSE;
      //}

      if (!ParseOperator(rTokens, OperatorBlockClose))
      {
         return FALSE;
      }

      return TRUE;
   }

   bool ParseField(_INOUT CClass& rClass, _INOUT CEnumeratorList<CToken>& rTokens, _IN bool bStatic, _IN bool bConstant, _IN ENUM eVisibility)
   {
      //KEYWORD: "Bool", ...
      CTokenType* pType = ParseType(rTokens);
      if (!pType)
      {
         //IDENTIFER: value type
         CTokenIdentifier* pClass = ParseIdentifier(rTokens);
         if (!pClass) { return FALSE; }
      }

      //IDENTIFER: constant name
      CTokenIdentifier* pName = ParseIdentifier(rTokens);
      if (!pName) { return FALSE; }

      CField* pField = new CField();
      pField->Visibility(static_cast<Visibilities>(eVisibility));
      pField->IsStatic(bStatic);
      pField->IsReadOnly(bConstant);
      pField->Name(pName->Text());
      
      rClass.FieldAdd(*pField);

      return TRUE;
   }

   bool ParseClassContents(_INOUT CClass& rClass, _INOUT CEnumeratorList<CToken>& rTokens)
   {
      //KEYWORD: "public" | "protected" | "private"
      ENUM eVisibility = ParseVisibility(rTokens);
      if (!eVisibility) { eVisibility = VisibilityPrivate; }

      ENUM eKeyword = ParseKeyword(rTokens);
      switch (eKeyword)
      {

      case KeywordStatic:
      {

         break;
      }
      case KeywordVirtual:
      case KeywordAbstract:
      case KeywordOverride:
      {
         //ParseMethod();

         break;
      }
      case KeywordConst:
      {
         return ParseField(rClass, rTokens, true, true, eVisibility);
      }

      case KeywordNew:
      {
         CMethod* pConstructor = new CMethod();

         if (!ParseFunctionArguments(*pConstructor, rTokens))
         {
            delete pConstructor;
            return FALSE;
         }
         if (!ParseFunctionBody(*pConstructor, rTokens))
         {
            delete pConstructor;
            return FALSE;
         }

         rClass.ConstructorAdd(*pConstructor);
         break;
      }
      case KeywordDelete:
      {
         if (!ParseFunctionArguments(rClass.Destructor(), rTokens))
         {
            return FALSE;
         }
         if (!ParseFunctionBody(rClass.Destructor(), rTokens))
         {
            return FALSE;
         }

         break;
      }

      default:
      {
         break;
      }
      }
   }
}

CParser::CParser(CCompiler& rCompiler)
{
	m_pCompiler = &rCompiler;
}

CParser::~CParser()
{
}

bool CParser::Parse(_IN CList<CToken>& rTokens, _INOUT CCompilationUnit& rCompilationUnit)
{
   CEnumeratorList<CToken> Tokens(rTokens);
   if (!Tokens.Next()) { return FALSE; }

   m_pCompilationUnit = &rCompilationUnit;

   if (!ParsePackage(Tokens)) { return FALSE; }
   if (!ParseImports(Tokens)) { return FALSE; }
   if (!ParseUDT(Tokens)) { return FALSE; }

   return TRUE;
}

bool CParser::ParsePackage(_INOUT CEnumeratorList<CToken>& rTokens)
{
   //KEYWORD: "package"
   if (!ParseKeyword(rTokens, KeywordPackage)) { return FALSE; }

   //IDENTIFER: packageName
   CTokenIdentifier* pTokenPackageName = ParseIdentifier(rTokens);
   if (!pTokenPackageName) { return FALSE; }

   //Select or create a package with the given name
   m_pCompilationUnit->Package(static_cast<CPackage*>(m_pCompiler->Packages().At(pTokenPackageName->Text())));

   if (m_pCompilationUnit->Package() == NULL)
   {
      CPackage* pPackage = new CPackage();

      pPackage->Name(pTokenPackageName->Text());

      m_pCompiler->Packages().AtPut(pPackage->Name(), *pPackage);
   }

   //OPERATOR: ";"
   return ParseOperator(rTokens, OperatorEnd);
}

ParseResults CParser::ParseImport(_INOUT CEnumeratorList<CToken>& rTokens)
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
   if (!ParseOperator(rTokens, OperatorEnd)) { return ResultError; }
   
   return ResultOkAndContinue;
}

bool CParser::ParseImports(_INOUT CEnumeratorList<CToken>& rTokens)
{
   ParseResults eResult = ResultOk;

   while ((eResult = ParseImport(rTokens)) == ResultOkAndContinue) {}

   if (eResult == ResultOk) { return TRUE; }
   else { return FALSE; }
}

bool CParser::ParseUDT(_INOUT CEnumeratorList<CToken>& rTokens)
{
   //KEYWORD: "public" | "protected" | "private"
   ENUM eVisibility = ParseVisibility(rTokens);
   if (!eVisibility) { eVisibility = VisibilityPrivate; }

   ENUM eUDT = ParseKeyword(rTokens);

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

      eUDT = ParseKeyword(rTokens);
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

bool CParser::ParseEnum(_INOUT CEnumeratorList<CToken>& rTokens, ENUM eVisibility)
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

bool CParser::ParseInterface(_INOUT CEnumeratorList<CToken>& rTokens, ENUM eVisibility)
{
	//IDENTIFER: interfaceName
	CTokenIdentifier* pTokenInterfaceName = ParseIdentifier(rTokens);
	if (!pTokenInterfaceName) { return FALSE; }

	//Create an interface
	CInterface* pInterface = new CInterface();
	pInterface->Name(pTokenInterfaceName->Text());
	
	m_pCompilationUnit->Interfaces().LastAdd(*pInterface);

	//KEYWORD: "extends"
	if (ParseKeyword(rTokens, KeywordExtends))
	{
		//IDENTIFER: extendedInterfaceName [ OPERATOR: "," IDENTIFER: extendedInterfaceName ]
		while (ParseExtendedInterfaceName(*pInterface, rTokens))
		{
			if (!ParseOperator(rTokens, OperatorSeparator))
			{
				break;
			}
		}
	}

	return ParseInterfaceContents(*pInterface, rTokens);
}

bool CParser::ParseClass(_INOUT CEnumeratorList<CToken>& rTokens, ENUM eVisibility, bool bAbstractClass)
{
	//IDENTIFER: className
	CTokenIdentifier* pTokenClassName = ParseIdentifier(rTokens);
	if (!pTokenClassName) { return FALSE; }

	CClass* pClass = new CClass();
	pClass->Name(pTokenClassName->Text());

	m_pCompilationUnit->Classes().LastAdd(*pClass);
	
	//KEYWORD: "extends"
	if (ParseKeyword(rTokens, KeywordExtends))
	{
		//IDENTIFER: extendedClassName
		CTokenIdentifier* pTokenKeywordExtendsClass = ParseIdentifier(rTokens);
		if (!pTokenKeywordExtendsClass) 
		{
			return FALSE; 
		}

		pClass->BaseClass(new CClassReference());
		pClass->BaseClass()->Name(pTokenKeywordExtendsClass->Text());
	}

	//KEYWORD: "implements"
	if (ParseKeyword(rTokens, KeywordImplements))
	{
		//IDENTIFER: implementedInterfaceName [ OPERATOR: "," IDENTIFER: implementedInterfaceName ]
		while (ParseImplementedInterfaceName(*pClass, rTokens))
		{
			if (!ParseOperator(rTokens, OperatorSeparator))
			{
				break;
			}
		}
	}

	if (!ParseOperator(rTokens, OperatorBlockOpen))
	{
		return FALSE;
	}

	while (ParseClassContents(*pClass, rTokens)) { }

	if (!ParseOperator(rTokens, OperatorBlockClose))
	{
		return FALSE;
	}

	return TRUE;
}


