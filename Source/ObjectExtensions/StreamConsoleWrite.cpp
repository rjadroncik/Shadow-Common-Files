#include "StreamConsoleWrite.h"

using namespace SCFBase;

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

HANDLE StreamConsoleWrite_ConsoleHandle = NULL;
SCF::UINT StreamConsoleWrite_uiCharsWritten = 0;

CStreamConsoleWrite::CStreamConsoleWrite()
{
}

CStreamConsoleWrite::~CStreamConsoleWrite()
{
}

void CStreamConsoleWrite::PutLine(_IN CString& rString)
{
	PutString(rString);
	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
//	_putwch_nolock('\n'); 
}

void CStreamConsoleWrite::PutLine()
{
	WriteConsole(StreamConsoleWrite_ConsoleHandle, SCFTEXT("\n"), 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
//	_putwch_nolock('\n'); 
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

	WriteConsole(StreamConsoleWrite_ConsoleHandle, rString.Value(), rString.Length(), (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
}

void CStreamConsoleWrite::PutString(_IN SCF::LPTSTR szString)
{
	SCF::UINT uiLength = 0;
	register SCF::LPTSTR szCurrentChar = szString;

	while (*szCurrentChar != 0)
	{
		uiLength++;
		szCurrentChar++;
	}
	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);

//	_cputws(szString);
}

void CStreamConsoleWrite::PutString(_IN SCF::LPTSTR szString, _IN SCF::UINT uiLength)
{
	//register SCF::UINT uiCharsLeft = uiLength;
	//register SCF::LPTSTR szCharsLeft = szString;

	//while (uiCharsLeft > 0)
	//{
	//	_putwch_nolock(*szCharsLeft);
	//	szCharsLeft++;
	//	uiCharsLeft--;
	//}

	WriteConsole(StreamConsoleWrite_ConsoleHandle, szString, uiLength, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
}

void CStreamConsoleWrite::PutChar(_IN SCF::TCHAR cChar)
{
	WriteConsole(StreamConsoleWrite_ConsoleHandle, &cChar, 1, (LPDWORD)&StreamConsoleWrite_uiCharsWritten, NULL);
}