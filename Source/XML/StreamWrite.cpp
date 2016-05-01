#include "StreamWrite.h"
#include "Attribute.h"

using namespace SCFXML;

CXMLStreamWrite::CXMLStreamWrite(_OUT _REF CXMLElement& rElement)
{
	m_pRoot = &rElement;
	ADDREF(*m_pRoot);

	m_pElement = m_pRoot;
}

CXMLStreamWrite::CXMLStreamWrite(_OUT _REF CXMLDocument& rDocument)
{
	if (!rDocument.RootElement())
	{
		rDocument.RootElement(new CXMLElement());
		rDocument.RootElement()->QName(STRING("root"));
	}

	m_pRoot = rDocument.RootElement();
	ADDREF(*m_pRoot);

	m_pElement = m_pRoot;
}

CXMLStreamWrite::CXMLStreamWrite(_OUT _REF CXMLDocument& rDocument, _IN CString& rRootName, _IN CString& rRootPrefix, _IN CString& rRootNamespace)
{
	if (rDocument.RootElement())
	{
		rDocument.RootElement()->DeleteWithValues();
	}

	CXMLAttribute* pNamespaceAttribute = new CXMLAttribute();
	pNamespaceAttribute->QName(STRING("xmlns:") + rRootPrefix);
	pNamespaceAttribute->Value(new CString(rRootNamespace));

	rDocument.RootElement(new CXMLElement());
	rDocument.RootElement()->QName(rRootName);
	rDocument.RootElement()->AttributeAdd(*pNamespaceAttribute);

	m_pRoot = rDocument.RootElement();
	ADDREF(*m_pRoot);

	m_pElement = m_pRoot;
}

CXMLStreamWrite::~CXMLStreamWrite()
{
	RELEASE(*m_pRoot);
}

//Creates a new sub-node of the current node
void CXMLStreamWrite::BlockStart(_IN CString& rName)
{
	CXMLElement* pElement = new CXMLElement();
	pElement->QName(rName); 

	m_pElement->ChildAdd(*pElement);
	m_pElement = pElement;
}

void CXMLStreamWrite::BlockEnd()
{
	//Must have a parent when closing a block
	_ASSERTE(m_pElement->Parent());

	m_pElement = m_pElement->Parent();
}

void CXMLStreamWrite::BlockValue(_IN CString& rValue)
{
	//Cannot put values into root
	_ASSERTE(m_pElement != m_pRoot);

	m_pElement->Value(&rValue);
}

void CXMLStreamWrite::PutValue(_IN CString& rName, _IN _REF CString& rValue)
{
	//Cannot put values into root
	_ASSERTE(m_pElement != m_pRoot);

	CXMLAttribute* pAttrib = new CXMLAttribute();
	pAttrib->Name (rName);
	pAttrib->Value(&rValue);

	m_pElement->AttributeAdd(*pAttrib);
}

