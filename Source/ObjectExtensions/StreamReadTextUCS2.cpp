#include "StreamReadTextUCS2.h"

using namespace SCFBase;

CStreamReadTextUCS2::CStreamReadTextUCS2(_INOUT IStreamRead& rStreamRead) : CStreamReadText(rStreamRead)
{
	if (m_pStream->GetChar() != 0xfeff)
	{
		m_pStream->UnGetBytes(sizeof(TCHAR));
	}
}

CStreamReadTextUCS2::~CStreamReadTextUCS2()
{
}

bool CStreamReadTextUCS2::GetString(_OUT CString& rOutString, _IN UINT uiLength)
{
	if (m_pStream->BytesLeft() > 1)
	{
		UINT uiBytes = (uiLength != 0) ? (__min((UINT)m_pStream->BytesLeft(), uiLength * sizeof(TCHAR))) : ((UINT)m_pStream->BytesLeft()); 
		rOutString.Resize(uiBytes / sizeof(TCHAR));

		m_pStream->GetBytes((BYTE*)rOutString.Value(), uiBytes);
		return true;
	}

	return false; 
}

bool CStreamReadTextUCS2::GetLine(_OUT CString& rOutString)
{
	if (m_pStream->BytesLeft() > 1)
	{
		rOutString.CharsReserve(256);
		UINT uiIndex = 0;

		while (m_pStream->BytesLeft() > 0)
		{
			TCHAR cChar = m_pStream->GetChar();

			if (cChar == '\n')
			{
				rOutString.Resize(uiIndex);
				return true;
			}
			else
			{
				if (rOutString.CharsReserved() <= uiIndex)
				{
					rOutString.CharsReserve(rOutString.CharsReserved() + 256);
				}

				rOutString.AtPut(uiIndex, cChar);
				uiIndex++;
			}
		}

		rOutString.Resize(uiIndex);
		return true;
	}

	return false;
}

TCHAR CStreamReadTextUCS2::GetChar()
{
	if (m_pStream->BytesLeft() > 1) { return m_pStream->GetWord(); }

	return 0;
}

bool CStreamReadTextUCS2::CharsLeft() _GET
{
	return (m_pStream->BytesLeft() > 1);
}