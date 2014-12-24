#pragma once

#include "StreamWrite.h"
#include "DictionaryInt64.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API IStreamWriteObject
	{
	public:
		//Writes a serialized representation of an object
		virtual bool Next(_IN CObjectSerializable& rObject) = 0;
	};

	class OBJECT_EXTENSIONS_API CStreamWriteObject : public CStream, public virtual IStreamWriteObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamWriteObject; }
		CString   ToString() _GET { return STRING("{StreamWriteObject}"); }

	public:
		CStreamWriteObject(_INOUT IStreamWrite& rStreamWrite);
		virtual ~CStreamWriteObject();

	public:
		bool Next(_IN CObjectSerializable& rObject);

	public:
		virtual void Close()       { m_pStream->Close(); }
		virtual bool IsOpen() _GET { return m_pStream->IsOpen(); }

	protected:
		CDictionaryInt64 m_ObjectsStored;
		IStreamWrite* m_pStream;
	};
};