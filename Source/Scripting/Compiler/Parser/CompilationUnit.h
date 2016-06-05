#pragma once

#include "../../Description/Enum.h"
#include "../../Description/Class.h"
#include "../../Description/Package.h"

namespace SCFCompiler
{
	class SCRIPTING_API CCompilationUnit : public SCFBase::CObject
	{
	public:
		CString ToString() _GET { return STRING("{CompilationUnit}"); }

	public:
		CCompilationUnit();
		virtual ~CCompilationUnit();

	public:
		SCFScripting::CPackage* Package()                                     _GET { return m_pPackage; }
		void                    Package(_IN SCFScripting::CPackage* pPackage) _SET { m_pPackage = (SCFScripting::CPackage*)pPackage; }

	public:
		CList<SCFScripting::CPackage>& Imports() _GET { return (CList<SCFScripting::CPackage>&)m_Imports; }
	
	public:
		CList<SCFScripting::CEnum>&      Enums()      _GET { return (CList<SCFScripting::CEnum>&)m_Enums; }
		CList<SCFScripting::CInterface>& Interfaces() _GET { return (CList<SCFScripting::CInterface>&)m_Interfaces; }
		CList<SCFScripting::CClass>&     Classes()    _GET { return (CList<SCFScripting::CClass>&)m_Classes; }

	protected:
		//The package to which this compilation unit belongs
		SCFScripting::CPackage* m_pPackage;

		//The list of the packages imported into this compilation unit
	protected:
		#pragma warning(disable:4251)
		CList<SCFScripting::CPackage> m_Imports;

	protected:
		//UDTs parsed in one compilation unit
		CList<SCFScripting::CEnum>      m_Enums;
		CList<SCFScripting::CInterface> m_Interfaces;
		CList<SCFScripting::CClass>     m_Classes;
		#pragma warning(default:4251)
	};
};
