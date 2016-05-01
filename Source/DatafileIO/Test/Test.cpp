#include "stdafx.h"

#include "TestDatafile.h"

int __cdecl main(int argc, ::TCHAR* argv[])
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

		CTestDatafile TestDatafile(StreamConsole);

		TestDatafile.Perform();
	}

	getchar();
	return 0;
}

