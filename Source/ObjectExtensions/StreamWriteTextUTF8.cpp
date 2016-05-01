#include "StreamWriteTextUTF8.h"

using namespace SCFBase;

inline void StreamWriteTextUTF8_WriteChar(_IN TCHAR ucChar, _OUT IStreamWrite& rStream)
{
	/*
	U-00000000 - U-0000007F 	0xxxxxxx
	U-00000080 - U-000007FF 	110xxxxx 10xxxxxx
	U-00000800 - U-0000FFFF 	1110xxxx 10xxxxxx 10xxxxxx
	U-00010000 - U-001FFFFF 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-00200000 - U-03FFFFFF 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-04000000 - U-7FFFFFFF 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/

	if (ucChar < 0x00000080) { rStream.PutByte((BYTE)ucChar); return; }
	if (ucChar < 0x00000800) 
	{ 
		USHORT usTransformed = 0x80C0;

		usTransformed |= ((ucChar << 2) & 0x1F00) >> 8;
		usTransformed |= ((ucChar)      & 0x003F) << 8;

		rStream.PutWord(usTransformed);
		return; 
	}

	BYTE usTransformed[3] = { 0xE0, 0x80, 0x80 };

	usTransformed[0] |= (BYTE)(ucChar >> 12) & 0x0F;
	usTransformed[1] |= (BYTE)(ucChar >> 6)  & 0x3F;
	usTransformed[2] |= (BYTE)(ucChar)       & 0x3F;

	rStream.PutBytes(usTransformed, 3);
}

CStreamWriteTextUTF8::CStreamWriteTextUTF8(_INOUT IStreamWrite& rStreamWrite, _IN bool bUseBOM) : CStreamWriteText(rStreamWrite)
{
	if (!rStreamWrite.Appending() && bUseBOM) 
	{
		StreamWriteTextUTF8_WriteChar(0xFEFF, *m_pStream);
	}
}

CStreamWriteTextUTF8::~CStreamWriteTextUTF8()
{
}

void CStreamWriteTextUTF8::PutString(_IN CString& rString)
{
	for (UINT i = 0; i < rString.Length(); i++)
	{
		StreamWriteTextUTF8_WriteChar(rString[i], *m_pStream);
	}
}

void CStreamWriteTextUTF8::PutString(_IN LPTSTR szString)
{
	LPTSTR szCurrent = szString;
	while (*szCurrent)
	{
		StreamWriteTextUTF8_WriteChar(*szCurrent, *m_pStream);
	}
}

void CStreamWriteTextUTF8::PutString(_IN LPTSTR szString, _IN UINT uiLength)
{
	for (UINT i = 0; i < uiLength; i++)
	{
		StreamWriteTextUTF8_WriteChar(szString[i], *m_pStream);
	}
}

void CStreamWriteTextUTF8::PutLine(_IN CString& rString)
{
	for (UINT i = 0; i < rString.Length(); i++)
	{
		StreamWriteTextUTF8_WriteChar(rString[i], *m_pStream);
	}

	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((BYTE)'\n'); }
}

void CStreamWriteTextUTF8::PutLine()
{
	if (m_bUseCR) { m_pStream->PutWord( '\r\n' ); }
	else          { m_pStream->PutByte((BYTE)'\n'); }
}

void CStreamWriteTextUTF8::PutChar(_IN TCHAR cChar)
{
	StreamWriteTextUTF8_WriteChar(cChar, *m_pStream);
}

UINT CStreamWriteTextUTF8::CharsWritten() _GET
{
	return (UINT)m_pStream->BytesWritten();
}