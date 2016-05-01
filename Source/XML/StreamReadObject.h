#pragma once

#include "StreamRead.h"

namespace SCFXML
{
	class XML_API CXMLObjectSerializable;

	class XML_API IXMLStreamReadObject
	{
	public:
		virtual void DelayDeserialization(_IN bool bDelay) _SET = 0;
		virtual bool DelayDeserialization()                _GET = 0;

	public:
		virtual bool Next(_OUT _OPT CString** ppOutID = NULL) = 0;

		virtual CXMLObjectSerializable* Current() = 0;
	};

	class XML_API CXMLStreamReadObject : public CXMLStreamRead, public virtual IXMLStreamReadObject
	{
	public:
		CXMLStreamReadObject(_IN _REF CXMLNode& rNode, _IN bool bDelayDeserialization = FALSE);
		virtual ~CXMLStreamReadObject();

	public:
		CXMLStreamReadObject(_IN _REF CXMLDocument& rDocument, _IN bool bDelayDeserialization = FALSE);

	public:
		void DelayDeserialization(_IN bool bDelay) _SET { m_bDelayDeserialization = bDelay; }
		bool DelayDeserialization()                _GET { return m_bDelayDeserialization; }

	public:
		bool Next(_OUT _OPT CString** ppOutID = NULL);

		CXMLObjectSerializable* Current();

	public:
		//These methods register objects as already read somewhere else (another document)
		//in order to prevent duplicate reading
		void ObjectsReadSubmit(_IN CDictionaryInt64&           rObjectIDs);
		void ObjectsReadSubmit(_IN CDictionaryString<CObject>& rObjects);

	protected:
		CXMLObjectSerializable* Next(_IN CXMLNode& rNode, _OUT _OPT CString** ppOutID);

	protected:
		#pragma warning(disable:4251)
		CDictionaryString<CObject> m_ObjectsExternal;
		CDictionaryString<CObject> m_Objects;
		#pragma warning(default:4251)

	protected:
		CXMLObjectSerializable* m_pCurrent;
		bool m_bCurrentDeserialized;

		bool m_bDelayDeserialization;
	};
};