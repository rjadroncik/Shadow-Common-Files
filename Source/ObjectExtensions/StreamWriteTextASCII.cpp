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
	for (SCF::UINT i = 0; i < rString.Length(); i++)
	{
		m_pStream->PutByte((SCF::BYTE)(rString[i]));
	}
}

void CStreamWriteTextASCII::PutString(_IN SCF::LPTSTR szString)
{
	SCF::LPTSTR szCurrent = szString;
	while (*szCurrent)
	{
		m_pStream->PutByte((SCF::BYTE)*szCurrent);
	}
}

void CStreamWriteTextASCII::PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength)
{
	for (SCF::UINT i = 0; i < uiLength; i++)
	{
		m_pStream->PutByte((SCF::BYTE)(szString[i]));
	}
}

void CStreamWriteTextASCII::PutLine(_IN CString& rString)
{
	for (SCF::UINT i = 0; i < rString.Length(); i++)
	{
		m_pStream->PutByte((SCF::BYTE)(rString[i]));
	}

	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((SCF::BYTE)'\n'); }
}

void CStreamWriteTextASCII::PutLine()
{
	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((SCF::BYTE)'\n'); }
}

void CStreamWriteTextASCII::PutChar(_IN SCF::TCHAR cChar)
{
	m_pStream->PutByte((SCF::BYTE)cChar);
}

SCF::UINT CStreamWriteTextASCII::CharsWritten() _GET
{
	return (SCF::UINT)m_pStream->BytesWritten();
}