#include "StreamMemoryWrite.h"
#include "Memory.h"

using namespace SCFBase;

CStreamMemoryWrite::CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN SCF::UINT uiOffsetStart) : CStreamMemory(rMemoryBlock)
{
	m_uiOffsetStart = uiOffsetStart;

	m_uiOffset = m_uiOffsetStart;
}

CStreamMemoryWrite::CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN bool bAppend) : CStreamMemory(rMemoryBlock)
{
	if (bAppend) { m_uiOffsetStart = rMemoryBlock.m_uiSize; m_bAppending = TRUE; }
	else         { m_uiOffsetStart = 0;                     m_bAppending = FALSE; }

	m_uiOffset = m_uiOffsetStart;
}

CStreamMemoryWrite::~CStreamMemoryWrite()
{
}

void CStreamMemoryWrite::PutByte(_IN SCF::BYTE ucValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(ucValue));

	*(SCF::BYTE*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = ucValue;
	m_uiOffset += sizeof(ucValue);
}

void CStreamMemoryWrite::PutWord(_IN SCF::WORD usValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(usValue));

	*(SCF::WORD*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = usValue;
	m_uiOffset += sizeof(usValue);
}

void CStreamMemoryWrite::PutInt(_IN int iValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(iValue));

	*(int*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = iValue;
	m_uiOffset += sizeof(iValue);
}

void CStreamMemoryWrite::PutInt64(_IN SCF::INT64 i64Value)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(i64Value));

	*(SCF::INT64*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = i64Value;
	m_uiOffset += sizeof(i64Value);
}

void CStreamMemoryWrite::PutFloat(_IN float fValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(fValue));

	*(float*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = fValue;
	m_uiOffset += sizeof(fValue);
}

void CStreamMemoryWrite::PutBool(_IN bool bValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(bValue));

	*(bool*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = bValue;
	m_uiOffset += sizeof(bValue);
}

void CStreamMemoryWrite::PutChar(_IN SCF::TCHAR cValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(cValue));

	*(SCF::TCHAR*)((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = cValue;
	m_uiOffset += sizeof(cValue);
}

void CStreamMemoryWrite::PutBytes(_IN void* vpBytes, _IN SCF::UINT uiCount)
{
	m_pMemoryBlock->Size(m_uiOffset + uiCount);

	CMemory::Copy(((SCF::BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset), vpBytes, uiCount);
	m_uiOffset += uiCount;
}

SCF::UINT64 CStreamMemoryWrite::BytesWritten() _GET
{
	return m_uiOffset - m_uiOffsetStart;
}

bool CStreamMemoryWrite::Appending() _GET
{
	return m_bAppending;
}