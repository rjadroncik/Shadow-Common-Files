#pragma once

#include "Function.h"
#include "Property.h"

namespace SCFScripting
{
	class SCRIPTING_API CGeneric : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassGeneric; }
		CString   AsString() _GET { return STRING("{Generic}"); }

	public:
		CGeneric();
		virtual ~CGeneric();
	};
};
