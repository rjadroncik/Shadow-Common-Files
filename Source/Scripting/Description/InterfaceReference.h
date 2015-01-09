#pragma once

#include "Interface.h"

namespace SCFScripting
{
	class SCRIPTING_API CInterfaceReference : public CInterface
	{
	public:
		//The class key returns the identifier of the class object, but bot the class it describes
		SCF::ENUM ClassKey() _GET{ return ClassInterfaceReference; }
		CString   ToString() _GET{ return STRING("{InterfaceReference} ") + m_Name; }

	public:
		CInterfaceReference();
		virtual ~CInterfaceReference();
	};
}