#include "String.h"
#include "Int.h"
#include "Format.h"
#include "FSBHeap.h"

#include <malloc.h>

using namespace SCFBase;

#ifdef _BETA

bool Object_bTracing = false;

bool CObject::Tracing()                  { return Object_bTracing; }
void CObject::Tracing(_IN bool bTracing) { Object_bTracing = bTracing; }

UINT Object_uiObjectCount = 0;

OBJECT_DEBUG_HOOK Object_DebugHookConstructor = nullptr;
OBJECT_DEBUG_HOOK Object_DebugHookDestructor  = nullptr;

void CObject::RegisterDebugHookConstructor(OBJECT_DEBUG_HOOK hookProc) { Object_DebugHookConstructor = hookProc; }
void CObject::RegisterDebugHookDestructor (OBJECT_DEBUG_HOOK hookProc) { Object_DebugHookDestructor  = hookProc; }

UINT CObject::TotalSystemCount() { return Object_uiObjectCount; }
#endif

CObject::CObject()
{
#ifdef _BETA
	m_uiRefCount = 0;
	Object_uiObjectCount++;

	if (Object_bTracing && Object_DebugHookConstructor) { Object_DebugHookConstructor(*this); }
#endif
}

CObject::~CObject()
{
#ifdef _BETA
	if (Object_bTracing && Object_DebugHookDestructor) { Object_DebugHookDestructor(*this); }

	_ASSERTE(m_uiRefCount == 0);

	Object_uiObjectCount--;
#endif
}

CString CObject::ToString()                     _GET { return "{CObject}"; }
CString CObject::ToString(_IN CFormat& rFormat) _GET { SCF_UNREFERENCED_PARAMETER(rFormat); return ToString(); }
 

///////// Dynamic object creation & run time type information management /////////

#define OBJECT_FSB_SIZE_MAX 128

SCFPrivate::CFSBHeap Object_Heap16 (16 + 4);
SCFPrivate::CFSBHeap Object_Heap32 (32 + 4);
SCFPrivate::CFSBHeap Object_Heap64 (64 + 4);
SCFPrivate::CFSBHeap Object_Heap128(128 + 4);

void* __stdcall CObject::operator new(size_t uiBytes)
{
	if (uiBytes < 17)  { void* vpMemory = Object_Heap16.Allocate();  *(void**)vpMemory = &Object_Heap16;  return (void**)vpMemory + 1; }
	if (uiBytes < 33)  { void* vpMemory = Object_Heap32.Allocate();  *(void**)vpMemory = &Object_Heap32;  return (void**)vpMemory + 1; }
	if (uiBytes < 65)  { void* vpMemory = Object_Heap64.Allocate();  *(void**)vpMemory = &Object_Heap64;  return (void**)vpMemory + 1; }
	if (uiBytes < 129) { void* vpMemory = Object_Heap128.Allocate(); *(void**)vpMemory = &Object_Heap128; return (void**)vpMemory + 1; }

	void* vpMemory = malloc(uiBytes + 4); *(void**)vpMemory = nullptr; return (void**)vpMemory + 1;
}

void __stdcall CObject::operator delete(void* vpObject)
{
	SCFPrivate::CFSBHeap* pHeap = *((SCFPrivate::CFSBHeap**)vpObject - 1);
	if (pHeap)
	{
		pHeap->Free((void**)vpObject - 1);
	}
	else { free((void**)vpObject - 1); }
}

void* __stdcall CObject::operator new[](size_t uiBytes)
{
	SCF_UNREFERENCED_PARAMETER(uiBytes);
	SCFError(ErrorIllegalOperation);
	return nullptr;
}
void  __stdcall CObject::operator delete[](void* vpObjects)
{
	SCF_UNREFERENCED_PARAMETER(vpObjects);
	SCFError(ErrorIllegalOperation);
}
