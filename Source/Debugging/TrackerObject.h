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
		SCF::ENUM ClassKey() _GET { return ClassTrackerObject; }
		CString   ToString() _GET { return STRING("{TrackerObject}"); }

	public:
		CTrackerObject(_IN _REF CVector<CEnum>* pClassKeys = NULL);
		virtual ~CTrackerObject();

	public:
		const CBagInt64& Objects() _GET { return m_Objects; }

	protected:
		CBagInt64 m_Objects;

	protected:
#pragma warning(disable:4251)
		//An optional list of class-keys to use as a filter (only track classes in the list)
		const CVector<CEnum>* m_pClassKeys;
#pragma warning(default:4251)

		////////////////////////////////// STATIC //////////////////////////////////////
	protected:
		static void Hook();
		static void Unhook();

	protected:
		//These two hooks are registered upon DLL load & perform the actual object tracking
		static void HookConstructor(_IN CObject& rObject);
		static void HookDestructor (_IN CObject& rObject);
	};
};

#endif


