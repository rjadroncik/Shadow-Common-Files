#include "Tracker.h"

using namespace SCFDebugging;
using namespace SCFBase;

#ifdef _BETA

CVector<CTracker> CTracker::s_Trackers;

CTracker::CTracker()
{
	m_bEnabled = FALSE;

	s_Trackers.LastAdd(*this);
}

CTracker::~CTracker() 
{
	s_Trackers.Remove(*this);
}

#endif