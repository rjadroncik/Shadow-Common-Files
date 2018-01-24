#include "StreamReadTextASCII.h"
#include "String.h"

using namespace SCFBase;

CStreamReadTextASCII::CStreamReadTextASCII(_INOUT IStreamRead& rStreamRead) : CStreamReadText(rStreamRead)
{
}

CStreamReadTextASCII::~CStreamReadTextASCII()
{
}

bool CStreamReadTextASCII::GetString(_OUT CString& rOutString, _IN UINT uiLength)
{
	if (m_pStream->BytesLeft() > 0)
	{
		UINT uiBytes = (uiLength != 0) ? (__min((UINT)m_pStream->BytesLeft(), uiLength)) : ((UINT)m_pStream->BytesLeft()); 
		rOutString.Resize(uiBytes);

		m_pStream->GetBytes((BYTE*)rOutString.Value(), uiBytes);

		for (int i = (uiBytes - 1); i >= 0; i--)
		{
			rOutString.AtPut(i, ((BYTE*)rOutString.Value())[i]);
		}

		return true;
	}

	return false;
}

bool CStreamReadTextASCII::GetLine(_OUT CString& rOutString)
{
	if (m_pStream->BytesLeft() > 0)
	{
		rOutString.CharsReserve(256);
		UINT uiIndex = 0;

		while (m_pStream->BytesLeft() > 0)
		{
			char cChar;
			m_pStream->GetBytes(&cChar, 1);

			if ((cChar == '\r') || (cChar == '\n'))
			{
				if (m_pStream->BytesLeft() > 0)
				{
					//Properly handle fucked up [\r\n] combinations
					char cChar2 = m_pStream->GetByte();
					if ((cChar2 == cChar) || ((cChar2 != '\r') && (cChar2 != '\n'))) { m_pStream->UnGetBytes(1); }
				}

				rOutString.Resize(uiIndex);
				return true;
			}
			else
			{
				if (rOutString.CharsReserved() <= (uiIndex * sizeof(TCHAR)))
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

TCHAR CStreamReadTextASCII::GetChar()
{
	if (m_pStream->BytesLeft() > 0) { return m_pStream->GetByte(); }

	return 0;
}

bool CStreamReadTextASCII::CharsLeft() _GET
{
	return (m_pStream->BytesLeft() > 0);
}
