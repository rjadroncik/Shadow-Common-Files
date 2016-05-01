#include "StreamWriteTextUCS2.h"

using namespace SCFBase;

#include <string.h>
#include <wchar.h>

CStreamWriteTextUCS2::CStreamWriteTextUCS2(_INOUT IStreamWrite& rStreamWrite, _IN bool bUseBOM) : CStreamWriteText(rStreamWrite)
{
	if (!rStreamWrite.Appending() && bUseBOM) { m_pStream->PutChar(0xFEFF); }
}

CStreamWriteTextUCS2::~CStreamWriteTextUCS2()
{
}

void CStreamWriteTextUCS2::PutString(_IN CString& rString)
{
	if (rString.Length())
	{
		m_pStream->PutBytes((BYTE*)rString.Value(), rString.Length() * sizeof(TCHAR));
	}
}

void CStreamWriteTextUCS2::PutString(_IN LPTSTR szString)
{
	const UINT uiLength = (UINT)wcslen(szString);
	if (uiLength)
	{
		m_pStream->PutBytes((BYTE*)szString, uiLength * sizeof(TCHAR));
	}
}

void CStreamWriteTextUCS2::PutString(_IN LPTSTR szString, _IN UINT uiLength)
{
	if (uiLength)
	{
		m_pStream->PutBytes((BYTE*)szString, uiLength * sizeof(TCHAR));
	}
}

void CStreamWriteTextUCS2::PutLine(_IN CString& rString)
{
	if (rString.Length())
	{
		m_pStream->PutBytes((BYTE*)rString.Value(), rString.Length() * sizeof(TCHAR));
	}
	//m_pStream->PutBytes((BYTE*)SCFTEXT("\n"), sizeof(SCFTEXT("\n")) - sizeof(TCHAR));
	m_pStream->PutWord('\n');
}

void CStreamWriteTextUCS2::PutLine()
{
	//m_pStream->PutBytes((BYTE*)SCFTEXT("\n"), sizeof(SCFTEXT("\n")) - sizeof(TCHAR));
	m_pStream->PutWord('\n');
}

void CStreamWriteTextUCS2::PutChar(_IN TCHAR cChar)
{
	m_pStream->PutWord(cChar);
}

UINT CStreamWriteTextUCS2::CharsWritten() _GET
{
	return (UINT)((m_pStream->BytesWritten() / 2) - 1);
}
