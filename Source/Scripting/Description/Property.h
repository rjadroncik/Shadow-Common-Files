#pragma once

#include "Descriptor.h"

namespace SCFScripting
{
	enum PropertyProperties
	{
		PropertyIsAddRefed      = 0x0001, 
		PropertyIsEnumeration   = 0x0002, 
		PropertyIsGeneric       = 0x0004, 
		PropertyIsSettable      = 0x0008, 
	};

	class SCRIPTING_API CClass;

	class SCRIPTING_API CProperty : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassProperty; }
		CString   AsString() _GET { return STRING("{Property}"); }

	public:
		CProperty();
		virtual ~CProperty();
	
	public:
		void          Type(_IN CClass& rType) _SET { m_pType = &rType; }
		const CClass& Type()                  _GET { return *m_pType; }

	private:
		const CClass* m_pType;
	};
};
