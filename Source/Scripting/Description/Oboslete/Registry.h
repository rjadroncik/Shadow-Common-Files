#pragma once

#include "Class.h"
#include "Enumeration.h"

#define CLASS_REGISTER_ARGS_CREATE(classFullyQualified, parentClassFullyQualified) STRING(#classFullyQualified), *(new classFullyQualified(sizeof(classFullyQualified), SCFScripting::CRegistry::Class(#parentClassFullyQualified)))

namespace SCFScripting
{
	class SCRIPTING_API CRegistry
	{
	public:
		//Registers a class & performs name mappings
		static void EnumerationRegister(_IN CString& rName, _IN CEnumeration& rEnumeration);
	
		//You can get an enumeration descriptor based on its name
		static CEnumeration* Enumeration(_IN CString& rName);

		//Never delete the returned object
		static CEnum*   EnumerationTranslate(_IN CString& rEnumeration, _IN CString& rValue);
		static CString* EnumerationTranslate(_IN CString& rEnumeration, _IN SCF::ENUM eValue);

	public:
		//Registers a class & performs name mappings
		static void ClassRegister(_IN SCF::ENUM eClassKey, _IN CString& rFullClassName, _INOUT CClass& rClass);

		//You can get a class descriptor based on a name or the class-key of the descibed class
		static CClass* Class(_IN CString& rFullClassName);
		static CClass* Class(_IN SCF::ENUM eClassKey);

	public:
		static SCF::ENUM ClassKey (_IN CString& rFullClassName);
		static CString   ClassName(_IN SCF::ENUM eClassKey);

	private:
		CRegistry();
		~CRegistry();
	};
};
