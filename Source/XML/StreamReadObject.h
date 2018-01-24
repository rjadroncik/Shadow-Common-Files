#pragma once

#include "StreamRead.h"

namespace SCFXML
{
	template class __declspec(dllexport) SCFBase::CDictionaryString<SCFBase::CObject>;

	class XML_API CXMLObjectSerializable;

	class XML_API IXMLStreamReadObject
	{
	public:
		virtual void DelayDeserialization(_IN bool bDelay) _SET = 0;
		virtual bool DelayDeserialization()                _GET = 0;

	public:
		virtual bool Next(_OUT _OPT CString** ppOutID = nullptr) = 0;

		virtual CXMLObjectSerializable* Current() = 0;
	};

	class XML_API CXMLStreamReadObject : public CXMLStreamRead, public virtual IXMLStreamReadObject
	{
	public:
		CXMLStreamReadObject(_IN _REF CXMLNode& rNode, _IN bool bDelayDeserialization = false);
		virtual ~CXMLStreamReadObject();

	public:
		CXMLStreamReadObject(_IN _REF CXMLDocument& rDocument, _IN bool bDelayDeserialization = false);

	public:
		void DelayDeserialization(_IN bool bDelay) _SET { m_bDelayDeserialization = bDelay; }
		bool DelayDeserialization()                _GET { return m_bDelayDeserialization; }

	public:
		bool Next(_OUT _OPT CString** ppOutID = nullptr);

		CXMLObjectSerializable* Current();

	public:
		//These methods register objects as already read somewhere else (another document)
		//in order to prevent duplicate reading
		void ObjectsReadSubmit(_IN CDictionaryInt64<CString>& rObjectIDs);
		void ObjectsReadSubmit(_IN CDictionaryString<CObject>& rObjects);

	protected:
		CXMLObjectSerializable* Next(_IN CXMLNode& rNode, _OUT _OPT CString** ppOutID);

	protected:
		CDictionaryString<CObject> m_ObjectsExternal;
		CDictionaryString<CObject> m_Objects;

	protected:
		CXMLObjectSerializable* m_pCurrent;
		bool m_bCurrentDeserialized;

		bool m_bDelayDeserialization;
	};
};