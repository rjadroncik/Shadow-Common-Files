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

	public:
		inline void       Properties(_IN SCF::DWORD dwProperties) _SET { m_dwProperties = dwProperties; }
		inline SCF::DWORD Properties()                            _GET { return m_dwProperties; }

	protected:
		CString m_Name;

	protected:
		SCF::DWORD m_dwProperties;
	};
};
