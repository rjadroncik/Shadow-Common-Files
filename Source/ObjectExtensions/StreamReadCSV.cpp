#include "StreamReadCSV.h"

#include "StringRange.h"

using namespace SCFBase;

TCHAR StreamReadCSV_usSeparator = ';';

void       CStreamReadCSV::Separator(_IN TCHAR usChar) { StreamReadCSV_usSeparator = usChar; }
TCHAR CStreamReadCSV::Separator()                      { return StreamReadCSV_usSeparator; }


CStreamReadCSV::CStreamReadCSV(_INOUT IStreamReadText& rStreamRead)
{
	m_pStream = &rStreamRead;

	m_uiBlockNextChar = 0;
}

CStreamReadCSV::~CStreamReadCSV()
{
}

bool CStreamReadCSV::GetValue(_OUT CString& rOutString)
{
	if (m_uiBlockNextChar >= m_csBlock.Length())
	{
		return FALSE;
	}

	for (UINT uiChar = m_uiBlockNextChar; uiChar < m_csBlock.Length(); uiChar++)
	{
		if (m_csBlock[uiChar] == StreamReadCSV_usSeparator)
		{
			rOutString = CStringRange(m_csBlock, m_uiBlockNextChar, uiChar - m_uiBlockNextChar);
			
			m_uiBlockNextChar = uiChar + 1;
			return TRUE;
		}
	}

	rOutString = CStringRange(m_csBlock, m_uiBlockNextChar, m_csBlock.Length() - m_uiBlockNextChar);
	
	m_uiBlockNextChar = m_csBlock.Length();
	return TRUE;
}

bool CStreamReadCSV::BlockNext()
{
	m_uiBlockNextChar = 0;
	bool bRetVal = m_pStream->GetLine(m_csBlock);

	if (m_csBlock[m_csBlock.Length() - 1] == StreamReadCSV_usSeparator) { m_csBlock.Resize(m_csBlock.Length() - 1); }

	return bRetVal;
}
