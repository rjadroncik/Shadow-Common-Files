#pragma once

#include <SCFObjectExtensions.h>

#include "../Classes.h"

namespace SCFScripting
{
	class SCRIPTING_API CDescriptor : public CObject
	{
	public:
		CDescriptor();
		virtual ~CDescriptor();

	public:
		inline void          Name(_IN CString& rName) _SET { m_Name = rName; }
		inline const CString Name()                   _GET { return STRINGREF(m_Name); }

	protected:
		CString m_Name;
	};
};
