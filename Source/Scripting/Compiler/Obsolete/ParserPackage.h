#pragma once

#include "Scanner.h"

namespace SCFCompiler
{
	class SCRIPTING_API CParserPackage
	{
	public:
		bool Parse(_INOUT CEnumeratorList& rTokens);
	};
};
