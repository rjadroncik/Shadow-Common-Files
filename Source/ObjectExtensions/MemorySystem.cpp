#include "MemorySystem.h"

#include <memory.h>
#include <malloc.h>

using namespace SCF;
using namespace SCFPrivate;

SCF::UINT64 MemorySystem_ui64AllocatedBytes = 0;
SCF::UINT   MemorySystem_uiBlockCount = 0;

SCF::UINT   CMemorySystem::BlockCount()     { return MemorySystem_uiBlockCount; }
SCF::UINT64 CMemorySystem::AllocatedBytes() { return MemorySystem_ui64AllocatedBytes; }

void* CMemorySystem::Allocate(_IN UINT uiBytes)
{
	_ASSERTE(uiBytes > 0);

	//Allocate a memory block + 4 bytes for storing the size of the block
	void* vpMemoryNew = (UINT*)malloc(uiBytes + sizeof(UINT)) + 1;

	//Store size of allocated memory (for debugging)
	MemorySystem_ui64AllocatedBytes += uiBytes;
	*((UINT*)vpMemoryNew - 1) = uiBytes;

	MemorySystem_uiBlockCount++;
	return vpMemoryNew;
}

void* CMemorySystem::Reallocate(_IN void* vpMemory, _IN UINT uiBytes)
{
	_ASSERTE(uiBytes > 0);

	if (vpMemory)
	{
		UINT64 ui64OldSize = *((UINT*)vpMemory - 1);

		void* vpMemoryNew = (UINT*)realloc((UINT*)vpMemory - 1, uiBytes + sizeof(UINT)) + 1;

		MemorySystem_ui64AllocatedBytes += (*((UINT*)vpMemoryNew - 1)) - ui64OldSize;

		return vpMemoryNew;
	}
	else { return Allocate(uiBytes); }
}

void CMemorySystem::Free(_IN void* vpMemory)
{
	if (vpMemory) 
	{ 
		MemorySystem_ui64AllocatedBytes -= *((UINT*)vpMemory - 1);

		free((UINT*)vpMemory - 1); 

		MemorySystem_uiBlockCount--;
	}
}

UINT CMemorySystem::SizeOf(_IN void* vpMemory)
{
	return (vpMemory) ? (*((UINT*)vpMemory - 1)) : (0);
}