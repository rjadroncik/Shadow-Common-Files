#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLComment : public CXMLNode
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLComment; }

	public:
		CXMLComment();
		virtual ~CXMLComment();

	public:
		inline const CString Text()                   _GET { return STRINGREF(m_Text); }
		inline void          Text(_IN CString& rText) _SET { m_Text = rText; }

	protected:
		CString m_Text;
	};
};