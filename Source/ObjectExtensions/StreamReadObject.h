#pragma once

#include "StreamRead.h"
#include "DictionaryInt64.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API IStreamReadObject
	{
	public:
		//Reads and de-serializes the next stored object, returns FALSE on end of file
		virtual bool Next() = 0;

		virtual CObjectSerializable* Current() _GET = 0;
	};

	class OBJECT_EXTENSIONS_API CStreamReadObject : public CStream, public virtual IStreamReadObject 
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamReadObject; }
		CString   ToString() _GET { return STRING("{StreamReadObject}"); }

	public:
		CStreamReadObject(_INOUT IStreamRead& rStreamRead);
		virtual ~CStreamReadObject();

	public:
		bool Next();

		CObjectSerializable* Current() _GET { return (CObjectSerializable*)m_pCurrent; }
	
	public:
		virtual void Close()       { m_pStream->Close(); m_ObjectsRead.AllRemove(); }
		virtual bool IsOpen() _GET { return m_pStream->IsOpen(); }
	
	public:
		virtual void CloseAndDeleteAll() { m_pStream->Close(); m_ObjectsRead.AllDelete(); }

	protected:
		CDictionaryInt64 m_ObjectsRead;
		IStreamRead* m_pStream;

		CObjectSerializable* m_pCurrent;
 	};
};