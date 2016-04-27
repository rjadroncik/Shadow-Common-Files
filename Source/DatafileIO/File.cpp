#include "Datafile.h"

using namespace SCF;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CDFFile::CDFFile(_IN CDFFile& rFile)
{
	m_Path      = rFile.m_Path;
	m_Name      = rFile.m_Name;
	m_Extension = rFile.m_Extension;

	m_pDatafile = rFile.m_pDatafile;
	ADDREF(*(m_pDatafile));
}

CDFFile::CDFFile(_INOUT CDatafile& rDatafile, _IN CString& rFullNameOrPath)
{
	ParsePath(rFullNameOrPath, rDatafile, &m_Path, &m_Name, &m_Extension);

	m_pDatafile = &rDatafile;
	ADDREF(*(m_pDatafile));
}

CDFFile::CDFFile(_INOUT CDatafile& rDatafile, _IN CString& rPath, _IN CString& rName, _IN CString& rExtension) : CFile(rPath, rName, rExtension)
{
	m_pDatafile = &rDatafile;
	ADDREF(*(m_pDatafile));
}

CDFFile::~CDFFile()
{
	RELEASE(*(m_pDatafile));
}

bool CDFFile::ParsePath(_IN CString& rPath, _IN CDatafile& rDatafile, _OUT CString* pOutPath, _OUT CString* pOutName, _OUT CString* pOutExtension)
{
	CStringSearch Search(rPath);

	int iExtPos  = Search.FindCharsRev(STRING("."));
	int iNamePos = Search.FindCharsRev(STRING("/\\"));

	if (iNamePos > iExtPos) { iExtPos = -1; }
		
	if (iExtPos != -1) { if (pOutExtension) { *pOutExtension = CStringRange(rPath, iExtPos + 1, rPath.Length() - iExtPos - 1); } }
	else               { if (pOutExtension) { *pOutExtension = CString(); } }


	if ((rPath[0] == '/') || (rPath[0] == '\\'))
	{
		if (iNamePos != -1)
		{
			if (iExtPos != -1) { if (pOutName) { *pOutName = CStringRange(rPath, iNamePos + 1, iExtPos - iNamePos - 1); } }
			else               { if (pOutName) { *pOutName = CStringRange(rPath, iNamePos + 1, rPath.Length() - iNamePos - 1); } }
			
			if (pOutPath) { *pOutPath = CStringRange(rPath, 0,            iNamePos + 1); }
			return TRUE;
		}
		else 
		{
			SCFError(ErrorBadPath);
			return FALSE;
		}
	}

	if (iNamePos != -1)
	{
		SCF::UINT uiRelPathStart = 0;

		SCF::UINT uiCWDEnd = __max(0, (int)rDatafile.CWD().Length() - 2);
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

		if (iExtPos != -1) { if (pOutName) { *pOutName = CStringRange(rPath, iNamePos + 1, iExtPos - iNamePos - 1); } }
		else               { if (pOutName) { *pOutName = CStringRange(rPath, iNamePos + 1, rPath.Length() - iNamePos - 1); } }

		if (pOutPath) 
		{
			*pOutPath  = CStringRange(csCWD, 0, uiCWDEnd + 1);
			*pOutPath += CStringRange(rPath, uiRelPathStart, iNamePos + 1 - uiRelPathStart); 
		}
	}
	else 
	{ 
		if (iExtPos != -1) { if (pOutName) { *pOutName = CStringRange(rPath, 0, iExtPos); } }
		else               { if (pOutName) { *pOutName = CStringRange(rPath, 0, rPath.Length()); } }

		if (pOutPath) { *pOutPath = rDatafile.CWD(); }
	}

	return TRUE;
}

bool CDFFile::Writable() _GET { return TRUE; }

bool CDFFile::Writable(_IN bool bWritable) _SET 
{
	SCF_UNREFERENCED_PARAMETER(bWritable);
	SCFError(ErrorFileFailedWritableSet); 
	return FALSE;
}

bool CDFFile::Encrypted() _GET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorDFFileFailedAttributeGet); 
		return FALSE;
	}

	return pRecord->Encrypted();
}

bool CDFFile::Encrypted(_IN bool bEncrypted) _SET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorDFFileFailedAttributeSet); 
		return FALSE;
	}

	pRecord->Encrypted(bEncrypted);
	return TRUE;
}

bool CDFFile::Compressed() _GET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorDFFileFailedAttributeGet); 
		return FALSE;
	}

	return pRecord->Compressed();
}

bool CDFFile::Compressed(_IN bool bCompressed) _SET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorDFFileFailedAttributeSet); 
		return FALSE;
	}

	pRecord->Compressed(bCompressed);
	return TRUE;
}

bool CDFFile::Exists() _GET { return m_pDatafile->m_pRecords->ContainsName(this->PathFull()); }

SCF::UINT64 CDFFile::Size() _GET
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedSizeGet); 
		return FALSE;
	}

	return pRecord->Size();
}

bool CDFFile::Create(_IN CFile& rSource, _IN bool bEraseExisting)
{
	CString csPathFull(this->PathFull());

	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(csPathFull);
	if (pRecord)
	{
		if (bEraseExisting)
		{ 
			m_pDatafile->m_pRecords->RemoveKey(csPathFull); 
			delete pRecord; 
		}
		else
		{ 
			SCFError(ErrorFileFailedCreate); 
			return FALSE;
		}
	}

	pRecord = new CRecordFile(rSource, 0x00);
	if (!pRecord)
	{ 
		SCFError(ErrorObjectFailedCreation);
		return FALSE;
	}

	m_pDatafile->m_pRecords->AtPut(csPathFull, *pRecord);

	return TRUE;
}

bool CDFFile::Delete()
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->RemoveKey(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorFileFailedDelete); 
		return FALSE;
	}

	delete pRecord;

	return TRUE;
}

bool CDFFile::Rename(_IN CString& rNewName)
{
	//Check whether a file with the given new name already exists
	if (m_pDatafile->m_pRecords->ContainsName(this->Path() + STRING("\\") + rNewName))
	{
		SCFError(ErrorFileFailedRename);
		return FALSE;
	}

	//Move the file record to the new position & update file name
	{
		CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->RemoveKey(this->PathFull());
		if (!pRecord) 
		{ 
			SCFError(ErrorFileFailedRename); 
			return FALSE;
		}

		ParsePath(rNewName, *m_pDatafile, NULL, &m_Name, &m_Extension);

		m_pDatafile->m_pRecords->AtPut(this->PathFull(), *pRecord);
	}

	return TRUE;
}

bool CDFFile::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
	if (bPathHasName)
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_pRecords->ContainsName(rNewPath))
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}

		//Move the file record to the new position & update file name
		{
			CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->RemoveKey(this->PathFull());
			if (!pRecord)
			{
				SCFError(ErrorFileFailedMove);
				return FALSE;
			}

			ParsePath(rNewPath, *m_pDatafile, &m_Path, &m_Name, &m_Extension);

			m_pDatafile->m_pRecords->AtPut(this->PathFull(), *pRecord);
		}
	}
	else
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_pRecords->ContainsName(rNewPath + this->NameFull()))
		{
			SCFError(ErrorFileFailedMove);
			return FALSE;
		}

		//Move the file record to the new position & update file name
		{
			CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->RemoveKey(this->PathFull());
			if (!pRecord)
			{
				SCFError(ErrorFileFailedMove);
				return FALSE;
			}

			ParsePath(rNewPath, *m_pDatafile, &m_Path, NULL, NULL);

			m_pDatafile->m_pRecords->AtPut(this->PathFull(), *pRecord);
		}
	}

	return TRUE;
}

//Copy file in DF into an file inside the DF 
bool CDFFile::Copy(_INOUT CDFFile& rDestination, _IN bool bOverwriteExisting)
{	
	CRecordFile* pRecordCopy = (CRecordFile*)m_pDatafile->m_pRecords->At(rDestination.PathFull());

	if (!bOverwriteExisting && pRecordCopy)
	{
		SCFError(ErrorFileFailedCopy);
		return FALSE;
	}

	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedCopy); 
		return FALSE;
	}

	if (!pRecordCopy)
	{
		pRecordCopy = new CRecordFile(*pRecord);
		if (!pRecordCopy)
		{ 
			SCFError(ErrorObjectFailedCreation); 
			return FALSE;
		}
	}

	m_pDatafile->m_pRecords->AtPut(rDestination.PathFull(), *pRecordCopy);

	return TRUE;
}

//Copy file in DF into an file outside the DF 
bool CDFFile::Copy(_INOUT CFile& rDestination, _IN bool bOverwriteExisting)
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_pRecords->At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedCopy); 
		return FALSE;
	}

	//Check whether th file is already stored in the datafile..
	if (!pRecord->m_pSource)
	{
		//..if it is stored, extract it & store it in the specified target file
		CMemoryBlock IOBuffer(m_pDatafile->IOBufferSize());

		CStreamFileRead  DFStreamRead(m_pDatafile->PathFull(), pRecord->m_ui64DataOffset, pRecord->m_ui64DataSize);
		CStreamFileWrite FileStreamWrite(rDestination.PathFull());
	
		while (DFStreamRead.BytesLeft() > 0)
		{
			SCF::UINT uiBytesToTransfer = (DFStreamRead.BytesLeft() > m_pDatafile->IOBufferSize()) ? (m_pDatafile->IOBufferSize()) : ((SCF::UINT)DFStreamRead.BytesLeft());

			DFStreamRead.GetBytes   (IOBuffer.Value(), uiBytesToTransfer);
			FileStreamWrite.PutBytes(IOBuffer.Value(), uiBytesToTransfer);
		}
	}
	else
	{
		//..if not, copy the source file into the specified target file
		switch (pRecord->m_pSource->ClassKey())
		{
		case ClassFile:
			{
				return ((CFile*)pRecord->m_pSource)->Copy(rDestination, bOverwriteExisting);
			}
		default: 
			{ 
				SCFError(ErrorDFFileSourceUnsupported); 
				return FALSE;
			}
		}
	}

	return TRUE;
}

void CDFFile::Serialize(_INOUT IStreamWrite& rStream) const
{
	m_Path.Serialize(rStream);
	m_Name.Serialize(rStream);
	m_Extension.Serialize(rStream);
}

void CDFFile::Deserialize(_INOUT IStreamRead& rStream)
{
	m_Path.Deserialize(rStream);
	m_Name.Deserialize(rStream);
	m_Extension.Deserialize(rStream);
}