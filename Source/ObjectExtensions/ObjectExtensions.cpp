#include <SCFObjectExtensions.h>
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;

extern HANDLE StreamConsoleWrite_ConsoleHandle;
extern HANDLE Memory_hHeap;

extern bool SCFObjectSerializableInitialize(void* hModule);
extern bool SCFObjectSerializableCleanUp();

bool SCFMemoryInitialize()
{
	Memory_hHeap = GetProcessHeap();
	return TRUE;
}

bool SCFStreamConsoleWriteInitialize()
{
	StreamConsoleWrite_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return (StreamConsoleWrite_ConsoleHandle != INVALID_HANDLE_VALUE);
}

bool SCFClassCleanUp()
{
	SCFObjectSerializableCleanUp();

	return TRUE;
}

void SCFObjectExtensionsInitialize(_IN HMODULE hModule)
{
	SCFMemoryInitialize();
	SCFErrorInitialize();
	SCFObjectSerializableInitialize(hModule);
	SCFStreamConsoleWriteInitialize();

	CLASS_SERIALIZABLE_REGISTER(ClassMemoryBlock, SCFBase::CMemoryBlock, (HMODULE)hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryString, SCFBase::CDictionaryStringRaw, (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryInt64,  SCFBase::CDictionaryInt64,		   (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryObject, SCFBase::CDictionaryObjectRaw, (HMODULE)hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassTreeSimple,  SCFBase::CTreeSimple,  (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassStack,       SCFBase::CStack,       (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassVector,      SCFBase::CVectorRaw,   (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassVectorRange, SCFBase::CVectorRange, (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassList,        SCFBase::CListRaw,     (HMODULE)hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassBool,     SCFBase::CBool,     (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat,    SCFBase::CFloat,    (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat2,   SCFBase::CFloat2,   (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat3,   SCFBase::CFloat3,   (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat4,   SCFBase::CFloat4,   (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassChar,     SCFBase::CChar,     (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassInt,      SCFBase::CInt,      (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassPointer,  SCFBase::CPointer,  (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDateTime, SCFBase::CDateTime, (HMODULE)hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassArrayFloat,  SCFBase::CArrayFloat,  (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassArrayInt,    SCFBase::CArrayInt,    (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassAssociation, SCFBase::CAssociation, (HMODULE)hModule);
	
	CLASS_SERIALIZABLE_REGISTER(ClassString,      SCFBase::CString,      (HMODULE)hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassStringRange, SCFBase::CStringRange, (HMODULE)hModule);
}

void SCFObjectExtensionsCleanUp()
{
	SCFClassCleanUp();
	SCFErrorCleanUp();
}

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
	return TRUE;
}

//extern "C"
//int __cdecl _purecall()
//{
//	MessageBox(NULL, SCFTEXT("Pure virtual call"), SCFTEXT("System error"), MB_ICONERROR);
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
//	return FALSE;
//}
