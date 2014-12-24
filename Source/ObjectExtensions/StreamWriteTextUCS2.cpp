#include "StreamWriteTextUCS2.h"

using namespace SCFBase;

#include <string.h>

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
		m_pStream->PutBytes((SCF::BYTE*)rString.Value(), rString.Length() * sizeof(SCF::TCHAR)); 
	}
}

void CStreamWriteTextUCS2::PutString(_IN SCF::LPTSTR szString)
{
	const SCF::UINT uiLength = (SCF::UINT)wcslen(szString);
	if (uiLength)
	{
		m_pStream->PutBytes((SCF::BYTE*)szString, uiLength * sizeof(SCF::TCHAR));
	}
}

void CStreamWriteTextUCS2::PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength)
{
	if (uiLength)
	{
		m_pStream->PutBytes((SCF::BYTE*)szString, uiLength * sizeof(SCF::TCHAR));
	}
}

void CStreamWriteTextUCS2::PutLine(_IN CString& rString)
{
	if (rString.Length())
	{
		m_pStream->PutBytes((SCF::BYTE*)rString.Value(), rString.Length() * sizeof(SCF::TCHAR)); 
	}
	//m_pStream->PutBytes((SCF::BYTE*)SCFTEXT("\n"), sizeof(SCFTEXT("\n")) - sizeof(SCF::TCHAR)); 
	m_pStream->PutWord('\n');
}

void CStreamWriteTextUCS2::PutLine()
{
	//m_pStream->PutBytes((SCF::BYTE*)SCFTEXT("\n"), sizeof(SCFTEXT("\n")) - sizeof(SCF::TCHAR)); 
	m_pStream->PutWord('\n');
}

void CStreamWriteTextUCS2::PutChar(_IN SCF::TCHAR cChar)
{
	m_pStream->PutWord(cChar);
}

SCF::UINT CStreamWriteTextUCS2::CharsWritten() _GET
{
	return (SCF::UINT)((m_pStream->BytesWritten() / 2) - 1);
}