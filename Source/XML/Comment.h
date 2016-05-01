#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLComment : public CXMLNode
	{
	public:
		CXMLComment();
		virtual ~CXMLComment();

	public:
		virtual NodeType Type() _GET { return XmlComment; }

	public:
		inline const CString Text()                   _GET { return STRINGREF(m_Text); }
		inline void          Text(_IN CString& rText) _SET { m_Text = rText; }

	protected:
		CString m_Text;
	};
};