#pragma once

#include "Document.h"

namespace SCFXML
{
	//This class is intended to be used purely fore xml-serialization
	class XML_API IXMLStreamWrite
	{
	public:
		//Stores a named value in the xml file
		virtual void PutValue(_IN CString& rName, _IN _REF CValue& rValue) = 0;

	public:
		//Creates a new sub-node of the current node
		virtual void BlockStart(_IN CString& rName) = 0;

		//Puts an unnamed value directly into the block
		virtual void BlockValue(_IN CValue& rValue) = 0;

		//Ends current block & makes its parent the new current block
		virtual void BlockEnd() = 0;
	};

	class XML_API CXMLStreamWrite : public SCFBase::CStream, public virtual IXMLStreamWrite
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLStreamWrite; }

	public:
		CXMLStreamWrite(_OUT _REF CXMLElement& rElement);
		virtual ~CXMLStreamWrite();

	public:
		CXMLStreamWrite(_OUT _REF CXMLDocument& rDocument);

	public:
		//Interface: IStreamBase
		void Close() {}
		bool IsOpen() _GET      { return TRUE; }

	public:
		//Stores a named value in the current block of the xml file
		void PutValue(_IN CString& rName, _IN _REF CValue& rValue);

	public:
		//Creates a new sub-node of the current node
		void BlockStart(_IN CString& rName);

		//Puts an unnamed value directly into the block
		void BlockValue(_IN CValue& rValue);

		//Ends current block & makes its parent the new current block
		void BlockEnd();
		
	public:
		CXMLNode& Root() _GET { return *m_pRoot; } 

	protected:
		CXMLElement* m_pRoot;
		CXMLElement* m_pElement;
	};
};