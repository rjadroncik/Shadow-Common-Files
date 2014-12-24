#pragma once 

#include "Object.h"

namespace SCFBase
{
	typedef void (__stdcall *MEMORY_DEBUG_HOOK) (_IN void* vpMemory);

	class OBJECT_EXTENSIONS_API CMemory
	{
	public:
		static int Compare(_IN void* vpMemory1, _IN void* vpMemory2, _IN SCF::UINT uiBytes);
	
		static void Erase(_INOUT void* vpMemory, _IN SCF::UINT uiBytes);
		static void Copy(_OUT void* vpDestination, _IN void* vpSource, _IN SCF::UINT uiBytes);
	
		//Performs copying backwards, from last to first byte
		//Can come in handy when copying between overlapping buffers
		static void CopyBackwards(_OUT void* vpDestination, _IN void* vpSource, _IN SCF::UINT uiBytes);

	public:
		static void RegisterDebugHookAllocate(MEMORY_DEBUG_HOOK hookProc);
		static void RegisterDebugHookFree    (MEMORY_DEBUG_HOOK hookProc);

	public:
		//Static functions for controlled allocation of memory
		static void*     Allocate  (                    _IN SCF::UINT uiBytes, _IN bool bErase = FALSE);
		static void*     Reallocate(_IN void* vpMemory, _IN SCF::UINT uiBytes);
		static void      Free      (_IN void* vpMemory);
		static SCF::UINT SizeOf    (_IN void* vpMemory);

		static void*     Aligned16Allocate  (                    _IN SCF::UINT uiBytes, _IN bool bErase = FALSE);
		static void*     Aligned16Reallocate(_IN void* vpMemory, _IN SCF::UINT uiBytes);
		static void      Aligned16Free      (_IN void* vpMemory);
		static SCF::UINT Aligned16SizeOf    (_IN void* vpMemory);

	public:
		static SCF::UINT   BlockCount();
		static SCF::UINT64 AllocatedBytes();

	private:
		CMemory();
		~CMemory();
	};
};
