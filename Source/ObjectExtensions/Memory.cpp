#include "Memory.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

SCF::UINT64 Memory_ui64AllocatedBytes = 0;
SCF::UINT   Memory_uiBlockCount = 0;

MEMORY_DEBUG_HOOK Memory_DebugHookAllocate = NULL;
MEMORY_DEBUG_HOOK Memory_DebugHookFree     = NULL;

HANDLE Memory_hHeap = NULL;

void CMemory::RegisterDebugHookAllocate(MEMORY_DEBUG_HOOK hookProc) { Memory_DebugHookAllocate = hookProc; }
void CMemory::RegisterDebugHookFree    (MEMORY_DEBUG_HOOK hookProc) { Memory_DebugHookFree     = hookProc; }

int CMemory::Compare(_IN void* vpMemory1, _IN void* vpMemory2, _IN UINT uiBytes)
{
	if (vpMemory1 && vpMemory2)
	{
		for (SCF::UINT i = 0; i < uiBytes; i++) 
		{
			if (((SCF::BYTE*)vpMemory1)[i] != ((SCF::BYTE*)vpMemory2)[i])
			{
				if (((SCF::BYTE*)vpMemory1)[i] > ((SCF::BYTE*)vpMemory2)[i]) { return 1;  }
				else { return -1; }
			}
		}
	
		return 0;
	}
	else { CError::Last(ErrorInvalidArgument); return 0; }
}
void CMemory::Erase(_INOUT void* vpMemory, _IN UINT uiBytes)
{ 
	if (vpMemory)
	{
		for (SCF::UINT i = 0; i < uiBytes; i++) { ((SCF::BYTE*)vpMemory)[i] = 0;}
	}
	else { CError::Last(ErrorInvalidArgument); }
}

void CMemory::Copy(_OUT void* vpDestination, _IN void* vpSource, _IN SCF::UINT uiBytes)
{
	if (vpSource && vpDestination)
	{
		for (SCF::UINT i = 0; i < uiBytes; i++) { ((SCF::BYTE*)vpDestination)[i] = ((SCF::BYTE*)vpSource)[i];}
	}
	else { CError::Last(ErrorInvalidArgument); }
}

void CMemory::CopyBackwards(_OUT void* vpDestination, _IN void* vpSource, _IN SCF::UINT uiBytes)
{
	if (vpSource && vpDestination)
	{
		for (int i = (int)uiBytes - 1; i >= 0; i--) { ((SCF::BYTE*)vpDestination)[i] = ((SCF::BYTE*)vpSource)[i];}
	}
	else { CError::Last(ErrorInvalidArgument); }
}

void* CMemory::Allocate(_IN UINT uiBytes, _IN bool bErase)
{
	_ASSERTE(uiBytes > 0);

	//Allocate a memory block + 4 bytes for storing the size of the block
	void* vpMemoryNew = (UINT*)HeapAlloc(Memory_hHeap, bErase ? HEAP_ZERO_MEMORY : 0, uiBytes + sizeof(UINT)) + 1;

	//Store size of allocated memory (for debugging)
	Memory_ui64AllocatedBytes += uiBytes;
	*((UINT*)vpMemoryNew - 1) = uiBytes;

	if (Memory_DebugHookAllocate) { Memory_DebugHookAllocate(vpMemoryNew); }

	Memory_uiBlockCount++;
	return vpMemoryNew;
}

void* CMemory::Reallocate(_IN void* vpMemory, _IN UINT uiBytes)
{
	_ASSERTE(uiBytes > 0);

	if (vpMemory)
	{
		UINT uiOldSize = (vpMemory) ? (*((UINT*)vpMemory - 1)) : (0);

		if (Memory_DebugHookFree) { Memory_DebugHookFree(vpMemory); }

		void* vpMemoryNew = (UINT*)HeapReAlloc(Memory_hHeap, 0, (UINT*)vpMemory - 1, uiBytes + sizeof(UINT)) + 1;
		*((UINT*)vpMemoryNew - 1) = uiBytes;

		if (Memory_DebugHookAllocate) { Memory_DebugHookAllocate(vpMemoryNew); }

		Memory_ui64AllocatedBytes += (int)(*((UINT*)vpMemoryNew - 1)) - (int)uiOldSize;

		return vpMemoryNew;
	}
	else { return Allocate(uiBytes); }
}

void CMemory::Free(_IN void* vpMemory)
{
	if (vpMemory) 
	{ 
		if (Memory_DebugHookFree) { Memory_DebugHookFree(vpMemory); }

		Memory_ui64AllocatedBytes -= *((UINT*)vpMemory - 1);
		HeapFree(Memory_hHeap, 0, (UINT*)vpMemory - 1); 

		Memory_uiBlockCount--;
	}
}

UINT CMemory::SizeOf(_IN void* vpMemory)
{
	return (vpMemory) ? (*((UINT*)vpMemory - 1)) : (0);
}

void* CMemory::Aligned16Allocate(_IN UINT uiBytes, _IN bool bErase)
{
	void* vpMemoryNew = Allocate(uiBytes + 20, bErase);
	void* vpMemoryNewAligned = (BYTE*)vpMemoryNew + 20 - ((UINT64)vpMemoryNew + 4) % 16; 

	*((UINT*)vpMemoryNewAligned - 1) = (UINT)((BYTE*)vpMemoryNewAligned - (BYTE*)vpMemoryNew);
	return vpMemoryNewAligned;
}

void* CMemory::Aligned16Reallocate(_IN void* vpMemory, _IN UINT uiBytes)
{
	_ASSERTE(uiBytes > 0);

	if (vpMemory)
	{
		void* vpMemoryNew = Reallocate((BYTE*)vpMemory - (*((UINT*)vpMemory - 1)), uiBytes + 20);
		void* vpMemoryNewAligned = (BYTE*)vpMemoryNew + 20 - ((UINT64)vpMemoryNew + 4) % 16; 

		*((UINT*)vpMemoryNewAligned - 1) = (UINT)((BYTE*)vpMemoryNewAligned - (BYTE*)vpMemoryNew);
		return vpMemoryNewAligned;
	}
	else { return Aligned16Allocate(uiBytes); }
}

void CMemory::Aligned16Free(_IN void* vpMemory)
{
	Free((BYTE*)vpMemory - (*((UINT*)vpMemory - 1)));
}

UINT CMemory::Aligned16SizeOf(_IN void* vpMemory)
{
	return SizeOf((BYTE*)vpMemory - (*((UINT*)vpMemory - 1))) - (*((UINT*)vpMemory - 1));
}

SCF::UINT   CMemory::BlockCount()     { return Memory_uiBlockCount; }
SCF::UINT64 CMemory::AllocatedBytes() { return Memory_ui64AllocatedBytes; }