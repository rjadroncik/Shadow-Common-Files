#pragma once

#include "StreamWrite.h"

namespace SCFXML
{
	class XML_API CXMLObjectSerializable;

	class XML_API IXMLStreamWriteObject
	{
	public:
		//Accepts NULL pointers, in which case it writes an object stub
		virtual bool Next(_IN CXMLObjectSerializable* pObject) = 0;
	};

	class XML_API CXMLStreamWriteObject : public CXMLStreamWrite, public virtual IXMLStreamWriteObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassXMLStreamWriteObject; }

	public:
		CXMLStreamWriteObject(_OUT _REF CXMLElement& rElement);
		virtual ~CXMLStreamWriteObject();

	public:
		CXMLStreamWriteObject(_OUT _REF CXMLDocument& rDocument);
		CXMLStreamWriteObject(_OUT _REF CXMLDocument& rDocument, _IN CString& rRootName, _IN CString& rRootPrefix, _IN CString& rRootNamespace);

	public:
		bool Next(_IN CXMLObjectSerializable* pObject);

	public:
		//These methods register objects as already written somewhere else (another document)
		//in order to prevent duplicate writing
		void ObjectsWrittenSubmit(_IN CDictionaryInt64&  rObjectIDs);
		void ObjectsWrittenSubmit(_IN CDictionaryString<CObject>& rObjects);

	protected:
		CDictionaryInt64 m_ObjectIDsExternal;
		CDictionaryInt64 m_ObjectIDs;
	};
};