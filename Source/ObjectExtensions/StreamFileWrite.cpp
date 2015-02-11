#include "StreamFileWrite.h"

using namespace SCFBase;

CStreamFileWrite::CStreamFileWrite(_INOUT FILE_HANDLE hFile, _IN SCF::UINT64 ui64OffsetStart) : CStreamFile(hFile)
{
	this->FilePointerSet((int)ui64OffsetStart);

	m_ui64BytesWritten = 0;
}

CStreamFileWrite::CStreamFileWrite(_INOUT FILE_HANDLE hFile, _IN bool bAppend) : CStreamFile(hFile)
{
	if (bAppend) { this->FilePointerSetToEnd(); }
	else         { this->FilePointerSet(0); }

	m_ui64BytesWritten = 0;
}

CStreamFileWrite::CStreamFileWrite(_IN CFile& rFile, _IN SCF::UINT64 ui64OffsetStart)
{
	this->FileOpenForWriting(rFile, FALSE);
	this->FilePointerMove((int)ui64OffsetStart);

	m_ui64BytesWritten = 0;
}

CStreamFileWrite::CStreamFileWrite(_IN CFile& rFile, _IN bool bAppend) : CStreamFile()
{
	//If we want to append then we should NOT erase the file, therefore the negation
	this->FileOpenForWriting(rFile, !bAppend);

	if (bAppend) { m_bAppending = TRUE; this->FilePointerMove((int)rFile.Size()); }
	else         { m_bAppending = FALSE; }

	m_ui64BytesWritten = 0;
}

CStreamFileWrite::~CStreamFileWrite()
{
	this->Close();
}

void CStreamFileWrite::Close()
{
	if (this->FileIsOpen()) { this->BufferCommit(); this->FileClose(); }
}

void CStreamFileWrite::PutByte(_IN SCF::BYTE ucValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&ucValue, sizeof(ucValue));
}

void CStreamFileWrite::PutWord(_IN SCF::WORD usValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&usValue, sizeof(usValue));
}

void CStreamFileWrite::PutInt(_IN int iValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&iValue, sizeof(iValue));
}

void CStreamFileWrite::PutInt64(_IN SCF::INT64 i64Value)
{
	m_ui64BytesWritten += this->BufferedWrite(&i64Value, sizeof(i64Value));
}

void CStreamFileWrite::PutFloat(_IN float fValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&fValue, sizeof(fValue));
}

void CStreamFileWrite::PutBool(_IN bool bValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&bValue, sizeof(bValue));
}

void CStreamFileWrite::PutChar(_IN SCF::TCHAR cValue)
{
	m_ui64BytesWritten += this->BufferedWrite(&cValue, sizeof(cValue));
}

void CStreamFileWrite::PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount)
{
	m_ui64BytesWritten += this->BufferedWrite(vpBytes, uiCount);
}

void CStreamFileWrite::Commit()
{
	this->BufferCommit();
	this->FileCommit();
}

SCF::UINT64 CStreamFileWrite::BytesWritten() _GET
{
	return m_ui64BytesWritten;
}

bool CStreamFileWrite::Appending() _GET
{
	return m_bAppending;
}
