#include "stdafx.h"

#include "TestFSBHeap.h"

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	SCF_UNREFERENCED_PARAMETER(argc);
	SCF_UNREFERENCED_PARAMETER(argv);

	CStreamConsoleWrite StreamConsole;

	CFloat::DecimalSeparator(',');
	CFloat::FractionDigits(0, 8);

	//CStreamConsoleWrite ConsoleWrite;

	for (UINT i = 0; i < 20; i++)
	{
		CTestFSBHeap TestFSBHeaps(StreamConsole);
		TestFSBHeaps.Perform();

		StreamConsole.PutString(STRING("\nTest run: "));
		StreamConsole.PutString(CInt(i + 1).ToString());
		StreamConsole.PutString(STRING("\n\n"));
	}

	getchar();
	return 0;
}

