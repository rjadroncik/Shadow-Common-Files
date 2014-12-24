#include "TokenOperator.h"

using namespace SCFCompiler;

CString CTokenOperator::TypeString() _GET
{
	CString RetVal;
	RetVal.Resize(m_pText->Length());

	for (SCF::UINT i = 0; i < m_pText->Length(); i++) { RetVal.AtPut(i, 'o'); }

	return RetVal;
}

CTokenOperator::CTokenOperator(_INOUT _REF CStringRange& rText) : CToken(rText)
{
	m_eOperator = 0;
}

CTokenOperator::~CTokenOperator()
{
}

bool CTokenOperator::Update()
{
	static CDictionaryString s_Operators;

	if (!s_Operators.Size())
	{
		s_Operators.AtPut(STRING("+"), *(new CEnum(OperatorAdd)));
		s_Operators.AtPut(STRING("-"), *(new CEnum(OperatorSubtract)));
		s_Operators.AtPut(STRING("*"), *(new CEnum(OperatorMultiply)));
		s_Operators.AtPut(STRING("/"), *(new CEnum(OperatorDivide)));
		s_Operators.AtPut(STRING("%"), *(new CEnum(OperatorModulo)));
		s_Operators.AtPut(STRING("!"), *(new CEnum(OperatorNegate)));

		s_Operators.AtPut(STRING("+="), *(new CEnum(OperatorAssignAdd)));
		s_Operators.AtPut(STRING("-="), *(new CEnum(OperatorAssignSubtract)));
		s_Operators.AtPut(STRING("*="), *(new CEnum(OperatorAssignMultiply)));
		s_Operators.AtPut(STRING("/="), *(new CEnum(OperatorAssignDivide)));
		s_Operators.AtPut(STRING("%="), *(new CEnum(OperatorAssignModulo)));
		s_Operators.AtPut(STRING("!="), *(new CEnum(OperatorAssignNegate)));

		s_Operators.AtPut(STRING("++"), *(new CEnum(OperatorIncrement)));
		s_Operators.AtPut(STRING("--"), *(new CEnum(OperatorDecrement)));

		s_Operators.AtPut(STRING("=="), *(new CEnum(OperatorEquals)));
		s_Operators.AtPut(STRING("<>"), *(new CEnum(OperatorUnequals)));
		s_Operators.AtPut(STRING(">"),  *(new CEnum(OperatorGreaterThan)));
		s_Operators.AtPut(STRING(">="), *(new CEnum(OperatorGreaterThanOrEqual)));
		s_Operators.AtPut(STRING("<"),  *(new CEnum(OperatorLessThan)));
		s_Operators.AtPut(STRING("<="), *(new CEnum(OperatorLessThanOrEqual)));

		s_Operators.AtPut(STRING("&&"), *(new CEnum(OperatorAnd)));
		s_Operators.AtPut(STRING("||"), *(new CEnum(OperatorOr)));

		s_Operators.AtPut(STRING("="), *(new CEnum(OperatorAssign)));
		s_Operators.AtPut(STRING("."), *(new CEnum(OperatorAccess)));
		s_Operators.AtPut(STRING(":"), *(new CEnum(OperatorPackage)));
		s_Operators.AtPut(STRING(";"), *(new CEnum(OperatorEnd)));
		s_Operators.AtPut(STRING(","), *(new CEnum(OperatorSeparator)));
		s_Operators.AtPut(STRING("~"), *(new CEnum(OperatorDestructor)));

		s_Operators.AtPut(STRING("["), *(new CEnum(OperatorArrayOpen)));
		s_Operators.AtPut(STRING("]"), *(new CEnum(OperatorArrayClose)));
		s_Operators.AtPut(STRING("("), *(new CEnum(OperatorBracketOpen)));
		s_Operators.AtPut(STRING(")"), *(new CEnum(OperatorBracketClose)));
		s_Operators.AtPut(STRING("{"), *(new CEnum(OperatorBlockOpen)));
		s_Operators.AtPut(STRING("}"), *(new CEnum(OperatorBlockClose)));
	}

	CEnum* pEnum = (CEnum*)s_Operators.At(*m_pText);
	if (pEnum) 
	{
		m_eOperator = pEnum->Value(); 
		return TRUE;
	}

	return FALSE;
}
