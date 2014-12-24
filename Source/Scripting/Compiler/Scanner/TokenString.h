#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenString : public CToken
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTokenString; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenString(_INOUT _REF CStringRange& rText);
		virtual ~CTokenString();

	protected:
		CTokenString() {}
	};
};
