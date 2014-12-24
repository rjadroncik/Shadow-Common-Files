#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLCData : public CXMLNode
	{

	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLCData; }

	public:
		CXMLCData();
		virtual ~CXMLCData();

	public:
		inline const CString Text()                   _GET { return STRINGREF(m_Text); }
		inline void          Text(_IN CString& rText) _SET { m_Text = rText; }

	protected:
		CString m_Text;
	};
};