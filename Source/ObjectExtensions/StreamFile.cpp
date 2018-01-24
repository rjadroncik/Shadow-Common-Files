#include "StreamFile.h"

using namespace SCFBase;

#ifdef WIN32
#else

#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define GetLastError() errno

#define ERROR_FILE_NOT_FOUND ENOENT
#define ERROR_ACCESS_DENIED  EACCES

#endif

CStreamFile::CStreamFile(_INOUT FILE_HANDLE hFile) : CStreamBuffered(4096)
{
	m_hFile = hFile;
	m_bOwnsHandle = false;
}

CStreamFile::CStreamFile() : CStreamBuffered(4096)
{
    m_hFile = FILE_HANDLE_NULL;
	m_bOwnsHandle = true;
}

CStreamFile::~CStreamFile()
{
	if (m_bOwnsHandle && this->FileIsOpen()) { this->FileClose(); }
}

UINT CStreamFile::BufferCommit()
{
	if (!this->BufferUsed()) { return 0; }

	_ASSERTE(this->FileIsOpen());

	UINT uiBytesWritten = 0;

	#ifdef WIN32
	WriteFile(m_hFile, this->Buffer(), this->BufferUsed(), (LPDWORD)&uiBytesWritten, nullptr);
    #else
    uiBytesWritten = write(m_hFile, this->Buffer(), this->BufferUsed());
    #endif

	if (this->BufferUsed() != uiBytesWritten)
	{
		SCFError(ErrorFileFailedWrite);
		this->BufferUsed(0);
		return 0;
	}

	this->BufferUsed(0);

	return uiBytesWritten;
}

UINT CStreamFile::BufferFill()
{
	_ASSERTE(this->FileIsOpen());

	UINT uiBytesRead = 0;

	#ifdef WIN32
	ReadFile(m_hFile, this->Buffer(), this->BufferSize(), (LPDWORD)&uiBytesRead, nullptr);
    #else
    uiBytesRead = read(m_hFile, this->Buffer(), this->BufferSize());
    #endif

	return uiBytesRead;
}

bool CStreamFile::FilePointerMove(_IN int iBytes)
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, iBytes - this->BufferUsed(), nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, iBytes - this->BufferUsed(), SEEK_CUR) == -1)
	#endif
    {
		SCFError(ErrorFileInvalidFilePosition);
		return false;
	}

	this->BufferUsed(0);
	return true;
}

bool CStreamFile::FilePointerSet(_IN int iBytes)
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, iBytes, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, iBytes, SEEK_SET) == -1)
	#endif
	{
		SCFError(ErrorFileInvalidFilePosition);
		return false;
	}

	this->BufferUsed(0);
	return true;
}

bool CStreamFile::FilePointerSetToEnd()
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, 0, nullptr, FILE_END) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, 0, SEEK_END) == -1)
	#endif
	{
		SCFError(ErrorFileInvalidFilePosition);
		return false;
	}

	this->BufferUsed(0);
	return true;
}

bool CStreamFile::FileClose()
{
	if (!m_bOwnsHandle) { return true; }

	_ASSERTE(this->FileIsOpen());

    #ifdef WIN32
	if (SUCCEEDED(CloseHandle(m_hFile)))
	#else
    if (close(m_hFile) == 0)
	#endif
	{
		m_hFile = FILE_HANDLE_NULL;
		return true;
	}
	else
	{
		SCFError(ErrorFileFailedClose);
		m_hFile = FILE_HANDLE_NULL;
		return false;
	}
}

bool CStreamFile::FileIsOpen() _GET
{
	return ((m_hFile != FILE_HANDLE_NULL) && (m_hFile != FILE_HANDLE_INVALID));
}

bool CStreamFile::FileOpenForReading(_IN CFile& rFile)
{
	_ASSERTE(m_bOwnsHandle);

    #ifdef WIN32
	m_hFile = (FILE_HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
	#else

	char szBuffer[MAX_PATH];
	rFile.PathFull().ToASCII(szBuffer);

 	m_hFile = open(szBuffer, O_RDONLY);
	#endif

	//_ASSERTE(m_hFile);
	//_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);

	if ((m_hFile == FILE_HANDLE_NULL) || (m_hFile == FILE_HANDLE_INVALID))
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND: { SCFError(ErrorFileNotFound); break; }
		case ERROR_ACCESS_DENIED:  { SCFError(ErrorAccessDenied); break; }
		default:                   { SCFError(ErrorFileFailedOpen); break; }
		}

		return false;
	}

	return true;
}

bool CStreamFile::FileOpenForWriting(_IN CFile& rFile, _IN bool bErase)
{
	_ASSERTE(m_bOwnsHandle);

    #ifdef WIN32
	m_hFile = (FILE_HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_WRITE, FILE_SHARE_READ, nullptr, bErase ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, nullptr);
    #else

	char szBuffer[MAX_PATH];
	rFile.PathFull().ToASCII(szBuffer);

	m_hFile = open(szBuffer, bErase ? O_WRONLY | O_TRUNC : O_WRONLY);
    #endif

	//_ASSERTE(m_hFile);
	//_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);

	if ((m_hFile == FILE_HANDLE_NULL) || (m_hFile == FILE_HANDLE_INVALID))
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND: { SCFError(ErrorFileNotFound); break; }
		case ERROR_ACCESS_DENIED:  { SCFError(ErrorAccessDenied); break; }
		default:                   { SCFError(ErrorFileFailedOpen); break; }
		}

		return false;
	}

	return true;
}

bool CStreamFile::FileSize(_OUT UINT64* ui64pFileSize)
{
    #ifdef WIN32

	*((DWORD*)ui64pFileSize) = GetFileSize(m_hFile, ((DWORD*)ui64pFileSize) + 1);

	return *((DWORD*)ui64pFileSize) != INVALID_FILE_SIZE;

	#else

	struct stat statbuf;

	if (fstat(m_hFile, &statbuf) != -1)
	{
        *ui64pFileSize = statbuf.st_size;
        return true;
    }

    return false;

	#endif
}

void CStreamFile::FileCommit()
{
    #ifdef WIN32
	FlushFileBuffers(m_hFile);
	#else
	fsync(m_hFile);
    #endif
}


