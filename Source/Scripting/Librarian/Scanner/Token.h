#pragma once

#include "../../Classes.h"

#include <SCFObjectExtensions.h>

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CToken : public CObjectSerializable
	{
	public:
		CString AsString() _GET { return *m_pText; }
	
	public:
		virtual CString TypeString() _GET = 0;

	public:
		virtual ~CToken();

	public:
		virtual bool Update() = 0;

	public:
		CStringRange& Text() _GET { return *m_pText; }

	public:
		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const { rStream.Next(*m_pText); }
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream)       { rStream.Next(); m_pText = (CStringRange*)rStream.Current(); }

	protected:
		CStringRange* m_pText;

	protected:
		CToken(_INOUT _REF CStringRange& rText);

	protected:
		CToken() {}
	};
};
