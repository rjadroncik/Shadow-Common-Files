#pragma once

#include "../Classes.h"

#include <SCFObjectExtensions.h>

using namespace SCFBase;

namespace SCFCompiler
{
	enum ObjectTypes
	{
		Expression = 0,
		Assignment,
		Variable,

		LoopFor,
		LoopWhile,
		LoopDoWhile,
		
		LiteralInt,
		LiteralFloat,
		LiteralChar,
		LiteralString,
	};

	class SCRIPTING_API CWord : public CObject
	{
	public:
		SCF::ENUM ClassKey() _GET  { return ClassWord; }
		CString   AsString() const { return *m_pText; }

	public:
		CWord(_IN CStringRange& rText, _IN SCF::ENUM eType);

	public:
		CWord(_IN CWord& rWord);

	public:
		virtual ~CWord();

	public:
		CStringRange& Text() _GET { return *m_pText; }

	public:
		SCF::ENUM Type()                    _GET { return m_eType; }
		void      Type(_IN SCF::ENUM eType) _SET { m_eType = eType; }

	protected:
		CStringRange* m_pText;
		SCF::ENUM     m_eType;
	};
};
