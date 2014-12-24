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
		OperatorAssignNegate,   // !=

		OperatorIncrement, // ++
		OperatorDecrement, // --

		OperatorEquals,             // ==
		OperatorUnequals,           // <>
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
		OperatorDestructor, // ~

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
		SCF::ENUM ClassKey() _GET { return ClassTokenOperator; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenOperator(_INOUT _REF CStringRange& rText);
		virtual ~CTokenOperator();

	public:
		SCF::ENUM Operator() _GET { return m_eOperator; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const { rStream.PutInt(m_eOperator); }
		void Deserialize(_INOUT IStreamRead&  rStream)       { m_eOperator = rStream.GetInt(); }


	protected:
		CTokenOperator() {}

	protected:
		SCF::ENUM m_eOperator;
	};
};
