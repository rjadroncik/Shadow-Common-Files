#include "StreamWriteCSV.h"

using namespace SCFBase;

TCHAR StreamWriteCSV_usSeparator = ';';

void       CStreamWriteCSV::Separator(_IN TCHAR usChar) { StreamWriteCSV_usSeparator = usChar; }
TCHAR CStreamWriteCSV::Separator()                      { return StreamWriteCSV_usSeparator; }

CStreamWriteCSV::CStreamWriteCSV(_INOUT IStreamWriteText& rStreamWrite)
{
	m_pStream = &rStreamWrite;
}

CStreamWriteCSV::~CStreamWriteCSV()
{
	if (m_csBlock.Length()) { m_pStream->PutString(m_csBlock); }
}

void CStreamWriteCSV::PutValue(_IN CObject& rValue)
{
	if (m_csBlock.Length()) { m_csBlock += StreamWriteCSV_usSeparator; }

	m_csBlock += rValue.ToString();
}

void CStreamWriteCSV::BlockNext()
{
	if (m_csBlock.Length()) 
	{ 
		m_pStream->PutLine(m_csBlock);

		m_csBlock.AtPut(0, 0);
		m_csBlock.LengthScan();
	}
}

void CStreamWriteCSV::Close()
{ 
	if (m_csBlock.Length()) { m_pStream->PutString(m_csBlock); }
	m_pStream->Close();
}
