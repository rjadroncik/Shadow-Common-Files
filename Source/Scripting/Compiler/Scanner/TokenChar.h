#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenChar : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenChar(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn);
		virtual ~CTokenChar();

	protected:
		CTokenChar() {}
	};
};
