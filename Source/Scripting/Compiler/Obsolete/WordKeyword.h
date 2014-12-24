#pragma once

#include "Word.h"

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CWordKeyword : public CWord
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassWordKeyword; }

	public:
		CWordKeyword(_IN CString& rText);

	protected:
		~CWordKeyword();
	};
};
