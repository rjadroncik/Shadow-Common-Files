#pragma once

#include "Interface.h"

namespace SCFScripting
{
	class SCRIPTING_API CInterfaceReference : public CInterface
	{
	public:
		CString ToString() _GET{ return STRING("{InterfaceReference} ") + m_Name; }

	public:
		CInterfaceReference();
		virtual ~CInterfaceReference();
	};
}