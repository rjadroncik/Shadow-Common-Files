#pragma once

#include "Node.h"
#include "Attribute.h"
#include "Declaration.h"

namespace SCFXML
{
	class XML_API CXMLWriter : public SCFBase::CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLWriter; }

	public:
		CXMLWriter(_INOUT IStreamWriteText& rStream, _INOUT CDictionaryString<CChar>* pEntities = NULL);
		~CXMLWriter();

	public:
		void WriteDeclaration(_IN CXMLDeclaration& rDeclaration);

		//Converts a given XML DOM tree into its XML string representation, using an optional entity dictionary
		//If no dictionary is supplied, the default dictionary used containing the default XML entities: {&, <, >, ', "}
		void WriteNode(_IN CXMLNode& rNode, _IN SCF::UINT uiIndent = 0);
		
		//Converts a single attribute into its XML representation
		void WriteAttribute(_IN CXMLAttribute& rAttribute);

	public:
		//Same as [WriteNode(..)], but doesn't emit a tag for the supplied node, just for its contents 
		void WriteRoot(_IN CXMLNode& rRoot);

	private:
		//Root of the printed
		IStreamWriteText* m_pStream;
		CDictionaryString<CChar>* m_pEntities;
	};
};