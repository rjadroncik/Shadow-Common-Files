#include "Datafile.h"

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
			return true;
		}
		else 
		{
			SCFError(ErrorBadPath);
			return false;
		}
	}

	if (iNamePos != -1)
	{
		UINT uiRelPathStart = 0;

		UINT uiCWDEnd = __max(0, (int)rDatafile.CWD().Length() - 2);
		CString csCWD(rDatafile.CWD(), (bool)false);

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

	return true;
}

bool CDFFile::Writable() _GET { return true; }

bool CDFFile::Writable(_IN bool bWritable) _SET 
{
	SCF_UNREFERENCED_PARAMETER(bWritable);
	SCFError(ErrorFileFailedWritableSet); 
	return false;
}

bool CDFFile::Encrypted() _GET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorDFFileFailedAttributeGet); 
		return false;
	}

	return pRecord->Encrypted();
}

bool CDFFile::Encrypted(_IN bool bEncrypted) _SET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorDFFileFailedAttributeSet); 
		return false;
	}

	pRecord->Encrypted(bEncrypted);
	return true;
}

bool CDFFile::Compressed() _GET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorDFFileFailedAttributeGet); 
		return false;
	}

	return pRecord->Compressed();
}

bool CDFFile::Compressed(_IN bool bCompressed) _SET
{ 
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorDFFileFailedAttributeSet); 
		return false;
	}

	pRecord->Compressed(bCompressed);
	return true;
}

bool CDFFile::Exists() _GET { return m_pDatafile->m_Records.ContainsName(this->PathFull()); }

UINT64 CDFFile::Size() _GET
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedSizeGet); 
		return false;
	}

	return pRecord->Size();
}

bool CDFFile::Create(_IN CFile& rSource, _IN bool bSourceIsFileSystem, _IN bool bEraseExisting)
{
	CString csPathFull(this->PathFull());

	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(csPathFull);
	if (pRecord)
	{
		if (bEraseExisting)
		{ 
			m_pDatafile->m_Records.RemoveKey(csPathFull); 
			delete pRecord; 
		}
		else
		{ 
			SCFError(ErrorFileFailedCreate); 
			return false;
		}
	}

	pRecord = new CRecordFile(rSource, 0x00, bSourceIsFileSystem);
	if (!pRecord)
	{ 
		SCFError(ErrorObjectFailedCreation);
		return false;
	}

	m_pDatafile->m_Records.AtPut(csPathFull, *pRecord);

	return true;
}

bool CDFFile::Delete()
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.RemoveKey(this->PathFull());
	if (!pRecord)
	{
		SCFError(ErrorFileFailedDelete); 
		return false;
	}

	delete pRecord;

	return true;
}

bool CDFFile::Rename(_IN CString& rNewName)
{
	//Check whether a file with the given new name already exists
	if (m_pDatafile->m_Records.ContainsName(this->Path() + STRING("\\") + rNewName))
	{
		SCFError(ErrorFileFailedRename);
		return false;
	}

	//Move the file record to the new position & update file name
	{
		CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.RemoveKey(this->PathFull());
		if (!pRecord) 
		{ 
			SCFError(ErrorFileFailedRename); 
			return false;
		}

		ParsePath(rNewName, *m_pDatafile, nullptr, &m_Name, &m_Extension);

		m_pDatafile->m_Records.AtPut(this->PathFull(), *pRecord);
	}

	return true;
}

bool CDFFile::Move(_IN CString& rNewPath, _IN bool bPathHasName)
{
	if (bPathHasName)
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_Records.ContainsName(rNewPath))
		{
			SCFError(ErrorFileFailedMove);
			return false;
		}

		//Move the file record to the new position & update file name
		{
			CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.RemoveKey(this->PathFull());
			if (!pRecord)
			{
				SCFError(ErrorFileFailedMove);
				return false;
			}

			ParsePath(rNewPath, *m_pDatafile, &m_Path, &m_Name, &m_Extension);

			m_pDatafile->m_Records.AtPut(this->PathFull(), *pRecord);
		}
	}
	else
	{
		//Check whether a file with the given new name already exists
		if (m_pDatafile->m_Records.ContainsName(rNewPath + this->NameFull()))
		{
			SCFError(ErrorFileFailedMove);
			return false;
		}

		//Move the file record to the new position & update file name
		{
			CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.RemoveKey(this->PathFull());
			if (!pRecord)
			{
				SCFError(ErrorFileFailedMove);
				return false;
			}

			ParsePath(rNewPath, *m_pDatafile, &m_Path, nullptr, nullptr);

			m_pDatafile->m_Records.AtPut(this->PathFull(), *pRecord);
		}
	}

	return true;
}

//Copy file in DF into an file inside the DF 
bool CDFFile::Copy(_INOUT CDFFile& rDestination, _IN bool bOverwriteExisting)
{	
	CRecordFile* pRecordCopy = (CRecordFile*)m_pDatafile->m_Records.At(rDestination.PathFull());

	if (!bOverwriteExisting && pRecordCopy)
	{
		SCFError(ErrorFileFailedCopy);
		return false;
	}

	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedCopy); 
		return false;
	}

	if (!pRecordCopy)
	{
		pRecordCopy = new CRecordFile(*pRecord);
		if (!pRecordCopy)
		{ 
			SCFError(ErrorObjectFailedCreation); 
			return false;
		}
	}

	m_pDatafile->m_Records.AtPut(rDestination.PathFull(), *pRecordCopy);

	return true;
}

//Copy file in DF into an file outside the DF 
bool CDFFile::Copy(_INOUT CFile& rDestination, _IN bool bOverwriteExisting)
{
	CRecordFile* pRecord = (CRecordFile*)m_pDatafile->m_Records.At(this->PathFull());
	if (!pRecord) 
	{ 
		SCFError(ErrorFileFailedCopy); 
		return false;
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
			UINT uiBytesToTransfer = (DFStreamRead.BytesLeft() > m_pDatafile->IOBufferSize()) ? (m_pDatafile->IOBufferSize()) : ((UINT)DFStreamRead.BytesLeft());

			DFStreamRead.GetBytes   (IOBuffer.Value(), uiBytesToTransfer);
			FileStreamWrite.PutBytes(IOBuffer.Value(), uiBytesToTransfer);
		}
	}
	else
	{
		//..if not, copy the source file into the specified target file
		if (pRecord->m_bSourceIsFileSystem)
		{
			return ((CFile*)pRecord->m_pSource)->Copy(rDestination, bOverwriteExisting);
		}
		else			
		{ 
			SCFError(ErrorDFFileSourceUnsupported); 
			return false;
		}
	}

	return true;
}

//void CDFFile::Serialize(_INOUT IStreamWrite& rStream) const
//{
//	m_Path.Serialize(rStream);
//	m_Name.Serialize(rStream);
//	m_Extension.Serialize(rStream);
//}
//
//void CDFFile::Deserialize(_INOUT IStreamRead& rStream)
//{
//	m_Path.Deserialize(rStream);
//	m_Name.Deserialize(rStream);
//	m_Extension.Deserialize(rStream);
//}