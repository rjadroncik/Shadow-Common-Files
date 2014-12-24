#pragma once

#include "Classes.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CString;

	class OBJECT_EXTENSIONS_API IUnknown
	{
	public:
		virtual ENUM    ClassKey()       _GET = 0;
		virtual CString ClassKeyString() _GET = 0;

	public:
		//Dynamic casting
		virtual void* AsType(_IN ENUM eType) const = 0; 

	public:
		//Utility functions which allow the detection of premature object deletion
		//(in the case when some other objects still keep a pointer/reference to the object being deleted)
		void AddRef()  const;
		void Release() const;
		UINT RefCount() _GET;
	};
};
