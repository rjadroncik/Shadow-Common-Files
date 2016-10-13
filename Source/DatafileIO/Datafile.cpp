#include "Datafile.h"
#include "StreamFileRead.h"

using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;
using namespace SCFBase;

#define FOURCC "SDF1"

CDatafile::CDatafile(_IN CString& rFullNameOrPath, _IN bool bKeepFileOpen) : CFile(rFullNameOrPath)
{
	m_ucAttributes = 0x00;

	m_uiIOBufferSize = 4096;
	m_pEncryptionKey = NULL;

	CDFDirectory Root(*this, STRING("\\"));
	Root.Create();

	m_CWD = STRING("\\");

	if (this->Exists() && bKeepFileOpen) { m_pStreamRead = new CStreamFileRead(*this); }
	else                                 { m_pStreamRead = NULL; }
}

CDatafile::CDatafile(_IN CString& rPath, _IN CString& rName, _IN CString& rExtension, _IN bool bKeepFileOpen) : CFile(rPath, rName, rExtension)
{
	m_ucAttributes = 0x00;

	m_uiIOBufferSize = 4096;
	m_pEncryptionKey = NULL;

	CDFDirectory Root(*this, STRING("\\"));
	Root.Create();

	m_CWD = STRING("\\");

	if (this->Exists() && bKeepFileOpen) { m_pStreamRead = new CStreamFileRead(*this); }
	else                                 { m_pStreamRead = NULL; }
}

CDatafile::~CDatafile()
{
	if (m_pEncryptionKey) { delete m_pEncryptionKey; }

	m_Records.AllDelete();

	if (m_pStreamRead) { delete m_pStreamRead; }
}

bool CDatafile::FilesWrite(_INOUT void* hFile)
{
	UINT64 ui64HeaderSize = HeaderSize();

	//Here we write all the files & directories into the temp file at their 
	//respective locations & update the file records in memory with the 
	//correct values
	CMemoryBlock IOBuffer(m_uiIOBufferSize);
	CStreamFileWrite StreamWrite(hFile, ui64HeaderSize);

	CEnumeratorDictionaryString<CRecord> enumerator(m_Records);
	while (enumerator.Next())
	{
		if (enumerator.Current()->Type() == RecordFile)
		{
			FileWrite(*(CRecordFile*)enumerator.Current(), IOBuffer, StreamWrite, ui64HeaderSize);
		}
	}

	return TRUE;
}

bool CDatafile::FileWrite(_INOUT CRecordFile& rRecord, _INOUT CMemoryBlock& rIOBuffer, _OUT CStreamFileWrite& rStreamWrite, _IN UINT64 ui64HeaderSize)
{
	if (rRecord.m_pSource)
	{
		if (rRecord.m_bSourceIsFileSystem)
		{
			CStreamFileRead StreamRead(*(CFile*)rRecord.m_pSource);

			rRecord.m_ui64Size = StreamRead.BytesLeft();
			rRecord.m_ui64DataOffset = ui64HeaderSize + rStreamWrite.BytesWritten();

			if (!(rRecord.m_ucAttributes & AttributeCompressed) &&
				!(rRecord.m_ucAttributes & AttributeEncrypted))
			{
				rRecord.m_ui64DataSize = FileWritePassThrough(StreamRead, rIOBuffer, rStreamWrite);
			}

			//Delete source object
			delete rRecord.m_pSource; rRecord.m_pSource = NULL;
			return TRUE;
		}
		else 
		{
			//Delete source object
			delete rRecord.m_pSource; rRecord.m_pSource = NULL;

			SCFError(ErrorDFFileSourceUnsupported);
			return FALSE;
		}
	}
	else
	{
		CDFStreamFileRead StreamRead(*this, rRecord);
	
		rRecord.m_ui64DataOffset = ui64HeaderSize + rStreamWrite.BytesWritten();

		if (((rRecord.m_ucAttributes & AttributeCompressed) == (rRecord.m_ucAttributesApplied & AttributeCompressed)) &&
			((rRecord.m_ucAttributes & AttributeEncrypted)  == (rRecord.m_ucAttributesApplied & AttributeEncrypted)))
		{
			FileWritePassThrough(StreamRead, rIOBuffer, rStreamWrite);
		}

		return TRUE;
	}
}

UINT64 CDatafile::FileWritePassThrough(_INOUT IStreamRead& rStreamRead, _INOUT CMemoryBlock& rIOBuffer, _OUT IStreamWrite& rStreamWrite)
{
	UINT64 ui64BytesWriten = 0;

	while (rStreamRead.BytesLeft() > 0)
	{
		UINT uiBytesToTransfer = (rStreamRead.BytesLeft() > m_uiIOBufferSize) ? (m_uiIOBufferSize) : ((UINT)rStreamRead.BytesLeft());

		rStreamRead.GetBytes (rIOBuffer.Value(), uiBytesToTransfer);
		rStreamWrite.PutBytes(rIOBuffer.Value(), uiBytesToTransfer);

		ui64BytesWriten += uiBytesToTransfer;
	}

	return ui64BytesWriten;
}

bool CDatafile::WriteAs(_INOUT CFile& rFile)
{
	CFile TempFile(rFile.TempPathFull());

	//Delete the read stream for the original file before we start saving
	if (m_pStreamRead) { delete m_pStreamRead; }

	//Create a write stream
	CStreamFileWrite* pStreamWrite = new CStreamFileWrite(TempFile); 

	if (Write(pStreamWrite->FileHandle()))
	{
		//Delete the write stream once we are finished writing
		delete pStreamWrite;
	
		//Delete the original/target file if it exists
		if (rFile.Exists()) { rFile.Delete(); }

		//Replace the original/target file with the temp file
		TempFile.Rename(rFile.NameFull());
	}

	//The datafile is now assigned to the new file
	m_Path      = rFile.Path();
	m_Name      = rFile.Name();
	m_Extension = rFile.Extension();

	//Re-create the read stream for the original file after saving
	if (m_pStreamRead) { m_pStreamRead = new CStreamFileRead(*this); }

	return TRUE;
}

bool CDatafile::Write()
{
	return WriteAs(*this);
}

bool CDatafile::Write(_INOUT void* hFile)
{
	if (FilesWrite(hFile))
	{
		return HeaderWrite(hFile);
	}

	return FALSE;
}

bool CDatafile::Read()
{
	m_Records.AllDelete();

	CStreamFileRead StreamRead(*this); 

	return HeaderRead(StreamRead.FileHandle());
}

UINT64 CDatafile::HeaderSize()
{
	//Here we just measure the size of the header, no writing is done yet!
	CStreamDummyWrite stream;

	stream.PutBytes(FOURCC, 4);
	stream.PutByte(m_ucAttributes);
	
	RecordsWrite(stream);

	return stream.BytesWritten();
}

bool CDatafile::HeaderWrite(_INOUT void* hFile)
{
	//Finally we store the header with the updated file records at the
	//beginning of the file
	CStreamFileWrite stream(hFile, (UINT64)0);

	stream.PutBytes(FOURCC, 4);
	stream.PutByte(m_ucAttributes);

	RecordsWrite(stream);

	return TRUE;
}

bool CDatafile::HeaderRead(_INOUT void* hFile)
{
	CStreamFileRead StreamRead(hFile);

	BYTE Buffer[4];
	StreamRead.GetBytes(Buffer, 4);

	if (CMemory::Compare(Buffer, FOURCC, 3)) 
	{ 
		SCFError(ErrorDatafileFormatWrong);
		return FALSE;
	}	

	if ((Buffer[3] > '0') && (Buffer[3] < '9'))
	{
		if (Buffer[3] < '1') { SCFError(ErrorDatafileFormatOld); return FALSE; }
		if (Buffer[3] > '1') { SCFError(ErrorDatafileFormatNew); return FALSE; }
	}
	else 
	{ 
		SCFError(ErrorDatafileFormatWrong); 
		return FALSE;
	}	

	m_ucAttributes = StreamRead.GetByte();

	RecordsRead(StreamRead);

	return TRUE;
}

void CDatafile::RecordsWrite(_INOUT IStreamWrite& rStream)
{
	rStream.PutInt(m_Records.Size());

	CEnumeratorDictionaryString<CRecord> enumerator(m_Records);

	while (enumerator.Next())
	{
		CString path(enumerator.CurrentPath(), FALSE, TRUE);

		rStream.PutInt(path.Length());
		rStream.PutBytes(enumerator.CurrentPath().Value(), path.BytesReserved());

		RecordType eRecordType = enumerator.Current()->Type();
		rStream.PutByte(eRecordType);

		if (eRecordType == RecordFile)
		{
			CRecordFile& rFile = *(CRecordFile*)enumerator.Current();

			rStream.PutInt64(rFile.m_ui64Size);
			rStream.PutByte(rFile.m_ucAttributes);
			rStream.PutInt64(rFile.m_ui64DataOffset);
			rStream.PutInt64(rFile.m_ui64DataSize);
		}
		else
		{
			CRecordDirectory& rDirectory = *(CRecordDirectory*)enumerator.Current();

			rStream.PutByte(rDirectory.m_ucAttributes);
		}
	}
}

void CDatafile::RecordsRead(_INOUT IStreamRead& rStream)
{
	UINT uiCount = rStream.GetInt();

	for (UINT i = 0; i < uiCount; i++)
	{
		CString path;
		path.Resize(rStream.GetInt());

		rStream.GetBytes(path.Value(), path.BytesReserved());

		RecordType eRecordType = (RecordType)rStream.GetByte();

		if (eRecordType == RecordFile)
		{
			CRecordFile& rFile = *(new CRecordFile());

			rFile.m_ui64Size = rStream.GetInt64();
			rFile.m_ucAttributes = rStream.GetByte();
			rFile.m_ui64DataOffset = rStream.GetInt64();
			rFile.m_ui64DataSize = rStream.GetInt64();

			m_Records.AtPut(path, rFile);
		}
		else
		{
			CRecordDirectory& rDirectory = *(new CRecordDirectory());

			rDirectory.m_ucAttributes = rStream.GetByte();

			m_Records.AtPut(path, rDirectory);
		}
	}
}