#pragma once

#include <SCFStandard.h>

#define  WIN32_LEAN_AND_MEAN
#include <Windows.h>

#define SCF_TIMER_2

namespace SCFTimerPrivate
{
	extern INT64 g_i64Frequency;							
	extern float g_fPeriod;

	extern INT64 g_i64Start01;	
	extern INT64 g_i64Start02;	
	extern INT64 g_i64Start03;	

	extern INT64 g_i64Temp;							
};

namespace SCFTimer
{
	class CTimer
	{
	public:
		inline void Reset() { QueryPerformanceCounter((LARGE_INTEGER*)&m_i64Start); }					
	
		inline float Query()      { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp); return (float)(SCFTimerPrivate::g_i64Temp - m_i64Start) * SCFTimerPrivate::g_fPeriod; }
		inline INT64 QueryCount() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp); return SCFTimerPrivate::g_i64Temp - m_i64Start; }

	public:
		static inline INT64 Frequency() { return SCFTimerPrivate::g_i64Frequency; }
		static inline float Period()    { return SCFTimerPrivate::g_fPeriod; }

	private:
		INT64 m_i64Start;
	};

	bool TimerInitialize();

	inline void ResetTime01() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Start01); }					
	inline void ResetTime02() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Start02); }
	inline void ResetTime03() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Start03); }

	inline float GetTime01() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp); return (float)(SCFTimerPrivate::g_i64Temp - SCFTimerPrivate::g_i64Start01) * SCFTimerPrivate::g_fPeriod; }
	inline float GetTime02() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp); return (float)(SCFTimerPrivate::g_i64Temp - SCFTimerPrivate::g_i64Start02) * SCFTimerPrivate::g_fPeriod; }				
	inline float GetTime03() { QueryPerformanceCounter((LARGE_INTEGER*)&SCFTimerPrivate::g_i64Temp); return (float)(SCFTimerPrivate::g_i64Temp - SCFTimerPrivate::g_i64Start03) * SCFTimerPrivate::g_fPeriod; }				
};