#include "StreamFileRead.h"
#include "String.h"

using namespace SCFBase;

CStreamFileRead::CStreamFileRead(_INOUT FILE_HANDLE hFile, _IN UINT64 ui64OffsetStart, _IN UINT64 ui64DataLength) : CStreamFile(hFile)
{
	this->FilePointerSet((int)ui64OffsetStart);

	m_ui64BytesLeft = ui64DataLength;
}

CStreamFileRead::CStreamFileRead(_INOUT FILE_HANDLE hFile) : CStreamFile(hFile)
{
	this->FilePointerSet(0);
	this->FileSize(&m_ui64BytesLeft);
}

CStreamFileRead::CStreamFileRead(_IN CFile& rFile, _IN UINT64 ui64OffsetStart, _IN UINT64 ui64DataLength)
{
	this->FileOpenForReading(rFile);
	this->FilePointerMove((int)ui64OffsetStart);

	m_ui64BytesLeft = ui64DataLength;
}

CStreamFileRead::CStreamFileRead(_IN CFile& rFile)
{
	this->FileOpenForReading(rFile);
	this->FileSize(&m_ui64BytesLeft);
}

CStreamFileRead::~CStreamFileRead()
{
	this->Close();
}

UINT64 CStreamFileRead::BytesLeft()
{
	return m_ui64BytesLeft;
}

UINT64 CStreamFileRead::BytesRead()
{
	UINT64 ui64FileSize;
	this->FileSize(&ui64FileSize);

	return ui64FileSize - m_ui64BytesLeft;
}

BYTE CStreamFileRead::GetByte()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(BYTE));

	m_ui64BytesLeft -= sizeof(BYTE);

	BYTE ucValue = 0;
	this->BufferedRead(&ucValue, sizeof(BYTE));
	return ucValue;
}

WORD CStreamFileRead::GetWord()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(WORD));

	m_ui64BytesLeft -= sizeof(WORD);

	WORD usValue = 0;
	this->BufferedRead(&usValue, sizeof(WORD));
	return usValue;
}

int CStreamFileRead::GetInt()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(int));

	m_ui64BytesLeft -= sizeof(int);

	int iValue = 0;
	this->BufferedRead(&iValue, sizeof(int));
	return iValue;
}

INT64 CStreamFileRead::GetInt64()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(INT64));

	m_ui64BytesLeft -= sizeof(INT64);

	INT64 i64Value = 0;
	this->BufferedRead(&i64Value, sizeof(INT64));
	return i64Value;
}

float CStreamFileRead::GetFloat()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(float));

	m_ui64BytesLeft -= sizeof(float);

	float fValue = 0;
	this->BufferedRead(&fValue, sizeof(float));
	return fValue;
}

bool CStreamFileRead::GetBool()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(bool));

	m_ui64BytesLeft -= sizeof(bool);

	bool bValue = 0;
	this->BufferedRead(&bValue, sizeof(bool));
	return bValue;
}

TCHAR CStreamFileRead::GetChar()
{
	_ASSERTE(m_ui64BytesLeft >= sizeof(TCHAR));

	m_ui64BytesLeft -= sizeof(TCHAR);

	TCHAR cValue = 0;
	this->BufferedRead(&cValue, sizeof(TCHAR));
	return cValue;
}

void CStreamFileRead::GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount)
{
	_ASSERTE(m_ui64BytesLeft >= uiCount);

	m_ui64BytesLeft -= uiCount;
	this->BufferedRead(vpOutBuffer, uiCount);
}

void CStreamFileRead::SkipBytes(_IN UINT uiCount)
{
	_ASSERTE(m_ui64BytesLeft >= uiCount);

	m_ui64BytesLeft -= uiCount;
	this->FilePointerMove(uiCount);
}

void CStreamFileRead::UnGetBytes(_IN UINT uiCount)
{
	_ASSERTE(this->BytesRead() >= uiCount);

	m_ui64BytesLeft += uiCount;
	this->FilePointerMove(-(int)uiCount);
}
