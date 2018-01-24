#include "Datafile.h"

using namespace SCFBase;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CRecordDirectory::CRecordDirectory(_IN CRecordDirectory& rDirectory) : CRecord(RecordDirectory)
{
	m_ucAttributes = rDirectory.m_ucAttributes;
}

CRecordDirectory::CRecordDirectory() : CRecord(RecordDirectory)
{
	m_ucAttributes = false;
}

CRecordDirectory::CRecordDirectory(_IN BYTE ucAttributes) : CRecord(RecordDirectory)
{
	m_ucAttributes = ucAttributes;
}

CRecordDirectory::~CRecordDirectory()
{
}
