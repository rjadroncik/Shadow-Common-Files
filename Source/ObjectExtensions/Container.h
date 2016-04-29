#pragma once

#include "Object.h"

namespace SCFBase
{
	class OBJECT_EXTENSIONS_API CContainer : public CObject
	{
	public:
		virtual SCF::UINT Size()    _GET = 0;
		virtual bool      IsEmpty() _GET = 0;
	};
};