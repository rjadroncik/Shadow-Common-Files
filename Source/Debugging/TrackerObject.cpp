#include "TrackerObject.h"

#ifdef _BETA

using namespace SCFDebugging;
using namespace SCFBase;

CTrackerObject::CTrackerObject(_IN CVector<CEnum>* pClassKeys)
{
	m_pClassKeys = pClassKeys;

	Hook();
}

CTrackerObject::~CTrackerObject() 
{
	SCF::UINT uiObjectTrackerCount = 0;

	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (s_Trackers[i].IsInstanceOf(ClassTrackerObject)) { uiObjectTrackerCount++; }
	}

	if (uiObjectTrackerCount == 1) { Unhook(); }
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
		if (s_Trackers[i].IsInstanceOf(ClassTrackerObject) && ((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerObject&)s_Trackers[i]).m_Objects.Add((SCF::UINT64)&rObject);
		}
	}
}

void CTrackerObject::HookDestructor(_IN CObject& rObject)
{
	for (SCF::UINT i = 0; i < s_Trackers.Size(); i++)
	{
		if (s_Trackers[i].IsInstanceOf(ClassTrackerObject) && ((CTracker&)s_Trackers[i]).Enabled())
		{
			((CTrackerObject&)s_Trackers[i]).m_Objects.Remove((SCF::UINT64)&rObject);
		}
	}
}
#endif
