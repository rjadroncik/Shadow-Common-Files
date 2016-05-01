#pragma once

#include "Object.h"

namespace SCFPrivate
{
	class OBJECT_EXTENSIONS_API CFSBHeap
	{
	public:
		void* __stdcall operator new(size_t uiBytes);
		void  __stdcall operator delete(void* vpObject);

	public:
		CFSBHeap(_IN UINT uiBlockSize = 4, _IN UINT uiSegmentSize = 8192);
		~CFSBHeap();

#ifdef _BETA
	public:
		static UINT   HeapCount();
		static UINT   BlockCountTotal();
		static UINT64 AllocatedBytesTotal();
#endif

	public:
		UINT AllocatedBytes() _GET;
		UINT BlockSize()      _GET { return m_uiBlockSize; }
		UINT SegmentSize()    _GET { return m_uiSegmentSize; }

	public:
		//Changing block & segment size is allowed only prior to 1st allocation
		bool BlockSize  (_IN UINT uiBlockSize)   _SET;
		bool SegmentSize(_IN UINT uiSegmentSize) _SET;

	public:
		void* Allocate();
		void  Free(_IN void* vpMemory);
	
	protected:
		//Settings
		UINT m_uiBlockSize;
		UINT m_uiSegmentSize;
	
		//Segment info
		UINT m_uiSegmentCount;
		void*     m_vpSegment;

		//Allocation control variable
		UINT m_uiUsed;
		UINT m_uiInfo;
	};
};