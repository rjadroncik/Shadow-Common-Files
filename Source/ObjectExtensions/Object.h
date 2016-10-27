#pragma once

#include <SCFStandard.h>

#include "Errors.h"

namespace SCFPrivate
{
	class OBJECT_EXTENSIONS_API CFSBHeap;
};

#define ALLOC_GRANULARITY_BYTES 64
#define ALLOC_GRANULARITY_PTRS  16

#define CALL_MEMBER(object, ptrToMember) ((object).*(ptrToMember))

#ifdef _BETA

#define BETAONLY(expression) expression
#define ADDREF(object) (object).AddRef();
#define RELEASE(object) (object).Release();

#else

#define BETAONLY(expression)
#define ADDREF(object)
#define RELEASE(object)

#endif

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CObject;
	class OBJECT_EXTENSIONS_API CString;
	class OBJECT_EXTENSIONS_API CFormat;

	BETAONLY(typedef void (__stdcall *OBJECT_DEBUG_HOOK) (_IN CObject& rObject);)

	class OBJECT_EXTENSIONS_API CObject
	{
	public:
		//Generic equality testing. Can come in handy. NOT IMPLEMENTED in most classes.
		//Designed to be overridden in specific cases only.
		virtual bool IsEqualTo(_IN CObject& rObject) _GET { return (this == &rObject); }

	public:
		//Basic object qualification
		//Values - are simple data types - ToString() returns converted data contained within
		virtual bool IsValue() _GET { return false; }

	public:
		//Operator new & delete are globally overridden & use a custom memory allocation scheme
		void* __stdcall operator new(size_t uiBytes);
		void  __stdcall operator delete(void* vpObject);

		//Currently unsupported :)
		void* __stdcall operator new[](size_t uiBytes);
		void  __stdcall operator delete[](void* vpObjects);

	public:
		//Destructor must always be virtual!!!
		virtual ~CObject();

	public:
		//Creates a string representation of the object.
		//Unless the object is derived from CValue, this string can be just informative.
		virtual CString ToString() _GET;
		virtual CString ToString(_IN CFormat& rFormat) _GET;

	public:
		//This method can be implemented as a common way of preparing objects for deletion
		//when for example cross-referencing problems occur
		virtual void Dispose() {}

	protected:
		//Can't create an instance of the superclass (has no class key & is abstract anyway)
		CObject();

#ifdef _BETA
	public:
		//Enables/disables the tracing of object creation & deletion ON A GLOBAL SCALE, tracing is off default
		//If this is set to "false", NO TRACKING WHATSOEVER will be performed
		static bool Tracing();
		static void Tracing(bool bTracing);

	public:
		//Debug hooks can be used to perform custom tracking of object creation & deletion, or perhaps some debug logging
		static void RegisterDebugHookConstructor(OBJECT_DEBUG_HOOK hookProc);
		static void RegisterDebugHookDestructor (OBJECT_DEBUG_HOOK hookProc);

	public:
		//This can come in handy when testing whether the app destroys all object properly
		static UINT TotalSystemCount();

	public:
		//Utility functions which allow the detection of premature object deletion
		//(in the case when some other objects still keep a pointer/reference to the object being deleted)
		void AddRef()  const { (*(UINT*)&m_uiRefCount)++; }
		void Release() const { (*(UINT*)&m_uiRefCount)--; }
		UINT RefCount() _GET { return m_uiRefCount; }

	private:
		UINT m_uiRefCount;
#endif
	};
};

