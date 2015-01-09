#pragma once

#include "MethodSignature.h"
#include "Visibilities.h"

namespace SCFScripting
{
	class SCRIPTING_API CMethod : public CMethodSignature
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassMethod; }
		CString   ToString() _GET { return STRING("{Method}"); }

	public:
		CMethod();
		virtual ~CMethod();

	public:
		void         Visibility(_IN Visibilities eVisibility) _SET{ m_eVisibility = eVisibility; }
		Visibilities Visibility()                             _GET{ return m_eVisibility; }

		void IsStatic(_IN bool bIsStatic) _SET { m_bIsStatic = bIsStatic; }
		bool IsStatic()                   _GET { return m_bIsStatic; }

		void IsAbstract(_IN bool bIsAbstract) _SET { m_bIsAbstract = bIsAbstract; }
		bool IsAbstract()                     _GET { return m_bIsAbstract; }

		void IsVirtual(_IN bool bIsVirtual) _SET { m_bIsVirtual = bIsVirtual; }
		bool IsVirtual()                    _GET { return m_bIsVirtual; }

	private:
		Visibilities m_eVisibility;

		bool m_bIsStatic;
		bool m_bIsAbstract;
		bool m_bIsVirtual;
	};
};
