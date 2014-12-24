#pragma once

#include "Descriptor.h"

namespace SCFScripting
{
	class SCRIPTING_API CClass;

	class SCRIPTING_API CArgument: public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassArgument; }
		CString   AsString() _GET { return STRING("{Argument}"); }

	public:
		CArgument();
		virtual ~CArgument();

	public:
		inline void          Type(_IN CClass& rType) _SET { m_pType = &rType; }
		inline const CClass& Type()                  _GET { return *m_pType; }

	public:
		inline void IsOut(_IN bool bIsOut) _SET { m_bIsOut = bIsOut; }
		inline bool IsOut()                _GET { return m_bIsOut; }

	private:
		const CClass* m_pType;
		bool m_bIsOut;
	};
};
