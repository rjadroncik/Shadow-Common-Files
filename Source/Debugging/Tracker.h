#pragma once

#include "Errors.h"

#include <SCFObjectExtensions.h>

#ifdef DEBUGGING_EXPORTS
#define DEBUGGING_API __declspec(dllexport)
#else
#define DEBUGGING_API __declspec(dllimport)
#endif

#ifdef _BETA

namespace SCFDebugging
{
	//Description: Base class for tracking classes.
	//
	//Usage: Create, enable, then run tracked code, disable, get results.
	//
	//Notes: These classes are NOT a singletons.
	//       Results depend on the nature of the tracker (i.e. memory leaks, lost objects)
	//
	//Version: 2.000

	class DEBUGGING_API CTracker : public SCFBase::CObject
	{
	public:
		void Enabled(_IN bool bEnabled) _SET { m_bEnabled = bEnabled; }
		bool Enabled()                  _GET { return m_bEnabled; }

	protected:
		bool m_bEnabled;

	protected:
		CTracker();
		virtual ~CTracker();
	};
};

#endif

