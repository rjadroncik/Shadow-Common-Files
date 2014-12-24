#include "TrackerMemory.h"

using namespace SCFDebugging;
using namespace SCFBase;

#ifdef _BETA

CTrackerMemory::CTrackerMemory()
{
	Hook();
}

CTrackerMemory::~CTrackerMemory() 
{
	SCF::UINT uiMemoryTrackerCount = 0;

	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (s_Trackers[i].IsInstanceOf(ClassTrackerMemory)) { uiMemoryTrackerCount++; }
	}

	if (uiMemoryTrackerCount == 1) { Unhook(); }
}

void CTrackerMemory::Hook()
{
	CMemory::RegisterDebugHookAllocate(HookAllocate);
	CMemory::RegisterDebugHookFree    (HookFree);
}

void CTrackerMemory::Unhook()
{
	CMemory::RegisterDebugHookAllocate(NULL);
	CMemory::RegisterDebugHookFree    (NULL);
}

void CTrackerMemory::HookAllocate(_IN void* pMemory)
{
	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (s_Trackers[i].IsInstanceOf(ClassTrackerMemory) && ((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerMemory&)s_Trackers[i]).m_Blocks.Add((SCF::UINT64)pMemory);
		}
	}
}

void CTrackerMemory::HookFree(_IN void* pMemory)
{
	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (s_Trackers[i].IsInstanceOf(ClassTrackerMemory) && ((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerMemory&)s_Trackers[i]).m_Blocks.Remove((SCF::UINT64)pMemory);
		}
	}
}

#endif