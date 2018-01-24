#include "Directory.h"

#include "StringRange.h"
#include "StringSearch.h"

#include "File.h"

using namespace SCFBase;

#if WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

#include <direct.h>

#else

#include <dirent.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/stat.h>

#endif // WIN32

CDirectory::CDirectory()
{
    #ifdef WIN32

	TCHAR szBuffer[MAX_PATH + 1];
	UINT uiLength = (UINT)GetCurrentDirectory(MAX_PATH, szBuffer);
	szBuffer[uiLength] = '\\';
	szBuffer[uiLength + 1] = 0;

    #else

    //TODO: verify whether path isn't '/' terminated (shouldn't be)
	char szBuffer[MAX_PATH + 1];
    getcwd(szBuffer, MAX_PATH);

    #endif // WIN32

	this->ParsePath(szBuffer, &m_Path, &m_Name);
}

CDirectory::CDirectory(_IN CString& rFullNameOrPath)
{
    #ifdef WIN32

	TCHAR  szBuffer[MAX_PATH];
	LPTSTR szFilePart = nullptr;

	GetFullPathName(rFullNameOrPath.Value(), MAX_PATH, szBuffer, &szFilePart);
	if (szFilePart)
	{
		SCFError(ErrorDirectoryNotFound);
	}

	#else

	char szPath[MAX_PATH];
	char szBuffer[MAX_PATH];

    rFullNameOrPath.ToASCII(szPath);

	realpath(szPath, szBuffer);

    #endif // WIN32

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

	if (!this->Read(&Files, &Directories)) { return false; }

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		CDirectory(this->PathFull(), (CString&)(Directories[i])).ListFiles(rOutFiles);
	}
	Directories.AllDelete();

	for (UINT i = 0; i < Files.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");
		*pPath += (CString&)(Files[i]);

		rOutFiles.LastAdd(*pPath);
	}
	Files.AllDelete();

	return true;
}

bool CDirectory::ListFilesDirs(_OUT CVector<CString>& rOutFiles, _OUT CVector<CString>& rOutDirectories) _GET
{
	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories)) { return false; }

	for (UINT i = 0; i < Directories.Size(); i++)
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

	for (UINT i = 0; i < Files.Size(); i++)
	{
		CString* pPath = new CString(this->m_Path);
		*pPath += this->m_Name;
		*pPath += STRING("\\");
		*pPath += (CString&)(Files[i]);

		rOutFiles.LastAdd(*pPath);
	}
	Files.AllDelete();

	return true;
}

bool CDirectory::ListDirs(_OUT CVector<CString>& rOutDirectories) _GET
{
	CVector<CString> Directories;

	if (!this->Read(nullptr, &Directories)) { return false; }

	for (UINT i = 0; i < Directories.Size(); i++)
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

	return true;
}

bool CDirectory::Read(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET
{
    #ifdef WIN32

	WIN32_FIND_DATA Data;
	HANDLE hSearch = FindFirstFile((STRING("\\\\?\\") + this->PathFull() + STRING("*")).Value(), &Data);
	if (hSearch == INVALID_HANDLE_VALUE) { return false; }

	UINT uiIndex = 0;

	static CString s_Self  (STRING("."));
	static CString s_Parent(STRING(".."));

	do
	{
		DWORD dwFileAttributes = GetFileAttributes((this->PathFull() + (LPTSTR)&Data.cFileName[0]).Value());

		if (dwFileAttributes != 0xFFFFFFFF)
		{
			if (dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
			{
				uiIndex++;
				if ((uiIndex < 3) && ((LPTSTR)&Data.cFileName[0] == s_Self) || ((LPTSTR)&Data.cFileName[0] == s_Parent))
				{
					continue;
				}

				if (pOutDirectories) { pOutDirectories->LastAdd(*(new CString((LPTSTR)&Data.cFileName[0]))); }
			}
			else
			{
				if (pOutFiles) { pOutFiles->LastAdd(*(new CString((LPTSTR)&Data.cFileName[0]))); }
			}
		}

	} while (FindNextFile(hSearch, &Data));

	FindClose(hSearch);

	#else

    DIR *dir;
    struct dirent *ent;

    char szBuffer[MAX_PATH + 1];
    this->PathFull().ToASCII(szBuffer);

    if ((dir = opendir(szBuffer)) != nullptr)
    {
        UINT uiIndex = 0;

        static CString s_Self  (STRING("."));
        static CString s_Parent(STRING(".."));

        while ((ent = readdir(dir)) != nullptr)
        {
            if (ent->d_type == DT_DIR)
            {
                uiIndex++;
                if ((uiIndex < 3) && ((char*)&ent->d_name[0] == s_Self) || ((char*)&ent->d_name[0] == s_Parent))
                {
                    continue;
                }

  				if (pOutDirectories) { pOutDirectories->LastAdd(*(new CString(ent->d_name))); }
            }
            else if (ent->d_type == DT_REG)
            {
 				if (pOutFiles) { pOutFiles->LastAdd(*(new CString(ent->d_name))); }
            }
        }

        closedir (dir);
    }
    else
    {
        return false;
    }

	#endif

	return true;
}

bool CDirectory::Exists() _GET
{
    #ifdef WIN32

    HANDLE hFile = CreateFile(this->PathFull().Value(), 0, 0, nullptr, OPEN_EXISTING, FILE_FLAG_BACKUP_SEMANTICS, nullptr);

	if ((hFile != 0) && (hFile != INVALID_HANDLE_VALUE)) { CloseHandle(hFile); return true; }

    #else

    char szBuffer[MAX_PATH + 1];
    this->PathFull().ToASCII(szBuffer);

    struct stat sb;

    if (stat(szBuffer, &sb) == 0 && S_ISDIR(sb.st_mode))
    {
        return true;
    }

    #endif // WIN32

	return false;
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

			#ifdef WIN32

			if (!CreateDirectory(this->PathFull().Value(), nullptr))
			{
				SCFError(ErrorDirectoryFailedCreate);
				return false;
			}

			#else

            char szBuffer[MAX_PATH + 1];
            this->PathFull().ToASCII(szBuffer);

            if (mkdir(szBuffer, S_IRWXU) != 0) { return false; }

			#endif
		}
	}
	else
	{
		#ifdef WIN32

		if (!CreateDirectory(this->PathFull().Value(), nullptr))
		{
			SCFError(ErrorDirectoryFailedCreate);
			return false;
		}
        #else

        char szBuffer[MAX_PATH + 1];
        this->PathFull().ToASCII(szBuffer);

        if (mkdir(szBuffer, S_IRWXU) != 0) { return false; }

        #endif
	}

	return true;
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
		return false;
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		if (!CFile(this->PathFull() + (CString&)(Files[i])).Delete())
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedErase);
			return false;
		}
	}

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		if (!CDirectory(this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Delete())
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedErase);
			return false;
		}
	}

	Directories.AllDelete();
	Files.AllDelete();

	return true;
}

bool CDirectory::Rename(_IN CString& rNewName)
{
    #ifdef WIN32

	if (MoveFile(this->PathFull().Value(), (this->Path() + rNewName + STRING("\\")).Value()))
	{
		m_Name = rNewName;
		return true;
	}
	else
	{
		SCFError(ErrorDirectoryFailedRename);
		return false;
	}

	#else

    //TODO:
    return false;

	#endif // WIN32
}

bool CDirectory::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
    #ifdef WIN32

	if (bPathHasName)
	{
		if (MoveFile(this->PathFull().Value(), rNewPath.Value()))
		{
			ParsePath(rNewPath, &m_Path, &m_Name);
			return true;
		}
		else
		{
			CDirectory Destination(rNewPath);

			if (!this->Copy(Destination, true))
			{
				SCFError(ErrorDirectoryFailedMove);
				return false;
			}
			if (!this->Delete())
			{
				SCFError(ErrorDirectoryFailedMove);
				return false;
			}

			m_Path = Destination.m_Path;
			m_Name = Destination.m_Name;
			return true;
		}
	}
	else
	{
		if (MoveFile(this->PathFull().Value(), (rNewPath + this->Name() + STRING("\\")).Value()))
		{
			m_Path = rNewPath;
			return true;
		}
		else
		{
			CDirectory Destination(rNewPath + this->Name() + STRING("\\"));

			if (!this->Copy(Destination))
			{
				SCFError(ErrorDirectoryFailedMove);
				return false;
			}
			if (!this->Delete())
			{
				SCFError(ErrorDirectoryFailedMove);
				return false;
			}

			m_Path = Destination.m_Path;
			return true;
		}
	}

	#else

    //TODO:
    return false;

	#endif // WIN32
}

bool CDirectory::Delete()
{
	this->Erase();

    #ifdef WIN32

	if (!RemoveDirectory(this->PathFull().Value()))
	{
		SCFError(ErrorDirectoryFailedDelete);
		return false;
	}

	#else

    //TODO:

	#endif // WIN32

	return true;
}

UINT64 CDirectory::Size()
{
    #ifdef WIN32

	UINT64 ui64Size = 0;

	CVector<CString> Directories;
	CVector<CString> Files;

	if (!this->Read(&Files, &Directories))
	{
		Directories.AllDelete();
		Files.AllDelete();

		SCFError(ErrorDirectoryFailedSizeGet);
		return (UINT64)-1;
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		ui64Size += CFile(this->PathFull() + (CString&)(Files[i])).Size();
	}

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		ui64Size += CDirectory(this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Size();
	}

	Directories.AllDelete();
	Files.AllDelete();

	return ui64Size;

	#else

    //TODO:
    return -1;

	#endif // WIN32
}

bool CDirectory::Writable() _GET
{
    #ifdef WIN32

	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorDirectoryFailedWritableGet);
		return false;
	}

	return (dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0;

	#else

    //TODO:
    return false;

	#endif // WIN32
}

bool CDirectory::Writable(_IN bool bWritable) _SET
{
    #ifdef WIN32

	DWORD dwFileAttributes = GetFileAttributes(this->PathFull().Value());

	if (dwFileAttributes == INVALID_FILE_ATTRIBUTES)
	{
		SCFError(ErrorDirectoryFailedWritableSet);
		return false;
	}

	if (((dwFileAttributes & FILE_ATTRIBUTE_READONLY) == 0) != bWritable)
	{
		dwFileAttributes |= FILE_ATTRIBUTE_READONLY;
		if (bWritable) { dwFileAttributes ^= FILE_ATTRIBUTE_READONLY; }

		SetFileAttributes(this->PathFull().Value(), dwFileAttributes);
	}

	return true;

	#else

     //TODO:
   return false;

	#endif // WIN32
}

bool CDirectory::Copy(_INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
{
	return Copy(*this, rDestination, bOverwriteExisting);
}

bool CDirectory::Copy(_INOUT CDirectory& rSource, _INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
{
    #ifdef WIN32

	if (!rDestination.Create(bOverwriteExisting))
	{
		SCFError(ErrorDirectoryFailedCopy);
		return false;
	}

	CVector<CString> Directories;
	CVector<CString> Files;

	if (!rSource.Read(&Files, &Directories))
	{
		Directories.AllDelete();
		Files.AllDelete();

		SCFError(ErrorDirectoryFailedCopy);
		return false;
	}

	CString csTerminator(STRING("\\"));

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		CDirectory DirectorySource     (rSource.PathFull()      + (CString&)(Directories[i]) + csTerminator);
		CDirectory DirectoryDestination(rDestination.PathFull() + (CString&)(Directories[i]) + csTerminator);

		if (!Copy(DirectorySource, DirectoryDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return false;
		}
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		CFile FileSource     (rSource.PathFull()      + (CString&)(Files[i]));
		CFile FileDestination(rDestination.PathFull() + (CString&)(Files[i]));

		if (!FileSource.Copy(FileDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return false;
		}
	}

	Directories.AllDelete();
	Files.AllDelete();

	return true;

	#else

    //TODO:
    return false;

	#endif // WIN32
}



