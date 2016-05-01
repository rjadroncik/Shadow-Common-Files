#pragma once
#include "StreamFile.h"
#include "StreamWrite.h"
#include "String.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamFileWrite : public CStreamFile, public virtual IStreamWrite
	{
	public:
		CString ToString() _GET { return STRING("{StreamFileWrite}"); }

	public:
		CStreamFileWrite(_INOUT FILE_HANDLE hFile, _IN UINT64 ui64OffsetStart);
		CStreamFileWrite(_INOUT FILE_HANDLE hFile, _IN bool bAppend = false);

		CStreamFileWrite(_IN CFile& rFile, _IN UINT64 ui64OffsetStart);
		CStreamFileWrite(_IN CFile& rFile, _IN bool bAppend = false);
		virtual ~CStreamFileWrite();

	public:
		void Commit();
		void Close();

		bool IsOpen() _GET { return this->FileIsOpen(); }

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
		UINT64 m_ui64BytesWritten;
		bool m_bAppending;
	};
};
