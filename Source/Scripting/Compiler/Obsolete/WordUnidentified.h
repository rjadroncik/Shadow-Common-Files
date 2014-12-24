#pragma once

#include "Word.h"

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CWordUnidentified : public CWord
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassWordUnidentified; }

	public:
		CWordUnidentified(_IN CString& rText);

	protected:
		~CWordUnidentified();
	};
};
