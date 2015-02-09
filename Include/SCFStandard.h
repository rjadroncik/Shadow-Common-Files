#pragma once

#define SCF_STANDARD_2

namespace SCF
{
	//Function parameter markers, input arguments are always constant.
	#define	_IN const

	//An input parameter reference which is directly stored in the object for which the method was invoked.
	//This means that the object keeps a reference to the input parameter, but may NOT delete the object automatically when destroyed.
	#define	_REF

	//Output parameters are always required, if not defined otherwise.
	#define	_OUT

	//Used when the function reads & writes to a pointer/reference
	#define	_INOUT

	//Parameter is optional, used with input arguments, its use is mostly defined by the preceding argument ( these are very rare :) )
	#define	_OPT

	//Functions that are yet not implemented or are to be changed/improved are marked as pending
	#define _PENDING

	//Marks a getter method, these methods can be invoked using a const this pointer
	#define _GET const

	//Setter methods modify objects, thus cannot be invoked using a const this pointer
	#define _SET

	//These are	still under consideration, may yet be removed!
	#ifdef ATL_EXPORT_TYPEDEFS

		//Specifies an integer-precision unsigned value, used for enumeration purposes, ENUMs should never be joined/fused using bitwise operators ( [|], [&], [^], .. )
		[export]
		typedef unsigned int   SCF::ENUM;

		[export]
		typedef unsigned char  UCHAR;
		[export]
		typedef unsigned short USHORT;
		[export]
		typedef unsigned int   UINT;
		[export]
		typedef unsigned long  ULONG;

		[export]
		typedef unsigned char  BYTE;
		[export]
		typedef unsigned short WORD;
		[export]
		typedef unsigned long  DWORD;

		[export]
		typedef unsigned __int64 UINT64;
		[export]
		typedef          __int64  INT64;

	#else

		//Specifies an integer-precision unsigned value, used for enumeration purposes, ENUMs should never be joined/fused using bitwise operators ( [|], [&], [^], .. )
		typedef unsigned int   ENUM;

		typedef unsigned char  UCHAR;
		typedef unsigned short USHORT;
		typedef unsigned int   UINT;
		typedef unsigned long  ULONG;

		typedef unsigned char  BYTE;
		typedef unsigned short WORD;
		typedef unsigned long  DWORD;

		typedef unsigned __int64 UINT64;
		typedef          __int64  INT64;

		#ifdef UNICODE

		typedef wchar_t  TCHAR;
		typedef wchar_t* LPTSTR;

		#else
		
		typedef unsigned char  TCHAR;
		typedef unsigned char* LPTSTR;

		#endif

	#endif

	#ifdef UNICODE                     
	#define __SCFTEXT(quote) L##quote      
	#else            
	#define __SCFTEXT(quote) quote         
	#endif

	#define SCFTEXT(quote) __SCFTEXT(quote)

	#define NULL  0
	#define FALSE 0
	#define TRUE  1
	
	//#define MAKEWORD(a, b) ((WORD)(((BYTE)((DWORD_PTR)(a) & 0xff)) | ((WORD)((BYTE)((DWORD_PTR)(b) & 0xff))) << 8))
	//#define MAKELONG(a, b) ((LONG)(((WORD)((DWORD_PTR)(a) & 0xffff)) | ((DWORD)((WORD)((DWORD_PTR)(b) & 0xffff))) << 16))
	//#define LOWORD(l)      ((WORD)((DWORD_PTR)(l) & 0xffff))
	//#define HIWORD(l)      ((WORD)((DWORD_PTR)(l) >> 16))
	//#define LOBYTE(w)      ((BYTE)((DWORD_PTR)(w) & 0xff))
	//#define HIBYTE(w)      ((BYTE)((DWORD_PTR)(w) >> 8))

	#define SCF_UNREFERENCED_PARAMETER(P) { P; }
};

#ifdef WIN32

#include <crtdbg.h>

#endif





