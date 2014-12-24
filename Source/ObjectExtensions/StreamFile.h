#pragma once
#include "StreamBuffered.h"
#include "File.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamFile : public CStreamBuffered
	{
	public:
		CStreamFile(_INOUT void* hFile);
		CStreamFile();
		virtual ~CStreamFile();

	public:
		void* FileHandle() _GET { return m_hFile; }

	protected:
		SCF::UINT BufferFill();
		SCF::UINT BufferCommit();

	protected:
		bool FileClose();
		bool FileIsOpen() _GET;

		bool FileOpenForReading(_IN CFile& rFile);
		bool FileOpenForWriting(_IN CFile& rFile, _IN bool bErase);

		void FileSize(_OUT SCF::UINT64* ui64pFileSize);
		void FileCommit();

		//Moves file pointer [iCount] bytes forward or back during a read operation
		bool FilePointerMove(_IN int iBytes);
		bool FilePointerSet (_IN int iBytes);
		bool FilePointerSetToEnd();

	private:
		void* m_hFile;
		bool m_bOwnsHandle;
	};
};