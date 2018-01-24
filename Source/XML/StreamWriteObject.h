#pragma once

#include "StreamWrite.h"

namespace SCFXML
{
	class XML_API CXMLObjectSerializable;

	template class __declspec(dllexport) SCFBase::CDictionaryInt64<SCFBase::CString>;

	class XML_API IXMLStreamWriteObject
	{
	public:
		//Accepts nullptr pointers, in which case it writes an object stub
		virtual bool Next(_IN CXMLObjectSerializable* pObject) = 0;
	};

	class XML_API CXMLStreamWriteObject : public CXMLStreamWrite, public virtual IXMLStreamWriteObject
	{
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
		void ObjectsWrittenSubmit(_IN CDictionaryInt64<CString>&  rObjectIDs);
		void ObjectsWrittenSubmit(_IN CDictionaryString<CObject>& rObjects);

	protected:
		CDictionaryInt64<CString> m_ObjectIDsExternal;
		CDictionaryInt64<CString> m_ObjectIDs;
	};
};