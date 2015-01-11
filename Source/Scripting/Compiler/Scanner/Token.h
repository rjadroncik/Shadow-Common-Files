#pragma once

#include "../../Classes.h"

#include <SCFObjectExtensions.h>

using namespace SCFBase;

namespace SCFCompiler
{
	class SCRIPTING_API CToken : public CObjectSerializable
	{
	public:
		CString ToString() _GET { return *m_pText; }
	
	public:
		virtual const CString TypeString() _GET = 0;

	public:
		virtual ~CToken();

	public:
		const CString& Text() _GET { return *m_pText; }

	public:
		SCF::UINT Line()   _GET { return m_uiLine; }
		SCF::UINT Column() _GET { return m_uiColumn; }

	public:
		void Serialize  (_INOUT IStreamWrite& rStream) const { rStream.PutInt(m_uiLine); rStream.PutInt(m_uiColumn); }
		void Deserialize(_INOUT IStreamRead&  rStream)       { m_uiLine = rStream.GetInt(); m_uiColumn = rStream.GetInt(); }

		void DependentsSerialize  (_INOUT IStreamWriteObject& rStream) const { rStream.Next(*m_pText);  }
		void DependentsDeserialize(_INOUT IStreamReadObject&  rStream)       { rStream.Next(); m_pText = (CStringRange*)rStream.Current(); }

	protected:
		CStringRange* m_pText;

	protected:
		SCF::UINT m_uiLine;
		SCF::UINT m_uiColumn;

	protected:
		//NOTE: [uiColumn] represents the index of the last/ending character of the token, but is transformed here
		CToken(_INOUT _REF CStringRange& rText, SCF::UINT uiLine, SCF::UINT uiColumn);

	protected:
		CToken() {}
	};
};
