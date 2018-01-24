#pragma once
#include "StreamMemory.h"
#include "StreamWrite.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamMemoryWrite : public CStreamMemory, public virtual IStreamWrite
	{
	public:
		CString ToString() _GET { return STRING("{StreamMemoryWrite}"); }

	public:
		CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN UINT uiOffsetStart);
		CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN bool bAppend = false);
		virtual ~CStreamMemoryWrite();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return true; }

	public:
		void PutByte (_IN BYTE  ucValue) ;
		void PutWord (_IN WORD  usValue) ;
		void PutInt  (_IN int   iValue)  ;
		void PutInt64(_IN INT64 i64Value);

		void PutFloat(_IN float fValue)  ;
		void PutBool (_IN bool  bValue)  ;
		void PutChar (_IN TCHAR cValue)  ;

		void PutBytes(_IN void* vpBytes, _IN UINT uiCount);

	public:
		UINT64 BytesWritten() _GET;
		bool        Appending()    _GET;

	protected:
		UINT m_uiOffsetStart;
		bool m_bAppending;
	};
};