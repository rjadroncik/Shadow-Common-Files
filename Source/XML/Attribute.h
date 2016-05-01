#pragma once

#include "Node.h"
#include "Element.h"

namespace SCFXML
{
	class XML_API CXMLStreamRead;

	class XML_API CXMLAttribute : public CXMLNode
	{
		friend class XML_API CXMLStreamRead;

	public:
		bool operator ==(_IN CXMLAttribute& rAttribute);

	public:
		CString ToString() _GET;

	public:
		CXMLAttribute();
		virtual ~CXMLAttribute();

	public:
		virtual NodeType Type() _GET { return XmlAttribute; }

	public:
		///////////////////////// Relatives ///////////////////////////

		inline CXMLElement* Parent()                       _GET { return (CXMLElement*)m_pParent; }
		inline void			Parent(_IN CXMLElement* pNode) _SET { m_pParent = (CXMLElement*)pNode; }

		inline CXMLAttribute* Previous()                              _GET { return (CXMLAttribute*)m_pPrevious; }
		inline void			  Previous(_IN CXMLAttribute* pAttribute) _SET { m_pPrevious = (CXMLAttribute*)pAttribute; }

		inline CXMLAttribute* Next()                              _GET { return (CXMLAttribute*)m_pNext; }
		inline void			  Next(_IN CXMLAttribute* pAttribute) _SET { m_pNext = (CXMLAttribute*)pAttribute; }
	};
};