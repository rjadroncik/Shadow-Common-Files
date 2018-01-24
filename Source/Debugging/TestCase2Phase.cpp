#include "TestCase2Phase.h"
#include "TrackerObject.h"

using namespace SCFDebugging;
using namespace SCFBase;

CTestCase2Phase::CTestCase2Phase(_IN CString& rName, _INOUT IStreamWriteText* pErrorStream) : CTestCase(rName, pErrorStream)
{
}

CTestCase2Phase::~CTestCase2Phase() 
{
}
 
bool CTestCase2Phase::Perform()
{
	CError::Stream()->PutString(STRING("==== Test Case: "));
	CError::Stream()->PutString(m_csName);
	CError::Stream()->PutString(STRING(" : Performing ====\n"));
	
#ifdef _BETA
	CError::Stream()->PutString(STRING("\nObject count: "));
	CError::Stream()->PutString(CInt(CObject::TotalSystemCount()).ToString());
	CError::Stream()->PutString(STRING("\n\n"));
	m_uiGlobalObjectCount = CObject::TotalSystemCount();

	CTrackerObject* pTracker = nullptr;

	if (m_bTracing)
	{
		pTracker = new CTrackerObject();
		pTracker->Enabled(true);

		CError::Stream()->PutString(STRING("Tracking object creation/deletion.\n\n"));
	}
#endif

	CError::Stream()->PutString(STRING("==== 1st Stage : Performing ====\n"));

	if (!CTestCase::PerformInternal()) { return false; }

	CError::Stream()->PutString(STRING("==== 1st Stage : Finished ====\n"));

	CError::Stream()->PutString(STRING("==== 2nd Stage : Performing ====\n"));

	CError::Stream()->PutString(STRING("Stage: Prepare\n"));
	if (!Prepare2())
	{
		ErrorReport(ErrorTestCase2ndPhasePrepareStageFailed, STRING("Prepare"));
		return false;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: Run\n"));
	if (!Run2())
	{
		ErrorReport(ErrorTestCase2ndPhaseRunStageFailed, STRING("Run"));
		return false;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: Check\n"));
	if (!Check2())
	{
		ErrorReport(ErrorTestCase2ndPhaseCheckStageFailed, STRING("Check"));
		return false;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: CleanUp\n"));
	if (!CleanUp2())
	{
		ErrorReport(ErrorTestCase2ndPhaseCleanUpStageFailed, STRING("CleanUp"));
		return false;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("==== 2nd Stage : Finished ====\n"));

#ifdef _BETA
	if (m_bTracing)
	{
		pTracker->Enabled(false);

		//Need to control enumerator life-span, must be deleted BEFORE the tracker
		if (pTracker->Objects().Size())
		{
			CEnumeratorSetInt64 ObjectEnumerator(pTracker->Objects());

			CError::Stream()->PutString(STRING("Tracked undeleted objects:\n"));
			CInt Counter(1);

			while (ObjectEnumerator.Next())
			{
				CObject* pCurrent = (CObject*)ObjectEnumerator.Current();

				CError::Stream()->PutString(Counter.ToString());
				CError::Stream()->PutString(STRING(": "));
				CError::Stream()->PutLine(pCurrent->ToString());
				Counter += 1;
			}
		}

		delete pTracker;
	}
	CError::Stream()->PutString(STRING("\nObject count: "));
	CError::Stream()->PutString(CInt(CObject::TotalSystemCount()).ToString());
	CError::Stream()->PutString(STRING("\n\n"));

	if (m_bTracing)
	{
		if (m_uiGlobalObjectCount != CObject::TotalSystemCount())
		{
			ErrorReport(ErrorTestCase2ndPhaseObjectsLeaked, STRING("Object Leak Test"));
			return false;
		}
	}

#endif

	CError::Stream()->PutString(STRING("==== Test Case: "));
	CError::Stream()->PutString(m_csName);
	CError::Stream()->PutString(STRING(" : Finished ====\n"));
	return true; 
}

bool CTestCase2Phase::Prepare2() { return true; }
bool CTestCase2Phase::Run2()     { return true; }
bool CTestCase2Phase::Check2()   { return true; }
bool CTestCase2Phase::CleanUp2() { return true; }
