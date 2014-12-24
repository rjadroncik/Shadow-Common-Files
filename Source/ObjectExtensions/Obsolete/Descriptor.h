#pragma once

#include "Object.h"

namespace SCFBase
{
	enum CallingConvention
	{
		CallStd   = 0x1000, 
		CallCDecl = 0x2000, 
		CallThis  = 0x4000, 
		
		CallType  = 0xf000, 
	};

	class OBJECT_EXTENSIONS_API CDescriptor : public CObject
	{
	public:
		CDescriptor();
		virtual ~CDescriptor();
	};
};
