#pragma once

#include "Tracker.h"

#ifdef _BETA

namespace SCFDebugging
{
	//Description: Allows tracking memory allocation.
	//
	//Usage: Create, enable, then run tracked code, disable, get blocks.
	//       The result of tracking are blocks which have been allocated but 
	//       NOT freed while the tracker was enabled.
	//
	//Notes: This class is NOT a singleton.
	//
	//Version: 1.000

	class DEBUGGING_API CTrackerMemory : public CTracker
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassTrackerMemory; }
		CString   ToString() _GET { return STRING("{TrackerMemory}"); }

	public:
		CTrackerMemory();
		virtual ~CTrackerMemory();

	public:
		const CBagInt64& Blocks() _GET { return m_Blocks; }

	protected:
		CBagInt64 m_Blocks;

		////////////////////////////////// STATIC //////////////////////////////////////
	protected:
		static void Hook();
		static void Unhook();

	protected:
		//These two hooks are registered upon DLL load & perform the actual object tracking
		static void HookAllocate(_IN void* pMemory);
		static void HookFree    (_IN void* pMemory);
	};
};

#endif


