#include "StreamConsoleWrite.h"

using namespace SCFBase;

#ifdef WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

HANDLE StreamConsoleWrite_ConsoleHandle = nullptr;
UINT StreamConsoleWrite_uiCharsWritten = 0;

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

	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);
//	_putwch_nolock('\n');

    #else

    std::wcout << '\n';

    #endif
}

void CStreamConsoleWrite::PutLine()
{
	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);
//	_putwch_nolock('\n');

    #else

    std::wcout << '\n';

    #endif
}

void CStreamConsoleWrite::PutString(_IN CString& rString)
{
	//register UINT uiCharsLeft = rString.Length();
	//register LPTSTR szCharsLeft = rString.Value();

	//while (uiCharsLeft > 0)
	//{
	//	_putwch_nolock(*szCharsLeft);
	//	szCharsLeft++;
	//	uiCharsLeft--;
	//}

	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, rString.Value(), rString.Length(), (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);

	#else

	std::wcout << rString.Value();

    #endif
}

void CStreamConsoleWrite::PutString(_IN LPTSTR szString)
{
	#ifdef WIN32

	UINT uiLength = 0;
	register LPTSTR szCurrentChar = szString;

	while (*szCurrentChar != 0)
	{
		uiLength++;
		szCurrentChar++;
	}

	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);
//	_cputws(szString);

	#else

	std::wcout << szString;

    #endif
}

void CStreamConsoleWrite::PutString(_IN LPTSTR szString, _IN UINT uiLength)
{
	#ifdef WIN32

	//register UINT uiCharsLeft = uiLength;
	//register LPTSTR szCharsLeft = szString;

	//while (uiCharsLeft > 0)
	//{
	//	_putwch_nolock(*szCharsLeft);
	//	szCharsLeft++;
	//	uiCharsLeft--;
	//}

	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);

	#else

    CString value(szString, uiLength, true);

	std::wcout << value.Value();

    #endif
}

void CStreamConsoleWrite::PutChar(_IN TCHAR cChar)
{
	#ifdef WIN32

	WriteConsole(StreamConsoleWrite_ConsoleHandle, &cChar, 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, nullptr);

	#else

	std::wcout << cChar;

    #endif
}
