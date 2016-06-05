#pragma once

#include "Token.h"

namespace SCFCompiler
{
	enum Types
	{
		TYPE_MIN = 1000,

		//Primitive data types
		TypeBool = TYPE_MIN,
		TypeInt,
		TypeLong,
		TypeFloat,
		TypeDouble,
		TypeChar,

		//Special data types
		TypeString = 2000,
		TypeObject,
		TypeVoid,
		TypeVar,

		TYPE_MAX = TypeVar
	};

	class SCRIPTING_API CTokenType : public CToken
	{
	public:
		const CString TypeString() _GET;

	public:
		CTokenType(_INOUT _REF CStringRange& rText, ENUM eType, UINT uiLine, UINT uiColumn);
		virtual ~CTokenType();

	public:
		ENUM Keyword() _GET { return m_eType; }

	protected:
		CTokenType() {}

	protected:
		ENUM m_eType;
	};
};
