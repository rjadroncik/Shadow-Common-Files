#include "StreamRead.h"

using namespace SCFXML;

CXMLStreamRead::CXMLStreamRead(_IN _REF CXMLNode& rNode)
{
	m_pEnumerator = new CXMLEnumerator(rNode);
	m_pAttribute = NULL;
}

CXMLStreamRead::CXMLStreamRead(_IN _REF CXMLDocument& rDocument)
{
	m_pEnumerator = new CXMLEnumerator(*rDocument.RootElement());
	m_pAttribute = NULL;
}

CXMLStreamRead::~CXMLStreamRead()
{
	delete m_pEnumerator;
}

CString* CXMLStreamRead::GetBlock()
{
	m_pAttribute = NULL;

	if (m_pEnumerator->Next())
	{
		while (m_pEnumerator->Current()->Type() != XmlElement) { m_pEnumerator->Next(); }

		if (m_pEnumerator->Current())
		{
			return &m_pEnumerator->Current()->m_QName;
		}
	}
	
	return NULL;
}

CString* CXMLStreamRead::GetValue(_OUT _OPT CString** ppOutName)
{
	if (m_pAttribute) { m_pAttribute = m_pAttribute->Next(); }
	else              { m_pAttribute = ((CXMLElement*)(m_pEnumerator->Current()))->AttributeFirst(); }

	if (m_pAttribute)
	{
		if (ppOutName) { *ppOutName = &m_pAttribute->m_QName; }

		return (CString*)m_pAttribute->m_pValue;
	}

	return NULL;
}

CString* CXMLStreamRead::GetBlockValue()
{
	return (CString*)(((CXMLElement*)(m_pEnumerator->Current()))->Value());
}