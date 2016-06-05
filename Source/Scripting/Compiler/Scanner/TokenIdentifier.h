#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenIdentifier : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenIdentifier(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn);
		virtual ~CTokenIdentifier();

	protected:
		CTokenIdentifier() {}
	};
};
