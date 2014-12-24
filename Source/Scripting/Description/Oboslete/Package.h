#pragma once

#include "Class.h"

namespace SCFScripting
{
	class SCRIPTING_API CPackage : public SCFBase::CObject
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassPackage; }

	public:
		CPackage(_IN CString& rName);
		virtual ~CPackage();

	public:
		void    ClassAdd  (_IN CString& rName, _IN _REF CClass& rClass) _SET;
		CClass* ClassNamed(_IN CString& rName) _GET;

	public:
		inline const CString Name()                   _GET { return STRINGREF(m_Name); }
		inline void          Name(_IN CString& rName) _SET { m_Name = rName; }

	private:
		//Dictionary containing all the classes in this package
		CDictionaryString m_Classes;

	private:
		CString m_Name;
	};
};
