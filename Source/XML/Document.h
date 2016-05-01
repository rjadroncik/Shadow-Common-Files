#pragma once

#include "Node.h"
#include "Element.h"
#include "Declaration.h"

namespace SCFXML
{
	class XML_API CXMLDocument : public CXMLNode
	{
		friend class XML_API CXMLEnumerator;

		friend class XML_API CXMLStreamRead;
		friend class XML_API CXMLStreamReadObject;

		friend class XML_API CXMLStreamWrite;
		friend class XML_API CXMLStreamWriteObject;
		friend class XML_API CXMLPath;

	public:
		CXMLDocument();
		CXMLDocument(_INOUT IStreamReadText& rStream);
		CXMLDocument(_INOUT _REF CXMLElement& rRootElement);

		virtual ~CXMLDocument();

	public:
		virtual NodeType Type() _GET { return XmlDocument; }

	public:
		bool Read (_INOUT IStreamReadText&  rStream);
		bool Write(_OUT   IStreamWriteText& rStream);

	public:
		bool Parse(_IN CString& rString);

	public:
		inline CXMLElement* RootElement()                                  _GET { return m_pRootElement; }
		       void         RootElement(_INOUT _REF CXMLElement* pElement) _SET;

	public:
		//Retrieves the root element node, separating it from the actual XML document object
		CXMLElement* RootElementRetrieve();

	public:
		//Properly removes all nodes associated with this document
		void NodesDeleteAll();

	protected:
		CXMLDeclaration m_Declaration;

	protected:
		CXMLElement* m_pRootElement;

	protected:
		#pragma warning(disable:4251)
		CDictionaryString<CChar> m_Entities;
		#pragma warning(default:4251)
	};
};