#include "UserInterface.h"
#include "Paint.h"

using namespace SCFUI;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	UNREFERENCED_PARAMETER(hModule);

	static ULONG_PTR s_pToken;

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		{
			GdiplusStartupInput Input;

			GdiplusStartup(&s_pToken, &Input, NULL);

			CPaint::Initialize();
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
			GdiplusShutdown(s_pToken);
			break;
		}
	}
	return TRUE;
} 