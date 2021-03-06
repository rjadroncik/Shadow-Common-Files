#include "FSBHeap.h"

#include <memory.h>
#include <malloc.h>

#include <SCFMathematicsTypes.h>

using namespace SCFPrivate;

#ifdef _BETA
UINT   FSBHeap_uiHeapCount = 0;
UINT   FSBHeap_uiBlockCount = 0;	   
UINT64 FSBHeap_ui64AllocatedBytes = 0;

UINT   CFSBHeap::HeapCount()           { return FSBHeap_uiHeapCount; }
UINT   CFSBHeap::BlockCountTotal()     { return FSBHeap_uiBlockCount; }
UINT64 CFSBHeap::AllocatedBytesTotal() { return FSBHeap_ui64AllocatedBytes; }
#endif

bool CFSBHeap::BlockSize(_IN UINT uiBlockSize) _SET
{
	if (m_uiSegmentCount) { return false; }

	if (uiBlockSize % sizeof(void*))
	{
		m_uiBlockSize = uiBlockSize + sizeof(void*) - (uiBlockSize % sizeof(void*));
	}
	else { m_uiBlockSize = uiBlockSize; }

	SegmentSize(m_uiSegmentSize);

	return true;
}

bool CFSBHeap::SegmentSize(_IN UINT uiSegmentSize) _SET
{
	if (m_uiSegmentCount) { return false; }

	m_uiSegmentSize = ((uiSegmentSize - (sizeof(void*) * 2)) / m_uiBlockSize) * m_uiBlockSize;

	return true;
}

CFSBHeap::CFSBHeap(_IN UINT uiBlockSize, _IN UINT uiSegmentSize)
{	
	BETAONLY(FSBHeap_uiHeapCount++;)

	m_uiSegmentCount = 0;
	m_vpSegment = nullptr;

	m_uiUsed = 0;
	m_uiInfo = 0;

	m_uiSegmentSize = uiSegmentSize;

	//This function calls [SegmentSize(..)] & makes sure segment size is a multiple of block size
	BlockSize(uiBlockSize);
}

CFSBHeap::~CFSBHeap()
{
	BETAONLY(FSBHeap_uiHeapCount--;)

	void* vpLastSegment = (m_vpSegment) ? (*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize))) : (nullptr);
	if (vpLastSegment)
	{
		free(((void**)vpLastSegment - 1));
	}

	//We use the "back" pointers stored at the start of the block/segment
	void* vpSegment = m_vpSegment;
	while (vpSegment)
	{
		void* vpSegmentNext = *((void**)vpSegment - 1);
	
		free(((void**)vpSegment - 1));

		vpSegment = vpSegmentNext;

		BETAONLY(m_uiSegmentCount--;)
	}

	_ASSERTE(m_uiSegmentCount == 0);
}

void* CFSBHeap::Allocate()
{
	BETAONLY(FSBHeap_ui64AllocatedBytes += m_uiBlockSize;)
	BETAONLY(FSBHeap_uiBlockCount++;)

	//Check if there is free space in the heap
	if (m_uiInfo)
	{
		m_uiInfo -= sizeof(void*);

		//Shrink heap & move to previous segment
		if (m_uiUsed == sizeof(void*))
		{
			m_uiSegmentCount--;
			m_uiUsed = m_uiSegmentSize;

			void* vpLastSegment = m_vpSegment;
			void* vpRetVal = *(void**)m_vpSegment;

			m_vpSegment = *((void**)m_vpSegment - 1);
			*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize)) = nullptr;

			free(((void**)vpLastSegment - 1));

			return vpRetVal;
		}
		else
		{
			m_uiUsed -= sizeof(void*);
			return *(void**)((BYTE*)m_vpSegment + m_uiUsed);
		}
	}
	else
	{
		//Resize heap if it is full
		if ((m_uiUsed % m_uiSegmentSize) == 0)
		{
			m_uiSegmentCount++;

			void* vpSegmentNew = (m_vpSegment) ? (*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize))) : (nullptr);
			if (!vpSegmentNew)
			{
				vpSegmentNew = (void**)malloc(m_uiSegmentSize + (sizeof(void*) * 2)) + 1;
			}

			if (m_vpSegment)
			{
				*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize)) = vpSegmentNew;
			}
			
			*((void**)vpSegmentNew - 1) = m_vpSegment;
			*((void**)((BYTE*)vpSegmentNew + m_uiSegmentSize)) = nullptr;

			m_vpSegment = vpSegmentNew;
			m_uiUsed = m_uiBlockSize;

			return (BYTE*)m_vpSegment;
		}
		else
		{
			m_uiUsed += m_uiBlockSize;
			return (BYTE*)m_vpSegment + m_uiUsed - m_uiBlockSize;
		}
	}
}

void CFSBHeap::Free(_IN void* vpMemory)
{
	_ASSERTE(m_uiUsed > 0);

	//Accept nullptr pointers
	if (!vpMemory) { return; }

	BETAONLY(FSBHeap_ui64AllocatedBytes -= m_uiBlockSize;)
	BETAONLY(FSBHeap_uiBlockCount--;)

	m_uiInfo += sizeof(void*);

	//Resize heap if it is full
	if (m_uiUsed == m_uiSegmentSize)
	{
		m_uiSegmentCount++;

		void* vpSegmentNew = (m_vpSegment) ? (*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize))) : (nullptr);
		if (!vpSegmentNew)
		{
			vpSegmentNew = (void**)malloc(m_uiSegmentSize + (sizeof(void*) * 2)) + 1;
		}

		if (m_vpSegment)
		{
			*((void**)((BYTE*)m_vpSegment + m_uiSegmentSize)) = vpSegmentNew;
		}

		*((void**)vpSegmentNew - 1) = m_vpSegment;
		*((void**)((BYTE*)vpSegmentNew + m_uiSegmentSize)) = nullptr;

		m_vpSegment = vpSegmentNew;
		m_uiUsed = sizeof(void*);

		*(const void**)m_vpSegment = vpMemory;
	}
	else
	{
		//Free specified block
		*(const void**)((BYTE*)m_vpSegment + m_uiUsed) = vpMemory;
		m_uiUsed += sizeof(void*);
	}
}

UINT CFSBHeap::AllocatedBytes() _GET
{
	return m_uiSegmentSize * __max((int)m_uiSegmentCount - 1, 0) + m_uiUsed - m_uiInfo - ((m_uiInfo / sizeof(void*)) * m_uiBlockSize); 
}

void *__stdcall CFSBHeap::operator new(size_t uiBytes)
{
	return malloc(uiBytes);
}

void __stdcall CFSBHeap::operator delete(void* vpObject)
{
	free(vpObject);
}
