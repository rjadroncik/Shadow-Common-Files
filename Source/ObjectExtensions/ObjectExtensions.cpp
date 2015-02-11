#include <SCFObjectExtensions.h>
#define WIN32_LEAN_AND_MEAN

using namespace SCFBase;

#ifdef WIN32

#include <Windows.h>

extern HANDLE StreamConsoleWrite_ConsoleHandle;

#else

#endif

extern bool SCFObjectSerializableInitialize(_IN void* hModule);
extern bool SCFObjectSerializableCleanUp();

bool SCFMemoryInitialize()
{
	return TRUE;
}

bool SCFStreamConsoleWriteInitialize()
{
    #ifdef WIN32

	StreamConsoleWrite_ConsoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);

	return (StreamConsoleWrite_ConsoleHandle != INVALID_HANDLE_VALUE);

    #else

    return TRUE;

    #endif
}

bool SCFClassCleanUp()
{
	SCFObjectSerializableCleanUp();

	return TRUE;
}

void SCFObjectExtensionsInitialize(_IN void* hModule)
{
	SCFMemoryInitialize();
	SCFErrorInitialize();
	SCFObjectSerializableInitialize(hModule);
	SCFStreamConsoleWriteInitialize();

	CLASS_SERIALIZABLE_REGISTER(ClassMemoryBlock, SCFBase::CMemoryBlock, hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryString, SCFBase::CDictionaryStringRaw, hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryInt64,  SCFBase::CDictionaryInt64,     hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDictionaryObject, SCFBase::CDictionaryObjectRaw, hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassTreeSimple,  SCFBase::CTreeSimple,     hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassStack,       SCFBase::CStack,          hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassVector,      SCFBase::CVectorRaw,      hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassVectorRange, SCFBase::CVectorRangeRaw, hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassList,        SCFBase::CListRaw,        hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassBool,     SCFBase::CBool,     hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat,    SCFBase::CFloat,    hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat2,   SCFBase::CFloat2,   hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat3,   SCFBase::CFloat3,   hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassFloat4,   SCFBase::CFloat4,   hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassChar,     SCFBase::CChar,     hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassInt,      SCFBase::CInt,      hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassPointer,  SCFBase::CPointer,  hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassDateTime, SCFBase::CDateTime, hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassArrayFloat,  SCFBase::CArrayFloat,  hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassArrayInt,    SCFBase::CArrayInt,    hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassAssociation, SCFBase::CAssociation, hModule);

	CLASS_SERIALIZABLE_REGISTER(ClassString,      SCFBase::CString,      hModule);
	CLASS_SERIALIZABLE_REGISTER(ClassStringRange, SCFBase::CStringRange, hModule);
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
	return TRUE;
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
