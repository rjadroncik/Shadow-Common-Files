#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLNotation : public CXMLNode
	{
	public:
		CXMLNotation();
		virtual ~CXMLNotation();

	public:
		virtual NodeType Type() _GET { return XmlNotation; }

	public:
		inline const CString Text()                   _GET { return STRINGREF(m_Text); }
		inline void          Text(_IN CString& rText) _SET { m_Text = rText; }

	protected:
		CString m_Text;
	};
};