#pragma once

#include "Class.h"

namespace SCFScripting
{
	class SCRIPTING_API CClassReference : public CClass
	{
	public:
		CString ToString() _GET{ return STRING("{ClassReference} ") + m_Name; }

	public:
		CClassReference();
		virtual ~CClassReference();
	};
}
