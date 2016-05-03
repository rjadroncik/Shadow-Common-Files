#pragma once

#include "Object.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API IContainer
	{
	public:
		virtual UINT Size()    _GET = 0;
		virtual bool IsEmpty() _GET = 0;

	protected:
		virtual ~IContainer() {}
	};
};