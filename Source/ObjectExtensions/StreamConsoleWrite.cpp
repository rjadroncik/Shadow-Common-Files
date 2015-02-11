#include "StreamConsoleWrite.h"

using namespace SCFBase;

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

HANDLE StreamConsoleWrite_ConsoleHandle = NULL;
SCF::UINT StreamConsoleWrite_uiCharsWritten = 0;

#else

#include <iostream>

#endif

CStreamConsoleWrite::CStreamConsoleWrite()
{
}

CStreamConsoleWrite::~CStreamConsoleWrite()
{
}

void CStreamConsoleWrite::PutLine(_IN CString& rString)
{
	PutString(rString);

	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
//	_putwch_nolock('\n');

    #else

    std::wcout << '\n';

    #endif
}

void CStreamConsoleWrite::PutLine()
{
	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
//	_putwch_nolock('\n');

    #else

    std::wcout << '\n';

    #endif
}

void CStreamConsoleWrite::PutString(_IN CString& rString)
{
	//register SCF::UINT uiCharsLeft = rString.Length();
	//register SCF::LPTSTR szCharsLeft = rString.Value();

	//while (uiCharsLeft > 0)
	//{
	//	_putwch_nolock(*szCharsLeft);
	//	szCharsLeft++;
	//	uiCharsLeft--;
	//}

	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, rString.Value(), rString.Length(), (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);

	#else

	std::wcout << rString.Value();

    #endif
}

void CStreamConsoleWrite::PutString(_IN SCF::LPTSTR szString)
{
	#ifdef WIN32

	SCF::UINT uiLength = 0;
	register SCF::LPTSTR szCurrentChar = szString;

	while (*szCurrentChar != 0)
	{
		uiLength++;
		szCurrentChar++;
	}

	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
//	_cputws(szString);

	#else

	std::wcout << szString;

    #endif
}

void CStreamConsoleWrite::PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength)
{
	#ifdef WIN32

	//register SCF::UINT uiCharsLeft = uiLength;
	//register SCF::LPTSTR szCharsLeft = szString;

	//while (uiCharsLeft > 0)
	//{
	//	_putwch_nolock(*szCharsLeft);
	//	szCharsLeft++;
	//	uiCharsLeft--;
	//}

	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);

	#else

    CString value(szString, uiLength, true);

	std::wcout << value.Value();

    #endif
}

void CStreamConsoleWrite::PutChar(_IN SCF::TCHAR cChar)
{
	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, &cChar, 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);

	#else

	std::wcout << cChar;

    #endif
}
