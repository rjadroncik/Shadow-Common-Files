#include "Node.h"
#include "Writer.h"

using namespace SCF;
using namespace SCFXML;

CXMLElement::CXMLElement()
{
	m_pAttributeFirst = NULL;
	m_pAttributeLast  = NULL;
}

CXMLElement::~CXMLElement()
{
	CXMLAttribute* pCurAttribute  = m_pAttributeFirst;
	CXMLAttribute* pNextAttribute = NULL;

	while (pCurAttribute)
	{
		pNextAttribute = pCurAttribute->Next();
		delete pCurAttribute;
		pCurAttribute = pNextAttribute;
	}

	if (m_pValue) {  RELEASE(*(m_pValue)); delete m_pValue; m_pValue = NULL; }
}

CXMLNode* CXMLNode::ChildNamed(_IN CString rName) _GET
{
	register CXMLNode* pNode = ChildFirst();

	//Go through all children  & compare names
	while (pNode)
	{
		if (pNode->QName() == rName) { return pNode; }

		pNode = pNode->Next();
	}

	return NULL;
}

CXMLNode* CXMLNode::ChildOrSelfNamed(_IN CString rName) _GET
{
	if (this->QName() == rName) { return (CXMLNode*)this; }

	return ChildNamed(rName);
}

CXMLNode* CXMLNode::NextNamed(_IN CString rName) _GET
{
	register CXMLNode* pNode = (CXMLNode*)m_pNext;

	//Go through all siblings & compare names
	while (pNode)
	{
		if (pNode->QName() == rName) { return pNode; }

		pNode = pNode->Next();
	}

	return NULL;
}

void CXMLElement::AttributeAdd(_IN CString& rName, _IN CString& rValue)
{
	//Create a new attribute
	CXMLAttribute* pAttribute = new CXMLAttribute();
	
	pAttribute->Name(rName);

	//Create & store it's value
	pAttribute->Value(new CString(rValue));

	AttributeAdd(*pAttribute);
}

void CXMLElement::AttributeAdd(_INOUT _REF CXMLAttribute& rAttribute)
{
	rAttribute.Previous(m_pAttributeLast);
	rAttribute.Parent(this);

	if (!m_pAttributeLast)
	{
		m_pAttributeFirst = &rAttribute;
		m_pAttributeLast  = m_pAttributeFirst;
		return;
	}
	
	m_pAttributeLast->Next(&rAttribute);
	m_pAttributeLast = &rAttribute;
}


CXMLAttribute* CXMLElement::ID() _GET
{
	return AttributeNamed(STRING("id"));
}

CValue* CXMLElement::IDValue() _GET
{
	CXMLAttribute* pID = AttributeNamed(STRING("id"));

	if (pID) { return pID->Value(); }
	else     { return NULL; }
}

const CString CXMLElement::IDString() _GET
{
	CXMLAttribute* pID = AttributeNamed(STRING("id"));

	if (pID && pID->Value()) { return pID->Value()->AsString(); }
	else                     { return CString(); }
};

CXMLAttribute* CXMLElement::AttributeNamed(_IN CString& szName) _GET
{
	CXMLAttribute* pAttribute = m_pAttributeFirst;

	//Go through all attributes & compare their names with the one we are looking for
	while (pAttribute)
	{
		if (pAttribute->Name() == szName) { return pAttribute; }

		pAttribute = pAttribute->Next();
	}

	return NULL;
}

bool CXMLElement::operator ==(_IN CXMLElement& rNode)
{
	//Check node types
	if (m_eType != rNode.m_eType) { return FALSE; }

	//Check node names
	if (!(m_QName == rNode.m_QName)) { return FALSE; }
	
	//Check node leaf status
	if (IsLeaf() != rNode.IsLeaf()) { return FALSE; }

	//Check node values
	if (IsLeaf() && rNode.IsLeaf()) 
	{
		if ((m_pValue && !rNode.m_pValue) || (!m_pValue && rNode.m_pValue)) { return FALSE; }

		if (m_pValue && rNode.m_pValue)
		{
			if (m_pValue->IsSameTypeAs(*(rNode.m_pValue))) 
			{ 
				return m_pValue->IsEqualTo(*(rNode.m_pValue)); 
			}

			if (m_pValue->ClassKey() == ClassString)
			{
				return ((*(CString*)m_pValue) == rNode.m_pValue->AsString());
			}
			if (rNode.m_pValue->ClassKey() == ClassString)
			{
				return ((*(CString*)(rNode.m_pValue)) == m_pValue->AsString());
			}
		}
	}

	//Check node attributes
	CXMLAttribute* pAttribute    =       m_pAttributeFirst;
	CXMLAttribute* pAttribute2nd = rNode.m_pAttributeFirst;

	while (pAttribute && pAttribute2nd)
	{
		if (!(*pAttribute == *pAttribute2nd)) { return FALSE; }

		pAttribute    = pAttribute->Next();
		pAttribute2nd = pAttribute2nd->Next();
	}

	//Both should be NULL at this point if the nodes are equal
	if (pAttribute != pAttribute2nd) { return FALSE; }

	//Check ALL sub-nodes
	CXMLElement* pChild    = ChildFirst();
	CXMLElement* pChild2nd = rNode.ChildFirst();

	while (pChild && pChild2nd)
	{
		if (!(*pChild == *pChild2nd)) { return FALSE; }

		pChild    = pChild->Next();
		pChild2nd = pChild2nd->Next();
	}

	//Both should be NULL at this point if the nodes are equal
	if (pChild != pChild2nd) { return FALSE; }

	return TRUE;
}