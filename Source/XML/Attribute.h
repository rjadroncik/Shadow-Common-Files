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
		CString   ToString() _GET;
		SCF::ENUM ClassKey() _GET { return ClassXMLAttribute; }

	public:
		CXMLAttribute();
		virtual ~CXMLAttribute();

	public:
		///////////////////////// Relatives ///////////////////////////

		inline CXMLElement* Parent()                       _GET { return (CXMLElement*)m_pParent; }
		inline void			Parent(_IN CXMLElement* pNode) _SET { m_pParent = (CXMLElement*)pNode; }

		inline CXMLAttribute* Previous()                              _GET { return (CXMLAttribute*)m_pPrevious; }
		inline void			  Previous(_IN CXMLAttribute* pAttribute) _SET { m_pPrevious = (CXMLAttribute*)pAttribute; }

		inline CXMLAttribute* Next()                              _GET { return (CXMLAttribute*)m_pNext; }
		inline void			  Next(_IN CXMLAttribute* pAttribute) _SET { m_pNext = (CXMLAttribute*)pAttribute; }

	public:
		inline CValue* Value()                        _GET { return (CValue*)m_pValue; }
		inline void    Value(_IN _REF CValue* pValue) _SET { BETAONLY(if (m_pValue) { m_pValue->Release(); }) m_pValue = (CValue*)pValue; BETAONLY(if (m_pValue) { m_pValue->AddRef(); }) }
	};
};