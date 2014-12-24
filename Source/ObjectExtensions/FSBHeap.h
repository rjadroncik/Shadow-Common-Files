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
		CFSBHeap(_IN SCF::UINT uiBlockSize = 4, _IN SCF::UINT uiSegmentSize = 8192);
		~CFSBHeap();

#ifdef _BETA
	public:
		static SCF::UINT   HeapCount();
		static SCF::UINT   BlockCountTotal();
		static SCF::UINT64 AllocatedBytesTotal();
#endif

	public:
		SCF::UINT AllocatedBytes() _GET;
		SCF::UINT BlockSize()      _GET { return m_uiBlockSize; }
		SCF::UINT SegmentSize()    _GET { return m_uiSegmentSize; }

	public:
		//Changing block & segment size is allowed only prior to 1st allocation
		bool BlockSize  (_IN SCF::UINT uiBlockSize)   _SET;
		bool SegmentSize(_IN SCF::UINT uiSegmentSize) _SET;

	public:
		void* Allocate();
		void  Free(_IN void* vpMemory);
	
	protected:
		//Settings
		SCF::UINT m_uiBlockSize;
		SCF::UINT m_uiSegmentSize;
	
		//Segment info
		SCF::UINT m_uiSegmentCount;
		void*     m_vpSegment;

		//Allocation control variable
		SCF::UINT m_uiUsed;
		SCF::UINT m_uiInfo;
	};
};