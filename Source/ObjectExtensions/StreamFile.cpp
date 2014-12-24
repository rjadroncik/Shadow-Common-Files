#include "StreamFile.h"

using namespace SCFBase;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

CStreamFile::CStreamFile(_INOUT void* hFile) : CStreamBuffered(4096)
{
	m_hFile = hFile;
	m_bOwnsHandle = FALSE;
}

CStreamFile::CStreamFile() : CStreamBuffered(4096)
{
	m_hFile = NULL;
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
	WriteFile(m_hFile, this->Buffer(), this->BufferUsed(), (LPDWORD)&uiBytesWritten, NULL);

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
	ReadFile(m_hFile, this->Buffer(), this->BufferSize(), (LPDWORD)&uiBytesRead, NULL);

	return uiBytesRead;
}

bool CStreamFile::FilePointerMove(_IN int iBytes)
{
	if (SetFilePointer(m_hFile, iBytes - this->BufferUsed(), NULL, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
	{
		SCFError(ErrorFileInvalidFilePosition);
		return FALSE;
	}

	this->BufferUsed(0);
	return TRUE;
}

bool CStreamFile::FilePointerSet(_IN int iBytes)
{
	if (SetFilePointer(m_hFile, iBytes, NULL, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		SCFError(ErrorFileInvalidFilePosition);
		return FALSE;
	}

	this->BufferUsed(0);
	return TRUE;
}

bool CStreamFile::FilePointerSetToEnd()
{
	if (SetFilePointer(m_hFile, 0, NULL, FILE_END) == INVALID_SET_FILE_POINTER)
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

	if (SUCCEEDED(CloseHandle(m_hFile)))
	{
		m_hFile = NULL;
		return TRUE;
	}
	else 
	{
		SCFError(ErrorFileFailedClose);
		m_hFile = NULL;
		return FALSE; 
	}
}

bool CStreamFile::FileIsOpen() _GET
{
	return ((m_hFile != 0) && (m_hFile != INVALID_HANDLE_VALUE));
}

bool CStreamFile::FileOpenForReading(_IN CFile& rFile)
{
	_ASSERTE(m_bOwnsHandle);

	m_hFile = (HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	//_ASSERTE(m_hFile);
	//_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);

	if (!m_hFile || (m_hFile == INVALID_HANDLE_VALUE))
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND: { SCFError(ErrorFileNotFound); }
		case ERROR_ACCESS_DENIED:  { SCFError(ErrorAccessDenied); }
		default:                   { SCFError(ErrorFileFailedOpen); }
		}

		return FALSE;
	}

	return TRUE;
}

bool CStreamFile::FileOpenForWriting(_IN CFile& rFile, _IN bool bErase)
{
	_ASSERTE(m_bOwnsHandle);

	if (bErase)
	{
		m_hFile = (HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_WRITE, FILE_SHARE_READ, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	}						
	else
	{
		m_hFile = (HANDLE)CreateFile(rFile.PathFull().Value(), GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_ALWAYS,   FILE_ATTRIBUTE_NORMAL | FILE_FLAG_SEQUENTIAL_SCAN, NULL);
	}

	//_ASSERTE(m_hFile);
	//_ASSERTE(m_hFile != INVALID_HANDLE_VALUE);

	if (!m_hFile || (m_hFile == INVALID_HANDLE_VALUE))
	{
		switch (GetLastError())
		{
		case ERROR_FILE_NOT_FOUND: { SCFError(ErrorFileNotFound); }
		case ERROR_ACCESS_DENIED:  { SCFError(ErrorAccessDenied); }
		default:                   { SCFError(ErrorFileFailedOpen); }
		}

		return FALSE;
	}

	return TRUE;
}

void CStreamFile::FileSize(_OUT SCF::UINT64* ui64pFileSize)
{
	*((DWORD*)ui64pFileSize) = GetFileSize(m_hFile, ((DWORD*)ui64pFileSize) + 1);
}

void CStreamFile::FileCommit()
{
	FlushFileBuffers(m_hFile);
}


