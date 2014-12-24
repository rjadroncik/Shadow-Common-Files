#include "SCFDatafileIO.h"

#include <SCFWinAPIUndef.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	SCF_UNREFERENCED_PARAMETER(lpReserved);
	SCF_UNREFERENCED_PARAMETER(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			CLASS_SERIALIZABLE_REGISTER(SCFDatafileIOPrivate::ClassRecordFile,      SCFDatafileIOPrivate::CRecordFile,      (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(SCFDatafileIOPrivate::ClassRecordDirectory, SCFDatafileIOPrivate::CRecordDirectory, (HMODULE)hModule);
			break;
		}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH: { break; }
	}
    return TRUE;
}