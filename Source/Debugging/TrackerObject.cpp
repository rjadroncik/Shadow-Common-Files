#include "TrackerObject.h"

#ifdef _BETA

using namespace SCFDebugging;
using namespace SCFBase;

CVector<CTracker> CTrackerObject::s_Trackers;

CTrackerObject::CTrackerObject()
{
	s_Trackers.LastAdd(*this);

	Hook();
}

CTrackerObject::~CTrackerObject() 
{
	s_Trackers.Remove(*this);

	if (s_Trackers.Size() == 0) { Unhook(); }
}

void CTrackerObject::Hook()
{
	CObject::RegisterDebugHookConstructor(HookConstructor);
	CObject::RegisterDebugHookDestructor (HookDestructor);
}

void CTrackerObject::Unhook()
{
	CObject::RegisterDebugHookConstructor(NULL);
	CObject::RegisterDebugHookDestructor (NULL);
}

void CTrackerObject::HookConstructor(_IN CObject& rObject)
{
	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerObject&)s_Trackers[i]).m_Objects.Add((SCF::UINT64)&rObject);
		}
	}
}

void CTrackerObject::HookDestructor(_IN CObject& rObject)
{
	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerObject&)s_Trackers[i]).m_Objects.Remove((SCF::UINT64)&rObject);
		}
	}
}
#endif
