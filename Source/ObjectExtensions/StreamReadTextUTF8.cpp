#include "StreamReadTextUTF8.h"
#include "String.h"

using namespace SCFBase;

CStreamReadTextUTF8::CStreamReadTextUTF8(_INOUT IStreamRead& rStreamRead) : CStreamReadText(rStreamRead)
{
	if (m_pStream->GetByte() != 0xef) { m_pStream->UnGetBytes(1); }
	if (m_pStream->GetByte() != 0xbb) { m_pStream->UnGetBytes(1); }
	if (m_pStream->GetByte() != 0xbf) { m_pStream->UnGetBytes(1); }
}

CStreamReadTextUTF8::~CStreamReadTextUTF8()
{
}

inline TCHAR StreamReadTextUTF8_GetChar(_INOUT IStreamRead& rStream)
{
	/*
	U-00000000 - U-0000007F 	0xxxxxxx
	U-00000080 - U-000007FF 	110xxxxx 10xxxxxx
	U-00000800 - U-0000FFFF 	1110xxxx 10xxxxxx 10xxxxxx
	U-00010000 - U-001FFFFF 	11110xxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-00200000 - U-03FFFFFF 	111110xx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	U-04000000 - U-7FFFFFFF 	1111110x 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx 10xxxxxx
	*/

	BYTE ucByte1 = rStream.GetByte();
	if (ucByte1 < 0x00000080) { return ucByte1; }

	if ((ucByte1 & 0xE0) == 0xC0)
	{
		TCHAR usChar = (rStream.GetByte() & 0x3F);
		usChar |= (ucByte1 & 0x1F) << 6;

		return usChar;
	}

	TCHAR usChar = (rStream.GetByte() & 0x3F) << 6;
	usChar           |= (rStream.GetByte() & 0x3F);
	usChar           |= (ucByte1 & 0x0F) << 12;

	return usChar;
}

bool CStreamReadTextUTF8::GetString(_OUT CString& rOutString, _IN UINT uiLength)
{
	if (m_pStream->BytesLeft() > 0)
	{
		if (uiLength == 0)
		{
			rOutString.CharsReserve((UINT)m_pStream->BytesLeft());

			UINT uiIndex = 0;
			while (m_pStream->BytesLeft() > 0)
			{
				rOutString.AtPut(uiIndex, StreamReadTextUTF8_GetChar(*m_pStream));
				uiIndex++;
			}

			rOutString.Resize(uiIndex);
		}
		else
		{
			rOutString.Resize(uiLength);

			UINT uiIndex = 0;
			while ((m_pStream->BytesLeft() > 0) && (uiIndex < uiLength))
			{
				rOutString.AtPut(uiIndex, StreamReadTextUTF8_GetChar(*m_pStream));
				uiIndex++;
			}
		}

		return TRUE;
	}

	return FALSE;
}

bool CStreamReadTextUTF8::GetLine(_OUT CString& rOutString)
{
	if (m_pStream->BytesLeft() > 0)
	{
		rOutString.CharsReserve(256);
		UINT uiIndex = 0;

		while (m_pStream->BytesLeft() > 0)
		{
			TCHAR usChar = StreamReadTextUTF8_GetChar(*m_pStream);

			if ((usChar == '\r') || (usChar == '\n'))
			{
				if (m_pStream->BytesLeft() > 0)
				{
					//Properly handle fucked up [\r\n] combinations
					usChar = m_pStream->GetByte();
					if ((usChar != '\r') && (usChar != '\n')) { m_pStream->UnGetBytes(1); }
				}

				rOutString.Resize(uiIndex);
				return TRUE;
			}
			else
			{
				if (rOutString.CharsReserved() <= uiIndex)
				{
					rOutString.CharsReserve(rOutString.CharsReserved() + 256);
				}

				rOutString.AtPut(uiIndex, usChar);
				uiIndex++;
			}
		}

		rOutString.Resize(uiIndex);
		return TRUE;
	}

	return FALSE; 
}

TCHAR CStreamReadTextUTF8::GetChar()
{
	if (m_pStream->BytesLeft() > 0) { return StreamReadTextUTF8_GetChar(*m_pStream); }

	return 0;
}

bool CStreamReadTextUTF8::CharsLeft() _GET
{
	return (m_pStream->BytesLeft() > 0);
}
