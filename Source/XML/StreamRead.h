#pragma once

#include "Document.h"
#include "Enumerator.h"
#include "Attribute.h"

namespace SCFXML
{
	//This class is intended to be used purely fore xml-serialization
	class XML_API IXMLStreamRead
	{
	public:
		//Both the the return value & the name are stored as strings which MUST NOT be deleted by the caller
		//This applies to all 3 methods

		//Reads the next named value from the xml file
		virtual CString* GetValue(_OUT _OPT CString** ppOutName = NULL) = 0;

		//Moves over to the next block
		virtual CString* GetBlock() = 0;

		//Reads the unnamed string value inside the block itself
		virtual CString* GetBlockValue() = 0;
	};

	class XML_API CXMLStreamRead : public SCFBase::CStream, public virtual IXMLStreamRead
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLStreamRead; }

	public:
		CXMLStreamRead(_IN _REF CXMLNode& rNode);
		virtual ~CXMLStreamRead();

	public:
		//Keeps a reference to the node returned by [rDocument.RootElement()]
		CXMLStreamRead(_IN _REF CXMLDocument& rDocument);

	public:
		//Interface: IStreamBase
		void Close() {}
		bool IsOpen() _GET      { return TRUE; }

	public:
		//Reads the next named value from the xml file
		CString* GetValue(_OUT _OPT CString** ppOutName = NULL);

		//Moves over to the next block
		CString* GetBlock();

		//Reads the unnamed string value inside the block itself
		CString* GetBlockValue();

 	public:
 		CXMLNode& Root() _GET { return (CXMLNode&)(m_pEnumerator->Source()); } 

	protected:
		CXMLEnumerator* m_pEnumerator;
		CXMLAttribute*  m_pAttribute;
	};
};