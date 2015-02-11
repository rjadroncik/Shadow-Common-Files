#pragma once
#include "StreamFile.h"
#include "StreamRead.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamFileRead : public CStreamFile, public virtual IStreamRead
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassStreamFileRead; }
		CString   ToString() _GET { return STRING("{StreamFileRead}"); }

	public:
		//[ErrorFileNotFound]   if file is missing
		//[ErrorAccessDenied]   if file access denied by OS
		//[ErrorFileFailedOpen] if file opening failed for unknown(unhandled) reason
		CStreamFileRead(_INOUT FILE_HANDLE hFile, _IN SCF::UINT64 ui64OffsetStart, _IN SCF::UINT64 ui64DataLength);
		CStreamFileRead(_INOUT FILE_HANDLE hFile);

		CStreamFileRead(_IN CFile& rFile, _IN SCF::UINT64 ui64OffsetStart, _IN SCF::UINT64 ui64DataLength);
		CStreamFileRead(_IN CFile& rFile);
		virtual ~CStreamFileRead();

	public:
		void Close()       { if (this->FileIsOpen()) { this->FileClose(); } }
		bool IsOpen() _GET { return this->FileIsOpen(); }

	public:
		//************************* IStreamRead ********************************
		SCF::UINT64 BytesLeft();
		SCF::UINT64 BytesRead();

	public:
		SCF::BYTE  GetByte();
		SCF::WORD  GetWord();
		int        GetInt();
		SCF::INT64 GetInt64();

		float      GetFloat();
		bool       GetBool();
		SCF::TCHAR GetChar();

		void GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount);

	public:
		void SkipBytes (_IN SCF::UINT uiCount);
		void UnGetBytes(_IN SCF::UINT uiCount);

	protected:
		SCF::UINT64 m_ui64BytesLeft;
	};
};
