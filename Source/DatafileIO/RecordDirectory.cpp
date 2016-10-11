#include "Datafile.h"

using namespace SCFBase;
using namespace SCFDatafileIO;
using namespace SCFDatafileIOPrivate;
using namespace SCFXML;

CRecordDirectory::CRecordDirectory(_IN CRecordDirectory& rDirectory) : CRecord(RecordDirectory)
{
	m_ucAttributes = rDirectory.m_ucAttributes;
}

CRecordDirectory::CRecordDirectory() : CRecord(RecordDirectory)
{
	m_ucAttributes = FALSE;
}

CRecordDirectory::CRecordDirectory(_IN BYTE ucAttributes) : CRecord(RecordDirectory)
{
	m_ucAttributes = ucAttributes;
}

CRecordDirectory::~CRecordDirectory()
{
}

void CRecordDirectory::XMLSerialize(_INOUT IXMLStreamWrite& rWriter) const
{
	rWriter.PutValue(STRING("Attributes"), *(new STRINGREF(CInt(m_ucAttributes).ToString())));
}

void CRecordDirectory::XMLDeserialize(_INOUT IXMLStreamRead& rReader)
{
	m_ucAttributes = (BYTE)CInt(*rReader.GetValue()).Value();
}




