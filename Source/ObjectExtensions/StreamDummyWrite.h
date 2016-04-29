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
		void PutByte (_IN SCF::BYTE  ucValue);
		void PutWord (_IN SCF::WORD  usValue);
		void PutInt  (_IN int        iValue);
		void PutInt64(_IN SCF::INT64 i64Value);

		void PutFloat(_IN float      fValue);
		void PutBool (_IN bool       bValue);
		void PutChar (_IN SCF::TCHAR cValue);

		void PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount);

	public:
		SCF::UINT64 BytesWritten() _GET;
		bool        Appending()    _GET;

	protected:
		SCF::UINT64 m_uiBytesWritten;
	};
};