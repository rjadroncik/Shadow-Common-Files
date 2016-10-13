#include "StreamFileRead.h"
#include "Datafile.h"
#include "RecordFile.h"

using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CDFStreamFileRead::CDFStreamFileRead(_IN CDFFile& rFile) : CStreamFileRead(rFile.m_pDatafile->PathFull())
{
	CRecordFile* pRecord = (CRecordFile*)(rFile.m_pDatafile->m_Records.At(rFile.PathFull()));
	if (pRecord) 
	{ 
		this->FilePointerSet((int)pRecord->m_ui64DataOffset);

		m_ui64BytesLeft = pRecord->m_ui64DataSize;
	}
	else { SCFError(ErrorFileFailedRead); }
}

CDFStreamFileRead::CDFStreamFileRead(_IN CFile& rFile, _IN CRecordFile& rFileRecord) : CStreamFileRead(rFile)
{
	this->FilePointerSet((int)rFileRecord.m_ui64DataOffset);

	m_ui64BytesLeft = rFileRecord.m_ui64DataSize;
}

CDFStreamFileRead::~CDFStreamFileRead()
{
	this->Close();
}