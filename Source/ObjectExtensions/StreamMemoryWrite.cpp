#include "StreamMemoryWrite.h"
#include "Memory.h"

using namespace SCFBase;

CStreamMemoryWrite::CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN UINT uiOffsetStart) : CStreamMemory(rMemoryBlock)
{
	m_uiOffsetStart = uiOffsetStart;

	m_uiOffset = m_uiOffsetStart;
}

CStreamMemoryWrite::CStreamMemoryWrite(_INOUT CMemoryBlock& rMemoryBlock, _IN bool bAppend) : CStreamMemory(rMemoryBlock)
{
	if (bAppend) { m_uiOffsetStart = rMemoryBlock.m_uiSize; m_bAppending = true; }
	else         { m_uiOffsetStart = 0;                     m_bAppending = false; }

	m_uiOffset = m_uiOffsetStart;
}

CStreamMemoryWrite::~CStreamMemoryWrite()
{
}

void CStreamMemoryWrite::PutByte(_IN BYTE ucValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(ucValue));

	*(BYTE*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = ucValue;
	m_uiOffset += sizeof(ucValue);
}

void CStreamMemoryWrite::PutWord(_IN WORD usValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(usValue));

	*(WORD*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = usValue;
	m_uiOffset += sizeof(usValue);
}

void CStreamMemoryWrite::PutInt(_IN int iValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(iValue));

	*(int*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = iValue;
	m_uiOffset += sizeof(iValue);
}

void CStreamMemoryWrite::PutInt64(_IN INT64 i64Value)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(i64Value));

	*(INT64*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = i64Value;
	m_uiOffset += sizeof(i64Value);
}

void CStreamMemoryWrite::PutFloat(_IN float fValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(fValue));

	*(float*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = fValue;
	m_uiOffset += sizeof(fValue);
}

void CStreamMemoryWrite::PutBool(_IN bool bValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(bValue));

	*(bool*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = bValue;
	m_uiOffset += sizeof(bValue);
}

void CStreamMemoryWrite::PutChar(_IN TCHAR cValue)
{
	m_pMemoryBlock->Size(m_uiOffset + sizeof(cValue));

	*(TCHAR*)((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset) = cValue;
	m_uiOffset += sizeof(cValue);
}

void CStreamMemoryWrite::PutBytes(_IN void* vpBytes, _IN UINT uiCount)
{
	m_pMemoryBlock->Size(m_uiOffset + uiCount);

	CMemory::Copy(((BYTE*)m_pMemoryBlock->m_vpData + m_uiOffset), vpBytes, uiCount);
	m_uiOffset += uiCount;
}

UINT64 CStreamMemoryWrite::BytesWritten() _GET
{
	return m_uiOffset - m_uiOffsetStart;
}

bool CStreamMemoryWrite::Appending() _GET
{
	return m_bAppending;
}