#pragma once

#include "Descriptor.h"
#include "Visibilities.h"

namespace SCFScripting
{
	class SCRIPTING_API CClass;

	class SCRIPTING_API CProperty : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassProperty; }
		CString   ToString() _GET { return STRING("{Property} ") + m_Name; }

	public:
		CProperty();
		virtual ~CProperty();
	
	public:
		void         Visibility(_IN Visibilities eVisibility) _SET { m_eVisibility = eVisibility; }
		Visibilities Visibility()                             _GET { return m_eVisibility; }

		void          Type(_IN CClass& rType) _SET { m_pType = &rType; }
		const CClass& Type()                  _GET { return *m_pType; }

		void IsReadOnly(_IN bool bIsReadOnly) _SET { m_bIsReadOnly = bIsReadOnly; }
		bool IsReadOnly()                     _GET { return m_bIsReadOnly; }

		void IsStatic(_IN bool bIsStatic) _SET{ m_bIsStatic = bIsStatic; }
		bool IsStatic()                     _GET{ return m_bIsStatic; }

	private:
		const CClass* m_pType;
		Visibilities m_eVisibility;
		bool m_bIsReadOnly;
		bool m_bIsStatic;
	};
};
