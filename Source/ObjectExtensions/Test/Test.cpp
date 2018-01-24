#include "stdafx.h"

#include "TestValues.h"
#include "TestFSBHeap.h"
#include "TestBags.h"
#include "TestCollections.h"
#include "TestDictionaries.h"
#include "TestFileSystem.h"
#include "TestStreams.h"
#include "TestCSV.h"
#include "TestShallowDelete.h"

#include <windows.h>

int __cdecl _tmain(int argc, _TCHAR* argv[])
{
	//RemoveDirectory(TEXT("D:\\Terra e.."));

	//CDirectory Dir("D:\\Terra e..\\");

	//Dir.Delete();

	//CInt::Print(17244, 10, 4);

	//CColor Farba(0xAABBCCDD);

	//BYTE R = Farba[0];
	//BYTE G = Farba[1];
	//BYTE B = Farba[2];
	//BYTE A = Farba[3];

	//A = SCF_RGBA_A(0xAABBCCDD);

	//Farba.AtPut(0, 0x11);
	//Farba.AtPut(1, 0x22);
	//Farba.AtPut(2, 0x33);
	//Farba.AtPut(3, 0x44);

	//Farba.R(0x55);
	//Farba.G(0xCC);
	//Farba.B(0xAA);
	//Farba.A(0x88);

	//CString FarbaText(Farba.ToString());

	//CColor Farba2(STRING(" #00BB33DD  "));

	SCF_UNREFERENCED_PARAMETER(argc);
	SCF_UNREFERENCED_PARAMETER(argv);

	CStreamConsoleWrite  StreamConsole;

	StreamConsole.PutLine(CInfoCPU::Report());

//	CStreamDummyWrite    StreamDummy;
//	CStreamWriteTextUCS2 StreamConsole(StreamDummy);

//	CDateTime DatumCas;
//	StreamConsole.PutLine(DatumCas.ToString());

	//CFloat::FractionDigits(2, 4);
	//
	//CFloat FloatNumber(999.9999);
	//StreamConsole.PutLine(FloatNumber.ToString());

	//CFormatFloat Format(1, 5, ',');
	//StreamConsole.PutLine(FloatNumber.ToString(Format));
	//StreamConsole.PutLine(FloatNumber.ToString());
	//StreamConsole.PutLine(FloatNumber.ToString(Format));
	//StreamConsole.PutLine(FloatNumber.ToString());
	//StreamConsole.PutLine(FloatNumber.ToString(Format));
	//StreamConsole.PutLine(FloatNumber.ToString());
	//StreamConsole.PutLine(FloatNumber.ToString(Format));
	//StreamConsole.PutLine(FloatNumber.ToString());

	//CString Retazec(FloatNumber.ToString(Format), false, true);
	//StreamConsole.PutLine(Retazec);
	//Retazec.Assign(FloatNumber.ToString());
	//StreamConsole.PutLine(Retazec);
	

	//FloatNumber = 0.995;
	//StreamConsole.PutLine(FloatNumber.ToString());

	//Test of string read/write streams
	//{
	//	CString Testicek;

	//	CStreamStringWrite TesticekSW(Testicek);
	//	CStreamWriteTextUCS2 TesticekWT(TesticekSW, false);

	//	TesticekWT.PutString(STRING("Retazec01 "));
	//	TesticekWT.PutLine(STRING("Retazec02"));
	//	TesticekWT.PutLine(STRING("Retazec03"));

	//	CStreamStringRead TesticekSR(Testicek);
	//	CStreamReadTextUCS2 TesticekRT(TesticekSR);

	//	CString Testicek2;

	//	TesticekRT.GetLine(Testicek2);
	//	TesticekRT.GetLine(Testicek2);

	//	UINT64 bytesLeft = TesticekSR.BytesLeft();
	//	UINT64 charsLeft = TesticekRT.CharsLeft();

	//	UNREFERENCED_PARAMETER(bytesLeft);
	//	UNREFERENCED_PARAMETER(charsLeft);
	//}

	for (UINT i = 0; i < 100; i++)
	{
		StreamConsole.PutString(STRING("\nTest run: "));
		StreamConsole.PutString(CInt(i + 1).ToString());
		StreamConsole.PutString(STRING("\n\n"));

		CTestValues        TestValues       (StreamConsole);
		CTestFSBHeap       TestFSBHeaps     (StreamConsole);
		CTestBags          TestFSBBags      (StreamConsole);
		CTestCollections   TestCollections  (StreamConsole);
		CTestDictionaries  TestDictionaries (StreamConsole);
		CTestFileSystem    TestFileSystem   (StreamConsole);
		CTestStreams       TestStreams      (StreamConsole);
		CTestCSV           TestCSV          (StreamConsole);
		CTestShallowDelete TestShallowDelete(StreamConsole);

#ifdef _BETA
 		if (i)
 		{
 			CObject::Tracing(true);
			
			TestValues.Tracing(true);
			TestFSBHeaps.Tracing(true);
			TestFSBBags.Tracing(true);
			TestCollections.Tracing(true);
			TestDictionaries.Tracing(true);
			TestFileSystem.Tracing(true);	
			TestStreams.Tracing(true);
			TestCSV.Tracing(true);
			TestShallowDelete.Tracing(true);			
 		}
#endif

		TestValues.Perform();
		TestFSBHeaps.Perform();
		TestFSBBags.Perform();
		TestCollections.Perform();
		TestDictionaries.Perform();
		TestFileSystem.Perform();	
		TestStreams.Perform();
		TestCSV.Perform();
		TestShallowDelete.Perform();

		StreamConsole.PutString(STRING("\nTest run: "));
		StreamConsole.PutString(CInt(i + 1).ToString());
		StreamConsole.PutString(STRING(" finished\n\n"));
	}

	getchar();
	return 0;
}

