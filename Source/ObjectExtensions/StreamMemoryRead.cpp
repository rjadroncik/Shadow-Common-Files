#include "StreamMemoryRead.h"
#include "Memory.h"

using namespace SCFBase;

CStreamMemoryRead::CStreamMemoryRead(_INOUT CMemoryBlock& rMemoryBlock) : CStreamMemory(rMemoryBlock)
{
}

CStreamMemoryRead::~CStreamMemoryRead()
{
}

UINT64 CStreamMemoryRead::BytesLeft()
{
	return m_pMemoryBlock->m_uiSize - m_uiOffset; 
}

UINT64 CStreamMemoryRead::BytesRead()
{
	return m_uiOffset; 
}

BYTE CStreamMemoryRead::GetByte()
{
	_ASSERTE((m_uiOffset + sizeof(BYTE)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(BYTE);
	return *(BYTE*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(BYTE));
}

WORD CStreamMemoryRead::GetWord()
{
	_ASSERTE((m_uiOffset + sizeof(WORD)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(WORD);
	return *(WORD*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(WORD));
}

int CStreamMemoryRead::GetInt()
{
	_ASSERTE((m_uiOffset + sizeof(int)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(int);
	return *(int*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(int));
}

INT64 CStreamMemoryRead::GetInt64()
{
	_ASSERTE((m_uiOffset + sizeof(INT64)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(INT64);
	return *(INT64*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(INT64));
}

float CStreamMemoryRead::GetFloat()
{
	_ASSERTE((m_uiOffset + sizeof(float)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(float);
	return *(float*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(float));
}

bool CStreamMemoryRead::GetBool()
{
	_ASSERTE((m_uiOffset + sizeof(bool)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(bool);
	return *(bool*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(bool));
}

TCHAR CStreamMemoryRead::GetChar()
{
	_ASSERTE((m_uiOffset + sizeof(TCHAR)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(TCHAR);
	return *(TCHAR*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(TCHAR));
}

void CStreamMemoryRead::GetBytes(_OUT void* vpOutBuffer, _IN UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += uiCount;
	_PENDING; //The " + m_uiOffset" wasn't here a long time .. 
	CMemory::Copy(vpOutBuffer, (BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset, uiCount);
}

void CStreamMemoryRead::SkipBytes(_IN UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += uiCount;
}

void CStreamMemoryRead::UnGetBytes(_IN UINT uiCount)
{
	_ASSERTE(m_uiOffset >= uiCount);

	m_uiOffset -= uiCount;
}
