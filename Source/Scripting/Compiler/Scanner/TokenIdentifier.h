#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenIdentifier : public CToken
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTokenIdentifier; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenIdentifier(_INOUT _REF CStringRange& rText);
		virtual ~CTokenIdentifier();

	protected:
		CTokenIdentifier() {}
	};
};
