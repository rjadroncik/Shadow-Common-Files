#pragma once

#include "Token.h"

namespace SCFCompiler
{
	class SCRIPTING_API CTokenNumber : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenNumber(_INOUT _REF CStringRange& rText, UINT uiLine, UINT uiColumn);
		virtual ~CTokenNumber();

	public:
		CValue& Value() _GET { return *m_pValue; }

	public:
		CInt&   ValueAsInt()   _GET { return   *(CInt*)m_pValue; }
		CFloat& ValueAsFloat() _GET { return *(CFloat*)m_pValue; }

	protected:
		CTokenNumber() {}

	private:
		CValue* m_pValue;
	};
};
