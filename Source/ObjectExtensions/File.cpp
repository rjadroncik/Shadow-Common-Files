#include "File.h"

#include "StringRange.h"
#include "StringSearch.h"

using namespace SCFBase;

#if WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <Rpc.h>

#else

#endif // WIN32

CFile::CFile(_IN CString& rFullNameOrPath)
{
    #if WIN32

	TCHAR  szBuffer[MAX_PATH];
	LPTSTR szFilePart = NULL;

	GetFullPathName(rFullNameOrPath.Value(), MAX_PATH, szBuffer, &szFilePart);
	ParsePath(CString(szBuffer), &m_Path, &m_Name, &m_Extension);

	#else
    //TODO:
	#endif // WIN32
}

CFile::CFile(_IN CFile& rFile)
{
	m_Path      = rFile.m_Path;
	m_Name      = rFile.m_Name;
	m_Extension = rFile.m_Extension;
}

CFile::CFile(_IN CString& rPath, _IN CString& rName, _IN CString& rExtension)
{
	m_Path      = rPath;
	m_Name      = rName;
	m_Extension = rExtension;
}

CFile::~CFile()
{
}

void CFile::ParsePath(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension)
{
	CStringSearch Search(rPath);

	int iExtPos  = Search.FindCharsRev(STRING("."));
	int iNamePos = Search.FindCharsRev(STRING("/\\"));

	if (iNamePos != -1)
	{
		if (pOutExtension)
		{
			if ((iExtPos != -1) && (iExtPos > iNamePos)) { *pOutExtension = CStringRange(rPath, iExtPos + 1, rPath.Length() - iExtPos - 1); }
			else                                         { *pOutExtension = CString(); }
		}
		if (pOutName)
		{
			if ((iExtPos != -1) && (iExtPos > iNamePos)) { *pOutName = CStringRange(rPath, iNamePos + 1, iExtPos - iNamePos - 1); }
			else                                         { *pOutName = CStringRange(rPath, iNamePos + 1); }
		}
		if (pOutPath) { *pOutPath = CStringRange(rPath, 0, iNamePos + 1); }
	}
	else
	{
		if (pOutExtension)
		{
			if (iExtPos != -1) { *pOutExtension = CStringRange(rPath, iExtPos + 1, rPath.Length() - iExtPos - 1); }
			else               { *pOutExtension = CString(); }
		}

		if (pOutName)
		{
			if (iExtPos != -1) { *pOutName = CStringRange(rPath, 0, iExtPos); }
			else               { *pOutName = rPath; }
		}
	}
}

bool CFile::Writable() _GET
{
    #if WIN32

	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorFileFailedWritableGet);
		return FALSE;
	}

	return (dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0;

	#else
    //TODO:
    return -1;
	#endif // WIN32
}

bool CFile::Writable(_IN bool bWritable) _SET
{
    #if WIN32

	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorFileFailedWritableSet);
		return FALSE;
	}

	if (((dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0) != bWritable)
	{
		dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		if (bWritable) { dwFileAttributes ^= FILE_ATTRIBUTE_READONLY; }

		SetFileAttributes(this->PathFull().Value(), dwFileAttributes);
	}

	return TRUE;

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

const CString CFile::TempPath()      _GET { return m_Path; }
const CString CFile::TempName()      _GET { return STRING("!") + m_Name; }
const CString CFile::TempExtension() _GET { return STRING("tmp!"); }

const CString CFile::TempPathFull()  _GET { return (m_Path + this->TempNameFull()); }
const CString CFile::TempNameFull()  _GET { return this->TempName() + STRING(".") + this->TempExtension(); }

const CString CFile::PathFull() _GET
{
	if (m_Extension.Length())
	{
		return (m_Path + m_Name + STRING(".") + m_Extension);
	}
	else { return (m_Path + m_Name); }
}

const CString CFile::NameFull() _GET
{
	if (m_Extension.Length())
	{
		return (m_Name + STRING(".") + m_Extension);
	}
	else { return m_Name; }
}

bool CFile::Exists() _GET
{
    #if WIN32

	HANDLE hFile = CreateFile(this->PathFull().Value(), 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE)) { CloseHandle(hFile); return TRUE; }

	return FALSE;

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Accessible() _GET
{
    #if WIN32

	HANDLE hFile = CreateFile(this->PathFull().Value(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_NO_BUFFERING, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE)) { CloseHandle(hFile); return TRUE; }

	return FALSE;

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

UINT64 CFile::Size() _GET
{
    #if WIN32

	HANDLE hFile = CreateFile(this->PathFull().Value(), 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		UINT64 ui64Size = 0;
		*((DWORD*)&ui64Size) = GetFileSize(hFile, ((DWORD*)&ui64Size) + 1);

		CloseHandle(hFile);
		return ui64Size;
	}
	else
	{
		SCFError(ErrorFileFailedSizeGet);
		return (UINT64)-1;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Size(UINT64 ui64Size) _SET
{
    #if WIN32

	HANDLE hFile = CreateFile(this->PathFull().Value(), GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		SetFilePointer(hFile, (DWORD)ui64Size, (PLONG)(((DWORD*)&ui64Size) + 1), FILE_BEGIN);
		SetEndOfFile(hFile);

		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedSizeSet);
		return FALSE;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Create(_IN bool bEraseExisting)
{
    #if WIN32

	HANDLE hFile;

	if (bEraseExisting) { hFile = CreateFile(this->PathFull().Value(), 0, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL); }
	else                { hFile = CreateFile(this->PathFull().Value(), 0, 0, NULL, CREATE_NEW,    FILE_ATTRIBUTE_NORMAL, NULL); }

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedCreate);
		return FALSE;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Delete()
{
    #if WIN32

	if (!DeleteFile(this->PathFull().Value()))
	{
		SCFError(ErrorFileFailedDelete);
		return FALSE;
	}

	return TRUE;

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Erase()
{
    #if WIN32

	HANDLE hFile = CreateFile(this->PathFull().Value(), GENERIC_WRITE, 0, NULL, TRUNCATE_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE))
	{
		CloseHandle(hFile);
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedErase);
		return FALSE;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Rename(_IN CString& rNewName)
{
    #if WIN32

	if (MoveFile(this->PathFull().Value(), (this->Path() + rNewName).Value()))
	{
		ParsePath(rNewName, NULL, &m_Name, &m_Extension);
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedRename);
		return FALSE;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
    #if WIN32

	if (bPathHasName)
	{
		if (MoveFile(this->PathFull().Value(), rNewPath.Value()))
		{
			ParsePath(rNewPath, &m_Path, &m_Name, &m_Extension);
			return TRUE;
		}
		else
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}
	}
	else
	{
		if (MoveFile(this->PathFull().Value(), (rNewPath + this->NameFull()).Value()))
		{
			ParsePath(rNewPath, &m_Path, NULL, NULL);
			return TRUE;
		}
		else
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}

bool CFile::Copy(_INOUT CFile& rDestination, _IN bool bOverwriteExisting)
{
    #if WIN32

	if (CopyFile(this->PathFull().Value(), rDestination.PathFull().Value(), !bOverwriteExisting))
	{
		return TRUE;
	}
	else
	{
		SCFError(ErrorFileFailedCopy);
		return FALSE;
	}

	#else
    //TODO:
    return FALSE;
	#endif // WIN32
}
