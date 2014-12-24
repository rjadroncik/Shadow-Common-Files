#pragma once

#include "Node.h"

namespace SCFXML
{
	class XML_API CXMLStreamRead;

	class XML_API CXMLElement : public CXMLNode
	{
		friend class XML_API CXMLStreamRead;

	public:
		bool operator ==(_IN CXMLElement& rNode);

	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLElement; }

	public:
		CXMLElement();
		virtual ~CXMLElement();
	
	public:
		///////////////////////// Children ///////////////////////////

		CXMLElement* ChildNamed      (_IN CString rName) _GET;
		CXMLElement* ChildOrSelfNamed(_IN CString rName) _GET;

	public:
		///////////////////////// Siblings ///////////////////////////

		CXMLElement* NextNamed(_IN CString rName) _GET;
	
	public:
		///////////////////////// Attributes ///////////////////////////

		inline CXMLAttribute* AttributeFirst()                                   _GET { return m_pAttributeFirst; }
		inline void           AttributeFirst(_IN _REF CXMLAttribute* pAttribute) _SET { m_pAttributeFirst = (CXMLAttribute*)pAttribute; }
		
		inline CXMLAttribute* AttributeLast()                                    _GET { return m_pAttributeLast; }
		inline void           AttributeLast (_IN _REF CXMLAttribute* pAttribute) _SET { m_pAttributeLast = (CXMLAttribute*)pAttribute; }

		//Creates an object from given data & enlists it
		void AttributeAdd(_IN CString& rName, _IN CString& rValue);
		//Add/enlist an already existing object as sub-object
		void AttributeAdd(_INOUT _REF CXMLAttribute& rAttribute);

		//Get a specific attribute inside this node
		CXMLAttribute* AttributeNamed(_IN CString& szName) _GET;

	public:
		//Utility function which make it easier to access the common 'id' attribute
		CXMLAttribute* ID()       _GET;
		CValue*        IDValue()  _GET;
		const CString  IDString() _GET;
		
	public:
		///////////////////////// Relatives ///////////////////////////

		inline CXMLElement* Parent()                       _GET { return (CXMLElement*)m_pParent; }
		inline void			Parent(_IN CXMLElement* pNode) _SET { m_pParent = (CXMLElement*)pNode; }

		inline CXMLElement* Previous()                       _GET { return (CXMLElement*)m_pPrevious; }
		inline void			Previous(_IN CXMLElement* pNode) _SET { m_pPrevious = (CXMLElement*)pNode; }

		inline CXMLElement* Next()                       _GET { return (CXMLElement*)m_pNext; }
		inline void			Next(_IN CXMLElement* pNode) _SET { m_pNext = (CXMLElement*)pNode; }

	public:
		///////////////////////// Children ///////////////////////////

		inline CXMLElement* ChildFirst()                       _GET { return (CXMLElement*)m_pChildFirst; }
		inline void			ChildFirst(_IN CXMLElement* pNode) _SET { m_pChildFirst = (CXMLElement*)pNode; }

		inline CXMLElement* ChildLast()                       _GET { return (CXMLElement*)m_pChildLast; }
		inline void			ChildLast(_IN CXMLElement* pNode) _SET { m_pChildLast = (CXMLElement*)pNode; }

	private:
		//Attributes
		CXMLAttribute* m_pAttributeFirst;
		CXMLAttribute* m_pAttributeLast;
	};
};