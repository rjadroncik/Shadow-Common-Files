#pragma once

#include "../../Classes.h"

#include <SCFObjectExtensions.h>

using namespace SCFBase;

namespace SCFCompiler
{
	enum TokenKind 
	{
		//Scanner classes - represent a keyword or a name of a variable, class or function(method)
		TokenKeyword = 1,
		TokenIdentifier,
		TokenOperator,

		TokenChar,
		TokenString,
		TokenNumber,

		TokenType
	};

	class SCRIPTING_API CToken : public CObject
	{
	public:
		CString ToString() _GET { return *m_pText; }
	
	public:
		virtual const CString TypeString() _GET = 0;

	public:
		virtual ~CToken();

	public:
		const CString& Text() _GET { return *m_pText; }
		TokenKind Kind() _GET { return m_eKind; }

	public:
		UINT Line()   _GET { return m_uiLine; }
		UINT Column() _GET { return m_uiColumn; }

	protected:
		CStringRange* m_pText;
		TokenKind m_eKind;

	protected:
		UINT m_uiLine;
		UINT m_uiColumn;

	protected:
		//NOTE: [uiColumn] represents the index of the last/ending character of the token, but is transformed here
		CToken(_INOUT _REF CStringRange& rText, TokenKind eKind, UINT uiLine, UINT uiColumn);

	protected:
		CToken() {}
	};
};
