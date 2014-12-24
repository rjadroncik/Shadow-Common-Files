#pragma once

#include <SCFObjectExtensions.h>

#include "../Classes.h"

using namespace SCFBase;

namespace SCFScripting
{
	enum CallingConvention
	{
		CallStd   = 0x1000, 
		CallCDecl = 0x2000, 
		CallThis  = 0x4000, 
		
		CallType  = 0xf000, 
	};

	class SCRIPTING_API CDescriptor : public CObject
	{
	public:
		CDescriptor();
		virtual ~CDescriptor();
	};
};
