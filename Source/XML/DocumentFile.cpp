#include "DocumentFile.h"
#include "Declaration.h"

using namespace SCFXML;

CXMLDocumentFile::CXMLDocumentFile()
{
	m_pFile = NULL;
	m_bOwnFile = TRUE;
}

CXMLDocumentFile::CXMLDocumentFile(_IN CString& rFilePath)
{
	m_pFile = NULL;
	m_bOwnFile = FALSE;

	this->Read(*(new CFile(rFilePath)), TRUE);
}

CXMLDocumentFile::CXMLDocumentFile(_IN _REF CFile& rFile, _IN bool bOwnFile)
{
	m_pFile = NULL;
	m_bOwnFile = FALSE;

	this->Read(rFile, bOwnFile);
}

CXMLDocumentFile::~CXMLDocumentFile()
{
	if (m_pFile) { RELEASE(*(m_pFile)); if (m_bOwnFile) { delete m_pFile; } }
}

bool CXMLDocumentFile::Read(_IN _REF CFile& rFile, _IN bool bOwnFile)
{
	if (m_pFile) { RELEASE(*(m_pFile)); if (m_bOwnFile) { delete m_pFile; } }

	m_pFile = &rFile;
	m_bOwnFile = bOwnFile;

	ADDREF(*(m_pFile));

	if (m_pFile->Exists())
	{
		CStreamFileRead        StreamFileRead(rFile);
		CStreamReadTextGeneric StreamReadText(StreamFileRead);

		return CXMLDocument::Read(StreamReadText);
	}
	
	return FALSE;
}

bool CXMLDocumentFile::Write(_IN _REF CFile& rFile, _IN ENUM eEncoding)
{
	if (m_pFile) { RELEASE(*(m_pFile)); if (m_bOwnFile) { delete m_pFile; } }
	m_pFile = &rFile;
	
	ADDREF(*(m_pFile));

	return Write(eEncoding);
}

bool CXMLDocumentFile::Write(_IN ENUM eEncoding)
{
	CStreamFileWrite        StreamFileWrite(*m_pFile);
	CStreamWriteTextGeneric StreamWriteText(StreamFileWrite, eEncoding);

	PrepareXMLDeclaration(eEncoding);

	return CXMLDocument::Write(StreamWriteText);
}

bool CXMLDocumentFile::PrepareXMLDeclaration(_IN ENUM eEncoding)
{
	switch (eEncoding)
	{
	case EncodingUCS2: { m_Declaration.Encoding(STRING("utf-16")); break; }
	case EncodingUTF8:
	default:           { m_Declaration.Encoding(STRING("utf-8")); break; }
	}

	return TRUE;
}

bool CXMLDocumentFile::ResolveIncludes()
{
	CXMLElement* pInclude = NULL;

	while ((pInclude = (CXMLElement*)(this->DescendantsNamed(STRING("include")))) != 0)
	{
		CXMLAttribute* pFile = pInclude->AttributeNamed(STRING("file"));
		if (!pFile) { return FALSE; }

		CFile File(m_pFile->Path() + STRING("\\") + pFile->ValueString());

		if (File.Exists())
		{
			CStreamFileRead        ReadStream(File);
			CStreamReadTextGeneric ReadStreamText(ReadStream);

			CXMLDocument* pDocument = new CXMLDocument(ReadStreamText);

			if (pDocument)
			{
				//This replaces the include node with the root element node from the resolved document (severs the ties to the XML document object)
				pInclude->ReplaceWith(*pDocument->RootElementRetrieve());

				delete pInclude;
				delete pDocument;
			}
			else { return FALSE; }
		}
		else { return FALSE; }
	}

	return TRUE;
}