#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenString : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenString(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn);
		virtual ~CTokenString();

	protected:
		CTokenString() {}
	};
};
