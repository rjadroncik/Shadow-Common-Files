#include <SCFObjectExtensions.h>
#define WIN32_LEAN_AND_MEAN

using namespace SCFBase;

#ifdef WIN32

#include <Windows.h>

extern HANDLE StreamConsoleWrite_ConsoleHandle;

#else

#endif

bool SCFMemoryInitialize()
{
	return true;
}

bool SCFStreamConsoleWriteInitialize()
{
    #ifdef WIN32

	StreamConsoleWrite_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return (StreamConsoleWrite_ConsoleHandle != INVALID_HANDLE_VALUE);

    #else

    return true;

    #endif
}

bool SCFClassCleanUp()
{
	return true;
}

void SCFObjectExtensionsInitialize(_IN void* hModule)
{
	SCF_UNREFERENCED_PARAMETER(hModule);

	SCFMemoryInitialize();
	SCFErrorInitialize();
	SCFStreamConsoleWriteInitialize();
}

void SCFObjectExtensionsCleanUp()
{
	SCFClassCleanUp();
	SCFErrorCleanUp();
}

#ifdef WIN32

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	SCF_UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		{
			//SYSTEM_INFO SysInfo;
			//GetSystemInfo(&SysInfo);

			//intptr_t hCrtHeap = _get_heap_handle();
			//ULONG ulEnableLFH = 2;

			//HeapSetInformation((PVOID)hCrtHeap, HeapCompatibilityInformation, &ulEnableLFH, sizeof(ulEnableLFH));

			//_set_sbh_threshold(128);

			SCFObjectExtensionsInitialize(hModule);
			break;
		}
	case DLL_THREAD_ATTACH:
		{
			break;
		}
	case DLL_THREAD_DETACH:
		{
			break;
		}
	case DLL_PROCESS_DETACH:
		{
			SCFObjectExtensionsCleanUp();
			break;
		}
	}
	return true;
}

#else

int main()
{
    return 0;
}

__attribute__((constructor))
static void SCFObjectExtensionsInitializeGCC()
{

}

__attribute__((destructor))
static void SCFObjectExtensionsCleanUpGCC()
{
    SCFObjectExtensionsCleanUp();
}

#endif

//extern "C"
//int __cdecl _purecall()
//{
//	MessageBox(nullptr, SCFTEXT("Pure virtual call"), SCFTEXT("System error"), MB_ICONERROR);
//	DebugBreak();
//	return 0;
//}
//
//extern "C"
//int __cdecl atexit(void (__cdecl *func) (void))
//{
//	return 0;
//}
//
//extern "C"
//BOOL APIENTRY _DllMainCRTStartup(HANDLE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
//{
//
//	return false;
//}
