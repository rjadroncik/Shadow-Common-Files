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
		SCF::ENUM ClassKey() _GET { return ClassTokenType; }

	public:
		const CString TypeString() _GET;

	public:
		CTokenType(_INOUT _REF CStringRange& rText, SCF::ENUM eType, SCF::UINT uiLine, SCF::UINT uiColumn);
		virtual ~CTokenType();

	public:
		SCF::ENUM Keyword() _GET { return m_eType; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const { rStream.PutInt(m_eType); }
		void Deserialize(_INOUT IStreamRead&  rStream)       { m_eType = rStream.GetInt(); }

	protected:
		CTokenType() {}

	protected:
		SCF::ENUM m_eType;
	};
};
