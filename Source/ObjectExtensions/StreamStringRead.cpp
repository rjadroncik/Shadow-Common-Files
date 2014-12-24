#include "StreamStringRead.h"
#include "Memory.h"

using namespace SCFBase;

CStreamStringRead::CStreamStringRead(_INOUT CString& rString) : CStreamString(rString)
{
}

CStreamStringRead::~CStreamStringRead()
{
}

SCF::UINT64 CStreamStringRead::BytesLeft()
{
	return (m_pString->m_uiLength * sizeof(SCF::TCHAR)) - m_uiOffset; 
}

SCF::UINT64 CStreamStringRead::BytesRead()
{
	return m_uiOffset; 
}

SCF::BYTE CStreamStringRead::GetByte()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::BYTE)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(SCF::BYTE);
	return *(SCF::BYTE*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(SCF::BYTE));
}

SCF::WORD CStreamStringRead::GetWord()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::WORD)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(SCF::WORD);
	return *(SCF::WORD*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(SCF::WORD));
}

int CStreamStringRead::GetInt()
{
	_ASSERTE((m_uiOffset + sizeof(int)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(int);
	return *(int*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(int));
}

SCF::INT64 CStreamStringRead::GetInt64()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::INT64)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(SCF::INT64);
	return *(SCF::INT64*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(SCF::INT64));
}

float CStreamStringRead::GetFloat()
{
	_ASSERTE((m_uiOffset + sizeof(float)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(float);
	return *(float*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(float));
}

bool CStreamStringRead::GetBool()
{
	_ASSERTE((m_uiOffset + sizeof(bool)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(bool);
	return *(bool*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(bool));
}

SCF::TCHAR CStreamStringRead::GetChar()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::TCHAR)) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += sizeof(SCF::TCHAR);
	return *(SCF::TCHAR*)((SCF::BYTE*)m_pString->m_szValue + m_uiOffset - sizeof(SCF::TCHAR));
}

void CStreamStringRead::GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += uiCount;
	CMemory::Copy(vpOutBuffer, m_pString->m_szValue + m_uiOffset, uiCount);
}

void CStreamStringRead::SkipBytes(_IN SCF::UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= (m_pString->m_uiLength * sizeof(SCF::TCHAR)));

	m_uiOffset += uiCount;
}

void CStreamStringRead::UnGetBytes(_IN SCF::UINT uiCount)
{
	_ASSERTE(m_uiOffset >= uiCount);

	m_uiOffset -= uiCount;
}
