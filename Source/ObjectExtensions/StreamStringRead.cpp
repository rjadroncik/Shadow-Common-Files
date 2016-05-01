#include "StreamStringRead.h"
#include "Memory.h"

using namespace SCFBase;

CStreamStringRead::CStreamStringRead(_INOUT CString& rString) : CStreamString(rString)
{
}

CStreamStringRead::~CStreamStringRead()
{
}

UINT64 CStreamStringRead::BytesLeft()
{
	return (m_pString->m_uiLength * sizeof(TCHAR)) - m_uiOffset; 
}

UINT64 CStreamStringRead::BytesRead()
{
	return m_uiOffset; 
}

BYTE CStreamStringRead::GetByte()
{
	_ASSERTE((m_uiOffset + sizeof(BYTE)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(BYTE);
	return *(BYTE*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(BYTE));
}

WORD CStreamStringRead::GetWord()
{
	_ASSERTE((m_uiOffset + sizeof(WORD)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(WORD);
	return *(WORD*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(WORD));
}

int CStreamStringRead::GetInt()
{
	_ASSERTE((m_uiOffset + sizeof(int)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(int);
	return *(int*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(int));
}

INT64 CStreamStringRead::GetInt64()
{
	_ASSERTE((m_uiOffset + sizeof(INT64)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(INT64);
	return *(INT64*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(INT64));
}

float CStreamStringRead::GetFloat()
{
	_ASSERTE((m_uiOffset + sizeof(float)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(float);
	return *(float*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(float));
}

bool CStreamStringRead::GetBool()
{
	_ASSERTE((m_uiOffset + sizeof(bool)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(bool);
	return *(bool*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(bool));
}

TCHAR CStreamStringRead::GetChar()
{
	_ASSERTE((m_uiOffset + sizeof(TCHAR)) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += sizeof(TCHAR);
	return *(TCHAR*)((BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(TCHAR));
}

void CStreamStringRead::GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += uiCount;
	CMemory::Copy(vpOutBuffer, m_pString->m_szValue + m_uiOffset, uiCount);
}

void CStreamStringRead::SkipBytes(_IN UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= (m_pString->m_uiLength * sizeof(TCHAR)));

	m_uiOffset += uiCount;
}

void CStreamStringRead::UnGetBytes(_IN UINT uiCount)
{
	_ASSERTE(m_uiOffset >= uiCount);

	m_uiOffset -= uiCount;
}
