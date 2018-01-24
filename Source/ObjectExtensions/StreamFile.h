#pragma once
#include "StreamBuffered.h"
#include "File.h"

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define FILE_HANDLE void*
#define FILE_HANDLE_NULL nullptr
#define FILE_HANDLE_INVALID INVALID_HANDLE_VALUE

#else

#define FILE_HANDLE int
#define FILE_HANDLE_NULL 0
#define FILE_HANDLE_INVALID -1

#endif // WIN32

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamFile : public CStreamBuffered
	{
	public:
		CStreamFile(_INOUT FILE_HANDLE hFile);
		CStreamFile();
		virtual ~CStreamFile();

	public:
		FILE_HANDLE FileHandle() _GET { return m_hFile; }

	protected:
		UINT BufferFill();
		UINT BufferCommit();

	protected:
		bool FileClose();
		bool FileIsOpen() _GET;

		bool FileOpenForReading(_IN CFile& rFile);
		bool FileOpenForWriting(_IN CFile& rFile, _IN bool bErase);

		bool FileSize(_OUT UINT64* ui64pFileSize);
		void FileCommit();

		//Moves file pointer [iCount] bytes forward or back during a read operation
		bool FilePointerMove(_IN int iBytes);
		bool FilePointerSet (_IN int iBytes);
		bool FilePointerSetToEnd();

	private:
		FILE_HANDLE m_hFile;
		bool m_bOwnsHandle;
	};
};
