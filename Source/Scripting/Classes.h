#pragma once

#ifdef SCRIPTING_EXPORTS
#define SCRIPTING_API __declspec(dllexport)
#else
#define SCRIPTING_API __declspec(dllimport)
#endif

namespace SCFScripting
{
	//Reserved class keys/error codes: 0x0301 - 0x0320

	enum Classes
	{
		//Type information
		ClassArgument = 0x0301,
		ClassMethod,
		ClassMethodSignature,
		ClassProperty,
		ClassField,

		ClassEnum,
		ClassInterface,
		ClassClass,

		ClassPackage,

		ClassExecutable,
		ClassThread,
		ClassProcess,

		ClassClassReference,
		ClassInterfaceReference,
	};
};

namespace SCFCompiler
{
	//Reserved class keys/error codes: 0x0321 - 0x0340
	//Made some room for librarian classes 0x0341 - 0x0350

	enum Classes
	{
		//Scanner classes - represent a keyword or a name of a variable, class or function(method)
		ClassTokenKeyword = 0x0321,
		ClassTokenIdentifier,
		ClassTokenOperator,

		ClassTokenChar,
		ClassTokenString,
		ClassTokenNumber,
		ClassTokenType,

		//Parser classes
		ClassCompilationUnit,

		//Compiler components
		ClassScanner,
		ClassParser,
		ClassCompiler,
	};
};

