#include "TrackerMemory.h"

using namespace SCFDebugging;
using namespace SCFBase;

#ifdef _BETA

CVector<CTracker> CTrackerMemory::s_Trackers;

CTrackerMemory::CTrackerMemory()
{
	s_Trackers.LastAdd(*this);

	Hook();
}

CTrackerMemory::~CTrackerMemory() 
{
	s_Trackers.Remove(*this);

	if (s_Trackers.Size() == 0) { Unhook(); }
}

void CTrackerMemory::Hook()
{
	CMemory::RegisterDebugHookAllocate(HookAllocate);
	CMemory::RegisterDebugHookFree    (HookFree);
}

void CTrackerMemory::Unhook()
{
	CMemory::RegisterDebugHookAllocate(nullptr);
	CMemory::RegisterDebugHookFree    (nullptr);
}

void CTrackerMemory::HookAllocate(_IN void* pMemory)
{
	for (UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerMemory&)s_Trackers[i]).m_Blocks.Add((UINT64)pMemory);
		}
	}
}

void CTrackerMemory::HookFree(_IN void* pMemory)
{
	for (UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerMemory&)s_Trackers[i]).m_Blocks.Remove((UINT64)pMemory);
		}
	}
}

#endif