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
		CTokenString(_INOUT _REF CStringRange& rText, SCF::UINT uiLine, SCF::UINT uiColumn);
		virtual ~CTokenString();

	protected:
		CTokenString() {}
	};
};
