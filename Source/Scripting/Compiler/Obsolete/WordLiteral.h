#pragma once

#include "Word.h"

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CWordLiteral : public CWord
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassWordLiteral; }

	public:
		CWordLiteral(_IN CString& rText);

	protected:
		~CWordLiteral();
	};
};
