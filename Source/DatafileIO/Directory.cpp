#include "Datafile.h"

#include "EnumeratorDirectory.h"

using namespace SCF;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CDFDirectory::CDFDirectory(_INOUT CDatafile& rDatafile)
{
	this->ParsePath(rDatafile.m_CWD, rDatafile, &m_Path, &m_Name);

	m_pDatafile = &rDatafile;
	ADDREF(*(m_pDatafile));
}

CDFDirectory::CDFDirectory(_INOUT CDatafile& rDatafile, _IN CString& rFullNameOrPath)
{
	this->ParsePath(rFullNameOrPath, rDatafile, &m_Path, &m_Name);

	m_pDatafile = &rDatafile;
	ADDREF(*(m_pDatafile));
}

CDFDirectory::CDFDirectory(_INOUT CDatafile& rDatafile, _IN CString& rPath, _IN CString& rName) : CDirectory(rPath, rName)
{
	m_pDatafile = &rDatafile;
	ADDREF(*(m_pDatafile));
}

CDFDirectory::~CDFDirectory()
{
	RELEASE(*(m_pDatafile));
}

bool CDFDirectory::ParsePath(_IN CString& rPath, _IN CDatafile& rDatafile, _OUT CString* pOutPath, _OUT CString* pOutName)
{
	//Directory path must be '/' or '\' terminated
	if ((rPath[rPath.Length() - 1] != '/') && (rPath[rPath.Length() - 1] != '\\'))
	{
		SCFError(ErrorBadPath);
		return FALSE;
	}

	//We cut off the terminating char :)
	CStringRange FullNameRange(rPath, 0, rPath.Length() - 1);
	CStringSearch Search(FullNameRange);

	//First we find the last '/' or '\', thus we get the start of the name
	int iNamePos = Search.FindCharsRev(STRING("/\\"));

	//Handle full/absolute paths (these must NOT contain '../' sequences)
	if ((rPath[0] == '/') || (rPath[0] == '\\'))
	{
		if (iNamePos != -1)
		{
			if (pOutName) { *pOutName = CStringRange(FullNameRange, iNamePos + 1, FullNameRange.Length() - iNamePos - 1); }
			if (pOutPath) { *pOutPath = CStringRange(FullNameRange, 0, iNamePos + 1); }
		}
		else 
		{
			if (pOutName) { *pOutName = CString(); }
			if (pOutPath) { *pOutPath = CString(); }
		}

		return TRUE;
	}

	//Check whether we found any '/' or '\', if yes we have a relative path
	if (iNamePos != -1)
	{
		UINT uiRelPathStart = 0;

		UINT uiCWDEnd = __max(0, (int)rDatafile.CWD().Length() - 2);
		CString csCWD(rDatafile.CWD(), (bool)FALSE);

		while (((uiRelPathStart + 2) < rPath.Length()) &&
			  ((rPath[uiRelPathStart]     == '.') && (rPath[uiRelPathStart + 1] == '.') && 
			  ((rPath[uiRelPathStart + 2] == '/') || (rPath[uiRelPathStart + 2] == '\\'))))
		{
			uiRelPathStart += 3;

			while ((csCWD[uiCWDEnd] != '/') && (csCWD[uiCWDEnd] != '\\') && (uiCWDEnd != 0)) 
			{ 
				uiCWDEnd--; 
			}
		}

		//Write results
		if (pOutName) { *pOutName = CStringRange(FullNameRange, iNamePos + 1, FullNameRange.Length() - iNamePos - 1); }
		if (pOutPath) 
		{
			*pOutPath  = CStringRange(csCWD, 0, uiCWDEnd + 1);
			*pOutPath += CStringRange(FullNameRange, uiRelPathStart, iNamePos + 1 - uiRelPathStart); 
		}
	}
	else 
	{ 
		//We have a relative path consisting of just a name
		if (pOutName) { *pOutName = FullNameRange; } 
		if (pOutPath) { *pOutPath = rDatafile.CWD(); }
	}

	return TRUE;
}

bool CDFDirectory::Read(_OUT CVector<CString>* pOutFiles, _OUT CVector<CString>* pOutDirectories) _GET
{
	CEnumeratorDirectory Enumerator(*(m_pDatafile->m_pRecords), this->PathFull());

	while (Enumerator.Next())
	{
		switch(Enumerator.Current()->ClassKey())
		{
		case ClassRecordFile:
			{
				if (pOutFiles)
				{
					CString Name;
					CString Extension;

					CDFFile::ParsePath(Enumerator.CurrentPath(), *m_pDatafile, NULL, &Name, &Extension);

					CString* pFullName = new CString();

					*pFullName += Name;
					*pFullName += STRING(".");
					*pFullName += Extension;

					pOutFiles->LastAdd(*pFullName); 
				}
				break;
			}
		case ClassRecordDirectory:
			{
				if (pOutDirectories) 
				{
					CString* pFullName = new CString();

					CDFDirectory::ParsePath(Enumerator.CurrentPath(), *m_pDatafile, NULL, pFullName);

					pOutDirectories->LastAdd(*pFullName); 
				}
				break;
			}
		}
	}

	return TRUE;
}

bool CDFDirectory::Exists() _GET { return m_pDatafile->m_pRecords->ContainsName(this->PathFull()); }

bool CDFDirectory::Create(_IN bool bEraseExisting)
{
	//If this is a top level directory, it has no parent..
	if (this->Path().Length() > 0)
	{
		//Check whether parent exists
		CRecordDirectory* pRecordParent = (CRecordDirectory*)m_pDatafile->m_pRecords->At(this->Path());
		if (!pRecordParent)
		{
			SCFError(ErrorDirectoryFailedCreate); 
			return FALSE;
		}
	}

	CString csPathToRecord(this->PathFull());

	CRecordDirectory* pRecord = (CRecordDirectory*)m_pDatafile->m_pRecords->At(csPathToRecord);
	if (pRecord)
	{
		if (bEraseExisting)
		{ 
			m_pDatafile->m_pRecords->RemoveKey(csPathToRecord); 
			delete pRecord; 
		}
		else
		{ 
			SCFError(ErrorDirectoryFailedCreate); 
			return FALSE;
		}
	}

	pRecord = new CRecordDirectory();
	if (!pRecord) 
	{
		SCFError(ErrorDirectoryFailedCreate); 
		return FALSE;
	}

	m_pDatafile->m_pRecords->AtPut(csPathToRecord, *pRecord);
	return TRUE;
}

bool CDFDirectory::Erase()
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

	for (UINT i = 0; i < Files.Size(); i++)
	{
		if (!CDFFile(this->Datafile(), this->PathFull() + (CString&)(Files[i])).Delete())
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedErase);
			return FALSE;
		}
	}

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		if (!CDFDirectory(this->Datafile(), this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Delete())
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

bool CDFDirectory::Copy(_INOUT CDFDirectory& rDestination, _IN bool bOverwriteExisting)
{
	return Copy(*this, rDestination, bOverwriteExisting);
}

bool CDFDirectory::Copy(_INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
{
	return Copy(*this, rDestination, bOverwriteExisting);
}

bool CDFDirectory::Copy(_INOUT CDFDirectory& rSource, _INOUT CDFDirectory& rDestination, _IN bool bOverwriteExisting)
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

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		CDFDirectory DirectorySource     (rSource.Datafile(),      rSource.PathFull()      + (CString&)(Directories[i]) + csTerminator);
		CDFDirectory DirectoryDestination(rDestination.Datafile(), rDestination.PathFull() + (CString&)(Directories[i]) + csTerminator);

		if (!Copy(DirectorySource, DirectoryDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return FALSE;
		}
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		CDFFile FileSource     (rSource.Datafile(),      rSource.PathFull()      + (CString&)(Files[i]));
		CDFFile FileDestination(rDestination.Datafile(), rDestination.PathFull() + (CString&)(Files[i]));

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

bool CDFDirectory::Copy(_INOUT CDFDirectory& rSource, _INOUT CDirectory& rDestination, _IN bool bOverwriteExisting)
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

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		CDFDirectory DirectorySource     (rSource.Datafile(), rSource.PathFull()      + (CString&)(Directories[i]) + csTerminator);
		CDirectory   DirectoryDestination(                    rDestination.PathFull() + (CString&)(Directories[i]) + csTerminator);

		if (!Copy(DirectorySource, DirectoryDestination, bOverwriteExisting))
		{
			Directories.AllDelete();
			Files.AllDelete();

			SCFError(ErrorDirectoryFailedCopy);
			return FALSE;
		}
	}

	for (UINT i = 0; i < Files.Size(); i++)
	{
		CDFFile FileSource     (rSource.Datafile(), rSource.PathFull()      + (CString&)(Files[i]));
		CFile   FileDestination(                    rDestination.PathFull() + (CString&)(Files[i]));

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

bool CDFDirectory::Rename(_IN CString& rNewName)
{
	//Check whether a directory with the given new name already exists
	if (m_pDatafile->m_pRecords->ContainsName(this->Path() + STRING("\\") + rNewName + STRING("\\")))
	{
		SCFError(ErrorDirectoryFailedRename);
		return FALSE;
	}

	CDFDirectory Destination(*m_pDatafile, this->Path() + rNewName + STRING("\\"));

	if (!this->Copy(Destination))
	{
		SCFError(ErrorDirectoryFailedRename);
		return FALSE;
	}
	if (!this->Delete())
	{
		SCFError(ErrorDirectoryFailedRename);
		return FALSE;
	}

	m_Name = rNewName;
	return TRUE;
}

bool CDFDirectory::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
	if (bPathHasName)
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_pRecords->ContainsName(rNewPath))
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}

		CDFDirectory Destination(*m_pDatafile, rNewPath);

		if (!this->Copy(Destination))
		{
			SCFError(ErrorDirectoryFailedRename);
			return FALSE;
		}
		if (!this->Delete())
		{
			SCFError(ErrorDirectoryFailedRename);
			return FALSE;
		}

		m_Path = Destination.m_Path;
		m_Name = Destination.m_Name;
		return TRUE;
	}
	else
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_pRecords->ContainsName(rNewPath + this->Name() + STRING("\\")))
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}

		CDFDirectory Destination(*m_pDatafile, rNewPath + this->Name() + STRING("\\"));

		if (!this->Copy(Destination))
		{
			SCFError(ErrorDirectoryFailedRename);
			return FALSE;
		}
		if (!this->Delete())
		{
			SCFError(ErrorDirectoryFailedRename);
			return FALSE;
		}

		m_Path = Destination.m_Path;
		return TRUE;
	}
}

bool CDFDirectory::Delete()
{
	if (!this->Erase())
	{ 
		SCFError(ErrorFileFailedDelete); 
		return FALSE;
	}

	CRecordDirectory* pRecord = (CRecordDirectory*)m_pDatafile->m_pRecords->RemoveKey(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedDelete); 
		return FALSE;
	}

	delete pRecord;

	return TRUE;
}

UINT64 CDFDirectory::Size()
{
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
		ui64Size += CDFFile(*m_pDatafile, this->PathFull() + (CString&)(Files[i])).Size();
	}

	for (UINT i = 0; i < Directories.Size(); i++)
	{
		ui64Size += CDFDirectory(*m_pDatafile, this->PathFull() + (CString&)(Directories[i]) + STRING("\\")).Size();
	}

	Directories.AllDelete();
	Files.AllDelete();
	 
	return ui64Size;
}

bool CDFDirectory::Writable() _GET { return TRUE; }

bool CDFDirectory::Writable(_IN bool bWritable) _SET 
{
	SCF_UNREFERENCED_PARAMETER(bWritable);
	SCFError(ErrorDirectoryFailedWritableSet); 
	return FALSE;
}

bool CDFDirectory::Encrypted() _GET
{ 
	CRecordDirectory* pRecord = (CRecordDirectory*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorDFDirectoryFailedAttributeGet); 
		return FALSE;
	}

	return pRecord->Encrypted();
}

bool CDFDirectory::Encrypted(_IN bool bEncrypted) _SET
{ 
	CRecordDirectory* pRecord = (CRecordDirectory*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorDFDirectoryFailedAttributeSet); 
		return FALSE;
	}

	pRecord->Encrypted(bEncrypted);
	return TRUE;
}

void CDFDirectory::Serialize(_INOUT IStreamWrite& rStream) const
{
	m_Path.Serialize(rStream);
	m_Name.Serialize(rStream);
}

void CDFDirectory::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Path.Deserialize(rStream);
	m_Name.Deserialize(rStream);
}

void CDFDirectory::CWD() { m_pDatafile->CWD(*this); }