#pragma once

#include "Class.h"

namespace SCFScripting
{
	class SCRIPTING_API CClassReference : public CClass
	{
	public:
		//The class key returns the identifier of the class object, but not the class it describes
		SCF::ENUM ClassKey() _GET{ return ClassClassReference; }
		CString   ToString() _GET{ return STRING("{ClassReference} ") + m_Name; }

	public:
		CClassReference();
		virtual ~CClassReference();
	};
}
