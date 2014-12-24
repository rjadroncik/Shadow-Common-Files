#pragma once

#include "MethodSignature.h"

namespace SCFScripting
{
	class SCRIPTING_API CMethod : public CMethodSignature
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassMethod; }
		CString   AsString() _GET { return STRING("{Method}"); }

	public:
		CMethod();
		virtual ~CMethod();
	};
};
