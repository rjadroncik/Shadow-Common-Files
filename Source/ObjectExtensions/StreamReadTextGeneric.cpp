#include "StreamReadTextGeneric.h"

#include "StreamReadTextUCS2.h"
#include "StreamReadTextUTF8.h"

using namespace SCFBase;

CStreamReadTextGeneric::CStreamReadTextGeneric(_INOUT IStreamRead& rStreamRead)
{
	//If there is no Unicode ID/Magic number/FourCC, return file pointer to start
	if (rStreamRead.BytesLeft())
	{
		if (rStreamRead.GetChar() == 0xfeff)
		{
			rStreamRead.UnGetBytes(sizeof(SCF::TCHAR));
			m_pStream = new CStreamReadTextUCS2(rStreamRead);
		}
		else
		{
			rStreamRead.UnGetBytes(sizeof(SCF::TCHAR));
			m_pStream = new CStreamReadTextUTF8(rStreamRead);
		}
	}
	else { m_pStream = new CStreamReadTextUCS2(rStreamRead); }
}

CStreamReadTextGeneric::~CStreamReadTextGeneric()
{
	delete m_pStream;
}

bool CStreamReadTextGeneric::GetString(_OUT CString& rOutString, _IN SCF::UINT uiLength)
{
	return m_pStream->GetString(rOutString, uiLength);
}

bool CStreamReadTextGeneric::GetLine(_OUT CString& rOutString)
{
	return m_pStream->GetLine(rOutString);
}

SCF::TCHAR CStreamReadTextGeneric::GetChar()
{
	return m_pStream->GetChar();
}

bool CStreamReadTextGeneric::CharsLeft() _GET
{
	return m_pStream->CharsLeft();
}
