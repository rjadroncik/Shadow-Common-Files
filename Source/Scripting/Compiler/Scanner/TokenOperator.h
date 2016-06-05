#pragma once

#include "Token.h"

namespace SCFCompiler
{
	enum Operators
	{
		OperatorAdd = 1,  // +
		OperatorSubtract, // -
		OperatorMultiply, // *
		OperatorDivide,   // /
		OperatorModulo,   // %
		OperatorNegate,   // !

		OperatorAssignAdd,      // +=
		OperatorAssignSubtract, // -=
		OperatorAssignMultiply, // *=
		OperatorAssignDivide,   // /=
		OperatorAssignModulo,   // %=

		OperatorIncrement, // ++
		OperatorDecrement, // --

		OperatorEquals,             // ==
		OperatorNotEquals,          // !=
		OperatorGreaterThan,        // >
		OperatorGreaterThanOrEqual, // >=
		OperatorLessThan,           // <
		OperatorLessThanOrEqual,    // <=

		OperatorAnd, // &&
		OperatorOr,  // ||

		OperatorAssign,     // =
		OperatorAccess,     // .
		OperatorPackage,    // :
		OperatorEnd,        // ;
		OperatorSeparator,  // ,

		OperatorArrayOpen,    // [
		OperatorArrayClose,   // ]
		OperatorBracketOpen,  // (
		OperatorBracketClose, // )
		OperatorBlockOpen,    // {
		OperatorBlockClose,   // }
	};

	class SCRIPTING_API CTokenOperator : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenOperator(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn);
		virtual ~CTokenOperator();

	public:
		ENUM Operator() _GET { return m_eOperator; }

	protected:
		CTokenOperator() {}

	protected:
		ENUM m_eOperator;
	};
};
