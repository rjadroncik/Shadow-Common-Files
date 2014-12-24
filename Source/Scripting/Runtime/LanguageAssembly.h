#pragma once

namespace SCFScripting
{
	enum Operations
	{
		OperationNew,
		OperationDelete,

		OperationCallStd,
		OperationCallThis,
		OperationCallSIL,

		OperationIfElse,
		OperationFor,
		OperationWhile,
		OperationDoWhile,

		OperationLoadValue,

		OperationArithmeticInt,   //Add, Sub, Mul, Div, Rmd
		OperationArithmeticFloat, //Add, Sub, Mul, Div, Rmd, Sqrt, Log

		OperationsCount,
	};
};