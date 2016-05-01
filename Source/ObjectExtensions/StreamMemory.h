#pragma once
#include "Stream.h"
#include "MemoryBlock.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CStreamMemory : public CStream
	{
	public:
		CStreamMemory(_INOUT CMemoryBlock& rMemoryBlock);
		virtual ~CStreamMemory();

	protected:
		CMemoryBlock* m_pMemoryBlock;
		UINT     m_uiOffset;
	};
};