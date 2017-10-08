#include "TestCase.h"
#include "TrackerObject.h"

using namespace SCFDebugging;
using namespace SCFBase;

CTestCase::CTestCase(_IN CString& rName, _INOUT IStreamWriteText* pErrorStream)
{
	m_csName = rName;
	BETAONLY(m_bTracing = FALSE;)

	if (pErrorStream) { CError::Stream(pErrorStream); m_pErrorStream = pErrorStream; }
	else { m_pErrorStream = NULL; }
}

CTestCase::~CTestCase() 
{
	if (m_pErrorStream) { CError::Stream(m_pErrorStream); }
}

bool CTestCase::PerformInternal()
{
	CError::Stream()->PutString(STRING("Stage: Prepare\n"));
	if (!Prepare())
	{
		ErrorReport(ErrorTestCasePrepareStageFailed, STRING("Prepare"));
		return FALSE;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: Run\n"));
	if (!Run())
	{
		ErrorReport(ErrorTestCaseRunStageFailed, STRING("Run"));
		return FALSE;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: Check\n"));
	if (!Check())
	{
		ErrorReport(ErrorTestCaseCheckStageFailed, STRING("Check"));
		return FALSE;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	CError::Stream()->PutString(STRING("Stage: CleanUp\n"));
	if (!CleanUp())
	{
		ErrorReport(ErrorTestCaseCleanUpStageFailed, STRING("CleanUp"));
		return FALSE;
	}
	CError::Stream()->PutString(STRING("OK\n"));

	return TRUE;
}

bool CTestCase::Perform()
{
	CError::Stream()->PutString(STRING("==== Test Case: "));
	CError::Stream()->PutString(m_csName);
	CError::Stream()->PutString(STRING(" : Performing ====\n"));

#ifdef _BETA
	CError::Stream()->PutString(STRING("\nObject count: "));
	CError::Stream()->PutString(CInt(CObject::TotalSystemCount()).ToString());
	CError::Stream()->PutString(STRING("\n\n"));
	m_uiGlobalObjectCount = CObject::TotalSystemCount();

	CTrackerObject* pTracker = NULL;

	if (m_bTracing)
	{
		pTracker = new CTrackerObject();
		pTracker->Enabled(TRUE);

		CError::Stream()->PutString(STRING("Tracking object creation/deletion.\n\n"));
	}
#endif

	if (!PerformInternal()) { return FALSE; }

#ifdef _BETA
	if (m_bTracing)
	{
		pTracker->Enabled(FALSE);

		//Need to control enumerator life-span, must be deleted BEFORE the tracker
		if (pTracker->Objects().Size())
		{
			CEnumeratorSetInt64 ObjectEnumerator(pTracker->Objects());

			CError::Stream()->PutString(STRING("Tracked undeleted objects:\n"));
			CInt Counter(1);

			while (ObjectEnumerator.Next())
			{
			   auto pCurrent = reinterpret_cast<CObject*>(ObjectEnumerator.Current());

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
	 		ErrorReport(ErrorTestCaseObjectsLeaked, STRING("Object Leak Test"));
	 		return FALSE;
	 	}
	}

#endif

	CError::Stream()->PutString(STRING("==== Test Case: "));
	CError::Stream()->PutString(m_csName);
	CError::Stream()->PutString(STRING(" : Finished ====\n"));
	return TRUE; 
}

bool CTestCase::Prepare() { return TRUE; }
bool CTestCase::Run()     { return TRUE; }
bool CTestCase::Check()   { return TRUE; }
bool CTestCase::CleanUp() { return TRUE; }

void CTestCase::ErrorReport(_IN ENUM eError, _IN CString& rStage) 
{
	CError::Stream()->PutString(STRING("error : "));
	CError::Stream()->PutString(m_csName);
	CError::Stream()->PutString(STRING(" : '"));
	CError::Stream()->PutString(rStage);
	CError::Stream()->PutString(STRING("' : test case stage failed\n"));

	SCFError(eError);
}
