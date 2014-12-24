#pragma once

namespace SCFDebugging
{
	enum Errors
	{
		ErrorTestCasePrepareStageFailed = 0x0201,
		ErrorTestCaseRunStageFailed,
		ErrorTestCaseCheckStageFailed,
		ErrorTestCaseCleanUpStageFailed,
		ErrorTestCaseObjectsLeaked,

		ErrorTestCase2ndPhasePrepareStageFailed = 0x0201,
		ErrorTestCase2ndPhaseRunStageFailed,
		ErrorTestCase2ndPhaseCheckStageFailed,
		ErrorTestCase2ndPhaseCleanUpStageFailed,
		ErrorTestCase2ndPhaseObjectsLeaked,
	};
};