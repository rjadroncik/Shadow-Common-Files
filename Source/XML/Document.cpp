#include "Document.h"
#include "Reader.h"
#include "Writer.h"

using namespace SCFXML;

CXMLDocument::CXMLDocument()
{
	m_pRootElement = nullptr;

	m_Entities.AtPut(STRING("amp"),  *(new CChar('&')));
	m_Entities.AtPut(STRING("lt"),   *(new CChar('<')));
	m_Entities.AtPut(STRING("gt"),   *(new CChar('>')));
	m_Entities.AtPut(STRING("apos"), *(new CChar('\'')));
	m_Entities.AtPut(STRING("quot"), *(new CChar('"')));
}

CXMLDocument::CXMLDocument(_INOUT IStreamReadText& rStream)
{
	m_pRootElement = nullptr;

	m_Entities.AtPut(STRING("amp"),  *(new CChar('&')));
	m_Entities.AtPut(STRING("lt"),   *(new CChar('<')));
	m_Entities.AtPut(STRING("gt"),   *(new CChar('>')));
	m_Entities.AtPut(STRING("apos"), *(new CChar('\'')));
	m_Entities.AtPut(STRING("quot"), *(new CChar('"')));

	this->Read(rStream);
}

CXMLDocument::CXMLDocument(_INOUT _REF CXMLElement& rRootElement)
{
	m_pRootElement = &rRootElement;

	this->ChildAdd(rRootElement);
}

CXMLDocument::~CXMLDocument()
{
	m_Entities.AllDelete();
}

CXMLElement* CXMLDocument::RootElementRetrieve()
{
	if (m_pRootElement) { this->ChildRemove(*m_pRootElement); }
	return m_pRootElement;
}

void CXMLDocument::RootElement(_INOUT _REF CXMLElement* pElement) _SET
{
	if (pElement)
	{
		if (m_pRootElement) { this->ChildReplace(*m_pRootElement, *pElement); }
		else                { this->ChildAdd(*pElement); }
	}
	else
	{
		if (m_pRootElement) { this->ChildRemove(*m_pRootElement); }
	}

	m_pRootElement = pElement;
}

void CXMLDocument::NodesDeleteAll()
{
	CXMLNode* pNode = ChildFirst();
	CXMLNode* pNext = nullptr;

	//Go through all children
	while (pNode)
	{
		pNext = pNode->Next(); 
		delete pNode;
		pNode = pNext;
	}
}

bool CXMLDocument::Read(_INOUT IStreamReadText& rStream)
{
	//Write file contents as a string (automatically converts to Unicode)
	CString csFileString;
	rStream.GetString(csFileString, 0);

	return Parse(csFileString);
}

bool CXMLDocument::Parse(_IN CString& rString)
{
	//Clear memory (if re-parsing)
	if (this->ChildFirst()) { NodesDeleteAll(); }

	//Extract all nodes :)
	CXMLReader().Read(rString, *this, &m_Entities);

	//Select the last regular top-level node as document node
	CXMLNode* pNode = this->ChildFirst();
	while (pNode && pNode->Next() && (pNode->Type() != XmlElement)) { pNode = pNode->Next(); }

	m_pRootElement = (CXMLElement*)pNode;

	return true;
}

bool CXMLDocument::Write(_OUT IStreamWriteText& rStream)
{
	CXMLWriter Writer = CXMLWriter(rStream);

	Writer.WriteDeclaration(m_Declaration);
	Writer.WriteRoot(*this);

	return true;
}
