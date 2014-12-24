#pragma once
#include "String.h"

namespace SCFObjectExtensions
{
	class OBJECT_EXTENSIONS_API CStreamProcessor : public CObject 
	{
	public:
		CStreamProcessor();
		virtual ~CStreamProcessor();

	public:
		CString AsString() _GET { return STRING("{StreamProcessor}"); }
	};
};