#include "../../Include/SCFTimer.h"

namespace SCFTimerPrivate
{
	INT64 g_i64Frequency = 0;							
	float g_fPeriod      = 0;

	INT64 g_i64Start01 = 0;	
	INT64 g_i64Start02 = 0;	
	INT64 g_i64Start03 = 0;	

	INT64 g_i64Temp = 0;	
};

bool SCFTimer::TimerInitialize()								
{
	if (!QueryPerformanceFrequency((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Frequency)) { return FALSE; }
	else
	{
		QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp);	
		SCFTimerPrivate::g_fPeriod = (float) (1.0 / ((float)SCFTimerPrivate::g_i64Frequency));	
	}

	return TRUE;
}
