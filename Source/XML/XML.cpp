#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

extern bool SCFXMLObjectSerializableInitialize();
extern bool SCFXMLObjectSerializableCleanUp();

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(hModule);
	UNREFERENCED_PARAMETER(lpReserved);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		{
			SCFXMLObjectSerializableInitialize();
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
			SCFXMLObjectSerializableCleanUp();
			break;
		}
	}
	return TRUE;
}