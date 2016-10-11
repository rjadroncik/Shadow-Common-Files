#include "SCFDatafileIO.h"
#include <SCFXML.h>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

using namespace SCFBase;
using namespace SCFXML;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    SCF_UNREFERENCED_PARAMETER(lpReserved);

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: 
    { 
        CLASS_XMLSERIALIZABLE_REGISTER("Directory", SCFDatafileIOPrivate::CRecordDirectory, hModule);
        CLASS_XMLSERIALIZABLE_REGISTER("File",      SCFDatafileIOPrivate::CRecordFile,      hModule);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH: { break; }
    }
    return TRUE;
}