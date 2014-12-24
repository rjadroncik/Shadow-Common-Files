#include "Directory.h"

#include "StringRange.h"
#include "StringSearch.h"

#include "File.h"

using namespace SCFBase;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <direct.h>

CDirectory::CDirectory()
{
	SCF::TCHAR szBuffer[MAX_PATH + 1];

	SCF::UINT uiLength = (SCF::UINT)GetCurrentDirectory(MAX_PATH, szBuffer);
	szBuffer[uiLength] = '\\';
	szBuffer[uiLength + 1] = 0;

	//#ifdef UNICODE
	//	//_wgetcwd(szBuffer, MAX_PATH);

	//	//SCF::UINT i = (SCF::UINT)wcslen(szBuffer);
	//	//szBuffer[i] = '\\';
	//
	//#else
	//	GetCurrentDirectory(MAX_PATH, szBuffer);
	//	//_getcwd(szBuffer, MAX_PATH + 1);
	//
	//	SCF::UINT i = strlen(szBuffer);
	//	szBuffer[i] = '\\';

	//#endif

	this->ParsePath(szBuffer, &m_Path, &m_Name);
}

CDirectory::CDirectory(_IN CString& rFullNameOrPath)
{
	SCF::TCHAR  szBuffer[MAX_PATH];
	SCF::LPTSTR szFilePart = NULL;

	GetFullPathName(rFullNameOrPath.Value(), MAX_PATH, szBuffer, &szFilePart);
	if (szFilePart) 
	{ 
		SCFError(ErrorDirectoryNotFound);
	}

	this->ParsePath(szBuffer, &m_Path, &m_Name);
}

CDirectory::CDirectory(_IN CString& rPath, _IN CString& rName)
{
	m_Path = rPath;
	m_Name = rName;
}

CDirectory::~CDirectory()
{
}

void CDirectory::ParsePath(_IN CString& rPath, _OUT CString* pOutPath, _OUT CString* pOutName)
{
	CStringRange FullNameRange(rPath, 0, rPath.Length() - 1);
	CStringSearch Search(FullNameRange);

	int iNamePos = Search.FindCharsRev(STRING("/\\"));
	if (iNamePos != -1)
	{
		if (pOutName) { *pOutName = CStringRange(FullNameRange, iNamePos + 1, FullNameRange.Length() - iNamePos - 1); }
		if (pOutPath) { *pOutPath = CStringRange(FullNameRange, 0, iNamePos + 1); }
	}
	else { if (pOutName) { *pOutName = FullNameRange; } }
}

bool CDirectory::ListContents(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET
{
	if (pOutFiles && pOutDirectories)
	{
		return ListFilesDirs(*pOutFiles, *pOutDirectories);
	}
	else
	{
		if (pOutFiles)  { return ListFiles(*pOutFiles); }
		else            { return ListDirs(*pOutDirectories); }
	}
}

bool CDirectory::ListFiles(_OUT CVector<CString>& rOutFiles) _GET
{
	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories)) { return FALSE; }

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		CDirectory(this->PathFull(), (CString&)(Directories[i])).ListFiles(rOutFiles);
	}
	Directories.AllDelete();

	for (SCF::UINT i = 0; i < Files.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");
		*pPath += (CString&)(Files[i]);

		rOutFiles.LastAdd(*pPath);
	}
	Files.AllDelete();

	return TRUE;
}

bool CDirectory::ListFilesDirs(_OUT CVector<CString>& rOutFiles, _OUT CVector<CString>& rOutDirectories) _GET
{
	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories)) { return FALSE; }

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");
	
		CDirectory(*pPath, (CString&)(Directories[i])).ListFilesDirs(rOutFiles, rOutDirectories);
	
		*pPath += (CString&)(Directories[i]);
		*pPath += STRING("\\");

		rOutDirectories.LastAdd(*pPath);
	}
	Directories.AllDelete();

	for (SCF::UINT i = 0; i < Files.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");
		*pPath += (CString&)(Files[i]);
	
		rOutFiles.LastAdd(*pPath);
	}
	Files.AllDelete();

	return TRUE;
}

bool CDirectory::ListDirs(_OUT CVector<CString>& rOutDirectories) _GET
{
	CVector<CString> Directories;

	if (!this->Read(NULL, &Directories)) { return FALSE; }

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");

		CDirectory(*pPath, (CString&)(Directories[i])).ListDirs(rOutDirectories);

		*pPath += (CString&)(Directories[i]);
		*pPath += STRING("\\");

		rOutDirectories.LastAdd(*pPath);
	}
	Directories.AllDelete();

	return TRUE;
}

bool CDirectory::Read(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET
{
	WIN32_FIND_DATA Data;
	HANDLE hSearch = FindFirstFile((STRING("\\\\?\\") + this->PathFull() + STRING("*")).Value(), &Data);
	if (hSearch == INVALID_HANDLE_VALUE) { return FALSE; }

	SCF::UINT    uiIndex = 0;
	static CString s_Self  (STRING("."));
	static CString s_Parent(STRING(".."));

	do 
	{
		DWORD dwFileAttributes = GetFileAttributes((this->PathFull() + (SCF::LPTSTR)&Data.cFileName[0]).Value());

		if (dwFileAttributes != 0xFFFFFFFF) 
		{
			if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				uiIndex++;
				if ((uiIndex < 3) && ((SCF::LPTSTR)&Data.cFileName[0] == s_Self) || ((SCF::LPTSTR)&Data.cFileName[0] == s_Parent))
				{
					continue;
				}
				
				if (pOutDirectories) { pOutDirectories->LastAdd(*(new CString((SCF::LPTSTR)&Data.cFileName[0]))); }
			}
			else
			{
				if (pOutFiles) { pOutFiles->LastAdd(*(new CString((SCF::LPTSTR)&Data.cFileName[0]))); }
			}
		}

	} while (FindNextFile(hSearch, &Data));

	FindClose(hSearch);
	return TRUE;
}

bool CDirectory::Exists() _GET
{
	HANDLE hFile = CreateFile(this->PathFull().Value(), 0, 0, NULL, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, NULL);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE)) { CloseHandle(hFile); return TRUE; }

	return FALSE;
}

const CString CDirectory::PathFull() _GET
{
	return (m_Path + m_Name + STRING("\\"));
}

bool CDirectory::Create(_IN bool bEraseExisting)
{
	if (this->Exists())
	{
		if (bEraseExisting)
		{
			this->Delete();
			if (!CreateDirectory(this->PathFull().Value(), NULL))
			{
				SCFError(ErrorDirectoryFailedCreate);
				return FALSE;
			}
		}
	}
	else 
	{ 
		if (!CreateDirectory(this->PathFull().Value(), NULL))
		{
			SCFError(ErrorDirectoryFailedCreate);
			return FALSE;
		}
	}

	return TRUE;
}

bool CDirectory::Erase()
{
	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories))
	{
		Directories.AllDelete();
		Files.AllDelete();

		SCFError(ErrorDirectoryFailedErase);
		return FALSE;
	}

	for (SCF::UINT i = 0; i < Files.Size(); i++)
	{
		if (!CFile(this->PathFull() + (CString&)(Files[i])).Delete())
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedErase);
			return FALSE;
		}
	}

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		if (!CDirectory(this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Delete())
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedErase);
			return FALSE;
		}
	}

	Directories.AllDelete();
	Files.AllDelete();

	return TRUE;
}

bool CDirectory::Rename(_IN CString& rNewName)
{
	if (MoveFile(this->PathFull().Value(), (this->Path() + rNewName + STRING("\\")).Value()))
	{
		m_Name = rNewName;
		return TRUE;
	}
	else
	{ 
		SCFError(ErrorDirectoryFailedRename);
		return FALSE;
	}
}

bool CDirectory::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
	if (bPathHasName)
	{
		if (MoveFile(this->PathFull().Value(), rNewPath.Value()))
		{
			ParsePath(rNewPath, &m_Path, &m_Name);
			return TRUE;
		}
		else 
		{
			CDirectory Destination(rNewPath);

			if (!this->Copy(Destination, TRUE))
			{
				SCFError(ErrorDirectoryFailedMove);
				return FALSE;
			}
			if (!this->Delete())
			{
				SCFError(ErrorDirectoryFailedMove);
				return FALSE;
			}

			m_Path = Destination.m_Path;
			m_Name = Destination.m_Name;
			return TRUE;
		}
	}
	else
	{
		if (MoveFile(this->PathFull().Value(), (rNewPath + this->Name() + STRING("\\")).Value()))
		{
			m_Path = rNewPath;
			return TRUE;
		}
		else 
		{
			CDirectory Destination(rNewPath + this->Name() + STRING("\\"));

			if (!this->Copy(Destination))
			{
				SCFError(ErrorDirectoryFailedMove);
				return FALSE;
			}
			if (!this->Delete())
			{
				SCFError(ErrorDirectoryFailedMove);
				return FALSE;
			}

			m_Path = Destination.m_Path;
			return TRUE;
		}
	}
}

bool CDirectory::Delete()
{
	this->Erase();

	if (!RemoveDirectory(this->PathFull().Value()))
	{
		SCFError(ErrorDirectoryFailedDelete);
		return FALSE;
	}

	return TRUE;
}

SCF::UINT64 CDirectory::Size()
{
	SCF::UINT64 ui64Size = 0;

	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories))
	{
		Directories.AllDelete();
		Files.AllDelete();

		SCFError(ErrorDirectoryFailedSizeGet);
		return (SCF::UINT64)-1;
	}

	for (SCF::UINT i = 0; i < Files.Size(); i++)
	{
		ui64Size += CFile(this->PathFull() + (CString&)(Files[i])).Size();
	}

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		ui64Size += CDirectory(this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Size();
	}
	
	Directories.AllDelete();
	Files.AllDelete();

	return ui64Size;
}

bool CDirectory::Writable() _GET
{
	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorDirectoryFailedWritableGet); 
		return FALSE;
	}

	return (dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0;
}

bool CDirectory::Writable(_IN bool bWritable) _SET
{
	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorDirectoryFailedWritableSet); 
		return FALSE;
	}

	if (((dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0) != bWritable)
	{
		dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		if (bWritable) { dwFileAttributes ^= FILE_ATTRIBUTE_READONLY; }

		SetFileAttributes(this->PathFull().Value(), dwFileAttributes);
	}

	return TRUE;
}

void CDirectory::Serialize(_INOUT IStreamWrite& rStream) const
{
	m_Path.Serialize(rStream);
	m_Name.Serialize(rStream);
}

void CDirectory::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Path.Deserialize(rStream);
	m_Name.Deserialize(rStream);
}

bool CDirectory::Copy(_INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
{
	return Copy(*this, rDestination, bOverwriteExisting);
}

bool CDirectory::Copy(_INOUT CDirectory& rSource, _INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
{
	if (!rDestination.Create(bOverwriteExisting))
	{
		SCFError(ErrorDirectoryFailedCopy);
		return FALSE;
	}

	CVector<CString> Directories;
	CVector<CString> Files;

	if (!rSource.Read(&Files, &Directories))
	{
		Directories.AllDelete();
		Files.AllDelete();

		SCFError(ErrorDirectoryFailedCopy);
		return FALSE;
	}

	CString csTerminator(STRING("\\"));

	for (SCF::UINT i = 0; i < Directories.Size(); i++)
	{
		CDirectory DirectorySource     (rSource.PathFull()      + (CString&)(Directories[i]) + csTerminator);
		CDirectory DirectoryDestination(rDestination.PathFull() + (CString&)(Directories[i]) + csTerminator);

		if (!Copy(DirectorySource, DirectoryDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return FALSE;
		}
	}

	for (SCF::UINT i = 0; i < Files.Size(); i++)
	{
		CFile FileSource     (rSource.PathFull()      + (CString&)(Files[i]));
		CFile FileDestination(rDestination.PathFull() + (CString&)(Files[i]));

		if (!FileSource.Copy(FileDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return FALSE;
		}
	}

	Directories.AllDelete();
	Files.AllDelete();

	return TRUE;
}



