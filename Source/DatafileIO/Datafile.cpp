#include "Datafile.h"
#include "StreamFileRead.h"

using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;
using namespace SCFXML;


#define FOURCC "SDF1"

CDatafile::CDatafile(_IN CString& rFullNameOrPath, _IN bool bKeepFileOpen) : CFile(rFullNameOrPath)
{
	m_pRecords = new CDictionaryString<CRecord>();
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
	m_pRecords = new CDictionaryString<CRecord>();
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

	m_pRecords->AllDelete();
	delete m_pRecords;

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

	CEnumeratorDictionaryString<CRecord> Enumerator(*m_pRecords);
	while (Enumerator.Next())
	{
		switch (Enumerator.Current()->Type())
		{
		case RecordFile:
			{
				FileWrite(Enumerator, IOBuffer, StreamWrite, ui64HeaderSize);
				break;
			}
		}
	}

	return TRUE;
}

bool CDatafile::FileWrite(_IN CEnumeratorDictionaryString<CRecord>& rEnumerator, _INOUT CMemoryBlock& rIOBuffer, _OUT CStreamFileWrite& rStreamWrite, _IN UINT64 ui64HeaderSize)
{
	CRecordFile* pRecord = (CRecordFile*)rEnumerator.Current();

	if (pRecord->m_pSource)
	{
		if (pRecord->m_bSourceIsFileSystem)
		{
			CStreamFileRead StreamRead(*(CFile*)pRecord->m_pSource);

			pRecord->m_ui64Size = StreamRead.BytesLeft();
			pRecord->m_ui64DataOffset = ui64HeaderSize + rStreamWrite.BytesWritten();

			if (!(pRecord->m_ucAttributes & AttributeCompressed) &&
				!(pRecord->m_ucAttributes & AttributeEncrypted))
			{
				pRecord->m_ui64DataSize = FileWritePassThrough(StreamRead, rIOBuffer, rStreamWrite);
			}

			//Delete source object
			delete pRecord->m_pSource; pRecord->m_pSource = NULL;
			return TRUE;
		}
		else 
		{
			//Delete source object
			delete pRecord->m_pSource; pRecord->m_pSource = NULL;

			SCFError(ErrorDFFileSourceUnsupported);
			return FALSE;
		}
	}
	else
	{
		CDFStreamFileRead StreamRead(*this, *pRecord);
	
		pRecord->m_ui64DataOffset = ui64HeaderSize + rStreamWrite.BytesWritten();

		if (((pRecord->m_ucAttributes & AttributeCompressed) == (pRecord->m_ucAttributesApplied & AttributeCompressed)) &&
			((pRecord->m_ucAttributes & AttributeEncrypted)  == (pRecord->m_ucAttributesApplied & AttributeEncrypted)))
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
	else { return FALSE; }
}

bool CDatafile::Read()
{
	m_pRecords->AllDelete();
	delete m_pRecords;

	CStreamFileRead StreamRead(*this); 

	return HeaderRead(StreamRead.FileHandle());
}

UINT64 CDatafile::HeaderSize()
{
	//Here we just measure the size of the header, no writing is done yet!
	CStreamDummyWrite    StreamWrite;
	CStreamWriteTextUTF8 StreamWriteText(StreamWrite);

	CXMLDocument XmlDocument;
	CXMLStreamWriteObject XmlStream(XmlDocument);


	StreamWrite.PutBytes(FOURCC, 4);
	StreamWrite.PutByte(m_ucAttributes);


	for()

	XmlStream.Next(*m_pRecords);

	return StreamWrite.BytesWritten();
}

bool CDatafile::HeaderWrite(_INOUT void* hFile)
{
	//Finally we store the header with the updated file records at the
	//beginning of the file
	CStreamFileWrite   StreamWrite(hFile, (UINT64)0);

	StreamWrite.PutBytes(FOURCC, 4);
	StreamWrite.PutByte(m_ucAttributes);

	StreamWriteObject.Next(*m_pRecords);

	return TRUE;
}

bool CDatafile::HeaderRead(_INOUT void* hFile)
{
	CStreamFileRead   StreamRead(hFile);
	CStreamReadObject StreamReadObject(StreamRead);

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
	StreamReadObject.Next();
	m_pRecords = (CDictionaryString<CRecord>*)StreamReadObject.Current();

	return TRUE;
}
