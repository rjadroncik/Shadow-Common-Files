#include "Datafile.h"

using namespace SCFBase;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;
using namespace SCFXML;

CRecordFile::CRecordFile(_IN CRecordFile& rFile) : CRecord(RecordFile)
{
	m_ucAttributes        = rFile.m_ucAttributes;
	m_ucAttributesApplied = rFile.m_ucAttributesApplied;

	m_ui64Size = rFile.m_ui64Size;

	if (rFile.m_pSource)
	{
		if (rFile.m_bSourceIsFileSystem)
		{ 
			m_pSource = new CFile  ((const   CFile&)*rFile.m_pSource);
		}
		else 
		{
			m_pSource = new CDFFile((const CDFFile&)*rFile.m_pSource);
		}
	}
	else { m_pSource = NULL; }
	m_bSourceIsFileSystem = rFile.m_bSourceIsFileSystem;

	m_ui64DataOffset = rFile.m_ui64DataOffset;
	m_ui64DataSize   = rFile.m_ui64DataSize;
}

CRecordFile::CRecordFile() : CRecord(RecordDirectory)
{
	m_ucAttributes        = 0x00;
	m_ucAttributesApplied = 0x00;

	m_ui64Size = 0;

	m_pSource = NULL;
	m_bSourceIsFileSystem = false;

	m_ui64DataOffset = 0;
	m_ui64DataSize   = 0;
}

CRecordFile::CRecordFile(_IN CFile& rSource, _IN BYTE ucAttributes, IN bool bSourceIsFileSystem) : CRecord(RecordDirectory)
{
	m_ucAttributes        = ucAttributes;
	m_ucAttributesApplied = ucAttributes;

	m_ui64Size = rSource.Size();

	if (bSourceIsFileSystem)
	{
		m_pSource = new CFile  ((const   CFile&)*m_pSource);
	}
	else
	{
		m_pSource = new CDFFile((const CDFFile&)*m_pSource);
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

void CRecordFile::XMLSerialize(_INOUT IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("Attributes"), CInt(m_ucAttributes).ToString());
}

void CRecordFile::XMLDeserialize(_INOUT IXMLStreamRead& rReader)
{
	m_ucAttributes = CInt(*rReader.GetValue()).Value();
}

//void CRecordFile::Serialize(_INOUT IStreamWrite& rStream) const
//{
//	rStream.PutInt64(m_ui64Size);
//	rStream.PutByte(m_ucAttributes);
//
//	rStream.PutInt64(m_ui64DataOffset);
//	rStream.PutInt64(m_ui64DataSize);	
//}
//
//void CRecordFile::Deserialize(_INOUT IStreamRead& rStream)
//{
//	m_ui64Size     = rStream.GetInt64();
//	m_ucAttributes = rStream.GetByte();
//
//	m_ui64DataOffset = rStream.GetInt64();
//	m_ui64DataSize   = rStream.GetInt64();	
//}