#pragma once
#include "StreamString.h"
#include "StreamWrite.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamStringWrite : public CStreamString, public virtual IStreamWrite
	{
	public:
		CString ToString() _GET { return STRING("{StreamStringWrite}"); }

	public:
		CStreamStringWrite(_INOUT CString& rString, _IN UINT uiOffsetStart);
		CStreamStringWrite(_INOUT CString& rString, _IN bool bAppend = FALSE);
		virtual ~CStreamStringWrite();

	public:
		virtual void Close() {}
		virtual bool IsOpen() _GET { return TRUE; }

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