#pragma once

#include "TestScanner.h"
#include "TestParser.h"

#include "ClassA.h"
#include "ClassB.h"

#include <stdio.h>
#include <tchar.h>

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

using namespace SCFBase;

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	UNREFERENCED_PARAMETER(argc);
	UNREFERENCED_PARAMETER(argv);

	CStreamConsoleWrite StreamConsole;

	//CClass*    pClass     = (CClass*)CClass::ByKey(ClassClassA);
	//SCFScripting::CProperty* pProperty  = pClass->PropertyNamed(STRING("TestString"));
	//SCFScripting::CProperty* pProperty2 = pClass->PropertyNamed(STRING("TestInt"));

	//CString* pText = NULL;
	//CString DakyText(STRING_CREATE_ARGS("KOKOTINA"));
	//CClassA Testik(5);
	//{
	//	pProperty->Set(Testik, &DakyText);
	//	pText = (CString*)pProperty->Get(Testik);
	//}


	for (UINT i = 0; i < 100; i++)
	{
		StreamConsole.PutString(TEXT("\nTest run: "));
		StreamConsole.PutString(CInt(i + 1).ToString());
		StreamConsole.PutString(TEXT("\n\n"));

		CTestScanner TestScanner(StreamConsole);
		CTestParser  TestParser (StreamConsole);

// 		if (i)
// 		{
// 			CObject::Tracing(TRUE);
// 			TestLexicalAnalyzer.Tracing(TRUE);
// 		}

		TestScanner.Perform();
		TestParser.Perform();
	}

	getchar();
	return 0;
}

