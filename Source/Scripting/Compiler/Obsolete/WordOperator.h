#pragma once

#include "Word.h"

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CWordOperator : public CWord
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassWordOperator; }

	public:
		CWordOperator(_IN CString& rText);

	protected:
		~CWordOperator();
	};
};
