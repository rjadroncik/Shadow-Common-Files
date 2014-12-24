#include "Datafile.h"

using namespace SCF;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CRecordDirectory::CRecordDirectory(_IN CRecordDirectory& rDirectory)
{
	m_ucAttributes = rDirectory.m_ucAttributes;
}

CRecordDirectory::CRecordDirectory()
{
	m_ucAttributes = FALSE;
}

CRecordDirectory::CRecordDirectory(_IN BYTE ucAttributes)
{
	m_ucAttributes = ucAttributes;
}

CRecordDirectory::~CRecordDirectory()
{
}

void CRecordDirectory::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutByte(m_ucAttributes);
}

void CRecordDirectory::Deserialize(_INOUT IStreamRead& rStream)
{
	m_ucAttributes = rStream.GetByte();
}




