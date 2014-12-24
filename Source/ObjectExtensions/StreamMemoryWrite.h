#pragma once
#include "StreamMemory.h"
#include "StreamWrite.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamMemoryWrite : public CStreamMemory, public virtual IStreamWrite
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamMemoryWrite; }
		CString   ToString() _GET { return STRING("{StreamMemoryWrite}"); }

	public:
		CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN SCF::UINT uiOffsetStart);
		CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN bool bAppend = FALSE);
		virtual ~CStreamMemoryWrite();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return TRUE; }

	public:
		void PutByte (_IN SCF::BYTE  ucValue) ;
		void PutWord (_IN SCF::WORD  usValue) ;
		void PutInt  (_IN int   iValue)  ;
		void PutInt64(_IN SCF::INT64 i64Value);

		void PutFloat(_IN float fValue)  ;
		void PutBool (_IN bool  bValue)  ;
		void PutChar (_IN SCF::TCHAR cValue)  ;

		void PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount);

	public:
		SCF::UINT64 BytesWritten() _GET;
		bool        Appending()    _GET;

	protected:
		SCF::UINT m_uiOffsetStart;
		bool m_bAppending;
	};
};