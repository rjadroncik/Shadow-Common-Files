#pragma once
#include "StreamWrite.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamDummyWrite : public CStream, public virtual IStreamWrite
	{
	public:
		CString ToString() _GET { return STRING("{StreamDummyWrite}"); }

	public:
		CStreamDummyWrite();
		virtual ~CStreamDummyWrite();

	public:
		void Close() {}
		bool IsOpen() _GET { return TRUE; }

	public:
		void PutByte (_IN BYTE  ucValue);
		void PutWord (_IN WORD  usValue);
		void PutInt  (_IN int        iValue);
		void PutInt64(_IN INT64 i64Value);

		void PutFloat(_IN float      fValue);
		void PutBool (_IN bool       bValue);
		void PutChar (_IN TCHAR cValue);

		void PutBytes(_IN void* vpBytes, _IN UINT uiCount);

	public:
		UINT64 BytesWritten() _GET;
		bool        Appending()    _GET;

	protected:
		UINT64 m_uiBytesWritten;
	};
};