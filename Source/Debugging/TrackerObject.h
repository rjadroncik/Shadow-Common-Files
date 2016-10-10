#pragma once

#include "Tracker.h"

#ifdef _BETA

namespace SCFDebugging
{
	//Description: Allows tracking object creation & deletion.
	//
	//Usage: Create, enable, then run tracked code, disable, get objects.
	//       The result of tracking are objects which have been created but 
	//       NOT deleted while the tracker was enabled.
	//
	//Notes: This class is NOT a singleton.
	//
	//Version: 1.000

	class DEBUGGING_API CTrackerObject : public CTracker
	{
	public:
		CString ToString() _GET { return STRING("{TrackerObject}"); }

	public:
		CTrackerObject();
		virtual ~CTrackerObject();

	public:
		const CSetInt64& Objects() _GET { return m_Objects; }

	protected:
		CSetInt64 m_Objects;

		////////////////////////////////// STATIC //////////////////////////////////////
	protected:
		static void Hook();
		static void Unhook();

	protected:
		//These two hooks are registered upon DLL load & perform the actual object tracking
		static void HookConstructor(_IN CObject& rObject);
		static void HookDestructor (_IN CObject& rObject);

	protected:
		//Contains all the created trackers (this assures that we can find forgotten trackers :)
#pragma warning(disable:4251)
		static CVector<CTracker> s_Trackers;
#pragma warning(default:4251)
	};
};

#endif


