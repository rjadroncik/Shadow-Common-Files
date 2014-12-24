#include "StreamMemory.h"

using namespace SCFBase;

CStreamMemory::CStreamMemory(_INOUT CMemoryBlock& rMemoryBlock)
{
	m_pMemoryBlock = &rMemoryBlock;
	m_uiOffset = 0;

	ADDREF(*(m_pMemoryBlock))
}

CStreamMemory::~CStreamMemory()
{
	RELEASE(*(m_pMemoryBlock))
}