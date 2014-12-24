#include "StreamReadTextUCS2.h"

using namespace SCFBase;

CStreamReadTextUCS2::CStreamReadTextUCS2(_INOUT IStreamRead& rStreamRead) : CStreamReadText(rStreamRead)
{
	if (m_pStream->GetChar() != 0xfeff)
	{
		m_pStream->UnGetBytes(sizeof(SCF::TCHAR));
	}
}

CStreamReadTextUCS2::~CStreamReadTextUCS2()
{
}

bool CStreamReadTextUCS2::GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength)
{
	if (m_pStream->BytesLeft() > 1)
	{
		SCF::UINT uiBytes = (uiLength != 0) ? (__min((SCF::UINT)m_pStream->BytesLeft(), uiLength * sizeof(SCF::TCHAR))) : ((SCF::UINT)m_pStream->BytesLeft()); 
		rOutString.Resize(uiBytes / sizeof(SCF::TCHAR));

		m_pStream->GetBytes((SCF::BYTE*)rOutString.Value(), uiBytes);
		return TRUE;
	}

	return FALSE; 
}

bool CStreamReadTextUCS2::GetLine(_OUT CString& rOutString)
{
	if (m_pStream->BytesLeft() > 1)
	{
		rOutString.CharsReserve(256);
		SCF::UINT uiIndex = 0;

		while (m_pStream->BytesLeft() > 0)
		{
			SCF::TCHAR cChar = m_pStream->GetChar();

			if (cChar == '\n')
			{
				rOutString.Resize(uiIndex);
				return TRUE;
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
		return TRUE;
	}

	return FALSE;
}

SCF::TCHAR CStreamReadTextUCS2::GetChar()
{
	if (m_pStream->BytesLeft() > 1) { return m_pStream->GetWord(); }

	return 0;
}

bool CStreamReadTextUCS2::CharsLeft() _GET
{
	return (m_pStream->BytesLeft() > 1);
}