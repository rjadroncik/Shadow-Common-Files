#include "FSBHeap.h"

#include <memory.h>

#include <SCFMathematicsTypes.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern HANDLE Memory_hHeap;

using namespace SCFPrivate;

#ifdef _BETA
SCF::UINT   FSBHeap_uiHeapCount = 0;
SCF::UINT   FSBHeap_uiBlockCount = 0;	   
SCF::UINT64 FSBHeap_ui64AllocatedBytes = 0;

SCF::UINT   CFSBHeap::HeapCount()           { return FSBHeap_uiHeapCount; }
SCF::UINT   CFSBHeap::BlockCountTotal()     { return FSBHeap_uiBlockCount; }
SCF::UINT64 CFSBHeap::AllocatedBytesTotal() { return FSBHeap_ui64AllocatedBytes; }
#endif

bool CFSBHeap::BlockSize(_IN SCF::UINT uiBlockSize) _SET
{
	if (m_uiSegmentCount) { return FALSE; }

	if (uiBlockSize % sizeof(void*))
	{
		m_uiBlockSize = uiBlockSize + sizeof(void*) - (uiBlockSize % sizeof(void*));
	}
	else { m_uiBlockSize = uiBlockSize; }

	//m_uiSegmentSize = ((4096 - (sizeof(void*) * 2)) / m_uiBlockSize) * m_uiBlockSize;

	SegmentSize(m_uiSegmentSize);

	return TRUE;
}

bool CFSBHeap::SegmentSize(_IN SCF::UINT uiSegmentSize) _SET
{
	if (m_uiSegmentCount) { return FALSE; }

	m_uiSegmentSize = ((uiSegmentSize - (sizeof(void*) * 2)) / m_uiBlockSize) * m_uiBlockSize;

	/*if (uiSegmentSize % m_uiBlockSize)
	{
		m_uiSegmentSize = uiSegmentSize + m_uiBlockSize - (uiSegmentSize % m_uiBlockSize);
	}
	else { m_uiSegmentSize = uiSegmentSize; }*/

	return TRUE;
}

CFSBHeap::CFSBHeap(_IN SCF::UINT uiBlockSize, _IN SCF::UINT uiSegmentSize)
{	
	BETAONLY(FSBHeap_uiHeapCount++;)

	m_uiSegmentCount = 0;
	m_vpSegment = NULL;

	m_uiUsed = 0;
	m_uiInfo = 0;

	m_uiSegmentSize = uiSegmentSize;

	//This function calls [SegmentSize(..)] & makes sure segment size is a multiple of block size
	BlockSize(uiBlockSize);
}

CFSBHeap::~CFSBHeap()
{
	BETAONLY(FSBHeap_uiHeapCount--;)

	void* vpLastSegment = (m_vpSegment) ? (*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize))) : (NULL);
	if (vpLastSegment)
	{
		HeapFree(Memory_hHeap, 0, ((void**)vpLastSegment - 1));
	//	VirtualFree(((void**)vpLastSegment - 1), 0, MEM_RELEASE);
	}

	//We use the "back" pointers stored at the start of the block/segment
	void* vpSegment = m_vpSegment;
	while (vpSegment)
	{
		void* vpSegmentNext = *((void**)vpSegment - 1);
	
		HeapFree(Memory_hHeap, 0, ((void**)vpSegment - 1));
	//	VirtualFree(((void**)vpSegment - 1), 0, MEM_RELEASE);

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
			void* vpLastSegment = *((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize));
			if (vpLastSegment)
			{
				HeapFree(Memory_hHeap, 0, ((void**)vpLastSegment - 1));
			//	VirtualFree(((void**)vpLastSegment - 1), 0, MEM_RELEASE);
			}

			m_uiSegmentCount--;
			m_uiUsed = m_uiSegmentSize;

			void* vpRetVal = *(void**)m_vpSegment;

			m_vpSegment = *((void**)m_vpSegment - 1);
			*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize)) = NULL;

			return vpRetVal;
		}
		else
		{
			m_uiUsed -= sizeof(void*);
			return *(void**)((SCF::BYTE*)m_vpSegment + m_uiUsed);
		}
	}
	else
	{
		//Resize heap if it is full
		if ((m_uiUsed % m_uiSegmentSize) == 0)
		{
			m_uiSegmentCount++;

			void* vpSegmentNew = (m_vpSegment) ? (*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize))) : (NULL);
			if (!vpSegmentNew)
			{
				vpSegmentNew = (void**)HeapAlloc(Memory_hHeap, 0, m_uiSegmentSize + (sizeof(void*) * 2)) + 1;
			//	vpSegmentNew = (void**)VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) + 1;
			}

			if (m_vpSegment)
			{
				*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize)) = vpSegmentNew;
			}
			
			*((void**)vpSegmentNew - 1) = m_vpSegment;
			*((void**)((SCF::BYTE*)vpSegmentNew + m_uiSegmentSize)) = NULL;

			m_vpSegment = vpSegmentNew;
			m_uiUsed = m_uiBlockSize;

			return (SCF::BYTE*)m_vpSegment;
		}
		else
		{
			m_uiUsed += m_uiBlockSize;
			return (SCF::BYTE*)m_vpSegment + m_uiUsed - m_uiBlockSize;
		}
	}
}

void CFSBHeap::Free(_IN void* vpMemory)
{
	_ASSERTE(m_uiUsed > 0);

	//Accept NULL pointers
	if (!vpMemory) { return; }

	BETAONLY(FSBHeap_ui64AllocatedBytes -= m_uiBlockSize;)
	BETAONLY(FSBHeap_uiBlockCount--;)

	m_uiInfo += sizeof(void*);

	//Resize heap if it is full
	if (m_uiUsed == m_uiSegmentSize)
	{
		m_uiSegmentCount++;

		void* vpSegmentNew = (m_vpSegment) ? (*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize))) : (NULL);
		if (!vpSegmentNew)
		{
			vpSegmentNew = (void**)HeapAlloc(Memory_hHeap, 0, m_uiSegmentSize + (sizeof(void*) * 2)) + 1;
		//	vpSegmentNew = (void**)VirtualAlloc(NULL, 4096, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE) + 1;
		}

		if (m_vpSegment)
		{
			*((void**)((SCF::BYTE*)m_vpSegment + m_uiSegmentSize)) = vpSegmentNew;
		}

		*((void**)vpSegmentNew - 1) = m_vpSegment;
		*((void**)((SCF::BYTE*)vpSegmentNew + m_uiSegmentSize)) = NULL;

		m_vpSegment = vpSegmentNew;
		m_uiUsed = sizeof(void*);

		*(const void**)m_vpSegment = vpMemory;
	}
	else
	{
		//Free specified block
		*(const void**)((SCF::BYTE*)m_vpSegment + m_uiUsed) = vpMemory;
		m_uiUsed += sizeof(void*);
	}
}

SCF::UINT CFSBHeap::AllocatedBytes() _GET
{
	return m_uiSegmentSize * __max((int)m_uiSegmentCount - 1, 0) + m_uiUsed - m_uiInfo - ((m_uiInfo / sizeof(void*)) * m_uiBlockSize); 
}

void *__stdcall CFSBHeap::operator new(size_t uiBytes)
{
	return HeapAlloc(Memory_hHeap, 0, uiBytes);
}

void __stdcall CFSBHeap::operator delete(void* vpObject)
{
	HeapFree(Memory_hHeap, 0, vpObject);
}
