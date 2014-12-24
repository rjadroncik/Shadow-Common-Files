#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenChar : public CToken
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTokenChar; }

	public:
		CString TypeString() _GET;

	public:
		CTokenChar(_INOUT _REF CStringRange& rText);
		virtual ~CTokenChar();

	public:
		bool Update();

	protected:
		CTokenChar() {}
	};
};
