#include "StreamWriteTextGeneric.h"

#include "StreamWriteTextUCS2.h"
#include "StreamWriteTextASCII.h"
#include "StreamWriteTextUTF8.h"

using namespace SCFBase;

CStreamWriteTextGeneric::CStreamWriteTextGeneric(_INOUT IStreamWrite& rStreamWrite, _IN SCF::ENUM eEncoding, _IN bool bUseBOM)
{
	switch (eEncoding)
	{
	case EncodingASCII: { m_pStream = new CStreamWriteTextASCII(rStreamWrite);          break; }
	case EncodingUTF8:  { m_pStream = new CStreamWriteTextUTF8 (rStreamWrite, bUseBOM); break; }
	default:            { m_pStream = new CStreamWriteTextUCS2 (rStreamWrite, bUseBOM); break; }
	}
}

CStreamWriteTextGeneric::~CStreamWriteTextGeneric()
{
	delete m_pStream;
}

void CStreamWriteTextGeneric::PutString(_IN CString& rString)
{
	m_pStream->PutString(rString); 
}

void CStreamWriteTextGeneric::PutString(_IN SCF::LPTSTR szString)
{
	m_pStream->PutString(szString); 
}

void CStreamWriteTextGeneric::PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength)
{
	m_pStream->PutString(szString, uiLength); 
}

void CStreamWriteTextGeneric::PutLine(_IN CString& rString)
{
	m_pStream->PutLine(rString); 
}

void CStreamWriteTextGeneric::PutLine()
{
	m_pStream->PutLine(); 
}

void CStreamWriteTextGeneric::PutChar(_IN SCF::TCHAR cChar)
{
	m_pStream->PutChar(cChar);
}

SCF::UINT CStreamWriteTextGeneric::CharsWritten() _GET
{
	return m_pStream->CharsWritten();
}