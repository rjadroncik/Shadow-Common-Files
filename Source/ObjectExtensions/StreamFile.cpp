#include "StreamFile.h"

using namespace SCFBase;

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

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
	m_bOwnsHandle = FALSE;
}

CStreamFile::CStreamFile() : CStreamBuffered(4096)
{
    m_hFile = FILE_HANDLE_NULL;
	m_bOwnsHandle = TRUE;
}

CStreamFile::~CStreamFile()
{
	if (m_bOwnsHandle && this->FileIsOpen()) { this->FileClose(); }
}

SCF::UINT CStreamFile::BufferCommit()
{
	if (!this->BufferUsed()) { return 0; }

	_ASSERTE(this->FileIsOpen());

	SCF::UINT uiBytesWritten = 0;

	#ifdef WIN32
	WriteFile(m_hFile, this->Buffer(), this->BufferUsed(), (LPDWORD)&uiBytesWritten, NULL);
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

SCF::UINT CStreamFile::BufferFill()
{
	_ASSERTE(this->FileIsOpen());

	SCF::UINT uiBytesRead = 0;

	#ifdef WIN32
	ReadFile(m_hFile, this->Buffer(), this->BufferSize(), (LPDWORD)&uiBytesRead, NULL);
    #else
    uiBytesRead = read(m_hFile, this->Buffer(), this->BufferSize());
    #endif

	return uiBytesRead;
}

bool CStreamFile::FilePointerMove(_IN int iBytes)
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, iBytes - this->BufferUsed(), NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, iBytes - this->BufferUsed(), SEEK_CUR) == -1)
	#endif
    {
		SCFError(ErrorFileInvalidFilePosition);
		return FALSE;
	}

	this->BufferUsed(0);
	return TRUE;
}

bool CStreamFile::FilePointerSet(_IN int iBytes)
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, iBytes, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, iBytes, SEEK_SET) == -1)
	#endif
	{
		SCFError(ErrorFileInvalidFilePosition);
		return FALSE;
	}

	this->BufferUsed(0);
	return TRUE;
}

bool CStreamFile::FilePointerSetToEnd()
{
    #ifdef WIN32
	if (SetFilePointer(m_hFile, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
	#else
    if (lseek(m_hFile, 0, SEEK_END) == -1)
	#endif
	{
		SCFError(ErrorFileInvalidFilePosition);
		return FALSE;
	}

	this->BufferUsed(0);
	return TRUE;
}

bool CStreamFile::FileClose()
{
	if (!m_bOwnsHandle) { return TRUE; }

	_ASSERTE(this->FileIsOpen());

    #ifdef WIN32
	if (SUCCEEDED(CloseHandle(m_hFile)))
	#else
    if (close(m_hFile) == 0)
	#endif
	{
		m_hFile = FILE_HANDLE_NULL;
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedClose);
		m_hFile = FILE_HANDLE_NULL;
		return FALSE;
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
	m_hFile = (FILE_HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);
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

		return FALSE;
	}

	return TRUE;
}

bool CStreamFile::FileOpenForWriting(_IN CFile& rFile, _IN bool bErase)
{
	_ASSERTE(m_bOwnsHandle);

    #ifdef WIN32
	m_hFile = (FILE_HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_WRITE, FILE_SHARE_READ, NULL, bErase ? CREATE_ALWAYS : OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
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

		return FALSE;
	}

	return TRUE;
}

bool CStreamFile::FileSize(_OUT SCF::UINT64* ui64pFileSize)
{
    #ifdef WIN32

	*((DWORD*)ui64pFileSize) = GetFileSize(m_hFile, ((DWORD*)ui64pFileSize) + 1);

	return *((DWORD*)ui64pFileSize) != INVALID_FILE_SIZE;

	#else

	struct stat statbuf;

	if (fstat(m_hFile, &statbuf) != -1)
	{
        *ui64pFileSize = statbuf.st_size;
        return TRUE;
    }

    return FALSE;

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


