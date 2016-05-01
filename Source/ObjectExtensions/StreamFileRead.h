#pragma once
#include "StreamFile.h"
#include "StreamRead.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamFileRead : public CStreamFile, public virtual IStreamRead
	{
	public:
		CString ToString() _GET { return STRING("{StreamFileRead}"); }

	public:
		//[ErrorFileNotFound]   if file is missing
		//[ErrorAccessDenied]   if file access denied by OS
		//[ErrorFileFailedOpen] if file opening failed for unknown(unhandled) reason
		CStreamFileRead(_INOUT FILE_HANDLE hFile, _IN UINT64 ui64OffsetStart, _IN UINT64 ui64DataLength);
		CStreamFileRead(_INOUT FILE_HANDLE hFile);

		CStreamFileRead(_IN CFile& rFile, _IN UINT64 ui64OffsetStart, _IN UINT64 ui64DataLength);
		CStreamFileRead(_IN CFile& rFile);
		virtual ~CStreamFileRead();

	public:
		void Close()       { if (this->FileIsOpen()) { this->FileClose(); } }
		bool IsOpen() _GET { return this->FileIsOpen(); }

	public:
		//************************* IStreamRead ********************************
		UINT64 BytesLeft();
		UINT64 BytesRead();

	public:
		BYTE  GetByte();
		WORD  GetWord();
		int        GetInt();
		INT64 GetInt64();

		float      GetFloat();
		bool       GetBool();
		TCHAR GetChar();

		void GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount);

	public:
		void SkipBytes (_IN UINT uiCount);
		void UnGetBytes(_IN UINT uiCount);

	protected:
		UINT64 m_ui64BytesLeft;
	};
};
