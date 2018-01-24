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
	RegisterDebugHookConstructor(HookConstructor);
	RegisterDebugHookDestructor (HookDestructor);
}

void CTrackerObject::Unhook()
{
	RegisterDebugHookConstructor(nullptr);
	RegisterDebugHookDestructor (nullptr);
}

void CTrackerObject::HookConstructor(_IN CObject& rObject)
{
	for (UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (static_cast<CTracker&>(s_Trackers[i]).Enabled())
		{
			static_cast<CTrackerObject&>(s_Trackers[i]).m_Objects.Add(UINT64(&rObject));
		}
	}
}

void CTrackerObject::HookDestructor(_IN CObject& rObject)
{
	for (UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (static_cast<CTracker&>(s_Trackers[i]).Enabled())
		{
			static_cast<CTrackerObject&>(s_Trackers[i]).m_Objects.Remove(UINT64(&rObject));
		}
	}
}
#endif
