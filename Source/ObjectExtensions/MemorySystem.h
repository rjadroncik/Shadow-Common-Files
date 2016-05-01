#pragma once 

#include "Object.h"

namespace SCFPrivate
{
	class OBJECT_EXTENSIONS_API CMemorySystem
	{
	public:
		//Static functions for controlled allocation of memory
		static void*     Allocate  (                    _IN UINT uiBytes);
		static void*     Reallocate(_IN void* vpMemory, _IN UINT uiBytes);
		static void      Free      (_IN void* vpMemory);
		static UINT SizeOf    (_IN void* vpMemory);

	public:
		static UINT   BlockCount();
		static UINT64 AllocatedBytes();

	private:
		CMemorySystem();
		~CMemorySystem();
	};
};
