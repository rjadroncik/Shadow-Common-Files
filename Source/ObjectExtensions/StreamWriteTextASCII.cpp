#include "StreamWriteTextASCII.h"

using namespace SCFBase;

CStreamWriteTextASCII::CStreamWriteTextASCII(_INOUT IStreamWrite& rStreamWrite) : CStreamWriteText(rStreamWrite)
{
}

CStreamWriteTextASCII::~CStreamWriteTextASCII()
{
}

void CStreamWriteTextASCII::PutString(_IN CString& rString)
{
	for (UINT i = 0; i < rString.Length(); i++)
	{
		m_pStream->PutByte((BYTE)(rString[i]));
	}
}

void CStreamWriteTextASCII::PutString(_IN LPTSTR szString)
{
	LPTSTR szCurrent = szString;
	while (*szCurrent)
	{
		m_pStream->PutByte((BYTE)*szCurrent);
	}
}

void CStreamWriteTextASCII::PutString(_IN LPTSTR szString, _IN UINT uiLength)
{
	for (UINT i = 0; i < uiLength; i++)
	{
		m_pStream->PutByte((BYTE)(szString[i]));
	}
}

void CStreamWriteTextASCII::PutLine(_IN CString& rString)
{
	for (UINT i = 0; i < rString.Length(); i++)
	{
		m_pStream->PutByte((BYTE)(rString[i]));
	}

	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((BYTE)'\n'); }
}

void CStreamWriteTextASCII::PutLine()
{
	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((BYTE)'\n'); }
}

void CStreamWriteTextASCII::PutChar(_IN TCHAR cChar)
{
	m_pStream->PutByte((BYTE)cChar);
}

UINT CStreamWriteTextASCII::CharsWritten() _GET
{
	return (UINT)m_pStream->BytesWritten();
}