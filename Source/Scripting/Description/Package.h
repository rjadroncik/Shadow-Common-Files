#pragma once

#include "Class.h"

namespace SCFScripting
{
	class SCRIPTING_API CPackage : public CDescriptor
	{
	public:
		SCF::ENUM ClassKey() _GET { return ClassPackage; }
		CString   ToString() _GET { return STRING("{Package}"); }

	public:
		CPackage();
		virtual ~CPackage();

	public:
		bool ClassAdd(_IN _REF CClass& rClass) _SET;

	public:
		CClass* ClassNamed(_IN CString& rName) _GET;

	private:
		#pragma warning(disable:4251)
		//Dictionary containing all the classes in this package
		CDictionaryString<CClass> m_Classes;
		#pragma warning(default:4251)
	};
};
