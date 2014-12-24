#include "StreamMemoryRead.h"
#include "Memory.h"

using namespace SCFBase;

CStreamMemoryRead::CStreamMemoryRead(_INOUT CMemoryBlock& rMemoryBlock) : CStreamMemory(rMemoryBlock)
{
}

CStreamMemoryRead::~CStreamMemoryRead()
{
}

SCF::UINT64 CStreamMemoryRead::BytesLeft()
{
	return m_pMemoryBlock->m_uiSize - m_uiOffset; 
}

SCF::UINT64 CStreamMemoryRead::BytesRead()
{
	return m_uiOffset; 
}

SCF::BYTE CStreamMemoryRead::GetByte()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::BYTE)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(SCF::BYTE);
	return *(SCF::BYTE*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(SCF::BYTE));
}

SCF::WORD CStreamMemoryRead::GetWord()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::WORD)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(SCF::WORD);
	return *(SCF::WORD*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(SCF::WORD));
}

int CStreamMemoryRead::GetInt()
{
	_ASSERTE((m_uiOffset + sizeof(int)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(int);
	return *(int*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(int));
}

SCF::INT64 CStreamMemoryRead::GetInt64()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::INT64)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(SCF::INT64);
	return *(SCF::INT64*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(SCF::INT64));
}

float CStreamMemoryRead::GetFloat()
{
	_ASSERTE((m_uiOffset + sizeof(float)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(float);
	return *(float*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(float));
}

bool CStreamMemoryRead::GetBool()
{
	_ASSERTE((m_uiOffset + sizeof(bool)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(bool);
	return *(bool*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(bool));
}

SCF::TCHAR CStreamMemoryRead::GetChar()
{
	_ASSERTE((m_uiOffset + sizeof(SCF::TCHAR)) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += sizeof(SCF::TCHAR);
	return *(SCF::TCHAR*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset - sizeof(SCF::TCHAR));
}

void CStreamMemoryRead::GetBytes(_OUT void* vpOutBuffer, _IN SCF::UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += uiCount;
	_PENDING; //The " + m_uiOffset" wasn't here a long time .. 
	CMemory::Copy(vpOutBuffer, (SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset, uiCount);
}

void CStreamMemoryRead::SkipBytes(_IN SCF::UINT uiCount)
{
	_ASSERTE((m_uiOffset + uiCount) <= m_pMemoryBlock->m_uiSize);

	m_uiOffset += uiCount;
}

void CStreamMemoryRead::UnGetBytes(_IN SCF::UINT uiCount)
{
	_ASSERTE(m_uiOffset >= uiCount);

	m_uiOffset -= uiCount;
}
