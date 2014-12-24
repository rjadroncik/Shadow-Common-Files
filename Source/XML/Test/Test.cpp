#include <SCFXML.h>

#include "TestDOM.h"
#include "TestReader.h"
#include "TestDocument.h"
#include "TestPath.h"
#include "TestStreams.h"
#include "TestSerialization.h"

#include <stdio.h>
#include <tchar.h>

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace SCFBase;

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	SCF_UNREFERENCED_PARAMETER(argc);
	SCF_UNREFERENCED_PARAMETER(argv);

	CStreamConsoleWrite StreamConsole;
	StreamConsole.PutLine(CInfoCPU::Report());

	for (UINT i = 0; i < 100; i++)
	{
		StreamConsole.PutString(STRING("\nTest run: "));
		StreamConsole.PutString(CInt(i + 1).ToString());
		StreamConsole.PutString(STRING("\n\n"));

		CTestDOM           TestDOM          (StreamConsole);
		CTestReader        TestReader       (StreamConsole);
		CTestDocument      TestDocument     (StreamConsole);
		CTestPath          TestPath         (StreamConsole);
		CTestStreams       TestStreams      (StreamConsole);
		CTestSerialization TestSerialization(StreamConsole);

		BETAONLY(if (i > 0) { CObject::Tracing(TRUE);   })
		BETAONLY(if (i > 0) { TestReader.Tracing(TRUE); })
		BETAONLY(if (i > 0) { TestPath.Tracing(TRUE); })

		TestDOM.Perform();
		TestReader.Perform();
		TestDocument.Perform();
		TestPath.Perform();
		TestStreams.Perform();
		TestSerialization.Perform();
	}
	
	getchar();
	return 0;
}

