#include <SCFObjectExtensions.h>

#include <SCFScripting.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;
using namespace SCFScripting;
using namespace SCFCompiler;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	UNREFERENCED_PARAMETER(lpReserved);
	UNREFERENCED_PARAMETER(hModule);

	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH: 
		{
			CLASS_SERIALIZABLE_REGISTER(ClassTokenKeyword,    SCFCompiler::CTokenKeyword,    (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(ClassTokenIdentifier, SCFCompiler::CTokenIdentifier, (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(ClassTokenOperator,   SCFCompiler::CTokenOperator,   (HMODULE)hModule);

			CLASS_SERIALIZABLE_REGISTER(ClassTokenChar,   SCFCompiler::CTokenChar,   (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(ClassTokenString, SCFCompiler::CTokenString, (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(ClassTokenNumber, SCFCompiler::CTokenNumber, (HMODULE)hModule);
			CLASS_SERIALIZABLE_REGISTER(ClassTokenType,   SCFCompiler::CTokenType,   (HMODULE)hModule);

//			CAssembler::Initialize();
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
			break;
		}
	}
	return TRUE;
} 