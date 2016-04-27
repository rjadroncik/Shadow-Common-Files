#include "Datafile.h"

using namespace SCF;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;

CRecordFile::CRecordFile(_IN CRecordFile& rFile)
{
	m_ucAttributes        = rFile.m_ucAttributes;
	m_ucAttributesApplied = rFile.m_ucAttributesApplied;

	m_ui64Size = rFile.m_ui64Size;

	if (rFile.m_pSource)
	{
		switch (rFile.m_pSource->ClassKey())
		{
		case ClassFile:   { m_pSource = new CFile  ((const   CFile&)*rFile.m_pSource); break; }
		case ClassDFFile: { m_pSource = new CDFFile((const CDFFile&)*rFile.m_pSource); break; }
		default:          { m_pSource = NULL; break; }
		}
	}
	else { m_pSource = NULL; }

	m_ui64DataOffset = rFile.m_ui64DataOffset;
	m_ui64DataSize   = rFile.m_ui64DataSize;
}

CRecordFile::CRecordFile()
{
	m_ucAttributes        = 0x00;
	m_ucAttributesApplied = 0x00;

	m_ui64Size = 0;

	m_pSource = NULL;

	m_ui64DataOffset = 0;
	m_ui64DataSize   = 0;
}

CRecordFile::CRecordFile(_IN CFile& rSource, _IN SCF::BYTE ucAttributes)
{
	m_ucAttributes        = ucAttributes;
	m_ucAttributesApplied = ucAttributes;

	m_ui64Size = rSource.Size();

	switch (rSource.ClassKey())
	{
	case ClassFile:   { m_pSource = new CFile  ((const   CFile&)rSource); break; }
	case ClassDFFile: { m_pSource = new CDFFile((const CDFFile&)rSource); break; }
	default:          { m_pSource = NULL; break; }
	}
	
	m_ui64DataOffset = 0;
	m_ui64DataSize   = 0;
}

CRecordFile::~CRecordFile()
{
	if (m_pSource) { delete m_pSource; }
}

bool CRecordFile::Compressed() _GET { return (m_ucAttributes & AttributeCompressed) != 0; }

bool CRecordFile::Compressed(_IN bool bCompressed) _SET 
{ 
	m_ucAttributes |= AttributeCompressed;
	if (!bCompressed) { m_ucAttributes ^= AttributeCompressed; }

	return TRUE;
}

void CRecordFile::Serialize(_INOUT IStreamWrite& rStream) const
{
	rStream.PutInt64(m_ui64Size);
	rStream.PutByte(m_ucAttributes);

	rStream.PutInt64(m_ui64DataOffset);
	rStream.PutInt64(m_ui64DataSize);	
}

void CRecordFile::Deserialize(_INOUT IStreamRead& rStream)
{
	m_ui64Size     = rStream.GetInt64();
	m_ucAttributes = rStream.GetByte();

	m_ui64DataOffset = rStream.GetInt64();
	m_ui64DataSize   = rStream.GetInt64();	
}