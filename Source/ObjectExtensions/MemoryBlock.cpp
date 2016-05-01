#include "MemoryBlock.h"
#include "Memory.h"

#include <malloc.h>

using namespace SCFBase;

UINT   MemoryBlock_uiBlockCount = 0;
UINT64 MemoryBlock_ui64AllocatedBytes = 0;

#define ALLOC_GRANULARITY 512

CMemoryBlock::CMemoryBlock()
{
	m_uiSize = 0;
	m_vpData = NULL;

	MemoryBlock_uiBlockCount++;
}

CMemoryBlock::CMemoryBlock(_IN UINT uiBytes)
{
	m_uiSize = uiBytes;

	if (uiBytes > 0)
	{
		m_vpData = malloc(m_uiSize + ALLOC_GRANULARITY - (m_uiSize % ALLOC_GRANULARITY));
	}
	else { m_vpData = NULL; }

	MemoryBlock_uiBlockCount++;
	MemoryBlock_ui64AllocatedBytes += uiBytes;
}

CMemoryBlock::~CMemoryBlock()
{
	if (m_vpData)
	{
		free(m_vpData);
	}

	MemoryBlock_uiBlockCount--;
	MemoryBlock_ui64AllocatedBytes -= m_uiSize;
}

void CMemoryBlock::Size(_IN UINT uiBytes) _SET
{
	if (!m_vpData || (uiBytes >= (m_uiSize + ALLOC_GRANULARITY - (m_uiSize % ALLOC_GRANULARITY))))
	{
		MemoryBlock_ui64AllocatedBytes += (int)uiBytes - (int)m_uiSize;
		m_uiSize = uiBytes;

		if (m_vpData) { m_vpData = realloc(m_vpData, m_uiSize + ALLOC_GRANULARITY - (m_uiSize % ALLOC_GRANULARITY)); }
		else          { m_vpData = malloc (          m_uiSize + ALLOC_GRANULARITY - (m_uiSize % ALLOC_GRANULARITY)); }
	}
	else
	{
		MemoryBlock_ui64AllocatedBytes += (int)uiBytes - (int)m_uiSize;
		m_uiSize = uiBytes;
	}
}

void CMemoryBlock::Value(_IN CMemoryBlock& rMemoryBlock) _SET
{
	this->Size(rMemoryBlock.Size());

	CMemory::Copy(m_vpData, rMemoryBlock.Value(), this->Size());
}

void CMemoryBlock::Value(_IN void* pMemory, _IN UINT uiBytes) _SET
{
	this->Size(uiBytes);

	CMemory::Copy(m_vpData, pMemory, this->Size());
}

UINT   CMemoryBlock::BlockCount()     { return MemoryBlock_uiBlockCount; }
UINT64 CMemoryBlock::AllocatedBytes() { return MemoryBlock_ui64AllocatedBytes; }
