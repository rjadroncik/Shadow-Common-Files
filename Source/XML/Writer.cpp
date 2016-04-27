#include "Writer.h"

using namespace SCF;
using namespace SCFXML;

CDictionaryString<CChar>* Writer_pEntitiesDefault = NULL;

CXMLWriter::CXMLWriter(_INOUT IStreamWriteText& rStream, _INOUT CDictionaryString<CChar>* pEntities)
{
	if (pEntities) { m_pEntities = pEntities; }
	else
	{
		if (!Writer_pEntitiesDefault)
		{
			Writer_pEntitiesDefault = new CDictionaryString<CChar>();

			Writer_pEntitiesDefault->AtPut(STRING("amp"),  *(new CChar('&')));
			Writer_pEntitiesDefault->AtPut(STRING("lt"),   *(new CChar('<')));
			Writer_pEntitiesDefault->AtPut(STRING("gt"),   *(new CChar('>')));
			Writer_pEntitiesDefault->AtPut(STRING("apos"), *(new CChar('\'')));
			Writer_pEntitiesDefault->AtPut(STRING("quot"), *(new CChar('"')));
		}

		m_pEntities = Writer_pEntitiesDefault;
	}

	m_pStream = &rStream;
	m_pEntities = pEntities;
}

CXMLWriter::~CXMLWriter()
{

}

void CXMLWriter::WriteDeclaration(_IN CXMLDeclaration& rDeclaration)
{
	static CString s_TagDeclarationStart(STRING_CREATE_ARGS("<?xml "));
	static CString s_TagDeclarationEnd  (STRING_CREATE_ARGS("?>"));

	static CString s_TagDeclarationVersion(STRING_CREATE_ARGS("version=\""));
	static CString s_TagDeclarationEncoding(STRING_CREATE_ARGS("encoding=\""));
	static CString s_TagDeclarationStandalone(STRING_CREATE_ARGS("standalone=\""));
	static CString s_TagDeclarationAttributeEnd(STRING_CREATE_ARGS("\" "));
	
	m_pStream->PutString(s_TagDeclarationStart);

	m_pStream->PutString(s_TagDeclarationVersion);
	m_pStream->PutString(rDeclaration.Version());
	m_pStream->PutString(s_TagDeclarationAttributeEnd);

	m_pStream->PutString(s_TagDeclarationEncoding);
	m_pStream->PutString(rDeclaration.Encoding());
	m_pStream->PutString(s_TagDeclarationAttributeEnd);

	m_pStream->PutString(s_TagDeclarationStandalone);
	m_pStream->PutString(rDeclaration.Standalone());
	m_pStream->PutString(s_TagDeclarationAttributeEnd);

	m_pStream->PutString(s_TagDeclarationEnd);
}

void CXMLWriter::WriteRoot(_IN CXMLNode& rRoot)
{
	//Write all children
	CXMLNode* pChild = (CXMLNode*)rRoot.ChildFirst();
	while (pChild)
	{
		WriteNode(*pChild, 0);

		pChild = (CXMLNode*)pChild->Next();

		//Write child on new line
		m_pStream->PutLine();
	}
}

void CXMLWriter::WriteNode(_IN CXMLNode& rNode, _IN SCF::UINT uiIndent)
{
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(FALSE));

	static CString s_TagElementSingleEnd(STRING_CREATE_ARGS("/>"));
	static CString s_TagElementCloseStart(STRING_CREATE_ARGS("</"));

	static CString s_TagCommentStart(STRING_CREATE_ARGS("<!--"));
	static CString s_TagCommentEnd  (STRING_CREATE_ARGS("-->"));

	static CString s_TagNotationStart(STRING_CREATE_ARGS("<!"));

	static CString s_TagCDataStart(STRING_CREATE_ARGS("<![CDATA["));
	static CString s_TagCDataEnd  (STRING_CREATE_ARGS("]]>"));

	BETAONLY(CObject::Tracing(bTracing));

	//Write tabs to create indentation
	for (SCF::UINT i = 0; i < (uiIndent << 1); i++) { m_pStream->PutChar(' '); }

	//Write start of tag
	switch (rNode.ClassKey())
	{
	case ClassXMLElement:     { m_pStream->PutChar('<'); break; }
	case ClassXMLComment:     { m_pStream->PutString(s_TagCommentStart);     break; }
	case ClassXMLNotation:    { m_pStream->PutString(s_TagNotationStart);    break; }
	case ClassXMLCData:       { m_pStream->PutString(s_TagCDataStart);       break; }
	}

	//Write name
	m_pStream->PutString(rNode.QName());

	//Write end of tag
	switch (rNode.ClassKey())
	{
	case ClassXMLElement:			
		{
			//Write all attributes
			CXMLAttribute* pAttribute = ((CXMLElement&)rNode).AttributeFirst();
			while (pAttribute)
			{
				m_pStream->PutChar(' ');

				WriteAttribute(*pAttribute);

				pAttribute = pAttribute->Next();
			}

			//If this is a leaf without a value use the reduced single tag syntax 
			if (rNode.IsLeaf() && !rNode.Value()) { m_pStream->PutString(s_TagElementSingleEnd); }
			else                                  { m_pStream->PutChar('>'); }
			break; 
		}
	case ClassXMLNotation:    { m_pStream->PutChar('>'); break; }
	}

	//Write data
	//if (rNode.Value()) { m_pStream->PutString(rNode.Value()->ToString()); }

	//Write end of tag for special single tag nodes
	switch (rNode.ClassKey())
	{
	case ClassXMLComment: { m_pStream->PutString(s_TagCommentEnd); return; }
	case ClassXMLCData:   { m_pStream->PutString(s_TagCDataEnd);   return; }
	}

	//Write all children
	CXMLNode* pChild = (CXMLNode*)rNode.ChildFirst();
	while (pChild)
	{
		//Write child on new line
		m_pStream->PutLine();

		WriteNode(*pChild, uiIndent + 1);

		pChild = (CXMLNode*)pChild->Next();
	}

	//Start new line after writing all children
	if (rNode.ChildFirst()) { m_pStream->PutLine(); }

	//Write closing tag
	if (rNode.ClassKey() == ClassXMLElement)			
	{ 
		if (!rNode.IsLeaf() || rNode.Value())
		{
			//Write tabs to create indentation
			if (rNode.ChildFirst()) { for (SCF::UINT i = 0; i < (uiIndent << 1); i++) { m_pStream->PutChar(' '); } }

			m_pStream->PutString(s_TagElementCloseStart);
			m_pStream->PutString(rNode.QName());
			m_pStream->PutChar('>');
		}
	}
}

void CXMLWriter::WriteAttribute(_IN CXMLAttribute& rAttribute)
{
	BETAONLY(bool bTracing = CObject::Tracing(); CObject::Tracing(FALSE));
	
	static CString s_SeparatorStart(STRING_CREATE_ARGS("='"));
	static CString s_SeparatorEnd  (STRING_CREATE_ARGS("'"));
	
	BETAONLY(CObject::Tracing(bTracing));

	m_pStream->PutString(rAttribute.Name());
	m_pStream->PutString(s_SeparatorStart);

	if (rAttribute.Value()) { m_pStream->PutString(rAttribute.Value()->ToString()); }

	m_pStream->PutString(s_SeparatorEnd);
} 