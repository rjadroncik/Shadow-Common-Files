#pragma once 

#include "Object.h"

namespace SCFPrivate
{
	class OBJECT_EXTENSIONS_API CMemorySystem
	{
	public:
		//Static functions for controlled allocation of memory
		static void*     Allocate  (                    _IN SCF::UINT uiBytes);
		static void*     Reallocate(_IN void* vpMemory, _IN SCF::UINT uiBytes);
		static void      Free      (_IN void* vpMemory);
		static SCF::UINT SizeOf    (_IN void* vpMemory);

	public:
		static SCF::UINT   BlockCount();
		static SCF::UINT64 AllocatedBytes();

	private:
		CMemorySystem();
		~CMemorySystem();
	};
};
