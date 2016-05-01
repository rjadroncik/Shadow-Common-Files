#pragma once

#include "Document.h"

namespace SCFXML
{
	class XML_API CXMLDocumentFile : public CXMLDocument
	{
	public:
		CXMLDocumentFile();
		CXMLDocumentFile(_IN CString& rFilePath);
		CXMLDocumentFile(_IN _REF CFile& rFile, _IN bool bOwnFile = TRUE);

		virtual ~CXMLDocumentFile();

	public:
		const CFile& File() _GET { return *m_pFile; }

	public:
		bool Write(_IN ENUM eEncoding = EncodingUCS2);

	public:
		bool Read (_IN _REF CFile& rFile, _IN bool bOwnFile = TRUE);
		bool Write(_IN _REF CFile& rFile, _IN ENUM eEncoding = EncodingUCS2);

	public:
		bool ResolveIncludes();

	public:
		//Prepares a basic XML declaration for a given encoding
		bool PrepareXMLDeclaration(_IN ENUM eEncoding);

	protected:
		const CFile* m_pFile;
		bool m_bOwnFile;
	};
};